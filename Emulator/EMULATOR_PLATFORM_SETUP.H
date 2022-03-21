#ifndef EMULATOR_PLATFORM_SETUP_H
#define EMULATOR_PLATFORM_SETUP_H

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#define UWP
#endif


#ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
#endif

/*
 * Platform specific emulator setup
 */
#if (defined(_WINDOWS) || defined(__APPLE__) || defined(__linux__) || defined(__MINGW32__)) && !defined(__ANDROID__) && (!defined(UWP))

#if defined(_DEBUG)
#define DEBUG_PRINTS
#endif

//Open Graphics Library (Embedded Systems)
//#define OGLES
//#define OGLES_VERSION 3

//Open Graphics Library
//#define OGL

//Vulkan
#define VULKAN

//Direct3D9
//#define D3D9

//Direct3D11
//#define D3D11

//Direct3D12
//#define D3D12

#if defined(OGL) || defined(D3D9) || defined(D3D11) || defined(VULKAN)
#define SDL2
#define OPENAL
#endif

#if defined(OGL)
#define GLEW
#endif

#elif defined(__EMSCRIPTEN__)
#define OGLES
#define OGLES_VERSION (2)
#define SDL2
#elif defined(__ANDROID__)
#define OGLES
#define OGLES_VERSION (3)
#define SDL2
#elif defined(_XBOX)
#define XED3D
#endif
#if defined(OGLES)
#if OGLES_VERSION == 2
#define ES2_SHADERS
#elif OGLES_VERSION == 3
#define ES3_SHADERS
#endif
#endif
#if defined(UWP)//UWP
#define D3D11
#define XAUDIO2
#endif

#if defined(GLEW)
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glew.h>
#endif

/*
 * Emulator render options.
 */
#define RO_DOUBLE_BUFFERED

#if defined(__APPLE__) || defined(__EMSCRIPTEN__)
//#include <stddef.h>
#endif

/*
 * SDL.H inclusion.
 */

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined(__EMSCRIPTEN__)
#include <SDL2/SDL.h>
#elif (defined(_WINDOWS) || defined(__MINGW32__) || defined(__linux__)) && !defined(__ANDROID__) && !defined(UWP)
#include <SDL.h>
#if defined(VULKAN)
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <set>
#endif
#elif defined (__ANDROID__)
#include <SDL.h>
#if (OGLES_VERSION == 3)
#include <GLES3/gl3.h>
#endif
#endif

#if defined (OGL)
#include <SDL_opengl.h>///@FIXME see mac!
#elif defined (OGLES)
#include <SDL_syswm.h>
#if OGLES_VERSION == 2
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif OGLES_VERSION == 3
#include <GLES3/gl3.h>
#endif
#include <EGL/egl.h>
#elif defined(D3D9)
#include <Windows.h>
#include <d3d9.h>
#include <SDL_syswm.h>
#elif defined(D3D11)
#if defined(UWP)
#include <d3d11_3.h>
#else
#include <Windows.h>
#include <d3d11.h>
#endif
#if defined(SDL2)
#include <SDL_syswm.h>
#endif
#elif defined(D3D12)
#include <Windows.h>
#include <d3d12.h>
#include <SDL_syswm.h>
#elif defined(XED3D)
#include <xtl.h>
#include <xgraphics.h>
#if _XDK_VER != 21256
#error "Invalid XDK version"
#endif
#endif

#if !defined (__EMSCRIPTEN__)
//#include <SDL_gamecontroller.h>
#endif

#if defined(OGL)
#define TEXTURE_FORMAT GL_UNSIGNED_SHORT_1_5_5_5_REV
#elif defined(OGLES)
#define TEXTURE_FORMAT GL_UNSIGNED_SHORT_5_5_5_1
#endif

#endif