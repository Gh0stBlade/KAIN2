#ifndef EMULATOR_RENDER_GXM_H
#define EMULATOR_RENDER_GXM_H


#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#include <string.h>

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
#include <gxm.h>


#define BM_COUNT 5

typedef struct ShaderID
{
	SceGxmShaderPatcherId VSID;
	SceGxmShaderPatcherId FSID;

	SceGxmVertexProgram* VP;
	SceGxmFragmentProgram* FP[BM_COUNT];

	const SceGxmProgram* PRG;
	int isGTE;

} ShaderID;

#undef BM_COUNT

typedef struct DisplayData
{
	void* address;
} DisplayData;

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

typedef struct TextureID
{
	SceUID Uid;
    void *bufferData;
	SceGxmTexture texture;
} TextureID;

typedef unsigned int RenderTargetID;

extern int Emulator_InitialiseGXMContext(char* windowName);
extern void Emulator_BeginRenderScene();
extern void Emulator_EndRenderScene();

extern const SceGxmProgram	blit_shader_vs;
extern const SceGxmProgram	blit_shader_fs;

extern const SceGxmProgram	gte_shader_4_vs;
extern const SceGxmProgram	gte_shader_4_fs;

extern const SceGxmProgram	gte_shader_8_vs;
extern const SceGxmProgram	gte_shader_8_fs;

extern const SceGxmProgram	gte_shader_16_vs;
extern const SceGxmProgram	gte_shader_16_fs;

extern int g_activeIndexBuffer;

#define Shader_Compile(name, flag) Shader_Compile_Internal((const SceGxmProgram*)&name##_vs, (const SceGxmProgram*)&name##_fs, flag)

#endif

#endif