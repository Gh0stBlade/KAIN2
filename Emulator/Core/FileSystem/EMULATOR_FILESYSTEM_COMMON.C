#include "EMULATOR_FILESYSTEM_COMMON.H"

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#include <stdio.h>

#if !defined(NO_FILESYSTEM)

void Emulator_OpenRead(long fileHash, void* buff, int size)
{
#if defined(__EMSCRIPTEN__)
	Emulator_OpenReadEM(fileHash, buff, size);
#elif defined(_WIN32) || defined(_WIN64)
	Emulator_OpenReadWIN(fileHash, buff, size);
#endif
}

void Emulator_OpenReadFP(const char* filePath, void* buff, int size)
{
#if defined(__EMSCRIPTEN__)
	Emulator_OpenReadFPEM(filePath, buff, size);
#elif defined(_WIN32) || defined(_WIN64)
	Emulator_OpenReadFPWIN(filePath, buff, size);
#endif
}

void Emulator_ReadFile(const char* filePath, void* buff, int size)
{
#if defined(__EMSCRIPTEN__)
	Emulator_ReadFileEM(filePath, buff, size);
#elif defined(_WIN32) || defined(_WIN64)
	Emulator_ReadFileWIN(filePath, buff, size);
#endif
}

void Emulator_GetFileSize(const char* filePath, int* outSize)
{
#if defined(__EMSCRIPTEN__)
	Emulator_GetFileSizeEM(filePath, outSize);
#elif defined(_WIN32) || defined(_WIN64)
	Emulator_GetFileSizeWIN(filePath, outSize);
#endif
}

struct _BigFileEntry
{
	long fileHash;
	long fileLen;
	long filePos;
	long checkSumFull;
};

#define MAX_NUM_LOAD_ATTEMPTS 255

void* Emulator_GetBigFileEntryByHash(long hash)
{
	int fileSize = 0;
	Emulator_GetFileSize("bigfile.lst", &fileSize);
	char* fileBuffer = new char[fileSize];
	Emulator_ReadFile("bigfile.lst", fileBuffer, fileSize);

	char* fileName = NULL;
	struct _BigFileEntry bigFileEntry;
	long fileHash = 0;

	char* pLine = &fileBuffer[0];

	while (pLine != NULL && pLine < &fileBuffer[fileSize])
	{
		sscanf(pLine, "%x", &fileHash);
		fileName = &pLine[9];

		if (hash == fileHash)
		{
			Emulator_GetFileSize(fileName, &fileSize);

			bigFileEntry.checkSumFull = 0;
			bigFileEntry.fileHash = fileHash;
			bigFileEntry.fileLen = fileSize;
			bigFileEntry.filePos = 0;

			return &bigFileEntry;
		}

		pLine = strchr(pLine, 0) + 1;
	}

	delete[] fileBuffer;

	return NULL;
}
#endif