#include "EMULATOR_FILESYSTEM_COMMON.H"

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

void Emulator_OpenRead(long fileHash, void* buff, int size)
{
#if defined(__EMSCRIPTEN__)
	Emulator_OpenReadEM(fileHash, buff, size);
#elif defined(_WIN32) || defined(_WIN64)
	Emulator_OpenReadWIN(fileHash, buff, size);
#endif
}

void Emulator_GetFileSize(const char* filePath, int* outSize)
{
#if defined(__EMSCRIPTEN__)
	//Emulator_OpenFileEM(filePath, mode, outBuff, outSize);
#elif defined(_WIN32) || defined(_WIN64)
	Emulator_GetFileSizeWIN(filePath, outSize);
#endif
}