#include "UPGRADE.H"

#include <stdio.h>
#include <string>
#include <assert.h>
#include <iostream>
#include <vector>

#include "OBJECT.H"
#include "FILE.H"

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

void UPGRADE_DumpRaw(void* t, int size, int offset, FILE* f)
{
	if (f != NULL)
	{
		fseek(f, offset, SEEK_SET);
		fwrite(t, size, 1, f);
		fseek(f, 0, SEEK_END);
	}
}

void UPGRADE_DumpStructPointer(void* t, int size, int offset, FILE* f)
{
	if (f != NULL)
	{
		long long currentOffset = ftell(f);

		if (offset != -1)
		{
			fseek(f, offset, SEEK_SET);
			fwrite(&currentOffset, sizeof(long long), 1, f);
		}
		fseek(f, 0, SEEK_END);
	}
}

void UPGRADE_DumpStruct(void* t, int size, int offset, FILE* f)
{
	if (f != NULL)
	{
		long long currentOffset = ftell(f);

		fwrite(t, size, 1, f);
		if (offset != -1)
		{
			fseek(f, offset, SEEK_SET);
			fwrite(&currentOffset, sizeof(long long), 1, f);
		}
		fseek(f, 0, SEEK_END);
	}
}

void UPGRADE_DumpStructArray(void* t, int size, int offset, FILE* f)
{
	if (f != NULL)
	{
		long long currentOffset = ftell(f);

		fseek(f, offset, SEEK_SET);
		fwrite(&currentOffset, sizeof(long long), 1, f);
		fseek(f, 0, SEEK_END);
	}
}

void UPGRADE_DumpIndexed(void* t, int size, int offset, unsigned int arrayStart, int index, FILE* f)
{
	if (f != NULL)
	{
		long long currentOffset = arrayStart + (index * size);

		if (offset != -1)
		{
			fseek(f, offset, SEEK_SET);
			fwrite(&currentOffset, sizeof(long long), 1, f);
		}

		fseek(f, 0, SEEK_END);
	}
}

enum ObjectType UPGRADE_GetObjectType(char* objectName)
{
	if (!strcmp(objectName, "raziel__"))
	{
		return ObjectType::OBJ_PLAYER;
	}
	else if (!strcmp(objectName, "glphicon"))
	{
		return ObjectType::OBJ_GLYPH;
	}

	return ObjectType::OBJ_NONE;
}

