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

template <typename T>
struct identity
{
	using type = T;
};

template<typename T>
struct remove_all_pointers : std::conditional_t<
	std::is_pointer_v<T>,
	remove_all_pointers<
	std::remove_pointer_t<T>
	>,
	identity<T>
>
{};

template <typename T>
struct Delocate
{
	static inline void DumpStruct(T t, int size, int offset, FILE* f)
	{
		if (f != NULL)
		{
			long long currentOffset = ftell(f);

			fseek(f, offset, SEEK_SET);
			fwrite(&currentOffset, sizeof(long long), 1, f);
			fseek(f, 0, SEEK_END);
		}
	}

	static inline void DumpStruct(T* t, int size, int offset, FILE* f)
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

	static inline void DumpRaw(T* t, int size, int offset, FILE* f)
	{
		if (f != NULL)
		{
			fseek(f, offset, SEEK_SET);
			fwrite(t, size, 1, f);
		}
	}

	static inline void DumpStruct(T** t, int size, int offset, FILE* f, int count)
	{
		if (f != NULL)
		{
			for (int i = 0; i < count; i++)
			{
				Delocate<remove_all_pointers<T>::type>::DumpStruct(*t[i], sizeof(remove_all_pointers<T>::type), offset, f);
			}
		}
	}
};

