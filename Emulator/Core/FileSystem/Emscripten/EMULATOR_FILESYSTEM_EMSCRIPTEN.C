#include "EMULATOR_FILESYSTEM_EMSCRIPTEN.H"

#include "Core/Setup/Game/GAME_VERSION.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#include <stdio.h>

#if defined(__EMSCRIPTEN__)

#include <emscripten.h>

void Emulator_ReadFileEM(const char* filePath, void* buff, int size)
{
	int fileExists = 0;
	int fileExistsErr = 0;
	void* outBuff = NULL;
	int outSize;

	emscripten_idb_exists(SHORT_GAME_NAME, filePath, &fileExists, &fileExistsErr);

	if (fileExists && fileExistsErr == 0)
	{
		int err = 0;

		emscripten_idb_load(SHORT_GAME_NAME, filePath, &outBuff, &outSize, &err);

		if (err != 0)
		{
			printf("Failed to open file indexed db %s!\n", filePath);
			return;
		}

		if (outBuff != NULL)
		{
			memcpy(buff, outBuff, outSize);
			free(outBuff);
		}
	}
	else
	{
		const char* urlName = "https://legacyofkain.co.uk";
		char fullName[256];
		int err = 0;

		sprintf(fullName, "%s/%s/%s", urlName, SHORT_GAME_NAME, filePath);

		emscripten_wget_data(fullName, &outBuff, &outSize, &err);
		
		printf("WGET: %d bytes!", outSize);

		if (err != 0)
		{
			printf("Failed to open file wget %s!\n", filePath);
			return;
		}

		err = 0;

		emscripten_idb_store(SHORT_GAME_NAME, filePath, outBuff, outSize, &err);

		if (err != 0)
		{
			printf("Failed to store file indexed db %s!\n", filePath);

			return;
		}

		if (outBuff != NULL)
		{
			memcpy(buff, outBuff, outSize);
			free(outBuff);
		}
	}
}

void Emulator_OpenReadEM(long hash, void* buff, int size)
{
	int fileSize = 0;
	Emulator_GetFileSize("bigfile.lst", &fileSize);
	char* fileBuffer = new char[fileSize];
	Emulator_ReadFileEM("bigfile.lst", fileBuffer, fileSize);
	char* fileName = NULL;
	long fileHash = 0;

	char* pLine = &fileBuffer[0];

	while (pLine != NULL && pLine < &fileBuffer[fileSize])
	{
		sscanf(pLine, "%x", &fileHash);
		fileName = &pLine[9];

		if (hash == fileHash)
		{
			Emulator_ReadFileEM(fileName, buff, size);

			break;
		}

		pLine = strchr(pLine, 0) + 1;
	}

	return;
}

void Emulator_OpenReadFPEM(const char* filePath, void* buff, int size)
{
	Emulator_ReadFileEM(filePath, buff, size);
}

void* Emulator_OpenFile(const char* filePath, const char* mode, int* outSize)
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
	}
	else
	{
		const char* urlName = "https://legacyofkain.co.uk";
		char fullName[256];
		int err = 0;

		sprintf(fullName, "%s/%s/%s", urlName, SHORT_GAME_NAME, filePath);
		
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
	}

	return outBuff;
}

void Emulator_GetFileSizeEM(const char* filePath, int* outSize)
{
	void* outBuff = Emulator_OpenFile(filePath, "rb", outSize);
	free(outBuff);
}
#endif