#ifndef EMULATOR_PRIVATE_H
#define EMULATOR_PRIVATE_H

#include "Core/Setup/Game/GAME_VERSION.H"

//Required for SDL_main define.
#if defined(__ANDROID__)
    #include <SDL_main.h>
#endif

extern void Emulator_SaveVRAM(const char* outputFileName, int x, int y, int width, int height, int bReadFromFrameBuffer);

#ifdef __cplusplus
extern "C" {
#endif

/* Usually called at the beginning of main function */
extern void Emulator_Initialise(char* windowName, int screenWidth, int screenHeight);

#ifdef __cplusplus
}
#endif

/* Explicitly updates emulator input loop */
extern void Emulator_UpdateInput();

/* Explicitly updates emulator debug input loop */
extern void Emulator_UpdateInputDebug();

/* Usually called at the end of main function */
extern void Emulator_ShutDown();

/* Sets the maximum vertex count value for PGXP vertex caching */
extern void Emulator_SetPGXPVertexCount(int vertexCount);

/* Waits for timer */
extern void Emulator_WaitForTimestep(int count);

/* Stores framebuffer */
extern void Emulator_StoreFrameBuffer(int x, int y, int w, int h);

#endif