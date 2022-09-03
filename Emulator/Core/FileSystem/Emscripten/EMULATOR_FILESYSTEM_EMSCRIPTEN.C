#include "EMULATOR_FILESYSTEM_EMSCRIPTEN.H"

#include "Core/Setup/Game/GAME_VERSION.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#include <stdio.h>

#if defined(__EMSCRIPTEN__)

#include <emscripten.h>

struct _BigFileEntry
{
	long fileHash;
	long fileLen;
	long filePos;
	long checkSumFull;
};

void* Emulator_GetBigFileEntryByHash(long hash)
{
	char* fileName = NULL;
	struct _BigFileEntry bigFileEntry;
	long fileHash = 0;
	int fileSize = 0;
	
	FILE* f = Emulator_OpenFile("bigfile.txt", "r", &fileSize);

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

		bigFileEntry.checkSumFull = 0;
		bigFileEntry.fileHash = fileHash;
		bigFileEntry.fileLen = fileSize;
		bigFileEntry.filePos = 0;

		return &bigFileEntry;

	}

	return NULL;
}

void Emulator_OpenReadEM(long hash, void* buff, int size)
{
	int outSize = 0;
	FILE* f = Emulator_OpenFile("bigfile.txt", "r", &outSize);
	
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

		f = Emulator_OpenFile(fileName, "rb", &outSize);
		fread(buff, size, 1, f);
		fclose(f);
	}

	return;
}


FILE* Emulator_OpenFile(const char* filePath, const char* mode, int* outSize)
{
	int fileExists = 0;
	int fileExistsErr = 0;
	void* outBuff = NULL;

	emscripten_idb_exists(SHORT_GAME_NAME, filePath, &fileExists, &fileExistsErr);

	if (fileExists && fileExistsErr == 0)
	{
		int err = 0;

		emscripten_idb_load(SHORT_GAME_NAME, filePath, &outBuff, outSize, &err);

		if (err != 0)
		{
			printf("Failed to open file indexed db %s!\n", filePath);
			return NULL;
		}

		if (outBuff != NULL)
		{
			free(outBuff);
		}
	}
	else
	{
		const char* urlName = "https://legacyofkain.co.uk";
		char fullName[256];
		int err = 0;

		sprintf(fullName, "%s//%s//ASSETS//REVIEW//%s", urlName, SHORT_GAME_NAME, filePath);
		
		emscripten_wget_data(fullName, &outBuff, outSize, &err);

		if (err != 0)
		{
			printf("Failed to open file wget %s!\n", filePath);
			return NULL;
		}

		err = 0;
		emscripten_idb_store(SHORT_GAME_NAME, filePath, outBuff, *outSize, &err);

		if (err != 0)
		{
			printf("Failed to store file indexed db %s!\n", filePath);
			return NULL;
		}

		if (outBuff != NULL)
		{
			free(outBuff);
		}
	}

	FILE* f = fopen(filePath, mode);
	if (f == NULL)
	{
		printf("Failed to open file handle indexed db %s\n", filePath);
		return NULL;
	}

	return f;
}
#endif