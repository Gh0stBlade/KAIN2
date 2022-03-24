#ifndef EMULATOR_H
#define EMULATOR_H

#include "LIBGPU.H"

#include <stdio.h>
#include <stddef.h>

#include "EMULATOR_PLATFORM_SETUP.H"

#ifndef TRUE
#define TRUE		1
#endif
#ifndef FALSE
#define FALSE		0
#endif
#ifndef NULL
#define NULL		0
#endif

#if !((defined OGL) || (defined(OGLES) && OGLES_VERSION == 3))
#define glGenVertexArrays       glGenVertexArraysOES
#define glBindVertexArray       glBindVertexArrayOES
#define glDeleteVertexArrays    glDeleteVertexArraysOES
#endif

#ifdef _MSC_VER
#define FUNCNAME __FUNCTION__
#else
#define FUNCNAME __func__
#endif

#if defined(__ANDROID__)
#include <android/log.h>
#define LOG_TAG_EMU "[EMU] "

#if defined(_DEBUG)
#define eprintf(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG_EMU, fmt, ##__VA_ARGS__)
#define eprinterr(fmt, ...) eprintf("[%s] - " fmt, FUNCNAME, ##__VA_ARGS__);
#else
#define eprintf(fmt, ...) (0)
#define eprinterr(fmt, ...) (0)
#endif

#else
#if defined(DEBUG_PRINTS)
#define eprintf(fmt, ...) printf("[EMU] " fmt, ##__VA_ARGS__)
#define eprinterr(fmt, ...) eprintf("[%s] - " fmt, FUNCNAME, ##__VA_ARGS__);
#else
#define eprintf(...) 
#define eprinterr(...)
#endif
#endif

#if defined(__EMSCRIPTEN__) || !defined(DEBUG_PRINTS)
#define UNIMPLEMENTED()
#else
#define UNIMPLEMENTED() eprinterr("Unimplemented!\n");
#endif

#if defined(OGL)
#define VRAM_FORMAT            GL_RG
#define VRAM_INTERNAL_FORMAT   GL_RG8
#elif defined(OGLES)
#define VRAM_FORMAT            GL_LUMINANCE_ALPHA
#define VRAM_INTERNAL_FORMAT   GL_LUMINANCE_ALPHA
#endif

#define VRAM_WIDTH (1024)
#define VRAM_HEIGHT (512)

#define TPAGE_WIDTH (256)
#define TPAGE_HEIGHT (256)

#define CLUT_WIDTH (16)
#define CLUT_HEIGHT (1)

#define CTR_RUNNING (0)
#define CTR_STOPPED (1)

#define CTR_MODE_TO_FFFF (0)
#define CTR_MODE_TO_TARG (1)

#define CTR_CLOCK_SYS (0)
#define CTR_CLOCK_PIXEL (1)
#define CTR_HORIZ_RETRACE (1)

#define CTR_CLOCK_SYS_ONE (0)
#define CTR_CLOCK_SYS_ONE_EIGHTH (1)

#define GET_TPAGE_X(tpage)      (((tpage << 6) & 0x7C0) % (VRAM_WIDTH))
#define GET_TPAGE_Y(tpage)      (((((tpage << 4) & 0x100) + ((tpage >> 2) & 0x200))) % (VRAM_HEIGHT))
#define GET_TPAGE_FORMAT(tpage) ((TexFormat)((tpage >> 7) & 0x3))
#define GET_TPAGE_BLEND(tpage)  ((BlendMode)(((page >> 5) & 3) + 1))

// TODO
#define GET_TPAGE_DITHER(tpage) 1
//#define GET_TPAGE_DITHER(tpage) 1((tpage >> 9) & 0x1)

#define GET_CLUT_X(clut)        ((clut & 0x3F) << 4)
#define GET_CLUT_Y(clut)        (clut >> 6)

