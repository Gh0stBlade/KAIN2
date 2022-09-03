#include "EMULATOR_FILESYSTEM_WINDOWS.H"

#include "Core/Setup/Game/GAME_VERSION.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#include <stdio.h>

#if defined(_DEBUG)

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
	FILE* f = fopen("bigfile.txt", "r");
	char* fileName = NULL;
	struct _BigFileEntry bigFileEntry;
	long fileHash = 0;
	int fileSize = 0;

	if (f != NULL)
	{
		char buffer[64];

		while (fgets(buffer, 64, f)) 
		{
			buffer[strcspn(buffer, "\n")] = 0;
			sscanf(buffer, "%x", &fileHash);
			fileName = &buffer[9];

			if (hash == fileHash)
			{
				break;
			}
		}

		fclose(f);

		Emulator_GetFileSize(fileName, &fileSize);

		bigFileEntry.checkSumFull = 0;
		bigFileEntry.fileHash = fileHash;
		bigFileEntry.fileLen = fileSize;
		bigFileEntry.filePos = 0;

		return &bigFileEntry;

	}

	return NULL;
}

void Emulator_OpenReadWIN(long hash, void* buff, int size)
{
	FILE* f = fopen("bigfile.txt", "r");
	char* fileName = NULL;
	struct _BigFileEntry bigFileEntry;
	long fileHash = 0;
	int fileSize = 0;

	if (f != NULL)
	{
		char buffer[64];

		while (fgets(buffer, 64, f))
		{
			buffer[strcspn(buffer, "\n")] = 0;
			sscanf(buffer, "%x", &fileHash);
			fileName = &buffer[9];

			if (hash == fileHash)
			{
				break;
			}
		}

		fclose(f);

		f = fopen(fileName, "rb");
		fread(buff, size, 1, f);
		fclose(f);
	}

	return;
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