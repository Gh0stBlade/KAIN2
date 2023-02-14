#include "UPGRADE.H"

#include <stdio.h>
#include <string>
#include <assert.h>
#include <iostream>
#include <vector>

#include "OBJECT.H"
#include "FILE.H"
#include "SOUND.H"
#include "LOCALS.H"

enum DrmFileType : int
{
	NONE,
	OBJECT,
	AREA,
	MUSIC,
	SFX,
	TBL
};

char HashExtensions[7][4] = { "drm", "crm", "tim", "smp", "snd", "smf", "snf" };

unsigned int UPGRADE_HashName(char* string)
{
	long sum;
	long _xor;	// visual studio doesn't like 'xor' for whatever reason
	long length;
	long ext;
	char c;
	long strl;
	long endPos;
	long i;
	char* pos;

	sum = 0;
	_xor = 0;
	length = 0;
	ext = 0;

	strl = strlen(string) - 1;
	pos = strchr(string, '.');
	endPos = 0;

	if (pos != NULL)
	{
		pos++;

		for (i = 0; i < 7; i++)
		{
#if defined(PSXPC_VERSION)
			if (_strcmpi(pos, &HashExtensions[i][0]) == 0)
#else
			if (strcmpi(pos, &HashExtensions[i][0]) == 0)
#endif
			{
				ext = i;
				break;
			}
		}

		if (i < 7)
		{
			strl -= 4;
		}

		if (strl >= endPos)
		{
			for (; strl >= endPos; strl--)
			{
				c = string[strl];

				if (c != '\\')
				{
					if ((unsigned)(c - 0x61) < 0x1A)
					{
						c &= 0xDF;
					}

					c = (c - 0x1A) & 0xFF;
					sum = sum + c;
					_xor = _xor ^ (c * length++);
				}
			}
		}
	}

	return (length << 27) | (sum << 15) | (_xor << 3) | ext;
}

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

void UPGRADE_DumpStructPointer(int offset, FILE* f)
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

void UPGRADE_DumpStructArray(int size, int offset, FILE* f)
{
	if (f != NULL)
	{
		long long currentOffset = ftell(f);

		fseek(f, offset, SEEK_SET);
		fwrite(&currentOffset, sizeof(long long), 1, f);
		fseek(f, 0, SEEK_END);
	}
}

void UPGRADE_DumpStructArrayArray(void* t, int size, int offset, int offset_to, FILE* f)
{
	if (f != NULL)
	{
		long long currentOffset = offset_to;

		fseek(f, offset, SEEK_SET);
		fwrite(&currentOffset, sizeof(long long), 1, f);
		fseek(f, 0, SEEK_END);
	}
}

void UPGRADE_DumpIndexed(int size, int offset, unsigned int arrayStart, int index, FILE* f)
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
		printf("Converting player!\n");
		return ObjectType::OBJ_PLAYER;
	}
	else if (!strcmp(objectName, "glphicon"))
	{
		printf("Converting glyph!\n");
		return ObjectType::OBJ_GLYPH;
	}
	else if (!strcmp(objectName, "mcardx__") || !strcmp(objectName, "cinemax_"))
	{
		printf("Converting overlay!\n");
		return ObjectType::OBJ_OVERLAY;
	}
	else
	{
		printf("Failed to detect type: %s\n", objectName);
		return ObjectType::OBJ_NONE;
	}
}

