#include "LIBGPU.H"
#include "EMULATOR_RENDER_GXM.H"
#include "Core/Debug/EMULATOR_LOG.H"
#include "Core/Render/EMULATOR_RENDER_COMMON.H"
#include <stdio.h>

#if defined(GXM)

#include <stdio.h>
#include <kernel.h>
#include <kernel/threadmgr.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <scetypes.h>
#include <sceconst.h>
#include <kernel.h>
#include <display.h>
#include <ctrl.h>

SceGxmContext* g_context = NULL;
SceGxmShaderPatcher* g_shaderPatcher;
SceUID g_patcherBufUid;
SceUID g_patcherCombinedUsseUid;

#define	DISPLAY_WIDTH			960
#define	DISPLAY_HEIGHT			544
#define	DISPLAY_STRIDE			1024

#define	DISPLAY_BUFFER_COUNT	3
#define	DISPLAY_PENDING_SWAPS	2
#define	DISPLAY_BUFFER_SIZE		((4 * DISPLAY_STRIDE * DISPLAY_HEIGHT + 0xfffffU) & ~0xfffffU)
#define	DISPLAY_ALIGN_WIDTH		((DISPLAY_WIDTH  + SCE_GXM_TILE_SIZEX - 1) & ~(SCE_GXM_TILE_SIZEX - 1))
#define	DISPLAY_ALIGN_HEIGHT	((DISPLAY_HEIGHT + SCE_GXM_TILE_SIZEY - 1) & ~(SCE_GXM_TILE_SIZEY - 1))

#define	PATCHER_BUFFER_SIZE			(64*1024)
#define	PATCHER_COMBINED_USSE_SIZE	(64*1024)
#define SAMPLE_NAME SHORT_GAME_NAME

extern void Emulator_DoPollEvent();
extern void Emulator_WaitForTimestep(int count);
extern void Emulator_GenerateCommonTextures();
extern void Emulator_CreateGlobalShaders();
extern void Emulator_DestroyTextures();
extern void Emulator_DestroyGlobalShaders();
extern void Emulator_CreateVertexBuffer();

const char* renderBackendName = "OpenGL 3.3";

unsigned int dynamic_vertex_buffer;
unsigned int dynamic_vertex_array;

void* g_contextHost;

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

unsigned int u_Projection;

void* Emulator_GAlloc(SceKernelMemBlockType type, SceUInt32 size, SceUInt32 alignment, SceUInt32 attribs, SceInt32* uid)
{
	void* mem = NULL;
	int res;

	if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA)
	{
		if (alignment > 0x40000)
		{
			return NULL;
		}

		size = (size + 0x3FFFFU) & ~0x3FFFFU;
	}
	else 
	{
		if (alignment > 0x1000)
		{
			return NULL;
		}

		size = (size + 0xFFFU) & ~0xFFFU;
	}

	res = sceKernelAllocMemBlock(SAMPLE_NAME, type, size, NULL);
	if (res < SCE_OK)
	{
		return NULL;
	}

	*uid = res;

	res = sceKernelGetMemBlockBase(*uid, &mem);
	if (res != SCE_OK)
	{
		return NULL;
	}

	res = sceGxmMapMemory(mem, size, attribs);
	if (res != SCE_OK)
	{
		return NULL;
	}

	return mem;
}

void* Emulator_USSE_Alloc(uint32_t size, SceUID* uid, uint32_t* usseOffset)
{
	void* mem = NULL;
	int	res;

	size = (size + 0xFFFU) & ~0xFFFU;

	res = sceKernelAllocMemBlock(SAMPLE_NAME, SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);

	if (res < SCE_OK)
	{
		return NULL;
	}

	*uid = res;

	res = sceKernelGetMemBlockBase(*uid, &mem);

	if (res != SCE_OK)
	{	
		return NULL;
	}

	res = sceGxmMapFragmentUsseMemory(mem, size, usseOffset);
	if (res != SCE_OK)
	{
		return NULL;
	}

	return mem;
}


