#ifndef EMULATOR_FILESYSTEM_EMSCRIPTEN_H
#define EMULATOR_FILESYSTEM_EMSCRIPTEN_H

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include <stdio.h>

#if defined(__EMSCRIPTEN__)
extern void Emulator_OpenReadEM(long hash, void* buff, int size);
extern FILE* Emulator_OpenFile(const char* filePath, const char* mode, int* outSize);
#endif

#endif