void UPGRADE_Locals(long* data, unsigned int fileSize, char* filePath)
{
	struct LocalizationHeader* localsHeader = (struct LocalizationHeader*)data;
	unsigned int* stringOffsets = (unsigned int*)(localsHeader + 1);
	char* stringTable = (char*)(localsHeader + 1) + localsHeader->numStrings * sizeof(unsigned int);

	unsigned short* xaTable = (unsigned short*)((char*)localsHeader + localsHeader->XATableOffset);

	FILE* f = FILE_OpenWrite(filePath);

	UPGRADE_DumpRaw(localsHeader, sizeof(struct LocalizationHeader), ftell(f), f);

	for (int i = 0; i < localsHeader->numStrings; i++)
	{
		long long actualOffset = stringOffsets[i] + (localsHeader->numStrings * sizeof(unsigned int));

		UPGRADE_DumpRaw(&actualOffset, sizeof(long long), ftell(f), f);
	}

	UPGRADE_DumpRaw(stringTable, (char*)xaTable - (char*)stringTable, ftell(f), f);

	localsHeader->XATableOffset = ftell(f);

	UPGRADE_DumpRaw(xaTable, localsHeader->numXAfiles * sizeof(unsigned short), ftell(f), f);

	FILE_Seek(f, 0, SEEK_SET);

	UPGRADE_DumpRaw(localsHeader, sizeof(struct LocalizationHeader), ftell(f), f);

	FILE_Close(f);
}

void UPGRADE_SNF(long* data, unsigned int fileSize, char* filePath)
{
	struct _AadDynSfxFileHdr* sfxHeader = (struct _AadDynSfxFileHdr*)data;
	
#if !defined(_WIN64)
	FILE* f = FILE_OpenWrite(filePath);

	unsigned long long NULL_PTR = 0;

	UPGRADE_DumpRaw(&sfxHeader->snfID, sizeof(unsigned long), offsetof(_AadDynSfxFileHdr64, snfID), f);
	UPGRADE_DumpRaw(&sfxHeader->snfVersion, sizeof(unsigned short), offsetof(_AadDynSfxFileHdr64, snfVersion), f);
	UPGRADE_DumpRaw(&sfxHeader->uniqueID, sizeof(unsigned short), offsetof(_AadDynSfxFileHdr64, uniqueID), f);
	UPGRADE_DumpRaw(&sfxHeader->handle, sizeof(unsigned short), offsetof(_AadDynSfxFileHdr64, handle), f);

	UPGRADE_DumpRaw(&sfxHeader->numSfxInFile, sizeof(unsigned short), offsetof(_AadDynSfxFileHdr64, numSfxInFile), f);
	UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetof(_AadDynSfxFileHdr64, prevDynSfxFile), f);
	UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetof(_AadDynSfxFileHdr64, nextDynSfxFile), f);

	unsigned short* sfxData = (unsigned short*)(sfxHeader + 1);

	for (int i = 0; i < sfxHeader->numSfxInFile; i++, sfxData++)
	{
		UPGRADE_DumpRaw(sfxData, sizeof(unsigned short), ftell(f), f);
	}

	unsigned short term = 0;
	UPGRADE_DumpRaw(&term, sizeof(unsigned short), ftell(f), f);

	FILE_Close(f);
#endif
}

