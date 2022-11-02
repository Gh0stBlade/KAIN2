#include "LIBGPU.H"
#include "EMULATOR_RENDER_GXM.H"
#include "Core/Debug/EMULATOR_LOG.H"
#include "Core/Render/EMULATOR_RENDER_COMMON.H"
#include "Core/Setup/Game/GAME_VERSION.H"
#include <stdio.h>

#if defined(SN_TARGET_PSP2)

#include <string.h>
#include <stdlib.h>
#include <sceconst.h>
#include <libdbg.h>
#include <kernel.h>
#include <display.h>
#include <ctrl.h>
#include <gxm.h>
#include <math.h>
#include <stdio.h>

#include <gxm/program.h>

#define DISPLAY_WIDTH				960
#define DISPLAY_HEIGHT				544
#define DISPLAY_STRIDE_IN_PIXELS	1024

#define DISPLAY_COLOR_FORMAT		SCE_GXM_COLOR_FORMAT_A8B8G8R8
#define DISPLAY_PIXEL_FORMAT		SCE_DISPLAY_PIXELFORMAT_A8B8G8R8

#define DISPLAY_BUFFER_COUNT		2

#define DISPLAY_MAX_PENDING_SWAPS	3

#define ALIGN(x, a)					(((x) + ((a) - 1)) & ~((a) - 1))

#define SAMPLE_NAME SHORT_GAME_NAME

int g_CurrentBlendMode = BM_NONE;
SceGxmDepthStencilSurface g_depthSurface;
SceUID g_depthBufferUid;
void* g_depthBufferData = NULL;
SceGxmContext* g_context = NULL;
SceGxmShaderPatcher* g_shaderPatcher = NULL;
SceGxmRenderTarget* g_renderTarget = NULL;
void* g_displayBufferData[DISPLAY_BUFFER_COUNT];
SceGxmColorSurface g_displaySurface[DISPLAY_BUFFER_COUNT];
SceGxmSyncObject* g_displayBufferSync[DISPLAY_BUFFER_COUNT];
SceUID g_displayBufferUid[DISPLAY_BUFFER_COUNT];
SceUID g_patcherBufUid;
SceUID g_patcherCombinedUsseUid;
unsigned int g_backBufferIndex = 0;
unsigned int g_frontBufferIndex = 0;

SceCtrlData	g_ctrlData;

static void displayCallback(const void *callbackData);

static void *patcherHostAlloc(void *userData, uint32_t size);

static void patcherHostFree(void *userData, void *mem);

static void *graphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID *uid);

static void graphicsFree(SceUID uid);

static void *vertexUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset);

static void vertexUsseFree(SceUID uid);

static void *fragmentUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset);

// Helper function to free memory mapped as fragment USSE code for the GPU
static void fragmentUsseFree(SceUID uid);

// Mark variable as used
#define	UNUSED(a)					(void)(a)

// User main thread parameters
extern const char			sceUserMainThreadName[]		= "libgxm_basic_main_thr";
extern const int			sceUserMainThreadPriority	= SCE_KERNEL_DEFAULT_PRIORITY_USER;
extern const unsigned int	sceUserMainThreadStackSize	= SCE_KERNEL_STACK_SIZE_DEFAULT_USER_MAIN;

// Define a 1MiB heap for this sample
unsigned int	sceLibcHeapSize	= 1*1024*1024;

void displayCallback(const void *callbackData)
{
	SceDisplayFrameBuf framebuf;
	int err = SCE_OK;
	UNUSED(err);

	const DisplayData *displayData = (const DisplayData *)callbackData;

	memset(&framebuf, 0x00, sizeof(SceDisplayFrameBuf));
	framebuf.size        = sizeof(SceDisplayFrameBuf);
	framebuf.base        = displayData->address;
	framebuf.pitch       = DISPLAY_STRIDE_IN_PIXELS;
	framebuf.pixelformat = DISPLAY_PIXEL_FORMAT;
	framebuf.width       = DISPLAY_WIDTH;
	framebuf.height      = DISPLAY_HEIGHT;
	err = sceDisplaySetFrameBuf(&framebuf, SCE_DISPLAY_UPDATETIMING_NEXTVSYNC);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceDisplayWaitVblankStart();
	SCE_DBG_ASSERT(err == SCE_OK);
}

static void *patcherHostAlloc(void *userData, uint32_t size)
{
	UNUSED(userData);
	return malloc(size);
}

static void patcherHostFree(void *userData, void *mem)
{
	UNUSED(userData);
	free(mem);
}

static void *graphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID *uid)
{
	int err = SCE_OK;
	UNUSED(err);

	/*	Since we are using sceKernelAllocMemBlock directly, we cannot directly
		use the alignment parameter.  Instead, we must allocate the size to the
		minimum for this memblock type, and just assert that this will cover
		our desired alignment.

		Developers using their own heaps should be able to use the alignment
		parameter directly for more minimal padding.
	*/
	if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA) {
		// CDRAM memblocks must be 256KiB aligned
		SCE_DBG_ASSERT(alignment <= 256*1024);
		size = ALIGN(size, 256*1024);
	} else {
		// LPDDR memblocks must be 4KiB aligned
		SCE_DBG_ASSERT(alignment <= 4*1024);
		size = ALIGN(size, 4*1024);
	}
	UNUSED(alignment);

	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", type, size, NULL);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// map for the GPU
	err = sceGxmMapMemory(mem, size, attribs);
	SCE_DBG_ASSERT(err == SCE_OK);

	// done
	return mem;
}

static void graphicsFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

