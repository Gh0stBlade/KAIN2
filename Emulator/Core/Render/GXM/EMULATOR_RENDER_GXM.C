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

SceGxmContext* g_context = NULL;
SceGxmShaderPatcher* g_shaderPatcher;
SceUID g_patcherBufUid;
SceUID g_patcherCombinedUsseUid;

#define DISPLAY_WIDTH				960
#define DISPLAY_HEIGHT				544
#define DISPLAY_STRIDE_IN_PIXELS	1024

#define DISPLAY_COLOR_FORMAT		SCE_GXM_COLOR_FORMAT_A8B8G8R8
#define DISPLAY_PIXEL_FORMAT		SCE_DISPLAY_PIXELFORMAT_A8B8G8R8

#define DISPLAY_BUFFER_COUNT		3

#define DISPLAY_MAX_PENDING_SWAPS	2

#define ALIGN(x, a)					(((x) + ((a) - 1)) & ~((a) - 1))

#define SAMPLE_NAME SHORT_GAME_NAME

extern void Emulator_DoPollEvent();
extern void Emulator_WaitForTimestep(int count);
extern void Emulator_GenerateCommonTextures();
extern void Emulator_CreateGlobalShaders();
extern void Emulator_DestroyTextures();
extern void Emulator_DestroyGlobalShaders();
extern void Emulator_CreateVertexBuffer();

const char* renderBackendName = "GXM";

unsigned int dynamic_vertex_buffer;
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

SceGxmTexture vramTextureCtl;
SceGxmTexture whiteTextureCtl;
SceGxmTexture rg8lutTextureCtl;

#define	UNUSED(a)					(void)(a)

const SceGxmProgramParameter* u_Projection;

void* Emulator_GraphicsAlloc(SceKernelMemBlockType type, uint32_t size, uint32_t alignment, uint32_t attribs, SceUID* uid)
{
	int err = SCE_OK;
	UNUSED(err);

	if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA) 
	{
		SCE_DBG_ASSERT(alignment <= 256*1024);
		size = ALIGN(size, 256*1024);
	} 
	else 
	{
		SCE_DBG_ASSERT(alignment <= 4*1024);
		size = ALIGN(size, 4*1024);
	}

	UNUSED(alignment);

	*uid = sceKernelAllocMemBlock("basic", type, size, NULL);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	void *mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceGxmMapMemory(mem, size, attribs);
	SCE_DBG_ASSERT(err == SCE_OK);
	
	return mem;
}