void UPGRADE_Object(struct RedirectList* redirectList, long* data, long* baseAddr, unsigned int fileSize, char* filePath)
{
	struct Object* object;

	UPGRADE_Relocate(redirectList, data, baseAddr);

	object = (struct Object*)data;
#if !defined(_WIN64)
	FILE* f = FILE_OpenWrite("TEMP.DRM");
#else
	FILE* f = NULL;
#endif

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
	UPGRADE_DumpStructPointer(offsetof(Object64, modelList), f);

	if((object->oflags & 0x4))
	{
		//assert(false);
		//TODO object->soundData
	}

	for (int i = 0; i < object->numModels; i++)
	{
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), FILE_GetOffset(f), f);
	}

	relocationTable.push_back(offsetof(Object64, animList));
	UPGRADE_DumpStructPointer(offsetof(Object64, animList), f);

	long offsetModelList = FILE_GetOffset(f);

	for (int i = 0; i < object->numAnims; i++)
	{
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), FILE_GetOffset(f), f);
	}

	long offsetAnimList = FILE_GetOffset(f);

	for (int i = 0; i < object->numModels; i++)
	{
		struct _Model* model = object->modelList[i];

		unsigned int offsetOfModel = FILE_GetOffset(f);

		relocationTable.push_back(offsetModelList - ((object->numModels - i) * sizeof(long long)));

		for (int i = 0; i < object->numModels; i++)
		{
			UPGRADE_DumpStructPointer(offsetModelList - ((object->numModels - i) * sizeof(long long)), f);
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

			unsigned int offsetOfSegment = FILE_GetOffset(f);

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
				UPGRADE_DumpStructPointer(offsetOfSegment + offsetof(_Segment64, hInfo), f);
			}
			else
			{
				UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfSegment + offsetof(_Segment64, hInfo), f);
			}
		}
		

		if (model->startTextures != NULL && model->endTextures != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, startTextures));

			UPGRADE_DumpStructPointer(offsetOfModel + offsetof(_Model64, startTextures), f);

			if (offsetOfTextures == -1)
			{
				offsetOfTextures = FILE_GetOffset(f);
			}

			for (struct TextureMT3* currentTexture = model->startTextures; currentTexture < model->endTextures; currentTexture++)
			{
				UPGRADE_DumpStruct(currentTexture, sizeof(struct TextureMT3), -1, f);
			}

			relocationTable.push_back(offsetOfModel + offsetof(_Model64, endTextures));
			UPGRADE_DumpStructPointer(offsetOfModel + offsetof(_Model64, endTextures), f);
		}
		else
		{
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, startTextures), f);
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, endTextures), f);
		}

		if (model->aniTextures != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, aniTextures));
			UPGRADE_DumpStructPointer(offsetOfModel + offsetof(_Model64, aniTextures), f);

			for (int j = 0; j < model->aniTextures->numAniTextues; j++)
			{
				unsigned int offsetOfAniTexture = FILE_GetOffset(f);

				struct AniTexInfo* aniTexture = &model->aniTextures->aniTexInfo[j];

				if (aniTexture->texture != NULL)
				{
					relocationTable.push_back(offsetOfModel + offsetof(_Model64, aniTextures));

					UPGRADE_DumpIndexed(sizeof(struct TextureMT3), offsetOfAniTexture + offsetof(AniTexInfo64, texture), offsetOfTextures, aniTexture->texture - model->startTextures, f);
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

		unsigned int offsetOfAnim = FILE_GetOffset(f);

		relocationTable.push_back(offsetAnimList - ((object->numAnims - i) * sizeof(long long)));

		UPGRADE_DumpStructPointer( offsetAnimList - ((object->numAnims - i) * sizeof(long long)), f);

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
			printf("FIXME: anim->fxList untested!\n");
			//assert(false);
			relocationTable.push_back(offsetOfAnim + offsetof(_G2AnimKeylist_Type64, fxList));

			UPGRADE_DumpStructPointer(offsetOfAnim + offsetof(_G2AnimKeylist_Type64, fxList), f);
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

		UPGRADE_DumpRaw(pFirstSection, pNextSection - pFirstSection, FILE_GetOffset(f), f);

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
				UPGRADE_DumpRaw(pCurrentSection, sectionSize, FILE_GetOffset(f), f);
			}
		}

		unsigned int offsetOfEnd = FILE_GetOffset(f);

		FILE_Seek(f, offsetOfEnd, SEEK_SET);
	}

	enum ObjectType objectType = UPGRADE_GetObjectType(object->name);

	long objectDataOffset = FILE_GetOffset(f);
	relocationTable.push_back(offsetof(Object64, data));
	UPGRADE_DumpStructPointer(offsetof(Object64, data), f);

	switch (objectType)
	{
	case ObjectType::OBJ_PLAYER:
	{
		struct RazielData* data = (struct RazielData*)object->data;

		UPGRADE_DumpRaw(&data->version, sizeof(unsigned long), objectDataOffset + offsetof(RazielData64, version), f);
		UPGRADE_DumpRaw(&data->nonBurningRibbonStartColor, sizeof(unsigned long), objectDataOffset + offsetof(RazielData64, nonBurningRibbonStartColor), f);
		UPGRADE_DumpRaw(&data->nonBurningRibbonEndColor, sizeof(unsigned long), objectDataOffset + offsetof(RazielData64, nonBurningRibbonEndColor), f);
		
		///@FIXME Need data example
#if 0
		struct __Idle*** idleListPtrArray = data->idleList;

		while (idleListPtrArray++ != NULL)
		{

		}
#else
		//idleList
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), objectDataOffset + offsetof(RazielData64, idleList), f);
		//attackList
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), objectDataOffset + offsetof(RazielData64, attackList), f);
		//throwList
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), objectDataOffset + offsetof(RazielData64, throwList), f);
		//virtualAnimations
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), objectDataOffset + offsetof(RazielData64, virtualAnimations), f);
		//virtualAnimSingle
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), objectDataOffset + offsetof(RazielData64, virtualAnimSingle), f);
		//stringAnimations
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), objectDataOffset + offsetof(RazielData64, stringAnimations), f);

		UPGRADE_DumpRaw(&data->throwFadeValue, sizeof(short), objectDataOffset + offsetof(RazielData64, throwFadeValue), f);
		UPGRADE_DumpRaw(&data->throwFadeInRate, sizeof(short), objectDataOffset + offsetof(RazielData64, throwFadeInRate), f);
		UPGRADE_DumpRaw(&data->throwFadeOutRate, sizeof(int), objectDataOffset + offsetof(RazielData64, throwFadeOutRate), f);
		UPGRADE_DumpRaw(&data->throwManualDistance, sizeof(int), objectDataOffset + offsetof(RazielData64, throwManualDistance), f);

		UPGRADE_DumpRaw(&data->healthMaterialRate, sizeof(short), objectDataOffset + offsetof(RazielData64, healthMaterialRate), f);
		UPGRADE_DumpRaw(&data->healthSpectralRate, sizeof(short), objectDataOffset + offsetof(RazielData64, healthSpectralRate), f);
		UPGRADE_DumpRaw(&data->healthInvinciblePostHit, sizeof(short), objectDataOffset + offsetof(RazielData64, healthInvinciblePostHit), f);
		UPGRADE_DumpRaw(&data->healthInvinciblePostShunt, sizeof(short), objectDataOffset + offsetof(RazielData64, healthInvinciblePostShunt), f);
		UPGRADE_DumpRaw(&data->forceMinPitch, sizeof(short), objectDataOffset + offsetof(RazielData64, forceMinPitch), f);
		UPGRADE_DumpRaw(&data->forceMaxPitch, sizeof(short), objectDataOffset + offsetof(RazielData64, forceMaxPitch), f);
		UPGRADE_DumpRaw(&data->forceMinVolume, sizeof(short), objectDataOffset + offsetof(RazielData64, forceMinVolume), f);
		UPGRADE_DumpRaw(&data->forceMaxVolume, sizeof(short), objectDataOffset + offsetof(RazielData64, forceMaxVolume), f);
		UPGRADE_DumpRaw(&data->forceRampTime, sizeof(unsigned long), objectDataOffset + offsetof(RazielData64, forceRampTime), f);

		UPGRADE_DumpRaw(&data->SwimPhysicsFallDamping, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsFallDamping), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsWaterFrequency, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsWaterFrequency), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsWaterAmplitude, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsWaterAmplitude), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsUnderDeceleration, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsUnderDeceleration), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsUnderKickVelocity, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsUnderKickVelocity), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsUnderKickAccel, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsUnderKickAccel), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsUnderVelocity, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsUnderVelocity), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsUnderKickDecel, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsUnderKickDecel), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsUnderStealthAdjust, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsUnderStealthAdjust), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsCoilVelocity, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsCoilVelocity), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsCoilDecelerationIn, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsCoilDecelerationIn), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsCoilDecelerationOut, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsCoilDecelerationOut), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsShotVelocity, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsShotVelocity), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsShotAccelerationIn, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsShotAccelerationIn), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsShotAccelerationOut, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsShotAccelerationOut), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsSurfVelocity, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsSurfVelocity), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsSurfAccelerationIn, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsSurfAccelerationIn), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsSurfAccelerationOut, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsSurfAccelerationOut), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsSurfKickVelocity, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsSurfKickVelocity), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsSurfKickAccel, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsSurfKickAccel), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsSurfMinRotation, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsSurfMinRotation), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsSurfMaxRotation, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsSurfMaxRotation), f);
		UPGRADE_DumpRaw(&data->SwimPhysicsSurfKickDecel, sizeof(int), objectDataOffset + offsetof(RazielData64, SwimPhysicsSurfKickDecel), f);
