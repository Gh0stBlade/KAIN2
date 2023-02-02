#include "UPGRADE.H"

#include <stdio.h>
#include <string>
#include <assert.h>

char* UPGRADE_ReadFile(char* filePath, unsigned int* outSize)
{
	FILE* f = fopen(filePath, "rb");

	if (f != NULL)
	{
		fseek(f, 0, SEEK_END);
		
		long fileSize = ftell(f);
		outSize[0] = fileSize;
		
		fseek(f, 0, SEEK_SET);

		char* fileData = new char[fileSize];
		
		fread(fileData, fileSize, 1, f);
		
		fclose(f);
	
		return fileData;
	}

	return NULL;
}

void UPGRADE_SaveDRM(char* fileData, unsigned int fileSize, char* filePath)
{
	FILE* f = fopen(filePath, "wb+");

	if (f != NULL)
	{
		fwrite(fileData, 1, fileSize, f);
		fclose(f);
	}
}

void UPGRADE_Pointers(struct RedirectList* redirectList, long* data, long* baseAddr, unsigned int fileSize, char* filePath)
{
	long* rdList;
	int i;
	long* handle;
	long* relocHandle;

	rdList = redirectList->data;

	char* newFileData = new char[fileSize + (redirectList->numPointers * sizeof(unsigned int))];
	memset(newFileData, 0, fileSize + (redirectList->numPointers * sizeof(unsigned int)));

	char* pCopyDst = newFileData;
	char* pCopySrc = (char*)redirectList;

	//Copy in redirect lists.
	memcpy(pCopyDst, pCopySrc, sizeof(unsigned int));
	pCopyDst += sizeof(unsigned int);

	//Calc new redirect list
	long* relocList2[4096];
	memset(relocList2, 0, sizeof(relocList2));
	long* rdList2 = rdList;
	
	for (i = 0; i < redirectList->numPointers; i++, rdList2++)
	{
		handle = (long*)((char*)data + *rdList2);
		relocHandle = (long*)((char*)data + *handle);

		if (i > 0)
		{
			*rdList2 += 4;
		}
	}
	pCopySrc = (char*)rdList;
	memcpy(pCopyDst, pCopySrc, (redirectList->numPointers * sizeof(unsigned int)));

	int tableSize = (redirectList->numPointers + 512 < 0) ? (redirectList->numPointers + 1023) : (redirectList->numPointers + 512);
	tableSize /= 512;
	tableSize *= 512;

	pCopyDst = newFileData + (tableSize * sizeof(unsigned int));
	pCopySrc = (char*)data;

	long* relocList[4096];
	memset(relocList, 0, sizeof(relocList));

	for (i = 0; i < redirectList->numPointers; i++, rdList++)
	{
		handle = (long*)((char*)data + *rdList);
		relocHandle = (long*)((char*)data + *handle);

		for (int j = 0; j < redirectList->numPointers; j++)
		{
			if (relocList[j] != NULL && relocList[j] < relocHandle)
			{
				*handle += 4;
			}
		}

		memcpy(pCopyDst, pCopySrc, ((char*)handle - pCopySrc) + sizeof(unsigned int));
	
		pCopyDst += ((char*)handle - pCopySrc) + (sizeof(unsigned int) * 2);
		pCopySrc += ((char*)handle - pCopySrc) + sizeof(unsigned int);
	}

	memcpy(pCopyDst, pCopySrc, fileSize - (pCopySrc - (char*)baseAddr));

	UPGRADE_SaveDRM(newFileData, fileSize + (redirectList->numPointers * sizeof(unsigned int)), filePath);

	delete[] newFileData;
}

void UPGRADE_ProcessRedirectList(long* data, unsigned int fileSize, char* filePath)
{
	long* lastMoveDest;
	long tableSize;
	struct RedirectList redirectListX;
	struct RedirectList* redirectList;

	redirectList = &redirectListX;

	redirectList->data = data + 1;

	redirectList->numPointers = data[0];

	tableSize = (redirectList->numPointers + 512 < 0) ? (redirectList->numPointers + 1023) : (redirectList->numPointers + 512);
	tableSize /= 512;
	tableSize *= 512;

	UPGRADE_Pointers(redirectList, &data[tableSize], data, fileSize, filePath);

	return;
}

void UPGRADE_OpenDRM(char* drmFilePath)
{
	unsigned int outSize = 0;
	char* pFileData = UPGRADE_ReadFile(drmFilePath, &outSize);

	if (pFileData != NULL)
	{
		UPGRADE_ProcessRedirectList((long*)pFileData, outSize, drmFilePath);
	}
}
