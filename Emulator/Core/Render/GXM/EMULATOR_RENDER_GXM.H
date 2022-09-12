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

typedef struct 
{
	SceGxmShaderPatcherId VSID;
	SceGxmShaderPatcherId FSID;
} ShaderID;

typedef struct 
{
	void* address;
} Display;

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

typedef SceUID TextureID;

typedef unsigned int RenderTargetID;

extern int Emulator_InitialiseGXMContext(char* windowName);

extern const SceGxmProgram*	blit_shader_vs;
extern const SceGxmProgram*	blit_shader_fs;

extern const SceGxmProgram*	gte_shader_4_vs;
extern const SceGxmProgram*	gte_shader_4_fs;

extern const SceGxmProgram*	gte_shader_8_vs;
extern const SceGxmProgram*	gte_shader_8_fs;

extern const SceGxmProgram*	gte_shader_16_vs;
extern const SceGxmProgram*	gte_shader_16_fs;

#define Shader_Compile(name) Shader_Compile_Internal((const SceGxmProgram*)name##_vs, (const SceGxmProgram*)name##_fs)

#endif

#endif