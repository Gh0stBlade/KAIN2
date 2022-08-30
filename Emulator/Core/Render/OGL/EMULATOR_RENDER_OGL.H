#ifndef EMULATOR_RENDER_OGL_H
#define EMULATOR_RENDER_OGL_H

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#include <string.h>

#if defined(OGL)

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

typedef GLuint TextureID;
typedef GLuint ShaderID;
typedef GLuint RenderTargetID;

extern int Emulator_InitialiseGLContext(char* windowName);
extern ShaderID Shader_Compile(const char* source);

#endif

#endif