static void *vertexUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	// align to memblock alignment for LPDDR
	size = ALIGN(size, 4096);
	
	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// map as vertex USSE code for the GPU
	err = sceGxmMapVertexUsseMemory(mem, size, usseOffset);
	SCE_DBG_ASSERT(err == SCE_OK);

	// done
	return mem;
}

static void vertexUsseFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapVertexUsseMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

static void *fragmentUsseAlloc(uint32_t size, SceUID *uid, uint32_t *usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	// align to memblock alignment for LPDDR
	size = ALIGN(size, 4096);
	
	// allocate some memory
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// map as fragment USSE code for the GPU
	err = sceGxmMapFragmentUsseMemory(mem, size, usseOffset);
	SCE_DBG_ASSERT(err == SCE_OK);

	// done
	return mem;
}

static void fragmentUsseFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	// grab the base address
	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// unmap memory
	err = sceGxmUnmapFragmentUsseMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	// free the memory block
	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

//END SAMPLE

extern void Emulator_DoPollEvent();
extern void Emulator_WaitForTimestep(int count);
extern void Emulator_GenerateCommonTextures();
extern void Emulator_CreateGlobalShaders();
extern void Emulator_DestroyTextures();
extern void Emulator_DestroyGlobalShaders();
extern void Emulator_CreateVertexBuffer();
extern void Emulator_CreateIndexBuffer();

const char* renderBackendName = "GXM";

SceUID dynamic_vertex_buffer_id;
SceUID dynamic_index_buffer_id;
struct Vertex* dynamic_vertex_buffer;
unsigned short* dynamic_index_buffer;
unsigned int dynamic_vertex_array;

void* g_contextHost;

int g_overrideWidth = -1;
int g_overrideHeight = -1;

SceUID g_vdmRingBufUid;
SceUID g_vertexRingBufUid;
SceUID g_fragmentRingBufUid;
SceUID g_fragmentUsseRingBufUid;
SceUInt32 ringbufFragmentUsseOffset;

void* vramTextureBuff = NULL;
void* whiteTextureBuff = NULL;
void* rg8lutTextureBuff = NULL;

#define	UNUSED(a)					(void)(a)

const SceGxmProgramParameter* u_Projection;

SceGxmBlendInfo Emulator_GetBlendInfo(int blendMode)
{
	SceGxmBlendInfo blendInfo;

	switch (blendMode)
	{
	case BM_NONE:
		blendInfo.colorSrc = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.colorDst = SCE_GXM_BLEND_FACTOR_ZERO;
		blendInfo.colorFunc = SCE_GXM_BLEND_FUNC_ADD;
		blendInfo.alphaSrc = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.alphaDst = SCE_GXM_BLEND_FACTOR_ZERO;
		blendInfo.alphaFunc = SCE_GXM_BLEND_FUNC_ADD;
		blendInfo.colorMask = SCE_GXM_COLOR_MASK_ALL;
		break;
	case BM_AVERAGE:
		blendInfo.colorSrc = SCE_GXM_BLEND_FACTOR_SRC_COLOR;
		blendInfo.colorDst = SCE_GXM_BLEND_FACTOR_DST_COLOR;
		blendInfo.colorFunc = SCE_GXM_BLEND_FUNC_ADD;
		blendInfo.alphaSrc = SCE_GXM_BLEND_FACTOR_SRC_COLOR;
		blendInfo.alphaDst = SCE_GXM_BLEND_FACTOR_DST_COLOR;
		blendInfo.alphaFunc = SCE_GXM_BLEND_FUNC_ADD;
		blendInfo.colorMask = SCE_GXM_COLOR_MASK_ALL;
		break;
	case BM_ADD:
		blendInfo.colorSrc = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.colorDst = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.colorFunc = SCE_GXM_BLEND_FUNC_ADD;
		blendInfo.alphaSrc = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.alphaDst = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.alphaFunc = SCE_GXM_BLEND_FUNC_ADD;
		blendInfo.colorMask = SCE_GXM_COLOR_MASK_ALL;
		break;
	case BM_SUBTRACT:
		blendInfo.colorSrc = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.colorDst = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.colorFunc = SCE_GXM_BLEND_FUNC_REVERSE_SUBTRACT;
		blendInfo.alphaSrc = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.alphaDst = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.alphaFunc = SCE_GXM_BLEND_FUNC_REVERSE_SUBTRACT;
		blendInfo.colorMask = SCE_GXM_COLOR_MASK_ALL;
		break;
	case BM_ADD_QUATER_SOURCE:
		blendInfo.colorSrc = SCE_GXM_BLEND_FACTOR_SRC_COLOR;
		blendInfo.colorDst = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.colorFunc = SCE_GXM_BLEND_FUNC_ADD;
		blendInfo.alphaSrc = SCE_GXM_BLEND_FACTOR_SRC_COLOR;
		blendInfo.alphaDst = SCE_GXM_BLEND_FACTOR_ONE;
		blendInfo.alphaFunc = SCE_GXM_BLEND_FUNC_ADD;
		blendInfo.colorMask = SCE_GXM_COLOR_MASK_ALL;
		break;
	}

	return blendInfo;
}