void Emulator_GraphicsFree(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	void *mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceGxmUnmapMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

void* Emulator_Vertex_USSE_Alloc(uint32_t size, SceUID* uid, uint32_t* usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	size = ALIGN(size, 4096);
	
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	void* mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceGxmMapVertexUsseMemory(mem, size, usseOffset);
	SCE_DBG_ASSERT(err == SCE_OK);

	return mem;
}

void Emulator_Vertex_USSE_Free(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	void* mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceGxmUnmapVertexUsseMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

void* Emulator_Fragment_USSE_Alloc(uint32_t size, SceUID *uid, uint32_t* usseOffset)
{
	int err = SCE_OK;
	UNUSED(err);

	size = ALIGN(size, 4096);
	
	*uid = sceKernelAllocMemBlock("basic", SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);
	SCE_DBG_ASSERT(*uid >= SCE_OK);

	void* mem = NULL;
	err = sceKernelGetMemBlockBase(*uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceGxmMapFragmentUsseMemory(mem, size, usseOffset);
	SCE_DBG_ASSERT(err == SCE_OK);

	return mem;
}

void Emulator_Fragment_USSE_Free(SceUID uid)
{
	int err = SCE_OK;
	UNUSED(err);

	void* mem = NULL;
	err = sceKernelGetMemBlockBase(uid, &mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceGxmUnmapFragmentUsseMemory(mem);
	SCE_DBG_ASSERT(err == SCE_OK);

	err = sceKernelFreeMemBlock(uid);
	SCE_DBG_ASSERT(err == SCE_OK);
}

void* Emulator_Patcher_HostAlloc(void* userData, uint32_t size)
{
	UNUSED(userData);
	return malloc(size);
}

void Emulator_Patcher_HostFree(void* userData, void* mem)
{
	UNUSED(userData);
	free(mem);
}

ShaderID Shader_Compile_Internal(const SceGxmProgram* source_vs, const SceGxmProgram* source_fs, int gte_shader)
{
	ShaderID shader;
	int err = SCE_OK;

#define OFFSETOF(T, E)     ((size_t)&(((T*)0)->E))

	if(gte_shader)
	{
		err = sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_vs, &shader.VSID);
		SCE_DBG_ASSERT(err == SCE_OK);
		err =sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_fs, &shader.FSID);
		SCE_DBG_ASSERT(err == SCE_OK);

		const SceGxmProgram* vProgram = sceGxmShaderPatcherGetProgramFromId(shader.VSID);
		SCE_DBG_ASSERT(vProgram);
	
		const SceGxmProgramParameter* paramPositionAttribute = sceGxmProgramFindParameterByName(vProgram, "In.a_position");
		SCE_DBG_ASSERT(paramPositionAttribute && (sceGxmProgramParameterGetCategory(paramPositionAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	
		const SceGxmProgramParameter* paramTexcoordAttribute = sceGxmProgramFindParameterByName(vProgram, "In.a_texcoord");
		SCE_DBG_ASSERT(paramTexcoordAttribute && (sceGxmProgramParameterGetCategory(paramTexcoordAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	
		const SceGxmProgramParameter* paramColorAttribute = sceGxmProgramFindParameterByName(vProgram, "In.a_color");
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
		basicVertexAttributes[1].componentCount = 2;
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

		err = sceGxmShaderPatcherCreateFragmentProgram(g_shaderPatcher, shader.FSID, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, SCE_GXM_MULTISAMPLE_NONE, NULL, sceGxmShaderPatcherGetProgramFromId(shader.VSID), &shader.FP);
		SCE_DBG_ASSERT(err == SCE_OK);
	}
	else
	{
		err = sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_vs, &shader.VSID);
		SCE_DBG_ASSERT(err == SCE_OK);
		err =sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, source_fs, &shader.FSID);
		SCE_DBG_ASSERT(err == SCE_OK);

		const SceGxmProgram* vProgram = sceGxmShaderPatcherGetProgramFromId(shader.VSID);
		SCE_DBG_ASSERT(vProgram);
	
		const SceGxmProgramParameter* paramPositionAttribute = sceGxmProgramFindParameterByName(vProgram, "In.a_position");
		SCE_DBG_ASSERT(paramPositionAttribute && (sceGxmProgramParameterGetCategory(paramPositionAttribute) == SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
	
		const SceGxmProgramParameter* paramTexcoordAttribute = sceGxmProgramFindParameterByName(vProgram, "In.a_texcoord");
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
		basicVertexAttributes[1].componentCount = 2;
		basicVertexAttributes[1].regIndex = sceGxmProgramParameterGetResourceIndex(paramTexcoordAttribute);
	
		basicVertexStreams[0].stride = sizeof(struct Vertex);
		basicVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;
	
		err = sceGxmShaderPatcherCreateVertexProgram(g_shaderPatcher, shader.VSID, basicVertexAttributes, 2, basicVertexStreams, 1, &shader.VP);
		SCE_DBG_ASSERT(err == SCE_OK);

		err = sceGxmShaderPatcherCreateFragmentProgram(g_shaderPatcher, shader.FSID, SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, SCE_GXM_MULTISAMPLE_NONE, NULL, sceGxmShaderPatcherGetProgramFromId(shader.VSID), &shader.FP);
		SCE_DBG_ASSERT(err == SCE_OK);
	}

	return shader;
}

void Emulator_DestroyVertexBuffer()
{
	dynamic_vertex_buffer = 0;
	dynamic_vertex_array = 0;
}

void Emulator_ResetDevice()
{
	if (!g_resettingDevice)
	{
		g_resettingDevice = TRUE;

		Emulator_DestroyVertexBuffer();

		Emulator_DestroyTextures();

		Emulator_DestroyGlobalShaders();

		Emulator_CreateGlobalShaders();

		Emulator_GenerateCommonTextures();

		Emulator_CreateVertexBuffer();

		g_resettingDevice = FALSE;
	}
}

void Emulator_DestroyTextures()
{

	vramTexture = 0;
	rg8lutTexture = 0;
	whiteTexture = 0;
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

int Emulator_InitialiseGXMContext(char* windowName)
{
	int err = SCE_OK;
	SceGxmInitializeParams initParam;
	memset(&initParam, 0, sizeof(SceGxmInitializeParams));
	initParam.flags = 0;
	initParam.displayQueueMaxPendingCount = DISPLAY_MAX_PENDING_SWAPS;
	initParam.displayQueueCallback = Emulator_DisplayCallback;
	initParam.displayQueueCallbackDataSize = sizeof(DisplayData);
	initParam.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;
	err = sceGxmInitialize(&initParam);
	SCE_DBG_ASSERT(err == SCE_OK);

	SceUID vdmRingBufferUid;
	void* vdmRingBuffer = Emulator_GraphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &vdmRingBufferUid);
	
	SceUID vertexRingBufferUid;
	void* vertexRingBuffer = Emulator_GraphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &vertexRingBufferUid);
	
	SceUID fragmentRingBufferUid;
	void* fragmentRingBuffer = Emulator_GraphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &fragmentRingBufferUid);
	SceUID fragmentUsseRingBufferUid;
	uint32_t fragmentUsseRingBufferOffset;
	void* fragmentUsseRingBuffer = Emulator_Fragment_USSE_Alloc(SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE, &fragmentUsseRingBufferUid, &fragmentUsseRingBufferOffset);

	SceGxmContextParams ctxParam;
	memset(&ctxParam, 0, sizeof(SceGxmContextParams));
	ctxParam.hostMem						= malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);
	ctxParam.hostMemSize					= SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
	ctxParam.vdmRingBufferMem				= vdmRingBuffer;
	ctxParam.vdmRingBufferMemSize			= SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
	ctxParam.vertexRingBufferMem			= vertexRingBuffer;
	ctxParam.vertexRingBufferMemSize		= SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
	ctxParam.fragmentRingBufferMem			= fragmentRingBuffer;
	ctxParam.fragmentRingBufferMemSize		= SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
	ctxParam.fragmentUsseRingBufferMem		= fragmentUsseRingBuffer;
	ctxParam.fragmentUsseRingBufferMemSize	= SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
	ctxParam.fragmentUsseRingBufferOffset	= fragmentUsseRingBufferOffset;
	
	SceGxmContext* context = NULL;
	err = sceGxmCreateContext(&ctxParam, &g_context);
	SCE_DBG_ASSERT(err == SCE_OK);

	SceGxmRenderTargetParams rtParam;
	memset(&rtParam, 0, sizeof(SceGxmRenderTargetParams));
	rtParam.flags				= 0;
	rtParam.width				= DISPLAY_WIDTH;
	rtParam.height				= DISPLAY_HEIGHT;
	rtParam.scenesPerFrame		= 1;
	rtParam.multisampleMode		= SCE_GXM_MULTISAMPLE_NONE;
	rtParam.multisampleLocations	= 0;
	rtParam.driverMemBlock		= SCE_UID_INVALID_UID;

	SceGxmRenderTarget *renderTarget;
	err = sceGxmCreateRenderTarget(&rtParam, &renderTarget);
	SCE_DBG_ASSERT(err == SCE_OK);

	void* displayBufferData[DISPLAY_BUFFER_COUNT];
	SceUID displayBufferUid[DISPLAY_BUFFER_COUNT];
	SceGxmColorSurface displaySurface[DISPLAY_BUFFER_COUNT];
	SceGxmSyncObject* displayBufferSync[DISPLAY_BUFFER_COUNT];

	for (uint32_t i = 0; i < DISPLAY_BUFFER_COUNT; i++) 
	{
		displayBufferData[i] = Emulator_GraphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, ALIGN(4*DISPLAY_STRIDE_IN_PIXELS*DISPLAY_HEIGHT, 1*1024*1024), SCE_GXM_COLOR_SURFACE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE, &displayBufferUid[i]);

		for (uint32_t y = 0; y < DISPLAY_HEIGHT; ++y) 
		{
			uint32_t* row = (uint32_t *)displayBufferData[i] + y*DISPLAY_STRIDE_IN_PIXELS;
			for (uint32_t x = 0; x < DISPLAY_WIDTH; ++x) 
			{
				row[x] = 0xff000000;
			}
		}

		err = sceGxmColorSurfaceInit(&displaySurface[i], DISPLAY_COLOR_FORMAT, SCE_GXM_COLOR_SURFACE_LINEAR, SCE_GXM_COLOR_SURFACE_SCALE_NONE, SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_STRIDE_IN_PIXELS, displayBufferData[i]);
		SCE_DBG_ASSERT(err == SCE_OK);

		err = sceGxmSyncObjectCreate(&displayBufferSync[i]);
		SCE_DBG_ASSERT(err == SCE_OK);
	}

	const uint32_t patcherBufferSize		= 64*1024;
	const uint32_t patcherVertexUsseSize 	= 64*1024;
	const uint32_t patcherFragmentUsseSize 	= 64*1024;
	
	SceUID patcherBufferUid;
	void* patcherBuffer = Emulator_GraphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, patcherBufferSize, 4, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE, &patcherBufferUid);
	SceUID patcherVertexUsseUid;
	uint32_t patcherVertexUsseOffset;
	void* patcherVertexUsse = Emulator_Vertex_USSE_Alloc(patcherVertexUsseSize, &patcherVertexUsseUid, &patcherVertexUsseOffset);
	SceUID patcherFragmentUsseUid;
	uint32_t patcherFragmentUsseOffset;
	void* patcherFragmentUsse = Emulator_Fragment_USSE_Alloc(patcherFragmentUsseSize, &patcherFragmentUsseUid, &patcherFragmentUsseOffset);

	SceGxmShaderPatcherParams patcherParams;
	memset(&patcherParams, 0, sizeof(SceGxmShaderPatcherParams));
	patcherParams.userData					= NULL;
	patcherParams.hostAllocCallback			= &Emulator_Patcher_HostAlloc;
	patcherParams.hostFreeCallback			= &Emulator_Patcher_HostFree;
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

	const SceGxmProgram* gte_4_program = sceGxmShaderPatcherGetProgramFromId(g_gte_shader_4.VSID);
	u_Projection = sceGxmProgramFindParameterByName(gte_4_program, "Projection");
}

void Emulator_GenerateCommonTextures()
{
#if 0
	unsigned int pixelData = 0xFFFFFFFF;

	whiteTextureBuff = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, 1 * 1 * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &whiteTexture);
	memcpy(whiteTextureBuff, &pixelData, 1 * 1 * sizeof(unsigned int));
	sceGxmTextureInitLinear(&whiteTextureCtl, whiteTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8R8G8B8, 1, 1, 1);
	sceGxmTextureSetMinFilter(&whiteTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&whiteTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&whiteTextureCtl, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);

	rg8lutTextureBuff = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &rg8lutTexture);
	memcpy(rg8lutTextureBuff, Emulator_GenerateRG8LUT(), LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int));
	sceGxmTextureInitLinear(&rg8lutTextureCtl, rg8lutTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8R8G8B8, LUT_WIDTH, LUT_HEIGHT, 1);
	sceGxmTextureSetMinFilter(&rg8lutTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&rg8lutTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&rg8lutTextureCtl, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
	
	vramTextureBuff = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, &vramTexture);
	sceGxmTextureInitLinear(&vramTextureCtl, vramTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8L8, LUT_WIDTH, LUT_HEIGHT, 1);
	sceGxmTextureSetMinFilter(&vramTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&vramTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&vramTextureCtl, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
#endif
}

void Emulator_CreateVertexBuffer()///@TODO OGLES
{
	
}

int Emulator_CreateCommonResources()
{
	memset(vram, 0, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned short));

	Emulator_GenerateCommonTextures();

	Emulator_CreateGlobalShaders();

	///glDisable(GL_DEPTH_TEST);
	//glBlendColor(0.5f, 0.5f, 0.5f, 0.25f);

	Emulator_CreateVertexBuffer();

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
	float z = (znear + zfar) / (znear - zfar);

	float ortho[16] = {
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		x, y, z, 1
	};
	
		void* vertexDefaultBuffer = NULL;
		sceGxmReserveVertexDefaultUniformBuffer(g_context, &vertexDefaultBuffer);
		sceGxmSetUniformDataF(vertexDefaultBuffer, u_Projection, 0, 16*4, ortho);
}

