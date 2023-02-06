#include "UPGRADE.H"

#include <stdio.h>
#include <string>
#include <assert.h>
#include <iostream>

#include "OBJECT.H"

#include "boost/pfr/core.hpp"
#include <sstream>

enum DrmFileType : int
{
	NONE,
	OBJECT,
	AREA,
	MUSIC,
	SFX,
};

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

void UPGRADE_Relocate(struct RedirectList* redirectList, long* data, long* baseAddr)
{
	long* rdList;
	int i;
	uintptr_t* handle;

	rdList = redirectList->data;

	for (i = redirectList->numPointers; i != 0; i--, rdList++)
	{
		handle = (uintptr_t*)((char*)data + *rdList);
		*handle += (uintptr_t)((char*)data);
	}
}

int dec_indents = 0;

void UPGRADE_ObjectNew(struct RedirectList* redirectList, long* data, long* baseAddr, unsigned int fileSize, char* filePath)
{
	struct Object* object;
	UPGRADE_Relocate(redirectList, data, baseAddr);

	FILE_OpenWrite("TEMP.DRM");

	int numPointerTypes = 0;// UPGRADE_CountPointerTypes<Object>::
}

void UPGRADE_Object(struct RedirectList* redirectList, long* data, long* baseAddr, unsigned int fileSize, char* filePath)
{
	struct Object* object;
	UPGRADE_Relocate(redirectList, data, baseAddr);

	FILE_OpenWrite("TEMP.DRM");

	//Object Pass 1
	FILE_PushSeek(2048);
	object = (struct Object*)data;
	UPGRADE_BaseDump<Object>::decode(*object);
	FILE_PopSeek();

	//Object Pass 2
	FILE_PushSeek(2048);
	object = (struct Object*)data;
	UPGRADE_DTPDump<Object>::decode(*object);
	FILE_PopSeek();

	//Models
	for (int i = 0; i < object->numModels; i++)
	{
		FILE_PushSeekEnd();
		UPGRADE_BaseDump<_Model>::decode(*object->modelList[i]);
		
		for (int j = 0; j < object->modelList[i]->numVertices; j++)
		{
			FILE_PushSeekEnd();
			UPGRADE_BaseDump<_MVertex>::decode(object->modelList[i]->vertexList[j]);
			FILE_PopSeek();
		}

		UPGRADE_DTPDump<_Model>::decode(*object->modelList[i]);
		FILE_PopSeek();
	}

	fseek(g_fileHandle, 0, SEEK_SET);
	fwrite(&g_lastRelocIndex, sizeof(unsigned int), 1, g_fileHandle);
	fclose(g_fileHandle);
	g_fileHandle = NULL;
}

void UPGRADE_ProcessRedirectList(long* data, unsigned int fileSize, char* filePath, DrmFileType fileType)
{
	long tableSize;
	struct RedirectList redirectListX;
	struct RedirectList* redirectList;

	redirectList = &redirectListX;

	redirectList->data = data + 1;

	redirectList->numPointers = data[0];

	tableSize = (redirectList->numPointers + 512 < 0) ? (redirectList->numPointers + 1023) : (redirectList->numPointers + 512);
	tableSize /= 512;
	tableSize *= 512;

	switch (fileType)
	{
	case OBJECT:
		UPGRADE_Object(redirectList, &data[tableSize], data, fileSize, filePath);
		break;
	}
	//UPGRADE_Pointers(redirectList, &data[tableSize], data, fileSize, filePath);

	return;
}

enum DrmFileType UPGRADE_GetFileType(const char* drmFilePath)
{
	if (strstr(drmFilePath, "kain2\\object\\") != NULL)
	{
		return DrmFileType::OBJECT;
	}

	if (strstr(drmFilePath, "kain2\\area\\") != NULL)
	{
		return DrmFileType::AREA;
	}

	if (strstr(drmFilePath, "kain2\\music\\") != NULL)
	{
		return DrmFileType::MUSIC;
	}

	if (strstr(drmFilePath, "kain2\\sfx\\") != NULL)
	{
		return DrmFileType::SFX;
	}

	return DrmFileType::NONE;
}

void UPGRADE_OpenDRM(char* drmFilePath)
{
	unsigned int outSize = 0;
	char* pFileData = UPGRADE_ReadFile(drmFilePath, &outSize);

	if (pFileData != NULL)
	{
		DrmFileType fileType = UPGRADE_GetFileType(drmFilePath);

		if (fileType != DrmFileType::NONE)
		{
			UPGRADE_ProcessRedirectList((long*)pFileData, outSize, drmFilePath, fileType);
		}
	}
}
