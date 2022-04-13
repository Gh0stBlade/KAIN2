#ifndef EMULATOR_INPUT_H
#define EMULATOR_INPUT_H

#include <SDL.h>

#define MAX_CONTROLLERS 2

extern void Emulator_InitialiseSDLInput(SDL_GameController** pad, const unsigned char** kbState, int isDebugInput);

extern SDL_GameController* padHandle[];
extern unsigned char* padRumbleData[];
extern unsigned char* padData[];
extern const unsigned char* keyboardState;

extern SDL_GameController* padHandleDebug[];
extern unsigned char* padDataDebug[];
extern const unsigned char* keyboardStateDebug;

extern int g_initialisedPadSubsystem;

#endif