void Emulator_SetShader(const ShaderID shader)
{
	sceGxmSetVertexProgram(g_context, shader.VP);
	sceGxmSetFragmentProgram(g_context, shader.FP);
	
	Emulator_Ortho2D(0.0f, activeDispEnv.disp.w, activeDispEnv.disp.h, 0.0f, 0.0f, 1.0f);
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

	if (g_lastBoundTexture[0] == texture && g_lastBoundTexture[1] == rg8lutTexture) {
		//return;
	}

	g_lastBoundTexture[0] = texture;
	g_lastBoundTexture[1] = rg8lutTexture;
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

	if (g_PreviousBlendMode == BM_NONE)
	{
		
	}

	g_PreviousBlendMode = blendMode;
}
void Emulator_DrawTriangles(int start_vertex, int triangles)
{
	if (triangles <= 0)
		return;
}

void Emulator_UpdateVertexBuffer(const struct Vertex* vertices, int num_vertices)
{
	eassert(num_vertices <= MAX_NUM_POLY_BUFFER_VERTICES);

	if (num_vertices <= 0)
		return;

	vbo_was_dirty_flag = TRUE;
}

void Emulator_SetViewPort(int x, int y, int width, int height)
{
	float offset_x = (float)activeDispEnv.screen.x;
	float offset_y = (float)activeDispEnv.screen.y;

}

void Emulator_SwapWindow()
{
	unsigned int timer = 1;

#if defined(SINGLE_THREADED_AUDIO)
	Emulator_CounterWrapper(0, &timer);
#endif

	Emulator_WaitForTimestep(1);
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