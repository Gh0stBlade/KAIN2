#ifndef EMULATOR_RENDER_NX_H
#define EMULATOR_RENDER_NX_H

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#include <string.h>

#if defined(PLATFORM_NX)

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

enum TextureSlot
{
	SLOT_VRAM,
	SLOT_LUT,
	SLOT_DUMMY,
	SLOT_MAX
};

typedef struct TextureID
{
	nn::gfx::Texture texture;
	nn::gfx::TextureView textureView;
	nn::gfx::DescriptorSlot textureDescriptor;
	int offset;
	int size;
} TextureID;

typedef struct ShaderID
{
	nn::gfx::Shader VS;
	nn::gfx::Shader PS;
	nn::gfx::VertexState vertexState;
	int u_projection;
	int textureBindings[SLOT_MAX];

}ShaderID;

typedef unsigned int RenderTargetID;


extern unsigned int dynamic_vertex_buffer_index;

extern int Emulator_InitialiseNNContext(char* windowName);
extern ShaderID* Shader_Compile(const char* path_vs, const char* path_fs);
extern void Emulator_BeginPass();
extern void Emulator_EndPass();
extern void Emulator_SetVertexBuffer();

#endif

#endif