#endif

		relocationTable.push_back(objectDataOffset + offsetof(RazielData64, attackList));
		UPGRADE_DumpStructPointer(objectDataOffset + offsetof(RazielData64, attackList), f);

		struct __AttackItem*** attackListPtrPtrPtr = data->attackList;
		int attackListPtrPtrPtrCount = 0;
		int attackListPtrPtrCount[4096];
		memset(attackListPtrPtrCount, 0, sizeof(attackListPtrPtrCount));

		long offsetAttackListPtrPtrPtr = -1;
		long offsetAttackListPtrPtr[4096];
		memset(offsetAttackListPtrPtr, 0, sizeof(offsetAttackListPtrPtr));

		//Get counts
		while (*attackListPtrPtrPtr)
		{
			struct __AttackItem** attackListPtrPtr = *attackListPtrPtrPtr++;

			while (*attackListPtrPtr)
			{
				struct __AttackItem* attackListPtr = *attackListPtrPtr++;

				attackListPtrPtrCount[attackListPtrPtrPtrCount]++;
			}

			attackListPtrPtrPtrCount++;
		}

		//Store offsets
		offsetAttackListPtrPtrPtr = FILE_GetOffset(f);
		for (int i = 0; i < attackListPtrPtrPtrCount; i++)
		{
			offsetAttackListPtrPtr[i] = ftell(f);

			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetAttackListPtrPtrPtr + (i * sizeof(long long)), f);
		}

		//Term
		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), FILE_GetOffset(f), f);

		//Store offsets
		for (int i = 0; i < attackListPtrPtrPtrCount; i++)
		{
			int offsetAttackListPtrPtrReal = ftell(f);

			for (int j = 0; j < attackListPtrPtrCount[i]; j++)
			{
				UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), ftell(f), f);
			}

			//Term
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), ftell(f), f);

			for (int j = 0; j < attackListPtrPtrCount[i]; j++)
			{
				if (j == 0)
				{
					relocationTable.push_back(offsetAttackListPtrPtrPtr + (i * sizeof(long long)));
					UPGRADE_DumpStructArrayArray(data->attackList[i], sizeof(struct __AttackItem**), offsetAttackListPtrPtrPtr + (i * sizeof(long long)), offsetAttackListPtrPtrReal + (j * sizeof(long long)), f);
				}

				relocationTable.push_back(offsetAttackListPtrPtrReal + (j * sizeof(long long)));
				UPGRADE_DumpStruct(data->attackList[i][j], sizeof(struct __AttackItem), offsetAttackListPtrPtrReal + (j * sizeof(long long)), f);
			}
		}

		relocationTable.push_back(objectDataOffset + offsetof(RazielData64, throwList));
		UPGRADE_DumpStructPointer(objectDataOffset + offsetof(RazielData64, throwList), f);

		long offsetThrowListPtr = -1;
		int offsetThrowListPtrCount = -1;
		struct __ThrowItem** throwListPtr = data->throwList;

		//Get counts
		while (*throwListPtr++)
		{
			offsetThrowListPtrCount++;
		}

		//Store offsets
		offsetThrowListPtr = ftell(f);
		for (int i = 0; i < offsetThrowListPtrCount; i++)
		{
			relocationTable.push_back(offsetThrowListPtr + (i * sizeof(long long)));
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetThrowListPtr + (i * sizeof(long long)), f);
		}

		//Store offsets
		for (int i = 0; i < offsetThrowListPtrCount; i++)
		{
			relocationTable.push_back(offsetThrowListPtr + (i * sizeof(long long)));
			UPGRADE_DumpStruct(data->throwList[i], sizeof(struct __ThrowItem), offsetThrowListPtr + (i * sizeof(long long)), f);
		}
		
		void* endOfStringAnimations = (void*)data->virtualAnimations;

		struct __SAnim* sAnim = *data->stringAnimations;
		relocationTable.push_back(objectDataOffset + offsetof(RazielData64, stringAnimations));
		UPGRADE_DumpStructPointer(objectDataOffset + offsetof(RazielData64, stringAnimations), f);

		long offsetsAnimPtrs = FILE_GetOffset(f);

		for (long sAnimIndex = 0; sAnim != NULL; sAnim = data->stringAnimations[++sAnimIndex])
		{
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), FILE_GetOffset(f), f);
		}

		UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), FILE_GetOffset(f), f);

		sAnim = *data->stringAnimations;
		for (long sAnimIndex = 0; sAnim != NULL; sAnim = data->stringAnimations[++sAnimIndex])
		{
			long sAnimOffset = FILE_GetOffset(f);

			relocationTable.push_back(offsetsAnimPtrs + (sAnimIndex * sizeof(long long)));

			UPGRADE_DumpStructPointer(offsetsAnimPtrs + (sAnimIndex * sizeof(long long)), f);

			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), sAnimOffset + offsetof(__SAnim64, anim), f);
			
			UPGRADE_DumpRaw(&NULL_PTR, sizeof(unsigned long long), sAnimOffset + offsetof(__SAnim64, nextAnim), f);

			UPGRADE_DumpRaw(&sAnim->mode, sizeof(short), sAnimOffset + offsetof(__SAnim64, mode), f);
			UPGRADE_DumpRaw(&sAnim->data, sizeof(short), sAnimOffset + offsetof(__SAnim64, data), f);
			UPGRADE_DumpRaw(&sAnim->speedAdjust, sizeof(short), sAnimOffset + offsetof(__SAnim64, speedAdjust), f);
			UPGRADE_DumpRaw(&sAnim->pad, sizeof(short), sAnimOffset + offsetof(__SAnim64, pad), f);

			if (sAnim->anim != NULL)
			{
				relocationTable.push_back(sAnimOffset + offsetof(__SAnim64, anim));
				UPGRADE_DumpStructPointer(sAnimOffset + offsetof(__SAnim64, anim), f);
				UPGRADE_DumpStruct(sAnim->anim, sizeof(struct __VAnim), sAnimOffset + offsetof(__SAnim64, anim), f);
			}

			if (sAnim->nextAnim != NULL)
			{
				relocationTable.push_back(sAnimOffset + offsetof(__SAnim64, nextAnim));
				UPGRADE_DumpStructPointer(sAnimOffset + offsetof(__SAnim64, nextAnim), f);
				UPGRADE_DumpStruct(sAnim->nextAnim, sizeof(struct __SAnim), sAnimOffset + offsetof(__SAnim64, nextAnim), f);
			}
		}

		relocationTable.push_back(objectDataOffset + offsetof(RazielData64, virtualAnimations));
		UPGRADE_DumpStruct(data->virtualAnimations, (char*)data->virtualAnimSingle - (char*)data->virtualAnimations, objectDataOffset + offsetof(RazielData64, virtualAnimations), f);

		relocationTable.push_back(objectDataOffset + offsetof(RazielData64, virtualAnimSingle));
		UPGRADE_DumpStruct(data->virtualAnimSingle, ((char*)baseAddr) + fileSize - (char*)data->virtualAnimSingle, objectDataOffset + offsetof(RazielData64, virtualAnimSingle), f);

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
		//assert(false);
		break;
	}

	long* relocPtr = (long*)object->relocList;
	long relocTableSize = 0;

	if (object->relocList != NULL)
	{
		long relocTableOffset = FILE_GetOffset(f);

		while (*relocPtr++ != -1)
		{
			relocTableSize += sizeof(long);
		}

		relocTableSize += sizeof(long);

		relocationTable.push_back(offsetof(Object64, relocList));

		UPGRADE_DumpStruct(object->relocList, relocTableSize, offsetof(Object64, relocList), f);
	}

	if (object->relocModule != NULL)
	{
		relocationTable.push_back(offsetof(Object64, relocModule));

		UPGRADE_DumpStruct(object->relocModule, (char*)baseAddr + fileSize - (char*)object->relocModule, offsetof(Object64, relocModule), f);
	}


	FILE_Close(f);

	f = FILE_OpenRead("TEMP.DRM");
	long resultFileSize = FILE_SeekEnd(f);
	FILE_Seek(f, 0, SEEK_SET);

	char* fileData = new char[resultFileSize];
	fread(fileData, resultFileSize, 1, f);
	FILE_Close(f);

	f = FILE_OpenWrite(filePath);

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
	
		fwrite(fileData, resultFileSize, 1, f);
	}

	remove("TEMP.DRM");

	delete[] fileData;

	FILE_Close(f);

