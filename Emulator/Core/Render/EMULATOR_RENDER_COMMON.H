#ifndef EMULATOR_RENDER_H
#define EMULATOR_RENDER_H

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"

#if defined (OGL)
#include "Core/Render/OGL/EMULATOR_RENDER_OGL.H"
#elif defined (OGLES)
#include "Core/Render/OGLES/EMULATOR_RENDER_OGLES.H"
#elif defined(D3D9)
#include "Core/Render/D3D9/EMULATOR_RENDER_D3D9.H"
#elif defined(D3D11)
#include "Core/Render/D3D11/EMULATOR_RENDER_D3D11.H"
#elif defined(D3D12)
#include "Core/Render/D3D12/EMULATOR_RENDER_D3D12.H"
#elif defined(VULKAN)
#include "Core/Render/VULKAN/EMULATOR_RENDER_D3D12.H"
#elif defined(XED3D)
#include "Core/Render/XED3D/EMULATOR_RENDER_XED3D.H"
#elif defined(SN_TARGET_PSP2)
#include "Core/Render/GXM/EMULATOR_RENDER_GXM.H"
#elif defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
#include "Core/Render/NVN/EMULATOR_RENDER_NVN.H"
#endif


typedef enum BlendMode
{
	BM_NONE,
	BM_AVERAGE,
	BM_ADD,
	BM_SUBTRACT,
	BM_ADD_QUATER_SOURCE,
	BM_COUNT

} BlendMode;

typedef enum TexFormat 
{
	TF_4_BIT,
	TF_8_BIT,
	TF_16_BIT,
	TF_COUNT,

} TexFormat;

#pragma pack(push,1)
struct Vertex
{
#if defined(PGXP)
	float  x, y, page, clut;
#else
	short  x, y, page, clut;
#endif
	unsigned char u, v, bright, dither;
	unsigned char r, g, b, a;
#if defined(PGXP)
	float z, w;
#endif
};
#pragma pack(pop)

struct VertexBufferSplit
{
#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
	TextureID*      textureId;
#else
	TextureID      textureId;
#endif
	unsigned short vIndex;
	unsigned short iIndex;
	unsigned short vCount;
	enum BlendMode blendMode;
	enum TexFormat texFormat;
};

extern void Emulator_ClearVBO();
extern void Emulator_ResetPolyState();
extern void Emulator_AggregatePTAGsToSplits(unsigned long* p, int singlePrimitive);
extern void Emulator_DrawSplit(const struct VertexBufferSplit* split);
extern void Emulator_DrawAggregatedSplits();

#if defined(TOUCH_UI)
extern void Emulator_DrawTouchUI();
#endif

extern int g_swapInterval;
extern int g_wireframeMode;
extern unsigned int g_swapTime;
extern int g_PreviousBlendMode;
extern int g_texturelessMode;
extern int windowWidth;
extern int windowHeight;
extern int g_overrideWidth[];
extern int g_overrideHeight[];
#if defined(D3D11) || (defined(OGL) && defined(_WINDOWS))
#include <Windows.h>//OGL
extern HWND g_overrideHWND[];
#endif
extern unsigned int g_resetDeviceOnNextFrame[6];
extern unsigned int g_closeOnNextFrame;
extern unsigned int g_resettingDevice;
#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
extern TextureID* g_lastBoundTexture[];
#else
extern TextureID thread_local g_lastBoundTexture[];
#endif
extern int begin_scene_flag;
extern int vbo_was_dirty_flag;

#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
extern ShaderID* g_gte_shader_4;
extern ShaderID* g_gte_shader_8;
extern ShaderID* g_gte_shader_16;
extern ShaderID* g_blit_shader;

extern TextureID* vramTexture;
extern TextureID* whiteTexture;
extern TextureID* rg8lutTexture;
#else
extern ShaderID thread_local g_gte_shader_4;
extern ShaderID thread_local g_gte_shader_8;
extern ShaderID thread_local g_gte_shader_16;
extern ShaderID thread_local g_blit_shader;

extern TextureID thread_local vramTexture;
extern TextureID thread_local whiteTexture;
extern TextureID thread_local rg8lutTexture;
#endif

extern unsigned short vram[];
extern unsigned char rgLUT[];

extern int vram_need_update;

#define LUT_WIDTH (256)
#define LUT_HEIGHT (256)

#define VRAM_WIDTH (1024)
#define VRAM_HEIGHT (512)

#define TPAGE_WIDTH (256)
#define TPAGE_HEIGHT (256)

#define CLUT_WIDTH (16)
#define CLUT_HEIGHT (1)

#define MAX_NUM_POLY_BUFFER_VERTICES (20000)
#define MAX_NUM_INDEX_BUFFER_INDICES (65535)
#define MAX_NUM_INDEX_BUFFERS        (4096)

#define VRAM_FORMAT            GL_RG
#define VRAM_INTERNAL_FORMAT   GL_RG8

#if defined(_DEBUG) && 0//60FPS
#define FIXED_TIME_STEP    16
#else
#define FIXED_TIME_STEP    33
#endif
#define SWAP_INTERVAL      1

extern void* Emulator_GenerateRG8LUT();
extern void Emulator_SwapWindow();
extern void Emulator_ResetDevice();
#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
extern void Emulator_SetShader(ShaderID* shader);
extern void Emulator_SetTextureAndShader(TextureID* texture, ShaderID* shader);
extern ShaderID* Emulator_GetGTEShader(TexFormat texFormat);
#else
extern void Emulator_SetTextureAndShader(TextureID texture, ShaderID shader);
extern ShaderID Emulator_GetGTEShader(TexFormat texFormat);
#endif

extern void Emulator_UpdateVertexBuffer(const struct Vertex* vertices, int num_vertices);
extern void Emulator_UpdateIndexBuffer(const unsigned short* indices, int num_indices);
extern int Emulator_CreateCommonResources();
extern void Emulator_Ortho2D(float left, float right, float bottom, float top, float znear, float zfar);
extern const char* renderBackendName;
extern int thread_local g_instanceIndex;

extern int Emulator_GetWindowWidth();
extern int Emulator_GetWindowHeight();

#endif