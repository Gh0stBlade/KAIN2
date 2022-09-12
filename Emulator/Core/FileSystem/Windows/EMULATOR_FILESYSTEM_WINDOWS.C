#include "EMULATOR_FILESYSTEM_WINDOWS.H"

#include "Core/Setup/Game/GAME_VERSION.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#include <stdio.h>

#if defined(_DEBUG) && !defined(NO_FILESYSTEM)

void Emulator_ReadFileWIN(const char* filePath, void* buff, int size)
{
	FILE* f = fopen(filePath, "rb");
	
	if (f != NULL)
	{
		fread(buff, size, 1, f);
		fclose(f);
	}
}

FILE* Emulator_OpenFile(const char* filePath, const char* mode, int* outSize)
{
	FILE* f = fopen(filePath, mode);
	
	if (f != NULL)
	{
		return f;
	}

	return NULL;
}

void Emulator_OpenReadWIN(long hash, void* buff, int size)
{
	int fileSize = 0;
	Emulator_GetFileSize("bigfile.lst", &fileSize);
	char* fileBuffer = new char[fileSize];
	Emulator_ReadFileWIN("bigfile.lst", fileBuffer, fileSize);
	char* fileName = NULL;
	long fileHash = 0;

	char* pLine = &fileBuffer[0];

	while (pLine != NULL && pLine < &fileBuffer[fileSize])
	{
		sscanf(pLine, "%x", &fileHash);
		fileName = &pLine[9];

		if (hash == fileHash)
		{
			FILE* f = fopen(fileName, "rb");
			if (f != NULL)
			{
				fread(buff, size, 1, f);
				fclose(f);
			}

			break;
		}

		pLine = strchr(pLine, 0) + 1;
	}

	return;
}

void Emulator_OpenReadFPWIN(const char* filePath, void* buff, int size)
{
	Emulator_ReadFileWIN(filePath, buff, size);
}

void Emulator_GetFileSizeWIN(const char* filePath, int* outSize)
{
	FILE* f = fopen(filePath, "rb");

	outSize[0] = 0;

	if (f != NULL)
	{
		fseek(f, 0, SEEK_END);
		outSize[0] = ftell(f);
		fclose(f);
	}
}

#endif