typedef struct
{
	unsigned int i_cycle;

	union
	{
		unsigned short cycle;
		unsigned short unk00;
	};

	unsigned int i_value;

	union
	{
		unsigned short value;
		unsigned short unk01;
	};

	unsigned int i_target;

	union
	{
		unsigned short target;
		unsigned short unk02;
	};

	typedef long (*CallbackProc)(void);

	CallbackProc padding00;
	unsigned int padding01;
} SysCounter;

extern SysCounter counters[3];

#pragma pack(push,1)
struct Vertex
{
#if defined(PGXP)
	float  x, y, page, clut;
#else
	short  x, y, page, clut;
#endif
	u_char u, v, bright, dither;
	u_char r, g, b, a;
#if defined(PGXP)
	float z, w;
#endif
};
#pragma pack(pop)

enum ShaderAttrib
{
	a_position,
	a_texcoord,
	a_color,
#if defined(PGXP)
	a_w,
	a_z
#endif
};

enum BlendMode
{
	BM_NONE,
	BM_AVERAGE,
	BM_ADD,
	BM_SUBTRACT,
	BM_ADD_QUATER_SOURCE,
	BM_COUNT
};

enum TexFormat
{
	TF_4_BIT,
	TF_8_BIT,
	TF_16_BIT
};

#define MAX_NUM_POLY_BUFFER_VERTICES (12040)
#define MAX_NUM_INDEX_BUFFERS        (4096)

#if defined(OGLES) || defined(OGL)
typedef GLuint TextureID;
typedef GLuint ShaderID;
typedef GLuint RenderTargetID;
#elif defined(D3D9) || defined(XED3D)
typedef IDirect3DTexture9* TextureID;

struct ShaderID {
	IDirect3DVertexShader9* VS;
	IDirect3DPixelShader9* PS;
};

typedef IDirect3DSurface9* RenderTargetID;
#elif defined(D3D11)
typedef ID3D11ShaderResourceView* TextureID;

struct ShaderID {
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11InputLayout* IL;
};

typedef ID3D11RenderTargetView* RenderTargetID;
#elif defined(D3D12)
typedef void* TextureID;

struct ShaderID {
	D3D12_SHADER_BYTECODE VS;
	D3D12_SHADER_BYTECODE PS;
	D3D12_INPUT_LAYOUT_DESC IL;
};

typedef D3D12_RENDER_TARGET_VIEW_DESC* RenderTargetID;
#elif defined(VULKAN)

struct TextureImage
{
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VkImageView textureImageView;
};

typedef TextureImage TextureID;

struct ShaderID 
{
	enum ShaderType
	{
		gte_4,
		gte_8,
		gte_16,
		blit,
	};

	enum ShaderType T;
	VkPipelineShaderStageCreateInfo VS;
	VkPipelineShaderStageCreateInfo PS;
	VkDescriptorSetLayout DL;
	VkSampler SS;
	VkImageView TIV;
	VkPipeline GP;
};

typedef void* RenderTargetID;

extern ShaderID* g_shaders[];

#else
	#error
#endif

extern int vram_need_update;
extern bool begin_scene_flag;
extern bool vbo_was_dirty_flag;
extern int  Emulator_Initialise();
extern void Emulator_SaveVRAM(const char* outputFileName, int x, int y, int width, int height, int bReadFromFrameBuffer);
extern void Emulator_CopyVRAM(unsigned short* src, int x, int y, int w, int h, int dst_x, int dst_y);
extern void Emulator_ReadVRAM(unsigned short* dst, int x, int y, int dst_w, int dst_h);
extern void Emulator_UpdateVRAM();
extern void Emulator_BlitVRAM();
extern void Emulator_ShutDown();
extern void Emulator_SwapWindow();
extern void Emulator_SetBlendMode(BlendMode blendMode);
extern void Emulator_CounterLoop();

extern void Emulator_GenerateLineArray(struct Vertex* vertex, short* p0, short* p1);
extern void Emulator_GenerateVertexArrayTriangle(struct Vertex* vertex, short* p0, short* p1, short* p2);
extern void Emulator_GenerateVertexArrayQuad(struct Vertex* vertex, short* p0, short* p1, short* p2, short* p3);
extern void Emulator_GenerateVertexArrayRect(struct Vertex* vertex, short* p0, short w, short h);