#if !defined(_WIN64)
	char nameBuff[4096];
	sprintf(nameBuff, "%s", filePath);

#if defined(UPDATE_LST) || 1
	f = fopen("bigfile.lst", "ab");

	char sep = 0;

	unsigned int hash = UPGRADE_HashName(strstr(nameBuff, "kain2"));///@TODO need to strip c:/path!

	fprintf(f, "%x", hash);
	fwrite(&sep, sizeof(char), 1, f);
	fprintf(f, "%s", nameBuff);
	fwrite(&sep, sizeof(char), 1, f);

	FILE_Close(f);
#endif

#endif
}

void UPGRADE_DumpDefaultFile(long* data, unsigned int fileSize, char* filePath)
{
	char nameBuff[4096];
	sprintf(nameBuff, "%s", filePath);

	FILE* f = fopen("bigfile.lst", "ab");

	if (f != NULL)
	{
		char sep = 0;

		unsigned int hash = UPGRADE_HashName(nameBuff);

		fprintf(f, "%x", hash);
		fwrite(&sep, sizeof(char), 1, f);
		fprintf(f, "%s", nameBuff);
		fwrite(&sep, sizeof(char), 1, f);

		FILE_Close(f);
	}

	f = fopen(nameBuff, "wb+");

	if (f != NULL)
	{
		fwrite(data, fileSize, 1, f);
		FILE_Close(f);
	}
}