void* Emulator_Combined_USSE_Alloc(uint32_t size, SceUID* uid, uint32_t* vertexUsseOffset, uint32_t* fragmentUsseOffset)
{
	void* mem = NULL;
	int	res;

	size = (size + 0xFFFU) & ~0xFFFU;

	res = sceKernelAllocMemBlock(SAMPLE_NAME, SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, size, NULL);
	if (res < SCE_OK)
	{
		return NULL;
	}

	*uid = res;

	res = sceKernelGetMemBlockBase(*uid, &mem);
	if (res != SCE_OK)
	{
		return NULL;
	}

	res = sceGxmMapVertexUsseMemory(mem, size, vertexUsseOffset);
	if (res != SCE_OK)
	{	
		return NULL;
	}
	res = sceGxmMapFragmentUsseMemory(mem, size, fragmentUsseOffset);
	if (res != SCE_OK)
	{
		return NULL;
	}

	return mem;
}

void* Emulator_PatcherHostAlloc(void* data, uint32_t size)
{
	return malloc(size);
}

void Emulator_PatcherHostFree(void* data, void* mem)
{
	free(mem);
}

ShaderID Shader_Compile_Internal(const SceGxmProgram source_vs, const SceGxmProgram source_fs)
{
	ShaderID shader;
 
	sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, &source_vs, &shader.VSID);
	sceGxmShaderPatcherRegisterProgram(g_shaderPatcher, &source_fs, &shader.FSID);

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
	SceGxmInitializeParams initParam;
	memset(&initParam, 0, sizeof(SceGxmInitializeParams));
	initParam.flags = 0;
	initParam.displayQueueMaxPendingCount = DISPLAY_PENDING_SWAPS;
	initParam.displayQueueCallback = Emulator_DisplayCallback;
	initParam.displayQueueCallbackDataSize = sizeof(Display);
	initParam.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;
	sceGxmInitialize(&initParam);

	g_contextHost = malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);

	void* ringbufVdm = NULL;
	void* ringbufVertex = NULL;
	void* ringbufFragment = NULL;
	void* ringbufFragmentUsse = NULL;

	ringbufVdm = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &g_vdmRingBufUid);
	ringbufVertex = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &g_vertexRingBufUid);
	ringbufFragment = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ, &g_fragmentRingBufUid);
	ringbufFragmentUsse = Emulator_USSE_Alloc(SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE, &g_fragmentUsseRingBufUid, &ringbufFragmentUsseOffset);

	SceGxmContextParams ctxParam;
	memset(&ctxParam, 0, sizeof(SceGxmContextParams));
	ctxParam.hostMem                       = g_contextHost;
	ctxParam.hostMemSize                   = SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
	ctxParam.vdmRingBufferMem              = ringbufVdm;
	ctxParam.vdmRingBufferMemSize          = SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
	ctxParam.vertexRingBufferMem           = ringbufVertex;
	ctxParam.vertexRingBufferMemSize       = SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
	ctxParam.fragmentRingBufferMem         = ringbufFragment;
	ctxParam.fragmentRingBufferMemSize     = SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
	ctxParam.fragmentUsseRingBufferMem     = ringbufFragmentUsse;
	ctxParam.fragmentUsseRingBufferMemSize = SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
	ctxParam.fragmentUsseRingBufferOffset  = ringbufFragmentUsseOffset;

	sceGxmCreateContext(&ctxParam, &g_context);

		void* patcherBuf = NULL;
	patcherBuf = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, PATCHER_BUFFER_SIZE, 4, SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE, &g_patcherBufUid);

	void* patcherCombinedUsse = NULL;
	SceUInt32 patcherVertexUsseOffset;
	SceUInt32 patcherFragmentUsseOffset;
	patcherCombinedUsse = Emulator_Combined_USSE_Alloc(PATCHER_COMBINED_USSE_SIZE, &g_patcherCombinedUsseUid, &patcherVertexUsseOffset, &patcherFragmentUsseOffset);

	SceGxmShaderPatcherParams ptchParam;
	memset(&ptchParam, 0, sizeof(SceGxmShaderPatcherParams));
	ptchParam.userData                  = NULL;
	ptchParam.hostAllocCallback         = &Emulator_PatcherHostAlloc;
	ptchParam.hostFreeCallback          = &Emulator_PatcherHostFree;
	ptchParam.bufferAllocCallback       = NULL;
	ptchParam.bufferFreeCallback        = NULL;
	ptchParam.bufferMem                 = patcherBuf;
	ptchParam.bufferMemSize             = PATCHER_BUFFER_SIZE;
	ptchParam.vertexUsseAllocCallback   = NULL;
	ptchParam.vertexUsseFreeCallback    = NULL;
	ptchParam.vertexUsseMem             = patcherCombinedUsse;
	ptchParam.vertexUsseMemSize         = PATCHER_COMBINED_USSE_SIZE;
	ptchParam.vertexUsseOffset          = patcherVertexUsseOffset;
	ptchParam.fragmentUsseAllocCallback = NULL;
	ptchParam.fragmentUsseFreeCallback  = NULL;
	ptchParam.fragmentUsseMem           = patcherCombinedUsse;
	ptchParam.fragmentUsseMemSize       = PATCHER_COMBINED_USSE_SIZE;
	ptchParam.fragmentUsseOffset        = patcherFragmentUsseOffset;

	sceGxmShaderPatcherCreate(&ptchParam, &g_shaderPatcher);

	return TRUE;
}