void UPGRADE_Object(struct RedirectList* redirectList, long* data, long* baseAddr, unsigned int fileSize, char* filePath)
{
	struct Object* object;

	UPGRADE_Relocate(redirectList, data, baseAddr);

	object = (struct Object*)data;
	
	FILE* f = FILE_OpenWrite("MAIN.DRM");
	unsigned long long NULL_PTR = 0;
	std::vector<unsigned int> relocationTable;

	UPGRADE_DumpRaw(&object->oflags, sizeof(long), offsetof(Object64, oflags), f);
	UPGRADE_DumpRaw(&object->id, sizeof(short), offsetof(Object64, id), f);
	UPGRADE_DumpRaw(&object->sfxFileHandle, sizeof(short), offsetof(Object64, sfxFileHandle), f);
	UPGRADE_DumpRaw(&object->numModels, sizeof(short), offsetof(Object64, numModels), f);
	UPGRADE_DumpRaw(&object->numAnims, sizeof(short), offsetof(Object64, numAnims), f);

	UPGRADE_DumpRaw(&object->introDist, sizeof(short), offsetof(Object64, introDist), f);
	UPGRADE_DumpRaw(&object->vvIntroDist, sizeof(short), offsetof(Object64, vvIntroDist), f);
	UPGRADE_DumpRaw(&object->removeDist, sizeof(short), offsetof(Object64, removeDist), f);
	UPGRADE_DumpRaw(&object->vvRemoveDist, sizeof(short), offsetof(Object64, vvRemoveDist), f);

	UPGRADE_DumpRaw(&object->oflags2, sizeof(long), offsetof(Object64, oflags2), f);
	UPGRADE_DumpRaw(&object->sectionA, sizeof(short), offsetof(Object64, sectionA), f);
	UPGRADE_DumpRaw(&object->sectionB, sizeof(short), offsetof(Object64, sectionB), f);
	UPGRADE_DumpRaw(&object->sectionC, sizeof(short), offsetof(Object64, sectionC), f);

	UPGRADE_DumpRaw(&object->numberOfEffects, sizeof(short), offsetof(Object64, numberOfEffects), f);

	UPGRADE_DumpRaw(&object->vramSize, sizeof(struct VramSize), offsetof(Object64, vramSize), f);

	relocationTable.push_back(offsetof(Object64, script));
	UPGRADE_DumpStruct(object->script, 9, offsetof(Object64, script), f);
	
	relocationTable.push_back(offsetof(Object64, name));
	UPGRADE_DumpStruct(object->name, 9, offsetof(Object64, name), f);

	relocationTable.push_back(offsetof(Object64, modelList));
	UPGRADE_DumpStructPointer(object->modelList, sizeof(unsigned long long), offsetof(Object64, modelList), f);

	for (int i = 0; i < object->numModels; i++)
	{
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), ftell(f), f);
	}

	relocationTable.push_back(offsetof(Object64, animList));
	UPGRADE_DumpStructPointer(object->animList, sizeof(unsigned long long), offsetof(Object64, animList), f);

	long offsetModelList = ftell(f);

	for (int i = 0; i < object->numAnims; i++)
	{
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long),ftell(f), f);
	}

	long offsetAnimList = ftell(f);

	for (int i = 0; i < object->numModels; i++)
	{
		struct _Model* model = object->modelList[i];

		unsigned int offsetOfModel = ftell(f);

		relocationTable.push_back(offsetModelList - ((object->numModels - i) * sizeof(long long)));

		for (int i = 0; i < object->numModels; i++)
		{
			UPGRADE_DumpStructPointer(object->modelList[i], sizeof(_Model*), offsetModelList - ((object->numModels - i) * sizeof(long long)), f);
		}

		UPGRADE_DumpRaw(&model->numVertices, sizeof(long), offsetOfModel + offsetof(_Model64, numVertices), f);
		UPGRADE_DumpRaw(&model->numNormals, sizeof(long), offsetOfModel + offsetof(_Model64, numNormals), f);
		UPGRADE_DumpRaw(&model->numFaces, sizeof(long), offsetOfModel + offsetof(_Model64, numFaces), f);
		UPGRADE_DumpRaw(&model->numSegments, sizeof(long), offsetOfModel + offsetof(_Model64, numSegments), f);
		UPGRADE_DumpRaw(&model->maxRad, sizeof(short), offsetOfModel + offsetof(_Model64, maxRad), f);
		UPGRADE_DumpRaw(&model->pad, sizeof(short), offsetOfModel + offsetof(_Model64, pad), f);
		UPGRADE_DumpRaw(&model->maxRadSq, sizeof(long), offsetOfModel + offsetof(_Model64, maxRadSq), f);

		if (model->vertexList != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, vertexList));
			UPGRADE_DumpStruct(model->vertexList, sizeof(struct _MVertex) * model->numVertices, offsetOfModel + offsetof(_Model64, vertexList), f);
		}
		else
		{
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, vertexList), f);
		}

		if (model->faceList != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, faceList));
			UPGRADE_DumpStruct(model->faceList, sizeof(struct _MFace) * model->numFaces, offsetOfModel + offsetof(_Model64, faceList), f);
		}
		else
		{
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, faceList), f);
		}

		if (model->normalList != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, normalList));
			UPGRADE_DumpStruct(model->normalList, sizeof(struct _MVertex) * model->numNormals, offsetOfModel + offsetof(_Model64, normalList), f);
		}
		else
		{
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, normalList), f);
		}

		relocationTable.push_back(offsetOfModel + offsetof(_Model64, segmentList));
	
		unsigned int offsetOfTextures = -1;

		for (int j = 0; j < model->numSegments; j++)
		{
			struct _Segment* segment = &model->segmentList[j];

			unsigned int offsetOfSegment = ftell(f);

			UPGRADE_DumpRaw(&segment->flags, sizeof(long), offsetOfSegment + offsetof(_Segment64, flags), f);
			UPGRADE_DumpRaw(&segment->firstTri, sizeof(short), offsetOfSegment + offsetof(_Segment64, firstTri), f);
			UPGRADE_DumpRaw(&segment->lastTri, sizeof(short), offsetOfSegment + offsetof(_Segment64, lastTri), f);
			UPGRADE_DumpRaw(&segment->firstVertex, sizeof(short), offsetOfSegment + offsetof(_Segment64, firstVertex), f);
			UPGRADE_DumpRaw(&segment->lastVertex, sizeof(short), offsetOfSegment + offsetof(_Segment64, lastVertex), f);
			UPGRADE_DumpRaw(&segment->px, sizeof(short), offsetOfSegment + offsetof(_Segment64, px), f);
			UPGRADE_DumpRaw(&segment->py, sizeof(short), offsetOfSegment + offsetof(_Segment64, py), f);
			UPGRADE_DumpRaw(&segment->pz, sizeof(short), offsetOfSegment + offsetof(_Segment64, pz), f);
			UPGRADE_DumpRaw(&segment->parent, sizeof(short), offsetOfSegment + offsetof(_Segment64, parent), f);

			if (segment->hInfo != NULL)
			{
				relocationTable.push_back(offsetOfSegment + offsetof(_Segment64, hInfo));
				UPGRADE_DumpStructPointer(segment->hInfo, sizeof(struct _HInfo*), offsetOfSegment + offsetof(_Segment64, hInfo), f);
			}
			else
			{
				UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfSegment + offsetof(_Segment64, hInfo), f);
			}
		}
		

		if (model->startTextures != NULL && model->endTextures != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, startTextures));

			UPGRADE_DumpStructPointer(model->startTextures, sizeof(struct TextureMT3*), offsetOfModel + offsetof(_Model64, startTextures), f);

			if (offsetOfTextures == -1)
			{
				offsetOfTextures = ftell(f);
			}

			for (struct TextureMT3* currentTexture = model->startTextures; currentTexture < model->endTextures; currentTexture++)
			{
				UPGRADE_DumpStruct(currentTexture, sizeof(struct TextureMT3), -1, f);
			}

			relocationTable.push_back(offsetOfModel + offsetof(_Model64, endTextures));
			UPGRADE_DumpStructPointer(model->endTextures, sizeof(struct TextureMT3*), offsetOfModel + offsetof(_Model64, endTextures), f);
		}
		else
		{
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, startTextures), f);
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, endTextures), f);
		}

		if (model->aniTextures != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, aniTextures));
			UPGRADE_DumpStructPointer(model->aniTextures, sizeof(struct AniTex*), offsetOfModel + offsetof(_Model64, aniTextures), f);

			for (int j = 0; j < model->aniTextures->numAniTextues; j++)
			{
				unsigned int offsetOfAniTexture = ftell(f);

				struct AniTexInfo* aniTexture = &model->aniTextures->aniTexInfo[j];

				if (aniTexture->texture != NULL)
				{
					relocationTable.push_back(offsetOfModel + offsetof(_Model64, aniTextures));

					UPGRADE_DumpIndexed(aniTexture->texture, sizeof(struct TextureMT3), offsetOfAniTexture + offsetof(AniTexInfo64, texture), offsetOfTextures, aniTexture->texture - model->startTextures, f);
				}
				else
				{
					UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfAniTexture + offsetof(AniTexInfo64, texture), f);
				}
				UPGRADE_DumpRaw(&aniTexture->numFrames, sizeof(long), offsetOfAniTexture + offsetof(AniTexInfo64, numFrames), f);
				UPGRADE_DumpRaw(&aniTexture->speed, sizeof(long), offsetOfAniTexture + offsetof(AniTexInfo64, speed), f);


			}
		}
		else
		{
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, normalList), f);
		}
	}

	for (int i = 0; i < object->numAnims; i++)
	{
		struct _G2AnimKeylist_Type* anim = object->animList[i];
		struct _G2AnimKeylist_Type* nextAnim = object->animList[i + 1];

		if (i + 1 == object->numAnims)
		{
			nextAnim = (struct _G2AnimKeylist_Type*)object->data;
		}

		unsigned int offsetOfAnim = ftell(f);

		relocationTable.push_back(offsetAnimList - ((object->numAnims - i) * sizeof(long long)));

		UPGRADE_DumpStructPointer(object->animList[i], sizeof(unsigned long long), offsetAnimList - ((object->numAnims - i) * sizeof(long long)), f);

		UPGRADE_DumpRaw(&anim->sectionCount, sizeof(unsigned char), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, sectionCount), f);
		UPGRADE_DumpRaw(&anim->s0TailTime, sizeof(unsigned char), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, s0TailTime), f);
		UPGRADE_DumpRaw(&anim->s1TailTime, sizeof(unsigned char), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, s1TailTime), f);
		UPGRADE_DumpRaw(&anim->s2TailTime, sizeof(unsigned char), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, s2TailTime), f);
		UPGRADE_DumpRaw(&anim->keyCount, sizeof(unsigned short), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, keyCount), f);
		UPGRADE_DumpRaw(&anim->timePerKey, sizeof(short), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, timePerKey), f);
		UPGRADE_DumpRaw(&anim->pad00, sizeof(unsigned short), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, pad00), f);
		UPGRADE_DumpRaw(&anim->pad01, sizeof(short), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, pad01), f);
		UPGRADE_DumpRaw(&anim->pad10, sizeof(unsigned short), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, pad10), f);
		UPGRADE_DumpRaw(&anim->pad11, sizeof(short), offsetOfAnim + offsetof(struct _G2AnimKeylist_Type64, pad11), f);

		if (anim->fxList != NULL)
		{
			relocationTable.push_back(offsetOfAnim + offsetof(_G2AnimKeylist_Type64, fxList));

			UPGRADE_DumpStructPointer(anim->fxList, sizeof(_G2AnimFxHeader_Type*), offsetOfAnim + offsetof(_G2AnimKeylist_Type64, fxList), f);
		}
		else
		{
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfAnim + offsetof(_G2AnimKeylist_Type64, fxList), f);
		}

		for (int j = 0; j < 3; j++)
		{
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfAnim + offsetof(_G2AnimKeylist_Type64, sectionData_[j]), f);
		}

		char* pFirstSection = (char*)(anim + 1);
		char* pNextSection = (char*)anim->sectionData[0];

		UPGRADE_DumpRaw(pFirstSection, pNextSection - pFirstSection, ftell(f), f);

		for (int j = 0; j < anim->sectionCount; j++)
		{
			unsigned short* pCurrentSection = anim->sectionData[j];
			unsigned short* pNextSection = anim->sectionData[j + 1];

			if (pCurrentSection != NULL)
			{
				if (j + 1 == anim->sectionCount)
				{
					pNextSection = (unsigned short*)nextAnim;
				}

				relocationTable.push_back(offsetOfAnim + offsetof(_G2AnimKeylist_Type64, sectionData_[j]));
				UPGRADE_DumpStruct(anim->sectionData[j], sizeof(unsigned short*), offsetOfAnim + offsetof(_G2AnimKeylist_Type64, sectionData_[j]), f);

				unsigned int sectionSize = (pNextSection - pCurrentSection) * sizeof(unsigned short);
				UPGRADE_DumpRaw(pCurrentSection, sectionSize, ftell(f), f);
			}
		}

		unsigned int offsetOfEnd = ftell(f);

		fseek(f, offsetOfEnd, SEEK_SET);
	}

	enum ObjectType objectType = UPGRADE_GetObjectType(object->name);

	switch (objectType)
	{
	case ObjectType::OBJ_PLAYER:
	{
		struct RazielData* data = (struct RazielData*)object->data;


		int testing = 0;
		testing++;
		break;
	}
	case ObjectType::OBJ_GLYPH:
	{
		struct _GlyphTuneData* data = (struct _GlyphTuneData*)object->data;

		relocationTable.push_back(offsetof(Object64, data));
		UPGRADE_DumpStruct(data, sizeof(struct _GlyphTuneData), offsetof(Object64, data), f);

		break;
	}
	default:
		assert(false);
		break;
	}

	if (object->relocModule != NULL)
	{
		relocationTable.push_back(offsetof(Object64, relocModule));

		UPGRADE_DumpStruct(object->relocModule, 0, offsetof(Object64, relocModule), f);
	}

	if (object->relocList != NULL)
	{
		relocationTable.push_back(offsetof(Object64, relocList));

		UPGRADE_DumpStruct(object->relocList, 0, offsetof(Object64, relocList), f);
	}

	FILE_Close(f);

	f = FILE_OpenWrite("RTBL.TBL");

	if (relocationTable.size())
	{
		long relocCount = relocationTable.size();
		fwrite(&relocCount, sizeof(long), 1, f);
		fwrite(&relocationTable[0], sizeof(unsigned int) * relocationTable.size(), 1, f);
		
		unsigned int tableSize = (relocationTable.size() + 512 < 0) ? (relocationTable.size() + 1023) : (relocationTable.size() + 512);
		tableSize /= 512;
		tableSize *= 512;

		fseek(f, tableSize * sizeof(unsigned int) - 1, SEEK_SET);
		char dummy = 0;
		fwrite(&dummy, sizeof(dummy), 1, f);
	}

	FILE_Close(f);
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