ShaderID Shader_Compile_Internal(const SceGxmProgram* source_vs, const SceGxmProgram* source_fs, int gte_shader)
{
	ShaderID shader;
	int err = SCE_OK;

#define OFFSETOF(T, E)     ((size_t)&(((T*)0)->E))

	shader.isGTE = gte_shader;

	if(gte_shader)
	{
		err = sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_vs, &shader.VSID);
		SCE_DBG_ASSERT(err == SCE_OK);
		err =sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_fs, &shader.FSID);
		SCE_DBG_ASSERT(err == SCE_OK);

		shader.PRG = sceGxmShaderPatcherGetProgramFromId(shader.VSID);
		SCE_DBG_ASSERT(shader.PRG);
	
		const SceGxmProgramParameter* paramPositionAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_position");
		SCE_DBG_ASSERT(paramPositionAttribute && (sceGxmProgramParameterGetCategory(paramPositionAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	
		const SceGxmProgramParameter* paramTexcoordAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_texcoord");
		SCE_DBG_ASSERT(paramTexcoordAttribute && (sceGxmProgramParameterGetCategory(paramTexcoordAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	
		const SceGxmProgramParameter* paramColorAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_color");
		SCE_DBG_ASSERT(paramColorAttribute && (sceGxmProgramParameterGetCategory(paramColorAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));

		SceGxmVertexAttribute basicVertexAttributes[3];
		SceGxmVertexStream basicVertexStreams[1];
		basicVertexAttributes[0].streamIndex = 0;
		basicVertexAttributes[0].offset = OFFSETOF(struct Vertex, x);
	#if defined(PGXP)
		basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
	#else
		basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_S16;
	#endif
		basicVertexAttributes[0].componentCount = 4;
		basicVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramPositionAttribute);
	
		basicVertexAttributes[1].streamIndex = 0;
		basicVertexAttributes[1].offset = OFFSETOF(struct Vertex, u);
		basicVertexAttributes[1].format = SCE_GXM_ATTRIBUTE_FORMAT_U8;
		basicVertexAttributes[1].componentCount = 4;
		basicVertexAttributes[1].regIndex = sceGxmProgramParameterGetResourceIndex(paramTexcoordAttribute);
	
		basicVertexAttributes[2].streamIndex = 0;
		basicVertexAttributes[2].offset = OFFSETOF(struct Vertex, r);
		basicVertexAttributes[2].format = SCE_GXM_ATTRIBUTE_FORMAT_U8N;
		basicVertexAttributes[2].componentCount = 4;
	
		basicVertexAttributes[2].regIndex = sceGxmProgramParameterGetResourceIndex(paramColorAttribute);
	
		basicVertexStreams[0].stride = sizeof(struct Vertex);
		basicVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
	
		err = sceGxmShaderPatcherCreateVertexProgram(g_shaderPatcher, shader.VSID, basicVertexAttributes, 3, basicVertexStreams, 1, &shader.VP);
		SCE_DBG_ASSERT(err == SCE_OK);

		for(int i = 0; i < BM_COUNT; i++)
		{
			SceGxmBlendInfo blendInfo = Emulator_GetBlendInfo(i);

			err = sceGxmShaderPatcherCreateFragmentProgram(g_shaderPatcher, shader.FSID, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, SCE_GXM_MULTISAMPLE_NONE, &blendInfo, shader.PRG, &shader.FP[i]);
			SCE_DBG_ASSERT(err == SCE_OK);
		}
	}
	else
	{
		err = sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_vs, &shader.VSID);
		SCE_DBG_ASSERT(err == SCE_OK);
		err =sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_fs, &shader.FSID);
		SCE_DBG_ASSERT(err == SCE_OK);

		shader.PRG = sceGxmShaderPatcherGetProgramFromId(shader.VSID);
		SCE_DBG_ASSERT(shader.PRG);
	
		const SceGxmProgramParameter* paramPositionAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_position");
		SCE_DBG_ASSERT(paramPositionAttribute && (sceGxmProgramParameterGetCategory(paramPositionAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	
		const SceGxmProgramParameter* paramTexcoordAttribute = sceGxmProgramFindParameterByName(shader.PRG, "In.a_texcoord");
		SCE_DBG_ASSERT(paramTexcoordAttribute && (sceGxmProgramParameterGetCategory(paramTexcoordAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));

		SceGxmVertexAttribute basicVertexAttributes[2];
		SceGxmVertexStream basicVertexStreams[1];
		basicVertexAttributes[0].streamIndex = 0;
		basicVertexAttributes[0].offset = OFFSETOF(struct Vertex, x);
#if defined(PGXP)
		basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
#else
		basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_S16;
#endif
		basicVertexAttributes[0].componentCount = 4;
		basicVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(paramPositionAttribute);
	
		basicVertexAttributes[1].streamIndex = 0;
		basicVertexAttributes[1].offset = OFFSETOF(struct Vertex, u);
		basicVertexAttributes[1].format = SCE_GXM_ATTRIBUTE_FORMAT_U8;
		basicVertexAttributes[1].componentCount = 4;
		basicVertexAttributes[1].regIndex = sceGxmProgramParameterGetResourceIndex(paramTexcoordAttribute);
	
		basicVertexStreams[0].stride = sizeof(struct Vertex);
		basicVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
	
		err = sceGxmShaderPatcherCreateVertexProgram(g_shaderPatcher, shader.VSID, basicVertexAttributes, 2, basicVertexStreams, 1, &shader.VP);
		SCE_DBG_ASSERT(err == SCE_OK);

		for(int i = 0; i < BM_COUNT; i++)
		{
			SceGxmBlendInfo blendInfo = Emulator_GetBlendInfo(i);

			err = sceGxmShaderPatcherCreateFragmentProgram(g_shaderPatcher, shader.FSID, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, SCE_GXM_MULTISAMPLE_NONE, &blendInfo, shader.PRG, &shader.FP[i]);
			SCE_DBG_ASSERT(err == SCE_OK);
		}
	}

	return shader;
}

void Emulator_DestroyVertexBuffer()
{
	dynamic_vertex_buffer = NULL;

	graphicsFree(dynamic_vertex_buffer_id);
}

void Emulator_DestroyIndexBuffer()
{
	dynamic_index_buffer = NULL;

	graphicsFree(dynamic_index_buffer_id);
}

void Emulator_ResetDevice()
{
	if (!g_resettingDevice)
	{
		g_resettingDevice = TRUE;

		Emulator_DestroyVertexBuffer();
		
		Emulator_DestroyIndexBuffer();

		Emulator_DestroyTextures();

		Emulator_DestroyGlobalShaders();

		Emulator_CreateGlobalShaders();

		Emulator_GenerateCommonTextures();

		Emulator_CreateVertexBuffer();

		Emulator_CreateIndexBuffer();

		g_resettingDevice = FALSE;
	}
}

void Emulator_DestroyTextures()
{

	//vramTexture = 0;
	//rg8lutTexture = 0;
	//whiteTexture = 0;
}

void Emulator_DestroyGlobalShaders()
{
	//g_gte_shader_4 = 0;
	//g_gte_shader_8 = 0;
	//g_gte_shader_16 = 0;
	//g_blit_shader = 0;
}

void Emulator_DisplayCallback(const void *data)
{


}

extern unsigned char* padData[2];

int begin_render_scene_flag = 0;

void Emulator_BeginRenderScene()
{
	SceCtrlData ct[6];
	unsigned int btn;
	int res;
	unsigned short kbInputs = 0xFFFF;

	res = sceCtrlReadBufferPositive(0, &ct[0], 6);

	btn = 0;
	for(int i = 0; i < res; i++)
	{
		btn |= ct[i].buttons;
	}

	if((btn & SCE_CTRL_CROSS))
	{
		printf("CROSS PRESSED!\n");
		kbInputs &= ~0x4000;
	}

	((unsigned short*)padData[0])[1] = kbInputs;
	((unsigned short*)padData[0])[2] = 128;//Maybe not required.
	((unsigned short*)padData[0])[3] = 128;

	if(!begin_render_scene_flag)
	{
		sceGxmBeginScene(g_context, 0, g_renderTarget, NULL, NULL, g_displayBufferSync[g_backBufferIndex], &g_displaySurface[g_backBufferIndex], &g_depthSurface);
	
		begin_render_scene_flag = 1;
	}
}

void Emulator_EndRenderScene()
{

	begin_render_scene_flag = 0;
	sceGxmEndScene(g_context, NULL, NULL);
	sceGxmPadHeartbeat(&g_displaySurface[g_backBufferIndex], g_displayBufferSync[g_backBufferIndex]);
}

int Emulator_InitialiseGXMContext(char* windowName)
{
	int err = SCE_OK;
	UNUSED(err);

#ifdef ENABLE_RAZOR_HUD
	err = sceSysmoduleLoadModule( SCE_SYSMODULE_RAZOR_HUD );
	SCE_DBG_ASSERT(err == SCE_OK);
#endif

#ifdef ENABLE_RAZOR_GPU_CAPTURE
	err = sceSysmoduleLoadModule( SCE_SYSMODULE_RAZOR_CAPTURE );
	SCE_DBG_ASSERT(err == SCE_OK);

	sceRazorGpuCaptureSetTrigger( 100, "app0:basic.sgx" );
#endif

	windowWidth = DISPLAY_WIDTH;
	windowHeight = DISPLAY_HEIGHT;

	SceGxmInitializeParams initializeParams;
	memset(&initializeParams, 0, sizeof(SceGxmInitializeParams));
	initializeParams.flags							= 0;
	initializeParams.displayQueueMaxPendingCount	= DISPLAY_MAX_PENDING_SWAPS;
	initializeParams.displayQueueCallback			= displayCallback;
	initializeParams.displayQueueCallbackDataSize	= sizeof(DisplayData);
	initializeParams.parameterBufferSize			= SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

	err = sceGxmInitialize(&initializeParams);
	SCE_DBG_ASSERT(err == SCE_OK);

	SceUID vdmRingBufferUid;
	void* vdmRingBuffer = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &vdmRingBufferUid);
	SceUID vertexRingBufferUid;
	void* vertexRingBuffer = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &vertexRingBufferUid);
	SceUID fragmentRingBufferUid;
	void* fragmentRingBuffer = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &fragmentRingBufferUid);
	SceUID fragmentUsseRingBufferUid;
	uint32_t fragmentUsseRingBufferOffset;
	void* fragmentUsseRingBuffer = fragmentUsseAlloc(SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE, &fragmentUsseRingBufferUid, &fragmentUsseRingBufferOffset);

	SceGxmContextParams contextParams;
	memset(&contextParams, 0, sizeof(SceGxmContextParams));
	contextParams.hostMem						= malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);
	contextParams.hostMemSize					= SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
	contextParams.vdmRingBufferMem				= vdmRingBuffer;
	contextParams.vdmRingBufferMemSize			= SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
	contextParams.vertexRingBufferMem			= vertexRingBuffer;
	contextParams.vertexRingBufferMemSize		= SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
	contextParams.fragmentRingBufferMem			= fragmentRingBuffer;
	contextParams.fragmentRingBufferMemSize		= SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferMem		= fragmentUsseRingBuffer;
	contextParams.fragmentUsseRingBufferMemSize	= SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
	contextParams.fragmentUsseRingBufferOffset	= fragmentUsseRingBufferOffset;
	
	err = sceGxmCreateContext(&contextParams, &g_context);
	SCE_DBG_ASSERT(err == SCE_OK);

	SceGxmRenderTargetParams renderTargetParams;
	memset(&renderTargetParams, 0, sizeof(SceGxmRenderTargetParams));
	renderTargetParams.flags				= 0;
	renderTargetParams.width				= DISPLAY_WIDTH;
	renderTargetParams.height				= DISPLAY_HEIGHT;
	renderTargetParams.scenesPerFrame		= 1;
	renderTargetParams.multisampleMode		= SCE_GXM_MULTISAMPLE_NONE;
	renderTargetParams.multisampleLocations	= 0;
	renderTargetParams.driverMemBlock		= SCE_UID_INVALID_UID;
	
#if MANUALLY_ALLOCATE_RT_MEMBLOCK
	{
		uint32_t driverMemSize;
		sceGxmGetRenderTargetMemSize(&renderTargetParams, &driverMemSize);

		renderTargetParams.driverMemBlock = sceKernelAllocMemBlock("SampleRT", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, driverMemSize, NULL);
	}
#endif

	err = sceGxmCreateRenderTarget(&renderTargetParams, &g_renderTarget);
	SCE_DBG_ASSERT(err == SCE_OK);

	for (unsigned int i = 0; i < DISPLAY_BUFFER_COUNT; ++i) 
	{
		g_displayBufferData[i] = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, ALIGN(4*DISPLAY_STRIDE_IN_PIXELS*DISPLAY_HEIGHT, 1*1024*1024), SCE_GXM_COLOR_SURFACE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE, &g_displayBufferUid[i]);

		for (uint32_t y = 0; y < DISPLAY_HEIGHT; ++y) 
		{
			uint32_t *row = (unsigned int*)g_displayBufferData[i] + y*DISPLAY_STRIDE_IN_PIXELS;
			for (uint32_t x = 0; x < DISPLAY_WIDTH; ++x) 
			{
				row[x] = 0xff000000;
			}
		}

		err = sceGxmColorSurfaceInit(&g_displaySurface[i], DISPLAY_COLOR_FORMAT, SCE_GXM_COLOR_SURFACE_LINEAR, SCE_GXM_COLOR_SURFACE_SCALE_NONE, SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_STRIDE_IN_PIXELS, g_displayBufferData[i]);
		SCE_DBG_ASSERT(err == SCE_OK);

		err = sceGxmSyncObjectCreate(&g_displayBufferSync[i]);
		SCE_DBG_ASSERT(err == SCE_OK);
	}

	const unsigned int alignedWidth = ALIGN(DISPLAY_WIDTH, SCE_GXM_TILE_SIZEX);
	const unsigned int alignedHeight = ALIGN(DISPLAY_HEIGHT, SCE_GXM_TILE_SIZEY);
	unsigned int sampleCount = alignedWidth*alignedHeight;
	unsigned int depthStrideInSamples = alignedWidth;

	g_depthBufferData = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, 4*sampleCount, SCE_GXM_DEPTHSTENCIL_SURFACE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE, &g_depthBufferUid);
	
	err = sceGxmDepthStencilSurfaceInit(&g_depthSurface, SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24, SCE_GXM_DEPTH_STENCIL_SURFACE_TILED, depthStrideInSamples, g_depthBufferData, NULL);

	SCE_DBG_ASSERT(err == SCE_OK);

	const uint32_t patcherBufferSize		= 64*1024;
	const uint32_t patcherVertexUsseSize 	= 64*1024;
	const uint32_t patcherFragmentUsseSize 	= 64*1024;
	
	SceUID patcherBufferUid;
	void* patcherBuffer = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, patcherBufferSize, 4, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE, &patcherBufferUid);
	SceUID patcherVertexUsseUid;
	uint32_t patcherVertexUsseOffset;
	void* patcherVertexUsse = vertexUsseAlloc(patcherVertexUsseSize, &patcherVertexUsseUid, &patcherVertexUsseOffset);
	SceUID patcherFragmentUsseUid;
	uint32_t patcherFragmentUsseOffset;
	void* patcherFragmentUsse = fragmentUsseAlloc(patcherFragmentUsseSize, &patcherFragmentUsseUid, &patcherFragmentUsseOffset);

	SceGxmShaderPatcherParams patcherParams;
	memset(&patcherParams, 0, sizeof(SceGxmShaderPatcherParams));
	patcherParams.userData					= NULL;
	patcherParams.hostAllocCallback			= &patcherHostAlloc;
	patcherParams.hostFreeCallback			= &patcherHostFree;
	patcherParams.bufferAllocCallback		= NULL;
	patcherParams.bufferFreeCallback		= NULL;
	patcherParams.bufferMem					= patcherBuffer;
	patcherParams.bufferMemSize				= patcherBufferSize;
	patcherParams.vertexUsseAllocCallback	= NULL;
	patcherParams.vertexUsseFreeCallback	= NULL;
	patcherParams.vertexUsseMem				= patcherVertexUsse;
	patcherParams.vertexUsseMemSize			= patcherVertexUsseSize;
	patcherParams.vertexUsseOffset			= patcherVertexUsseOffset;
	patcherParams.fragmentUsseAllocCallback	= NULL;
	patcherParams.fragmentUsseFreeCallback	= NULL;
	patcherParams.fragmentUsseMem			= patcherFragmentUsse;
	patcherParams.fragmentUsseMemSize		= patcherFragmentUsseSize;
	patcherParams.fragmentUsseOffset		= patcherFragmentUsseOffset;

	err = sceGxmShaderPatcherCreate(&patcherParams, &g_shaderPatcher);
	SCE_DBG_ASSERT(err == SCE_OK);

	return TRUE;
}