void Emulator_CreateGlobalShaders()
{
	g_gte_shader_4 = Shader_Compile(gte_shader_4);
	g_gte_shader_8 = Shader_Compile(gte_shader_8);
	g_gte_shader_16 = Shader_Compile(gte_shader_16);
	g_blit_shader = Shader_Compile(blit_shader);

	//u_Projection = glGetUniformLocation(g_gte_shader_4, "Projection");
}

void Emulator_GenerateCommonTextures()
{
	unsigned int pixelData = 0xFFFFFFFF;

	whiteTextureBuff = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, 1 * 1 * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, whiteTexture);
	memcpy(whiteTextureBuff, &pixelData, 1 * 1 * sizeof(unsigned int));
	sceGxmTextureInitLinear(&whiteTextureCtl, whiteTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8R8G8B8, 1, 1, 1);
	sceGxmTextureSetMinFilter(&whiteTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&whiteTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&whiteTextureCtl, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);

	rg8lutTextureBuff = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, rg8lutTexture);
	memcpy(rg8lutTextureBuff, Emulator_GenerateRG8LUT(), LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int));
	sceGxmTextureInitLinear(&rg8lutTextureCtl, rg8lutTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8R8G8B8, LUT_WIDTH, LUT_HEIGHT, 1);
	sceGxmTextureSetMinFilter(&rg8lutTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&rg8lutTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&rg8lutTextureCtl, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
	
	vramTextureBuff = Emulator_GAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA, VRAM_WIDTH * VRAM_HEIGHT * sizeof(unsigned int), SCE_GXM_TEXTURE_ALIGNMENT, SCE_GXM_MEMORY_ATTRIB_READ, rg8lutTexture);
	sceGxmTextureInitLinear(&vramTextureCtl, vramTextureBuff, SCE_GXM_TEXTURE_FORMAT_A8L8, LUT_WIDTH, LUT_HEIGHT, 1);
	sceGxmTextureSetMinFilter(&vramTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMagFilter(&vramTextureCtl, SCE_GXM_TEXTURE_FILTER_LINEAR);
	sceGxmTextureSetMipFilter(&vramTextureCtl, SCE_GXM_TEXTURE_MIP_FILTER_DISABLED);
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

	Emulator_ResetDevice();

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
	
	///@TODO this generates an error sometimes
	//eprinterr("GL Error: %x\n", glGetError());

	//glUniformMatrix4fv(u_Projection, 1, GL_FALSE, ortho);

	//eprinterr("GL Error: %x\n", glGetError());
}

void Emulator_SetShader(const ShaderID shader)
{
	//glUseProgram(shader);
	
	//eprinterr("GL_Error: %x\n", glGetError());
	Emulator_Ortho2D(0.0f, activeDispEnv.disp.w, activeDispEnv.disp.h, 0.0f, 0.0f, 1.0f);
	//eprinterr("GL_Error: %x\n", glGetError());
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

#if defined(SINGLE_THREADED)
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