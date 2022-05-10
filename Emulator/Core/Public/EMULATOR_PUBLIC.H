#ifndef EMULATOR_PUBLIC_H
#define EMULATOR_PUBLIC_H

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#define UWP
#endif

//Undefine nasty windows.h defs which collide with our PSX ones.
#ifdef LoadImage
#undef LoadImage
#endif
#ifdef OpenEvent
#undef OpenEvent
#endif

#include "Core/Setup/Game/GAME_VERSION.H"
#include "Core/Debug/EMULATOR_LOG.H"

#ifdef __cplusplus
extern "C" {
#endif

extern void Emulator_SaveVRAM(const char* outputFileName, int x, int y, int width, int height, int bReadFromFrameBuffer);

/* Usually called at the beginning of main function */
extern void Emulator_Initialise(char* windowName, int screenWidth, int screenHeight);

/* Explicitly updates emulator debug input loop */
extern void Emulator_UpdateInputDebug();

/* Sets the maximum vertex count value for PGXP vertex caching */
extern void Emulator_SetPGXPVertexCount(int vertexCount);

/* Waits for timer */
extern void Emulator_WaitForTimestep(int count);

/* Stores framebuffer */
extern void Emulator_StoreFrameBuffer(int x, int y, int w, int h);

/* Explicitly updates emulator input loop */
extern void Emulator_UpdateInput();

/* Usually called at the end of main function */
extern void Emulator_ShutDown();

#ifdef __cplusplus
}
#endif

#endif