void UPGRADE_ProcessRedirectList(long* data, unsigned int fileSize, char* filePath, DrmFileType fileType)
{
	long tableSize;
	struct RedirectList redirectListX;
	struct RedirectList* redirectList;

	if (fileType == DrmFileType::OBJECT)
	{
		redirectList = &redirectListX;

		redirectList->data = data + 1;

		redirectList->numPointers = data[0];

		tableSize = (redirectList->numPointers + 512 < 0) ? (redirectList->numPointers + 1023) : (redirectList->numPointers + 512);
		tableSize /= 512;
		tableSize *= 512;
	}

	switch (fileType)
	{
	case OBJECT:
		UPGRADE_Object(redirectList, &data[tableSize], data, fileSize, filePath);
		break;
	case SFX:
	{
		switch (data[0])
		{
		case 0x61534E46:
			UPGRADE_SNF(data, fileSize, filePath);
			break;
		}
		break;
	}
	case TBL:
	{
		UPGRADE_Locals(data, fileSize, filePath);
		break;
	}
	}
	//UPGRADE_Pointers(redirectList, &data[tableSize], data, fileSize, filePath);

	return;
}

enum DrmFileType UPGRADE_GetFileType(const char* drmFilePath)
{
	if (strstr(drmFilePath, "kain2\\object\\") != NULL)
	{
		printf("Found Object!\n");
		return DrmFileType::OBJECT;
	}

	if (strstr(drmFilePath, "kain2\\area\\") != NULL)
	{
		printf("Found Area!\n");

		return DrmFileType::AREA;
	}

	if (strstr(drmFilePath, "kain2\\music\\") != NULL)
	{
		printf("Found Music!\n");

		return DrmFileType::MUSIC;
	}

	if (strstr(drmFilePath, "kain2\\sfx\\") != NULL)
	{
		printf("Found SFX!\n");

		return DrmFileType::SFX;
	}

	if (!strcmp(".TBL", strchr(drmFilePath, '.')))
	{
		return DrmFileType::TBL;
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
		else
		{
			UPGRADE_DumpDefaultFile((long*)pFileData, outSize, drmFilePath);
		}
	}
}