void Emulator_CreateGlobalShaders()
{
	g_gte_shader_4 = Shader_Compile(gte_shader_4, 1);
	g_gte_shader_8 = Shader_Compile(gte_shader_8, 1);
	g_gte_shader_16 = Shader_Compile(gte_shader_16, 1);
	g_blit_shader = Shader_Compile(blit_shader, 0);
}

unsigned char pixelData[64 * 64 * sizeof(unsigned int)];

void Emulator_GenerateCommonTextures()
{
#if 1
	int err = SCE_OK;
	UNUSED(err);

	memset(pixelData, 0xFF, sizeof(pixelData));

	whiteTextureBuff = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, 64 * 64 * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &whiteTexture.Uid);
	memcpy(whiteTextureBuff, &pixelData, 64 * 64 * sizeof(unsigned int));
	sceGxmTextureInitLinear(&whiteTexture.texture, whiteTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8B8G8R8, 1, 1, 0);
	sceGxmTextureSetMinFilter(&whiteTexture.texture, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&whiteTexture.texture, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&whiteTexture.texture, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);

	rg8lutTextureBuff = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &rg8lutTexture.Uid);
	memcpy(rg8lutTextureBuff, Emulator_GenerateRG8LUT(), LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int));
	sceGxmTextureInitLinear(&rg8lutTexture.texture, rg8lutTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8B8G8R8, LUT_WIDTH, LUT_HEIGHT, 0);
	sceGxmTextureSetMinFilter(&rg8lutTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetMagFilter(&rg8lutTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetUAddrMode(&rg8lutTexture.texture, SCE_GXM_TEXTURE_ADDR_CLAMP);
	sceGxmTextureSetVAddrMode(&rg8lutTexture.texture, SCE_GXM_TEXTURE_ADDR_CLAMP);
	sceGxmTextureSetMipFilter(&rg8lutTexture.texture, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
	
	vramTextureBuff = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned short), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &vramTexture.Uid);
	sceGxmTextureInitLinear(&vramTexture.texture, vramTextureBuff, SCE_GXM_TEXTURE_FORMAT_U8U8_GR, VRAM_WIDTH, VRAM_HEIGHT, 0);
	sceGxmTextureSetMinFilter(&vramTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetMagFilter(&vramTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetUAddrMode(&vramTexture.texture, SCE_GXM_TEXTURE_ADDR_REPEAT);
	sceGxmTextureSetVAddrMode(&vramTexture.texture, SCE_GXM_TEXTURE_ADDR_REPEAT);
	sceGxmTextureSetMipFilter(&vramTexture.texture, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
#else
	int err = SCE_OK;
	UNUSED(err);

	memset(pixelData, 0xFF, sizeof(pixelData));

	err = sceGxmTextureInitLinear(&whiteTexture.texture, pixelData, SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR, 64, 64, 0);
	sceGxmTextureSetMinFilter(&whiteTexture.texture, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&whiteTexture.texture, SCE_GXM_TEXTURE_FILTER_LINEAR);
	//sceGxmTextureSetMipFilter(&whiteTexture.texture, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);

	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceGxmTextureInitLinear(&rg8lutTexture.texture, Emulator_GenerateRG8LUT(), SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_ABGR, LUT_WIDTH, LUT_HEIGHT, 0);
	sceGxmTextureSetMinFilter(&rg8lutTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetMagFilter(&rg8lutTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetUAddrMode(&rg8lutTexture.texture, SCE_GXM_TEXTURE_ADDR_CLAMP);
	sceGxmTextureSetVAddrMode(&rg8lutTexture.texture, SCE_GXM_TEXTURE_ADDR_CLAMP);
	//sceGxmTextureSetMipFilter(&rg8lutTexture.texture, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);

	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceGxmTextureInitLinear(&vramTexture.texture, vram, SCE_GXM_TEXTURE_FORMAT_U8U8_GR, VRAM_WIDTH, VRAM_HEIGHT, 0);
	SCE_DBG_ASSERT(err == SCE_OK);
	sceGxmTextureSetMinFilter(&vramTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetMagFilter(&vramTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetUAddrMode(&vramTexture.texture, SCE_GXM_TEXTURE_ADDR_REPEAT);
	sceGxmTextureSetVAddrMode(&vramTexture.texture, SCE_GXM_TEXTURE_ADDR_REPEAT);
	//sceGxmTextureSetMipFilter(&vramTexture.texture, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
#endif
}

void Emulator_CreateVertexBuffer()
{
	dynamic_vertex_buffer = (struct Vertex*)graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, sizeof(struct Vertex) * MAX_NUM_POLY_BUFFER_VERTICES, 4, SCE_GXM_MEMORY_ATTRIB_READ, &dynamic_vertex_buffer_id);
}

void Emulator_CreateIndexBuffer()
{
	dynamic_index_buffer = (unsigned short*)graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, sizeof(unsigned short) * MAX_NUM_INDEX_BUFFER_INDICES, 2, SCE_GXM_MEMORY_ATTRIB_READ, &dynamic_index_buffer_id);
}

int Emulator_CreateCommonResources()
{
	memset(vram, 0, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned short));

	Emulator_GenerateCommonTextures();

	Emulator_CreateGlobalShaders();

	Emulator_CreateVertexBuffer();

	Emulator_CreateIndexBuffer();

	//Emulator_ResetDevice();

	return TRUE;
}

void Emulator_Ortho2D(float left, float right, float bottom, float top, float znear, float zfar)
{
	float a = 2.0f / (right - left);
	float b = 2.0f / (top - bottom);
	float c = 2.0f / (znear - zfar);

	float x = (left + right) / (left - right);
	float y = (bottom + top) / (bottom - top);
	float z = znear / (znear - zfar);

	float ortho[16] = {
		a, 0, 0, x,
		0, b, 0, y,
		0, 0, c, z,
		0, 0, 0, 1
	};
	
	void* vertexDefaultBuffer = NULL;
	sceGxmReserveVertexDefaultUniformBuffer(g_context, &vertexDefaultBuffer);
	sceGxmSetUniformDataF(vertexDefaultBuffer, u_Projection, 0, 16, ortho);
}

void Emulator_SetShader(const ShaderID shader)
{
	if(shader.isGTE)
	{
		u_Projection = sceGxmProgramFindParameterByName(shader.PRG, "Projection");
		SCE_DBG_ASSERT(u_Projection && (sceGxmProgramParameterGetCategory(u_Projection) == SCE_GXM_PARAMETER_CATEGORY_UNIFORM));
	}

	sceGxmSetVertexProgram(g_context, shader.VP);
	sceGxmSetFragmentProgram(g_context, shader.FP[g_CurrentBlendMode]);

	if(shader.isGTE)
	{
		Emulator_Ortho2D(0.0f, activeDispEnv.disp.w, activeDispEnv.disp.h, 0.0f, 0.0f, 1.0f);
	}
}

void Emulator_SetTexture(TextureID texture, enum TexFormat texFormat)
{
	switch (texFormat)
	{
	case TF_4_BIT:
		Emulator_SetShader(g_gte_shader_4);
		break;
	case TF_8_BIT:
		Emulator_SetShader(g_gte_shader_8);
		break;
	case TF_16_BIT:
		Emulator_SetShader(g_gte_shader_16);
		break;
	}

	if (g_texturelessMode) {
		texture = whiteTexture;
	}

	//if (g_lastBoundTexture[0] == texture && g_lastBoundTexture[1] == rg8lutTexture) {
		//return;
	//}

	g_lastBoundTexture[0] = texture;
	g_lastBoundTexture[1] = rg8lutTexture;
}

void Emulator_SetTextureAndShader(TextureID texture, ShaderID shader)
{
	Emulator_SetShader(shader);

	if (g_texturelessMode) {
		texture = whiteTexture;
	}

	//if (g_lastBoundTexture[0] == texture && g_lastBoundTexture[1] == rg8lutTexture) {
		//return;
	//}

	printf("VRAM: %d\n", texture.Uid);
	printf("LUT: %d\n", rg8lutTexture.Uid);
	sceGxmSetFragmentTexture(g_context, 0, &texture.texture);
	sceGxmSetFragmentTexture(g_context, 1, &rg8lutTexture.texture);

	//g_lastBoundTexture[0] = texture;
	//g_lastBoundTexture[1] = rg8lutTexture;
}

void Emulator_DestroyTexture(TextureID texture)
{
	
}

void Emulator_Clear(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b)
{
	
}

#define NOFILE 0

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__)

void Emulator_SaveVRAM(const char* outputFileName, int x, int y, int width, int height, int bReadFromFrameBuffer)
{
#if NOFILE
	return;
#endif
	FILE* f = fopen(outputFileName, "wb");
	
	if (f == NULL)
	{
		return;
	}

	unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
	unsigned char header[6];
	header[0] = (width % 256);
	header[1] = (width / 256);
	header[2] = (height % 256);
	header[3] = (height / 256);
	header[4] = 16;
	header[5] = 0;

	fwrite(TGAheader, sizeof(unsigned char), 12, f);
	fwrite(header, sizeof(unsigned char), 6, f);

	//512 const is hdd sector size
	int numSectorsToWrite = (width * height * sizeof(unsigned short)) / 512;
	int numRemainingSectorsToWrite = (width * height * sizeof(unsigned short)) % 512;

	for (int i = 0; i < numSectorsToWrite; i++)
	{
		fwrite(&vram[i * 512 / sizeof(unsigned short)], 512, 1, f);
	}

	for (int i = 0; i < numRemainingSectorsToWrite; i++)
	{
		fwrite(&vram[numSectorsToWrite * 512 / sizeof(unsigned short)], numRemainingSectorsToWrite, 1, f);
	}

	fclose(f);
}
#endif

void Emulator_StoreFrameBuffer(int x, int y, int w, int h)
{
	short* fb = (short*)malloc(w * h * sizeof(short));

	int* data = (int*)malloc(w * h * sizeof(int));
	//glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

#define FLIP_Y (h - fy - 1)
#define SWAP_RB

	unsigned int* data_src = (unsigned int*)data;
	unsigned short* data_dst = (unsigned short*)fb;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			unsigned int  c = *data_src++;
			unsigned char b = ((c >> 3) & 0x1F);
			unsigned char g = ((c >> 11) & 0x1F);
			unsigned char r = ((c >> 19) & 0x1F);
#if defined(SWAP_RB)
			*data_dst++ = b | (g << 5) | (r << 10) | 0x8000;
#else
			* data_dst++ = r | (g << 5) | (b << 10) | 0x8000;
#endif
		}
	}

	short* ptr = (short*)vram + VRAM_WIDTH * y + x;

	for (int fy = 0; fy < h; fy++) {
		short* fb_ptr = fb + (h * FLIP_Y / h) * w;

		for (int fx = 0; fx < w; fx++) {
			ptr[fx] = fb_ptr[w * fx / w];
		}

		ptr += VRAM_WIDTH;
	}

	free(data);

#undef FLIP_Y

	free(fb);

	vram_need_update = TRUE;
}

void Emulator_UpdateVRAM()
{
	if (!vram_need_update) {
		return;
	}
	vram_need_update = FALSE;

	if(vramTexture.Uid != 0)
	{
		graphicsFree(vramTexture.Uid);
		vramTexture.Uid = 0;
	}
#if 1
	vramTextureBuff = graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned short), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &vramTexture.Uid);
	memcpy(vramTextureBuff, vram, VRAM_WIDTH * VRAM_HEIGHT* sizeof(unsigned short));
	sceGxmTextureInitLinear(&vramTexture.texture, vramTextureBuff, SCE_GXM_TEXTURE_FORMAT_U8U8_GR, VRAM_WIDTH, VRAM_HEIGHT, 0);
	sceGxmTextureSetMinFilter(&vramTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetMagFilter(&vramTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetUAddrMode(&vramTexture.texture, SCE_GXM_TEXTURE_ADDR_REPEAT);
	sceGxmTextureSetVAddrMode(&vramTexture.texture, SCE_GXM_TEXTURE_ADDR_REPEAT);
	sceGxmTextureSetMipFilter(&vramTexture.texture, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
#else
	int err = sceGxmTextureInitLinear(&vramTexture.texture, vram, SCE_GXM_TEXTURE_FORMAT_U8U8_GR, VRAM_WIDTH, VRAM_HEIGHT, 0);
	SCE_DBG_ASSERT(err == SCE_OK);
	sceGxmTextureSetMinFilter(&vramTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetMagFilter(&vramTexture.texture, SCE_GXM_TEXTURE_FILTER_POINT);
	sceGxmTextureSetUAddrMode(&vramTexture.texture, SCE_GXM_TEXTURE_ADDR_REPEAT);
	sceGxmTextureSetVAddrMode(&vramTexture.texture, SCE_GXM_TEXTURE_ADDR_REPEAT);
	sceGxmTextureSetMipFilter(&vramTexture.texture, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
#endif
}

void Emulator_SetWireframe(int enable)
{
}

void Emulator_SetBlendMode(enum BlendMode blendMode)
{
	if (g_PreviousBlendMode == blendMode)
	{
		return;
	}

	g_CurrentBlendMode = blendMode;
	g_PreviousBlendMode = blendMode;
}

void Emulator_DrawTriangles(int start_vertex, int triangles)
{
	if (triangles <= 0)
		return;

	printf("Drawing: %d Start: %d\n", triangles, start_vertex);
	sceGxmSetVertexStream(g_context, 0, dynamic_vertex_buffer + start_vertex);
	sceGxmDraw(g_context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16, dynamic_index_buffer, triangles * 3);
	sceGxmMidSceneFlush(g_context, SCE_GXM_MIDSCENE_PRESERVE_DEFAULT_UNIFORM_BUFFERS, NULL, NULL);
}

void Emulator_UpdateVertexBuffer(const struct Vertex* vertices, int num_vertices)
{
	eassert(num_vertices <= MAX_NUM_POLY_BUFFER_VERTICES);

	if (num_vertices <= 0)
		return;

	memcpy(dynamic_vertex_buffer, vertices, num_vertices * sizeof(struct Vertex));

	vbo_was_dirty_flag = TRUE;
}

void Emulator_UpdateIndexBuffer(const unsigned short* indices, int num_indices)
{
	eassert(num_indices <= MAX_NUM_INDEX_BUFFER_INDICES);

	if (num_indices <= 0)
		return;

	memcpy(dynamic_index_buffer, indices, num_indices * sizeof(unsigned short));
}

void Emulator_SetViewPort(int x, int y, int width, int height)
{
	int offset_x = activeDispEnv.screen.x;
	int offset_y = activeDispEnv.screen.y;

	int vh = DISPLAY_HEIGHT;
	int sw = width / 2;
    int sh = height / 2;

    sceGxmSetViewport(g_context, (float)((x + offset_x) + sw), (float)sw, (float)(vh - (y + -offset_y) - sh), (float)(-sh), 0.0f, 1.0f);
}

void Emulator_SwapWindow()
{
	unsigned int timer = 1;

#if defined(SINGLE_THREADED_AUDIO)
	Emulator_CounterWrapper(0, &timer);
#endif

	Emulator_WaitForTimestep(1);

	DisplayData displayData;
	displayData.address = g_displayBufferData[g_backBufferIndex];
	sceGxmDisplayQueueAddEntry(g_displayBufferSync[g_frontBufferIndex],	g_displayBufferSync[g_backBufferIndex], &displayData);

	g_frontBufferIndex = g_backBufferIndex;
	g_backBufferIndex = (g_backBufferIndex + 1) % DISPLAY_BUFFER_COUNT;
}

void Emulator_WaitForTimestep(int count)
{
#if 0
	if (g_swapInterval > 0)
	{
		int delta = g_swapTime + FIXED_TIME_STEP * count - SDL_GetTicks();

		if (delta > 0) {
			SDL_Delay(delta);
		}
	}

	g_swapTime = SDL_GetTicks();
#endif
}

void Emulator_SetRenderTarget(const RenderTargetID frameBufferObject)
{
}

#endif