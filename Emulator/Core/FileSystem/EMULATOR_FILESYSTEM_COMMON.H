#ifndef EMULATOR_FILESYSTEM_COMMON_H
#define EMULATOR_FILESYSTEM_COMMON_H


#if defined(_WIN32) || defined(_WIN64)
#include "Core/FileSystem/Windows/EMULATOR_FILESYSTEM_WINDOWS.H"
#elif defined(__EMSCRIPTEN__)
#include "Core/FileSystem/Emscripten/EMULATOR_FILESYSTEM_EMSCRIPTEN.H"
#endif

#if !defined(NO_FILESYSTEM)
extern void Emulator_OpenRead(long hash, void* buff, int size);
extern void Emulator_OpenReadFP(const char* filePath, void* buff, int size);
extern void Emulator_GetFileSize(const char* filePath, int* outSize);
#endif

#endif