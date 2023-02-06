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

void UPGRADE_Delocate(long* baseAddr, long* newBaseAddr)
{
	for (int i = 0; i < g_redirectList.size(); i++)
	{
		g_redirectList[i].pDTPConvertedFile[0] += g_redirectList[i].incr * sizeof(unsigned int);
		//g_redirectList[i].pDTPConvertedFile[0] -= (long)baseAddr + 0x800;
	}
}

void UPGRADE_Object(struct RedirectList* redirectList, long* data, long* baseAddr, unsigned int fileSize, char* filePath)
{
	struct Object* object;

	UPGRADE_Relocate(redirectList, data, baseAddr);

	FILE_OpenWrite("TEMP.DRM");

	object = (struct Object*)data;
	
	UPGRADE_BaseDump<Object>::decode(*object);

	for (int i = 0; i < object->numModels; i++)
	{
		UPGRADE_BaseDump<_Model>::decode(*object->modelList[i]);
	}
#if 0
	for (int i = 0; i < object->numModels; i++)
	{
		UPGRADE_BaseDump<_Model>::decode(*object->modelList[i]);
		
		for (int j = 0; j < object->modelList[i]->numVertices; j++)
		{
			UPGRADE_BaseDump<_MVertex>::decode(object->modelList[i]->vertexList[j]);
		}

		for (int j = 0; j < object->modelList[i]->numNormals; j++)
		{
			UPGRADE_BaseDump<_SVectorNoPad>::decode(object->modelList[i]->normalList[j]);
		}

		for (int j = 0; j < object->modelList[i]->numFaces; j++)
		{
			UPGRADE_BaseDump<_MFace>::decode(object->modelList[i]->faceList[j]);
		}

		for (int j = 0; j < object->modelList[i]->numSegments; j++)
		{
			UPGRADE_BaseDump<_Segment>::decode(object->modelList[i]->segmentList[j]);
		}

		if (object->modelList[i]->aniTextures != NULL)
		{
			UPGRADE_BaseDump<AniTex>::decode(*object->modelList[i]->aniTextures);

			for (int j = 0; j < object->modelList[i]->aniTextures->numAniTextues; j++)
			{
				UPGRADE_BaseDump<AniTexInfo>::decode(object->modelList[i]->aniTextures[i].aniTexInfo);
			}
		}
	}
#endif
	char* newFileData = new char[fileSize + (g_numPointerTypes * sizeof(unsigned int))];
	memset(newFileData, 0, fileSize + (g_numPointerTypes * sizeof(unsigned int)));
	g_pCurr = &newFileData[0];
	g_baseAddr = baseAddr;

	int lowestRelocOffset = 0xFFFFFF;
	UPGRADE_DTPDump<Object>::decode(*object, &lowestRelocOffset);

#if 0
	g_pCurr = &newFileData[lowestRelocOffset];

	for (int i = 0; i < object->numModels; i++)
	{
		lowestRelocOffset = 0;
		UPGRADE_DTPDump<DTPList>::decode(*(DTPList*)object->modelList[i], &lowestRelocOffset);
		UPGRADE_DTPDump<_Model>::decode(*object->modelList[i], &lowestRelocOffset);
	}

#if 0
	for (int i = 0; i < object->numModels; i++)
	{
		UPGRADE_DTPDump<_Model>::decode(*object->modelList[i]);

		for (int j = 0; j < object->modelList[i]->numVertices; j++)
		{
			UPGRADE_DTPDump<_MVertex>::decode(object->modelList[i]->vertexList[j]);
		}

		for (int j = 0; j < object->modelList[i]->numNormals; j++)
		{
			UPGRADE_DTPDump<_SVectorNoPad>::decode(object->modelList[i]->normalList[j]);
		}

		for (int j = 0; j < object->modelList[i]->numFaces; j++)
		{
			UPGRADE_DTPDump<_MFace>::decode(object->modelList[i]->faceList[j]);
		}

		for (int j = 0; j < object->modelList[i]->numSegments; j++)
		{
			UPGRADE_DTPDump<_Segment>::decode(object->modelList[i]->segmentList[j]);
		}

		if (object->modelList[i]->aniTextures != NULL)
		{
			UPGRADE_DTPDump<AniTex>::decode(*object->modelList[i]->aniTextures);

			for (int j = 0; j < object->modelList[i]->aniTextures->numAniTextues; j++)
			{
				UPGRADE_DTPDump<AniTexInfo>::decode(object->modelList[i]->aniTextures[i].aniTexInfo);
			}
		}
	}
#endif
#endif

	UPGRADE_Delocate(baseAddr, (long*)newFileData);

	fwrite(newFileData, fileSize + (g_numPointerTypes * sizeof(unsigned int)), 1, g_fileHandle);
	fclose(g_fileHandle);
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

UPGRADE_RedirectList* UPGRADE_FindRedirectList(long* DTP)
{
	for (int i = 0; i < g_redirectList.size(); i++)
	{
		UPGRADE_RedirectList* rdList = &g_redirectList[i];

		if (rdList->pDTP == DTP)
		{
			return rdList;
		}
	}

	return NULL;
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
