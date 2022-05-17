#ifndef EMULATOR_PLATFORM_SETUP_H
#define EMULATOR_PLATFORM_SETUP_H

//#define DEBUG_PRINTS

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
#define OGL

//Vulkan
//#define VULKAN

//Direct3D9
//#define D3D9

//Direct3D11
//#define D3D11

//Direct3D12
//#define D3D12

#if defined(OGL) || defined(D3D9) || defined(D3D11) || defined(D3D12) || defined(VULKAN)
#define SDL2
#define OPENAL
#endif

#if defined(OGL)
#define GLEW
#endif

#elif defined(__EMSCRIPTEN__)
#define OGLES
#define OGLES_VERSION (3)
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

 /*
  * Emulator render options.
  */
#define RO_DOUBLE_BUFFERED

#endif