void UPGRADE_Object(struct RedirectList* redirectList, long* data, long* baseAddr, unsigned int fileSize, char* filePath)
{
	struct Object* object;

	UPGRADE_Relocate(redirectList, data, baseAddr);

	object = (struct Object*)data;
	
	FILE* f = FILE_OpenWrite("MAIN.DRM");
	unsigned long long NULL_PTR = 0;
	std::vector<unsigned int> relocationTable;

	Delocate<long>::DumpRaw(&object->oflags, sizeof(long), offsetof(Object64, oflags), f);
	Delocate<short>::DumpRaw(&object->id, sizeof(short), offsetof(Object64, id), f);
	Delocate<short>::DumpRaw(&object->sfxFileHandle, sizeof(short), offsetof(Object64, sfxFileHandle), f);
	Delocate<short>::DumpRaw(&object->numModels, sizeof(short), offsetof(Object64, numModels), f);
	Delocate<short>::DumpRaw(&object->numAnims, sizeof(short), offsetof(Object64, numAnims), f);

	Delocate<short>::DumpRaw(&object->introDist, sizeof(short), offsetof(Object64, introDist), f);
	Delocate<short>::DumpRaw(&object->vvIntroDist, sizeof(short), offsetof(Object64, vvIntroDist), f);
	Delocate<short>::DumpRaw(&object->removeDist, sizeof(short), offsetof(Object64, removeDist), f);
	Delocate<short>::DumpRaw(&object->vvRemoveDist, sizeof(short), offsetof(Object64, vvRemoveDist), f);

	Delocate<long>::DumpRaw(&object->oflags2, sizeof(long), offsetof(Object64, oflags2), f);
	Delocate<short>::DumpRaw(&object->sectionA, sizeof(short), offsetof(Object64, sectionA), f);
	Delocate<short>::DumpRaw(&object->sectionB, sizeof(short), offsetof(Object64, sectionB), f);
	Delocate<short>::DumpRaw(&object->sectionC, sizeof(short), offsetof(Object64, sectionC), f);

	Delocate<short>::DumpRaw(&object->numberOfEffects, sizeof(short), offsetof(Object64, numberOfEffects), f);

	Delocate<VramSize>::DumpRaw(&object->vramSize, sizeof(struct VramSize), offsetof(Object64, vramSize), f);

	relocationTable.push_back(offsetof(Object64, script));
	Delocate<char>::DumpStruct(object->script, 9, offsetof(Object64, script), f);
	
	relocationTable.push_back(offsetof(Object64, name));
	Delocate<char>::DumpStruct(object->name, 9, offsetof(Object64, name), f);

	relocationTable.push_back(offsetof(Object64, modelList));
	Delocate<_Model**>::DumpStruct(object->modelList, sizeof(_Model**) , offsetof(Object64, modelList), f);

	for (int i = 0; i < object->numModels; i++)
	{
		Delocate<unsigned long long>::DumpRaw(&NULL_PTR, sizeof(unsigned long long), ftell(f), f);
	}

	long offsetModelList = ftell(f);

	relocationTable.push_back(offsetof(Object64, animList));
	Delocate<_G2AnimKeylist_Type**>::DumpStruct(object->animList, sizeof(_G2AnimKeylist_Type**) * object->numAnims, offsetof(Object64, animList), f);

	for (int i = 0; i < object->numAnims; i++)
	{
		Delocate<unsigned long long>::DumpRaw(&NULL_PTR, sizeof(unsigned long long),ftell(f), f);
	}

	long offsetAnimList = ftell(f);

	for (int i = 0; i < object->numModels; i++)
	{
		struct _Model* model = object->modelList[i];

		unsigned int offsetOfModel = ftell(f);

		relocationTable.push_back(offsetModelList - ((object->numModels - i) * sizeof(long long)));
		Delocate<_Model>::DumpStruct(object->modelList, sizeof(_Model**) * object->numModels, offsetModelList - ((object->numModels - i) * sizeof(long long)), f, object->numModels);

		Delocate<long>::DumpRaw(&model->numVertices, sizeof(long), offsetOfModel + offsetof(_Model64, numVertices), f);
		Delocate<long>::DumpRaw(&model->numNormals, sizeof(long), offsetOfModel + offsetof(_Model64, numNormals), f);
		Delocate<long>::DumpRaw(&model->numFaces, sizeof(long), offsetOfModel + offsetof(_Model64, numFaces), f);
		Delocate<long>::DumpRaw(&model->numSegments, sizeof(long), offsetOfModel + offsetof(_Model64, numSegments), f);
		Delocate<short>::DumpRaw(&model->maxRad, sizeof(short), offsetOfModel + offsetof(_Model64, maxRad), f);
		Delocate<short>::DumpRaw(&model->pad, sizeof(short), offsetOfModel + offsetof(_Model64, pad), f);
		Delocate<long>::DumpRaw(&model->maxRadSq, sizeof(long), offsetOfModel + offsetof(_Model64, maxRadSq), f);

		if (model->vertexList != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, vertexList));
			Delocate<_MVertex>::DumpStruct(model->vertexList, sizeof(struct _MVertex) * model->numVertices, offsetOfModel + offsetof(_Model64, vertexList), f);
		}
		else
		{
			Delocate<unsigned long long>::DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, vertexList), f);
		}

		if (model->faceList != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, faceList));
			Delocate<_MFace>::DumpStruct(model->faceList, sizeof(struct _MFace) * model->numFaces, offsetOfModel + offsetof(_Model64, faceList), f);
		}
		else
		{
			Delocate<unsigned long long>::DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, faceList), f);
		}

		if (model->normalList != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, normalList));
			Delocate<_SVectorNoPad>::DumpStruct(model->normalList, sizeof(struct _MVertex) * model->numNormals, offsetOfModel + offsetof(_Model64, normalList), f);
		}
		else
		{
			Delocate<unsigned long long>::DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, normalList), f);
		}

		relocationTable.push_back(offsetOfModel + offsetof(_Model64, segmentList));
	
		for (int j = 0; j < model->numSegments; j++)
		{
			struct _Segment* segment = &model->segmentList[j];

			unsigned int offsetOfSegment = ftell(f);

			Delocate<long>::DumpRaw(&segment->flags, sizeof(long), offsetOfSegment + offsetof(_Segment64, flags), f);
			Delocate<short>::DumpRaw(&segment->firstTri, sizeof(short), offsetOfSegment + offsetof(_Segment64, firstTri), f);
			Delocate<short>::DumpRaw(&segment->lastTri, sizeof(short), offsetOfSegment + offsetof(_Segment64, lastTri), f);
			Delocate<short>::DumpRaw(&segment->firstVertex, sizeof(short), offsetOfSegment + offsetof(_Segment64, firstVertex), f);
			Delocate<short>::DumpRaw(&segment->lastVertex, sizeof(short), offsetOfSegment + offsetof(_Segment64, lastVertex), f);
			Delocate<short>::DumpRaw(&segment->px, sizeof(short), offsetOfSegment + offsetof(_Segment64, px), f);
			Delocate<short>::DumpRaw(&segment->py, sizeof(short), offsetOfSegment + offsetof(_Segment64, py), f);
			Delocate<short>::DumpRaw(&segment->pz, sizeof(short), offsetOfSegment + offsetof(_Segment64, pz), f);
			Delocate<short>::DumpRaw(&segment->parent, sizeof(short), offsetOfSegment + offsetof(_Segment64, parent), f);

			if (segment->hInfo != NULL)
			{
				relocationTable.push_back(offsetOfSegment + offsetof(_Segment64, hInfo));
				Delocate<_HInfo>::DumpStruct(segment->hInfo, sizeof(struct _HInfo*), offsetOfSegment + offsetof(_Segment64, hInfo), f);
			}
			else
			{
				Delocate<unsigned long long>::DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfSegment + offsetof(_Segment64, hInfo), f);
			}
		}

		if (model->startTextures != NULL && model->endTextures != NULL)
		{
			relocationTable.push_back(offsetOfModel + offsetof(_Model64, startTextures));
			Delocate<TextureMT3*>::DumpStruct(model->startTextures, sizeof(struct TextureMT3*), offsetOfModel + offsetof(_Model64, startTextures), f);

			for (struct TextureMT3* currentTexture = model->startTextures; currentTexture < model->endTextures; currentTexture++)
			{
				Delocate<TextureMT3>::DumpStruct(currentTexture, sizeof(struct TextureMT3), -1, f);
			}

			relocationTable.push_back(offsetOfModel + offsetof(_Model64, endTextures));
			Delocate<TextureMT3*>::DumpStruct(model->endTextures, sizeof(struct TextureMT3*), offsetOfModel + offsetof(_Model64, endTextures), f);
		}
		else
		{
			Delocate<unsigned long long>::DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, startTextures), f);
			Delocate<unsigned long long>::DumpRaw(&NULL_PTR, sizeof(unsigned long long), offsetOfModel + offsetof(_Model64, endTextures), f);
		}
	}

	for (int i = 0; i < object->numAnims; i++)
	{
		struct _G2AnimKeylist_Type* anim = object->animList[i];

		unsigned int offsetOfAnim = ftell(f);
	
		Delocate<_G2AnimKeylist_Type>::DumpRaw(anim, sizeof(struct _G2AnimKeylist_Type), offsetOfAnim, f);

		unsigned int offsetOfEnd = ftell(f);

		relocationTable.push_back(offsetAnimList - ((object->numAnims - i) * sizeof(long long)));
		Delocate<_G2AnimKeylist_Type>::DumpStruct(object->animList, sizeof(struct _G2AnimKeylist_Type**) * object->numAnims, offsetAnimList - ((object->numAnims - i) * sizeof(long long)), f, object->numAnims);
		
		fseek(f, offsetOfEnd, SEEK_SET);
	}

	if (object->relocModule != NULL)
	{
		relocationTable.push_back(offsetof(Object64, relocModule));

		Delocate<int>::DumpStruct(object->relocModule, 0, offsetof(Object64, relocModule), f);
	}

	if (object->relocList != NULL)
	{
		relocationTable.push_back(offsetof(Object64, relocList));

		Delocate<long>::DumpStruct(object->relocList, 0, offsetof(Object64, relocList), f);
	}

	FILE_Close(f);

	f = FILE_OpenWrite("RTBL.TBL");

	if (relocationTable.size())
	{
		long relocCount = relocationTable.size();
		fwrite(&relocCount, sizeof(long), 1, f);
		fwrite(&relocationTable[0], sizeof(unsigned int) * relocationTable.size(), 1, f);
		fseek(f, ((relocationTable.size() * 512))-1, SEEK_SET);
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