extern void Emulator_GenerateTexcoordArrayTriangle(struct Vertex* vertex, unsigned char* uv0, unsigned char* uv1, unsigned char* uv2, short page, short clut, unsigned char dither);
extern void Emulator_GenerateTexcoordArrayQuad(struct Vertex* vertex, unsigned char* uv0, unsigned char* uv1, unsigned char* uv2, unsigned char* uv3, short page, short clut, unsigned char dither);
extern void Emulator_GenerateTexcoordArrayRect(struct Vertex* vertex, unsigned char* uv, short page, short clut, short w, short h);
extern void Emulator_GenerateTexcoordArrayLineZero(struct Vertex* vertex, unsigned char dither);
extern void Emulator_GenerateTexcoordArrayTriangleZero(struct Vertex* vertex, unsigned char dither);
extern void Emulator_GenerateTexcoordArrayQuadZero(struct Vertex* vertex, unsigned char dither);

extern void Emulator_GenerateColourArrayLine(struct Vertex* vertex, unsigned char* col0, unsigned char* col1);
extern void Emulator_GenerateColourArrayTriangle(struct Vertex* vertex, unsigned char* col0, unsigned char* col1, unsigned char* col2);
extern void Emulator_GenerateColourArrayQuad(struct Vertex* vertex, unsigned char* col0, unsigned char* col1, unsigned char* col2, unsigned char* col3, bool bMaxCol);

extern void Emulator_DestroyTexture(TextureID texture);
extern void Emulator_Clear(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
extern void Emulator_SetViewPort(int x, int y, int width, int height);
extern void Emulator_SetTexture(TextureID texture, TexFormat texFormat);
extern void Emulator_SetRenderTarget(const RenderTargetID& render_target);
extern void Emulator_SetWireframe(bool enable);
extern void Emulator_UpdateVertexBuffer(const Vertex* vertices, int count);
extern void Emulator_DrawTriangles(int start_vertex, int triangles);

extern void Emulator_CreateGlobalShaders();
extern void Emulator_DestroyGlobalShaders();
extern void Emulator_GenerateCommonTextures();

extern void Emulator_DoPollEvent();

#if defined(D3D11)
extern void Emulator_CreateConstantBuffers();
extern void Emulator_UpdateProjectionConstantBuffer(float* ortho);
extern void Emulator_SetConstantBuffers();
extern void Emulator_DestroyConstantBuffers();
extern void Emulator_CreateRasterState(int wireframe);
extern void Emulator_SetDefaultRenderTarget();
#elif defined(D3D12)
extern void Emulator_CreateRasterState(int wireframe);
extern void Emulator_WaitForPreviousFrame();
#elif defined(VULKAN)
extern void Emulator_CreateRasterState(int wireframe);
extern void Emulator_CreateVulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
extern void Emulator_BeginPass();
extern void Emulator_EndPass();
extern VkImageView Emulator_CreateImageView(VkImage image, VkFormat format);
extern unsigned int Emulator_FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
extern void Emulator_CreateBlitShaderDescriptors();
extern void Emulator_CreateConstantBuffers();
extern void Emulator_CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
extern void Emulator_EndSingleTimeCommands(VkCommandBuffer commandBuffer);
extern void Emulator_TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
extern void Emulator_UpdateProjectionConstantBuffer(float* ortho);
extern void Emulator_CreatePipelineState(ShaderID& shader, VkPipeline* pipeline);
extern int Emulator_BeginCommandBuffer();
extern void Emulator_EndCommandBuffer();
extern VkCommandBuffer Emulator_BeginSingleTimeCommands();

extern ShaderID g_gte_shader_4;
extern ShaderID g_gte_shader_8;
extern ShaderID g_gte_shader_16;
#endif

#if defined(D3D9) || defined(XED3D)
extern IDirect3DDevice9* d3ddev;
#endif

#endif