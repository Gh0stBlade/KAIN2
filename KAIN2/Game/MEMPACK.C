#include "CORE.H"
#include "MEMPACK.H"
#include "PSX/DRAWS.H"
#include "STREAM.H"
#include "DEBUG.H"
#include "PSX/AADLIB.H"
#include "LOAD3D.H"
#include "Game/EVENT.H"
#include "Game/PIPE3D.H"
#include <stddef.h>

#pragma warning( disable : 4700 )

extern void GXFilePrint(const char* fmt, ...);

struct NewMemTracker newMemTracker;
unsigned long mem_used, mem_total;

#if defined(PSXPC_VERSION)

#if defined(UWP)
char* memBuffer = NULL;
unsigned int overlayAddress = 0; // 0x800CE194
#else
#define OVERLAY_LENGTH 0xDDC8
#if defined(GAME_X64)
char memBuffer[OVERLAY_LENGTH + 0x11F18C + (ONE_MB * 5)];
#else
char memBuffer[OVERLAY_LENGTH + 0x11F18C + (ONE_MB * 4)];
#endif
uintptr_t overlayAddress = (uintptr_t)&memBuffer[0]; // 0x800CE194
#endif
#else
unsigned int overlayAddress; // For PSX this is quite clearly set by the linker script maybe.
#endif

void MEMPACK_Init()//Matching - 51.54%
{
#if defined(PSX_VERSION)
#if defined(PSXPC_VERSION)
#if defined(UWP)
	SYSTEM_INFO info;
	GetSystemInfo(&info);

#if defined(PLATFORM_DURANGO) || 1
	LPVOID base = (LPVOID)0x1FFFFFFF;
#else
	LPVOID base = (LPVOID)NULL;
#endif
	unsigned int totalSize = OVERLAY_LENGTH + 0x11F18C + (ONE_MB * 4);
	memBuffer = (char*)VirtualAlloc(base, totalSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	overlayAddress = (unsigned int)memBuffer;
	newMemTracker.totalMemory = totalSize;
	memset(&memBuffer[0], 0, totalSize);
#else
	newMemTracker.totalMemory = sizeof(memBuffer);
	memset(&memBuffer[0], 0, sizeof(memBuffer));
#endif
	
	newMemTracker.rootNode = (struct MemHeader*)&memBuffer[0];
#else
	newMemTracker.totalMemory = (BASE_ADDRESS + (TWO_MB - (ONE_MB / 256))) - overlayAddress;
	newMemTracker.rootNode = (struct MemHeader*)overlayAddress;
#endif
#endif
	
	newMemTracker.rootNode->magicNumber = DEFAULT_MEM_MAGIC;
	newMemTracker.rootNode->memStatus = 0;
	newMemTracker.rootNode->memType = 0;
	newMemTracker.rootNode->memSize = newMemTracker.totalMemory;
	newMemTracker.currentMemoryUsed = 0;
	newMemTracker.doingGarbageCollection = 0;
	newMemTracker.lastMemoryAddress = (char*)newMemTracker.rootNode + newMemTracker.rootNode->memSize;
}

struct MemHeader* MEMPACK_GetSmallestBlockTopBottom(long allocSize)//Matching - 100.0%
{ 
	struct MemHeader* address = newMemTracker.rootNode;
	struct MemHeader* bestAddress = NULL;

	while ((char*)address != (char*)newMemTracker.lastMemoryAddress)
	{
		if (address->memStatus == 0 && (long)address->memSize >= allocSize && bestAddress == NULL)
		{
			bestAddress = address;
			break;
		}

		address = (struct MemHeader*)((char*)address + address->memSize);
	}

	return bestAddress;
}

struct MemHeader* MEMPACK_GetSmallestBlockBottomTop(long allocSize)//Matching - 100.0%
{ 
	struct MemHeader* address = newMemTracker.rootNode;
	struct MemHeader* bestAddress = NULL;

	while ((char*)address != (char*)newMemTracker.lastMemoryAddress)
	{
		if (address->memStatus == 0 && (long)address->memSize >= allocSize && (bestAddress == NULL || (char*)bestAddress < (char*)address))
		{
			bestAddress = address;
		}

		address = (struct MemHeader*)((char*)address + address->memSize);
	}

	return bestAddress;
}

long MEMPACK_RelocatableType(long memType)//Matching - 100.0%
{
	if (memType - 1 < 2U || memType == 0x2C || memType == 0x2F || memType == 0x4)
	{
		return 1;
	}

	return 0;
}

char* MEMPACK_Malloc(unsigned long allocSize, unsigned char memType)//Matching - 99.25%
{
#if defined(UWP)
	struct MemHeader* bestAddress = NULL;
#else
	struct MemHeader* bestAddress;
#endif
	long relocatableMemory;
	int curMem;
	struct MemHeader* address;
	long topOffset;

	relocatableMemory = MEMPACK_RelocatableType(memType);

	allocSize = ((allocSize + 11) / 4) * 4;

	do
	{
		if (newMemTracker.doingGarbageCollection == 0 && relocatableMemory != 0)
		{
			MEMPACK_DoGarbageCollection();
		}

		if (relocatableMemory != 0)
		{
			bestAddress = MEMPACK_GetSmallestBlockTopBottom(allocSize);
		}
		else if (relocatableMemory == 0)
		{
			bestAddress = MEMPACK_GetSmallestBlockBottomTop(allocSize);
		}

		if (bestAddress == NULL)
		{
			curMem = newMemTracker.currentMemoryUsed;

			STREAM_TryAndDumpANonResidentObject();

			if (curMem == newMemTracker.currentMemoryUsed)
			{
				if (memType == 16)
				{
					return 0;
				}

				MEMPACK_ReportMemory2();
				DEBUG_FatalError("Trying to fit memory size %d Type = %d\nAvailable memory : used = % d, free = % d", allocSize, memType, newMemTracker.currentMemoryUsed, newMemTracker.totalMemory - newMemTracker.currentMemoryUsed);
				break;
			}
		}
		else
		{
			break;
		}
	} while (curMem != newMemTracker.currentMemoryUsed);

	topOffset = bestAddress->memSize;
	if ((unsigned int)topOffset - allocSize < 8)
	{
		allocSize = bestAddress->memSize;
	}

	if (allocSize != bestAddress->memSize)
	{
		if (relocatableMemory != 0)
		{
			address = (struct MemHeader*)((char*)bestAddress + allocSize);

			address->magicNumber = DEFAULT_MEM_MAGIC;
			address->memStatus = 0;
			address->memType = 0;
			address->memSize = bestAddress->memSize - allocSize;

			bestAddress->magicNumber = DEFAULT_MEM_MAGIC;
			bestAddress->memStatus = 1;
			bestAddress->memType = memType;
			bestAddress->memSize = allocSize;

			newMemTracker.currentMemoryUsed += allocSize;
		}
		else
		{
			address = (struct MemHeader*)((char*)bestAddress + (topOffset - allocSize));

			address->magicNumber = DEFAULT_MEM_MAGIC;
			address->memStatus = 1;
			address->memType = memType;
			address->memSize = allocSize;

			newMemTracker.currentMemoryUsed += allocSize;

			bestAddress->magicNumber = DEFAULT_MEM_MAGIC;
			bestAddress->memStatus = 0;
			bestAddress->memType = 0;
			bestAddress->memSize = (topOffset - allocSize);

			bestAddress = address;
		}
	}
	else
	{
		bestAddress->magicNumber = DEFAULT_MEM_MAGIC;
		bestAddress->memStatus = 1;
		bestAddress->memType = memType;
		bestAddress->memSize = allocSize;

		newMemTracker.currentMemoryUsed += allocSize;
	}

	return (char*)(bestAddress + 1);
}

void MEMORY_MergeAddresses(struct MemHeader* firstAddress, struct MemHeader* secondAddress)//Matching - 100.0%
{
	if (firstAddress->memStatus == 0 && secondAddress->memStatus == 0)
	{
		firstAddress->memSize += secondAddress->memSize;
		secondAddress->magicNumber = 0;
		secondAddress->memStatus = 1;
	}
}

void MEMPACK_Return(char* address, long takeBackSize)//Matching - 89.55%
{
	struct MemHeader* memAddress;
	struct MemHeader* nextAddress;

	takeBackSize = (takeBackSize >> 2) << 2;

	if (takeBackSize >= (int)sizeof(struct MemHeader))
	{
		memAddress = (struct MemHeader*)((char*)address - 8);
		memAddress->memSize -= takeBackSize;

		newMemTracker.currentMemoryUsed -= takeBackSize;
		memAddress = (struct MemHeader*)((char*)memAddress + memAddress->memSize);

		memAddress->magicNumber = DEFAULT_MEM_MAGIC;
		memAddress->memStatus = 0;
		memAddress->memType = 0;
		memAddress->memSize = takeBackSize;

		nextAddress = (struct MemHeader*)((char*)memAddress + takeBackSize);

		if ((char*)nextAddress != (char*)newMemTracker.lastMemoryAddress)
		{
			MEMORY_MergeAddresses(memAddress, nextAddress);
		}
	}
}

void MEMPACK_Free(char *address)//Matching - 84.20%
{ 
	struct MemHeader* memAddress;
	struct MemHeader* secondAddress;
	
	memAddress = (struct MemHeader*)(address - 8);
	memAddress->memStatus = 0;
	memAddress->memType = 0;

	newMemTracker.currentMemoryUsed -= memAddress->memSize;

	secondAddress = (struct MemHeader*)((char*)memAddress + memAddress->memSize);

	if ((char*)secondAddress != (char*)newMemTracker.lastMemoryAddress)
	{
		MEMORY_MergeAddresses(memAddress, secondAddress);
	}

	secondAddress = memAddress;
	memAddress = newMemTracker.rootNode;

	while ((char*)memAddress != (char*)newMemTracker.lastMemoryAddress)
	{
		if ((char*)memAddress + memAddress->memSize == (char*)secondAddress)
		{
			MEMORY_MergeAddresses(memAddress, (struct MemHeader*)((char*)memAddress + memAddress->memSize));
			break;
		}

		memAddress = (struct MemHeader*)((char*)memAddress + memAddress->memSize);
	}
}

void MEMPACK_FreeByType(unsigned char memType)//Matching - 90.53%
{
	struct MemHeader* address;
	int freed;

	do
	{
		freed = 0;
		address = newMemTracker.rootNode;
		while ((char*)address != newMemTracker.lastMemoryAddress)
		{

			if (address->memStatus == 1 && address->memType == memType)
			{
				address++;
				freed = 1;
				MEMPACK_Free((char*)address);
				break;
			}

			address = (struct MemHeader*)((char*)address + address->memSize);
		}
	} while (freed == 1);
}

unsigned long MEMPACK_Size(char *address)//Matching - 100.0%
{
	return ((int*)address)[-1] - sizeof(struct MemHeader);
}

unsigned long MEMPACK_ReportFreeMemory()//Matching - 45.00%
{
	return newMemTracker.totalMemory - newMemTracker.currentMemoryUsed;
}

void MEMPACK_ReportMemory2()//Matching - 97.50%
{
	struct MemHeader* address;

	address = newMemTracker.rootNode;

	while ((char*)address != newMemTracker.lastMemoryAddress)
	{
		address = (struct MemHeader*)((char*)address + address->memSize);
	}
}

void MEMPACK_ReportMemory()//Matching - 93.59%
{
	struct MemHeader* address;
	long i;
	long firstTime;

	address = newMemTracker.rootNode;

	while ((char*)address != newMemTracker.lastMemoryAddress)
	{
		address = (struct MemHeader*)((char*)address + address->memSize);
	}
	
	for (i = 0; i < 49; i++)
	{
		address = newMemTracker.rootNode;
		firstTime = 1;

		while ((char*)address != newMemTracker.lastMemoryAddress)
		{
			if (address->memStatus != 0 && address->memType == 1 && firstTime != 0)
			{
				firstTime = 0;
			}

			address = (struct MemHeader*)((char*)address + address->memSize);
		}
	}
}

void MEMPACK_SetMemoryBeingStreamed(char *address)//Matching - 100.0%
{
	address[-6] = 2;
}

void MEMPACK_SetMemoryDoneStreamed(char *address)//Matching - 100.0%
{ 
	address[-6] = 1;
}

long MEMPACK_MemoryValidFunc(char* address)//Matching - 100.0%
{
	if ((address != (char*)INVALID_MEM_MAGIC) && (address != NULL))
	{
		return (address[-6]) == 1;
	}

	return 0;
}


char* MEMPACK_GarbageCollectMalloc(unsigned long *allocSize, unsigned char memType, unsigned long *freeSize)//Matching - 98.62%
{
	struct MemHeader* bestAddress;
	
	allocSize[0] = ((allocSize[0] + 11) / 4) * 4;

	bestAddress = MEMPACK_GetSmallestBlockTopBottom(allocSize[0]);

	if (bestAddress == NULL)
	{
		STREAM_DumpNonResidentObjects();

		bestAddress = MEMPACK_GetSmallestBlockTopBottom(allocSize[0]);

		if (bestAddress == NULL)
		{
			if (memType != 16)
			{
				MEMPACK_ReportMemory();

				DEBUG_FatalError("Trying to fit memory size %d Type = %d\nAvalible memory : used = % d, free = %d\n", allocSize[0], memType, newMemTracker.currentMemoryUsed, newMemTracker.totalMemory - newMemTracker.currentMemoryUsed);
			}
			else
			{
				return NULL;
			}
		}
	}

	if (bestAddress->memSize - allocSize[0] < 8)
	{
		allocSize[0] = bestAddress->memSize;
	}

	if (allocSize[0] != bestAddress->memSize)
	{
		freeSize[0] = bestAddress->memSize - allocSize[0];

		bestAddress->magicNumber = DEFAULT_MEM_MAGIC;
		bestAddress->memStatus = 1;
		bestAddress->memType = memType;
		bestAddress->memSize = allocSize[0];

		newMemTracker.currentMemoryUsed += allocSize[0];
	}
	else
	{
		bestAddress->magicNumber = DEFAULT_MEM_MAGIC;
		bestAddress->memStatus = 1;
		bestAddress->memType = memType;
		bestAddress->memSize = allocSize[0];

		newMemTracker.currentMemoryUsed += allocSize[0];

		freeSize[0] = 0;
	}

	return (char*)(bestAddress + 1);
}

void MEMPACK_GarbageSplitMemoryNow(unsigned long allocSize, struct MemHeader* bestAddress, long memType, unsigned long freeSize)//Matching - 100%
{
	if (freeSize != 0)
	{
		struct MemHeader* address = (struct MemHeader*)((char*)bestAddress + allocSize);

		address->magicNumber = DEFAULT_MEM_MAGIC;
		address->memStatus = 0;
		address->memType = 0;
		address->memSize = freeSize;
	}
}


void MEMPACK_GarbageCollectFree(struct MemHeader *memAddress)//Matching - 95.11%
{
	struct MemHeader* secondAddress;
	
	memAddress->memStatus = 0;
	memAddress->memType = 0;

	newMemTracker.currentMemoryUsed -= memAddress->memSize;

	secondAddress = (struct MemHeader*)((char*)memAddress + memAddress->memSize);

	if ((char*)secondAddress != (char*)newMemTracker.lastMemoryAddress)
	{
		MEMORY_MergeAddresses(memAddress, secondAddress);
	}

	secondAddress = memAddress;
	memAddress = newMemTracker.rootNode;

	while ((char*)memAddress != (char*)newMemTracker.lastMemoryAddress)
	{
		if (((char*)memAddress + memAddress->memSize) == (char*)secondAddress)
		{
			MEMORY_MergeAddresses(memAddress, (struct MemHeader*)((char*)memAddress + memAddress->memSize));
			break;
		}

		memAddress = (struct MemHeader*)((char*)memAddress + memAddress->memSize);
	}
}

void MEMPACK_DoGarbageCollection()//Matching - 98.66%
{ 
	struct MemHeader* relocateAddress;
	long foundOpening;
	long done;
	long addressSize;
	long addressMemType;
	long holdSize;
	long freeSize;
	char* oldAddress;
	char* newAddress;

	done = 0;
	freeSize = 0;
	newMemTracker.doingGarbageCollection = 1;

	while (done == 0)
	{
		relocateAddress = (struct MemHeader*)newMemTracker.rootNode;
		
		foundOpening = 0;

		while ((char*)relocateAddress != (char*)newMemTracker.lastMemoryAddress)
		{
			if (relocateAddress->memStatus != 0)
			{
				if (MEMPACK_RelocatableType(relocateAddress->memType) != 0 && foundOpening == 1 && relocateAddress->memStatus != 2)
				{
					foundOpening = 2;
					break;
				}
			}
			else
			{
				foundOpening = 1;
			}

			relocateAddress = (struct MemHeader*)((char*)relocateAddress + relocateAddress->memSize);
		}

		if (foundOpening == 2)
		{
			addressMemType = relocateAddress->memType;
			addressSize = relocateAddress->memSize - sizeof(struct MemHeader);

			MEMPACK_GarbageCollectFree(relocateAddress);

			holdSize = addressSize;
			newAddress = MEMPACK_GarbageCollectMalloc((unsigned long*)&holdSize, (unsigned char)addressMemType, (unsigned long*)&freeSize);
			oldAddress = (char*)(relocateAddress + 1);

			if (newAddress != NULL)
			{
				if (addressMemType == 2)
				{
					RemoveIntroducedLights((struct Level*)oldAddress);
				}
				else if (addressMemType == 4)
				{
					aadRelocateMusicMemoryBegin();
				}

				memcpy(newAddress, oldAddress, addressSize);

				if (addressMemType == 2)
				{
					MEMPACK_RelocateAreaType((struct MemHeader*)(newAddress - 8), newAddress - oldAddress, (struct Level*)oldAddress);
				}
				else if (addressMemType == 1)
				{
					MEMPACK_RelocateObjectType((struct MemHeader*)(newAddress - 8), newAddress - oldAddress, (struct Object*)oldAddress);
				}
				else if (addressMemType == 14)
				{
					STREAM_UpdateInstanceCollisionInfo((struct _HModel*)oldAddress, (struct _HModel*)newAddress);
				}
				else if (addressMemType == 44)
				{
					MEMPACK_RelocateCDMemory((struct MemHeader*)(newAddress - 8), newAddress - oldAddress, (struct _BigFileDir*)oldAddress);
				}
				else if (addressMemType == 4)
				{
					aadRelocateMusicMemoryEnd((struct MemHeader*)oldAddress, newAddress - oldAddress);
				}
				else if (addressMemType == 47)
				{
					aadRelocateSfxMemory(oldAddress, newAddress - oldAddress);
				}

				MEMPACK_GarbageSplitMemoryNow(holdSize, (struct MemHeader*)(newAddress - 8), addressMemType, freeSize);
			}
		}
		else
		{
			done = 1;
		}
	}

	newMemTracker.doingGarbageCollection = 0;
}


void MEMPACK_RelocateAreaType(struct MemHeader* newAddress, long offset, struct Level* oldLevel) // Matching - 100%
{
	struct Level* level;
	struct _MultiSignal* msignal;
	long sizeOfLevel;
	long i;
	long d;

	level = (struct Level*)(newAddress + 1);

	sizeOfLevel = newAddress->memSize - sizeof(struct MemHeader);

	((struct Level*)(newAddress + 1))->terrain = (struct _Terrain*)OFFSET_DATA(((struct Level*)(newAddress + 1))->terrain, offset);

	level->lightList = (struct LightList*)OFFSET_DATA(level->lightList, offset);
	level->razielLightGroup = (struct LightGroup*)OFFSET_DATA(level->razielLightGroup, offset);
	level->razielSpectralLightGroup = (struct LightGroup*)OFFSET_DATA(level->razielSpectralLightGroup, offset);
	level->vmobjectList = (struct _VMObject*)OFFSET_DATA(level->vmobjectList, offset);
	level->spotLightList = (struct SpotLight*)OFFSET_DATA(level->spotLightList, offset);
	level->pointLightList = (struct PointLight*)OFFSET_DATA(level->pointLightList, offset);
	level->spotSpecturalLightList = (struct SpotLight*)OFFSET_DATA(level->spotSpecturalLightList, offset);
	level->pointSpecturalLightList = (struct PointLight*)OFFSET_DATA(level->pointSpecturalLightList, offset);
	level->bgObjectList = (struct _BGObject*)OFFSET_DATA(level->bgObjectList, offset);
	level->cameraList = (void*)OFFSET_DATA(level->cameraList, offset);
	level->vGroupList = (struct _VGroup*)OFFSET_DATA(level->vGroupList, offset);
	level->startSignal = (struct _MultiSignal*)OFFSET_DATA(level->startSignal, offset);
	level->introList = (struct Intro*)OFFSET_DATA(level->introList, offset);
	level->bgAniList = (struct DrMoveAniTex*)OFFSET_DATA(level->bgAniList, offset);
	level->hotSpotList = (struct _HotSpot*)OFFSET_DATA(level->hotSpotList, offset);
	level->objectNameList = (void*)OFFSET_DATA(level->objectNameList, offset);
	level->worldName = (char*)OFFSET_DATA(level->worldName, offset);
	level->enemyUnitsNames = (char**)OFFSET_DATA(level->enemyUnitsNames, offset);
	level->timesSignalList = (long**)OFFSET_DATA(level->timesSignalList, offset);
	level->spectralSignal = (struct _MultiSignal*)OFFSET_DATA(level->spectralSignal, offset);
	level->materialSignal = (struct _MultiSignal*)OFFSET_DATA(level->materialSignal, offset);
	level->startUnitLoadedSignal = (struct _MultiSignal*)OFFSET_DATA(level->startUnitLoadedSignal, offset);
	level->startUnitMainSignal = (struct _MultiSignal*)OFFSET_DATA(level->startUnitMainSignal, offset);
	level->startGoingIntoWaterSignal = (struct _MultiSignal*)OFFSET_DATA(level->startGoingIntoWaterSignal, offset);
	level->startGoingOutOfWaterSignal = (struct _MultiSignal*)OFFSET_DATA(level->startGoingOutOfWaterSignal, offset);
	level->SignalListStart = (struct _MultiSignal*)OFFSET_DATA(level->SignalListStart, offset);
	level->SignalListEnd = (struct _MultiSignal*)OFFSET_DATA(level->SignalListEnd, offset);
	level->PuzzleInstances = (struct EventPointers*)OFFSET_DATA(level->PuzzleInstances, offset);
	level->PlanMarkerList = (struct _PlanMkr*)OFFSET_DATA(level->PlanMarkerList, offset);
	level->SFXMarkerList = (struct _SFXMkr*)OFFSET_DATA(level->SFXMarkerList, offset);

	for (i = 0; i < level->NumberOfSFXMarkers; i++)
	{
		level->SFXMarkerList[i].soundData = (unsigned char*)OFFSET_DATA(level->SFXMarkerList[i].soundData, offset);
	}

	level->dynamicMusicName = (char*)OFFSET_DATA(level->dynamicMusicName, offset);
	level->spectrallightList = (struct LightList*)OFFSET_DATA(level->spectrallightList, offset);

	if (level->terrain != NULL)
	{
		struct _Terrain* terrain;

		terrain = level->terrain;

		terrain->introList = (struct Intro*)OFFSET_DATA(terrain->introList, offset);
		terrain->vertexList = (struct _TVertex*)OFFSET_DATA(terrain->vertexList, offset);
		terrain->faceList = (struct _TFace*)OFFSET_DATA(terrain->faceList, offset);
		terrain->normalList = (struct _Normal*)OFFSET_DATA(terrain->normalList, offset);
		terrain->aniList = (struct DrMoveAniTex*)OFFSET_DATA(terrain->aniList, offset);
		terrain->StreamUnits = (void*)OFFSET_DATA(terrain->StreamUnits, offset);
		terrain->StartTextureList = (struct TextureFT3*)OFFSET_DATA(terrain->StartTextureList, offset);
		terrain->EndTextureList = (struct TextureFT3*)OFFSET_DATA(terrain->EndTextureList, offset);
		terrain->MorphDiffList = (struct _MorphVertex*)OFFSET_DATA(terrain->MorphDiffList, offset);
		terrain->MorphColorList = (struct _MorphColor*)OFFSET_DATA(terrain->MorphColorList, offset);
		terrain->BSPTreeArray = (struct BSPTree*)OFFSET_DATA(terrain->BSPTreeArray, offset);
		terrain->morphNormalIdx = (short*)OFFSET_DATA(terrain->morphNormalIdx, offset);
		terrain->signals = (struct _MultiSignal*)OFFSET_DATA(terrain->signals, offset);

		for (i = 0; i < terrain->numIntros; i++)
		{
			struct Intro* intro;

			intro = &terrain->introList[i];

			intro->data = (void*)OFFSET_DATA(intro->data, offset);
			intro->multiSpline = (struct MultiSpline*)OFFSET_DATA(intro->multiSpline, offset);

			if (intro->multiSpline != NULL)
			{
				struct MultiSpline* multiSpline;

				multiSpline = intro->multiSpline;

				multiSpline->positional = (struct Spline*)OFFSET_DATA(multiSpline->positional, offset);
				multiSpline->rotational = (struct RSpline*)OFFSET_DATA(multiSpline->rotational, offset);
				multiSpline->scaling = (struct Spline*)OFFSET_DATA(multiSpline->scaling, offset);
				multiSpline->color = (struct Spline*)OFFSET_DATA(multiSpline->color, offset);

				if (multiSpline->positional != NULL)
				{
					multiSpline->positional->key = (struct SplineKey*)OFFSET_DATA(multiSpline->positional->key, offset);
				}

				if (multiSpline->rotational != NULL)
				{
					multiSpline->rotational->key = (struct SplineRotKey*)OFFSET_DATA(multiSpline->rotational->key, offset);
				}

				if (multiSpline->scaling != NULL)
				{
					multiSpline->scaling->key = (struct SplineKey*)OFFSET_DATA(multiSpline->scaling->key, offset);
				}

				if (multiSpline->color != NULL)
				{
					multiSpline->color->key = (struct SplineKey*)OFFSET_DATA(multiSpline->color->key, offset);
				}
			}

			intro->dsignal = (void*)OFFSET_DATA(intro->dsignal, offset);
		}

		if (terrain->aniList != NULL)
		{
			if (terrain->aniList->numAniTextues > 0)
			{
				struct DrMoveAniTexDestInfo** dest;

				dest = &terrain->aniList->aniTexInfo;

				for (d = 0; d < terrain->aniList->numAniTextues; d++)
				{
					dest[d] = (struct DrMoveAniTexDestInfo*)OFFSET_DATA(dest[d], offset);
				}
			}
		}

		if (level->lightList != NULL)
		{
			level->lightList->lightGroupList = (struct LightGroup*)OFFSET_DATA(level->lightList->lightGroupList, offset);
		}

		if (level->spectrallightList != NULL)
		{
			level->spectrallightList->lightGroupList = (struct LightGroup*)OFFSET_DATA(level->spectrallightList->lightGroupList, offset);
		}

		for (i = 0; i < level->numVMObjects; i++)
		{
			struct _VMObject* vmo;

			vmo = &level->vmobjectList[i];

			vmo->vmoffsetTableList = (struct _VMOffsetTable**)OFFSET_DATA(vmo->vmoffsetTableList, offset);

			if (vmo->curVMOffsetTable == vmo->vmoffsetTableList[vmo->currentIdx])
			{
				vmo->curVMOffsetTable = (struct _VMOffsetTable*)OFFSET_DATA(vmo->curVMOffsetTable, offset);
			}

			for (d = 0; d < vmo->numVMOffsetTables; d++)
			{
				vmo->vmoffsetTableList[d] = (struct _VMOffsetTable*)OFFSET_DATA(vmo->vmoffsetTableList[d], offset);
			}

			vmo->vmvertexList = (void*)OFFSET_DATA(vmo->vmvertexList, offset);
			vmo->vminterpolatedList = (struct _VMInterpolated*)OFFSET_DATA(vmo->vminterpolatedList, offset);
			vmo->name = (char*)OFFSET_DATA(vmo->name, offset);
		}
		{
			struct BSPTree* bsp;
			struct _BSPNode* node;
			struct _BSPLeaf* leaf;

			bsp = terrain->BSPTreeArray;

			for (d = 0; d < terrain->numBSPTrees; d++, bsp++)
			{
				bsp->bspRoot = (struct _BSPNode*)OFFSET_DATA(bsp->bspRoot, offset);
				bsp->startLeaves = (struct _BSPLeaf*)OFFSET_DATA(bsp->startLeaves, offset);
				bsp->endLeaves = (struct _BSPLeaf*)OFFSET_DATA(bsp->endLeaves, offset);

				for (node = bsp->bspRoot; (struct _BSPLeaf*)node < bsp->startLeaves; node++)
				{
					node->front = (void*)OFFSET_DATA(node->front, offset);
					node->back = (void*)OFFSET_DATA(node->back, offset);
				}

				for (leaf = bsp->startLeaves; leaf < bsp->endLeaves; leaf++)
				{
					leaf->faceList = (struct _TFace*)OFFSET_DATA(leaf->faceList, offset);
				}
			}
		}
	}

	for (msignal = level->SignalListStart; msignal < level->SignalListEnd; )
	{
		msignal = SIGNAL_RelocateSignal(msignal, offset);
	}

	EVENT_UpdatePuzzlePointers(level->PuzzleInstances, offset);
	STREAM_UpdateLevelPointer(oldLevel, level, sizeOfLevel);
	EVENT_RelocateInstanceList(oldLevel, level, sizeOfLevel);
}

void MEMPACK_RelocateG2AnimKeylistType(struct _G2AnimKeylist_Type** pKeylist, int offset, char* start, char* end) // Matching - 100%
{
	int j;
	struct _G2AnimKeylist_Type* keylist;

	if (((char*)*pKeylist >= start) && ((char*)*pKeylist < end))
	{
		*pKeylist = (struct _G2AnimKeylist_Type*)OFFSET_DATA(*pKeylist, offset);

		keylist = *pKeylist;

		if ((keylist->sectionCount != 255) || (keylist->s0TailTime != 224) || (keylist->s1TailTime != 172) || (keylist->s2TailTime != 15))
		{
			keylist->fxList = (struct _G2AnimFxHeader_Type*)OFFSET_DATA(keylist->fxList, offset);

			for (j = 0; j < keylist->sectionCount; j++)
			{
				keylist->sectionData[j] = (unsigned short*)OFFSET_DATA(keylist->sectionData[j], offset);
			}
		}
	}
}

void MEMPACK_RelocateObjectType(struct MemHeader* newAddress, long offset, struct Object* oldObject)  // Matching - 100%
{
	struct _Instance* instance;
	struct Object* object;
	int i;
	int j;
	int d;
	int sizeOfObject;
	struct _Model* model;

	object = (struct Object*)(newAddress + 1);

	sizeOfObject = newAddress->memSize - sizeof(struct MemHeader);

	object->modelList = (struct _Model**)OFFSET_DATA(object->modelList, offset);
	object->animList = (struct _G2AnimKeylist_Type**)OFFSET_DATA(object->animList, offset);
	object->soundData = (unsigned char*)OFFSET_DATA(object->soundData, offset);
	object->data = (void*)OFFSET_DATA(object->data, offset);
	object->script = (char*)OFFSET_DATA(object->script, offset);
	object->name = (char*)OFFSET_DATA(object->name, offset);
	object->effectList = (struct ObjectEffect*)OFFSET_DATA(object->effectList, offset);

	if (((struct Object*)(newAddress + 1))->oflags & 0x8000000)
	{
		object->relocList = (unsigned long*)OFFSET_DATA(object->relocList, offset);
		object->relocModule = (void*)OFFSET_DATA(object->relocModule, offset);
	}

	for (i = 0; i < object->numModels; i++)
	{
		object->modelList[i] = (struct _Model*)(long)OFFSET_DATA(object->modelList[i], offset);

		model = object->modelList[i];

		model->vertexList = (struct _MVertex*)OFFSET_DATA(model->vertexList, offset);
		model->normalList = (struct _SVectorNoPad*)OFFSET_DATA(model->normalList, offset);
		model->faceList = (struct _MFace*)OFFSET_DATA(model->faceList, offset);
		model->segmentList = (struct _Segment*)OFFSET_DATA(model->segmentList, offset);
		model->aniTextures = (struct AniTex*)OFFSET_DATA(model->aniTextures, offset);
		model->multiSpline = (struct MultiSpline*)OFFSET_DATA(model->multiSpline, offset);
		model->startTextures = (struct TextureMT3*)OFFSET_DATA(model->startTextures, offset);
		model->endTextures = (struct TextureMT3*)OFFSET_DATA(model->endTextures, offset);

		for (d = 0; d < model->numFaces; d++)
		{
			struct _MFace* mface;

			mface = &model->faceList[d];

			if (mface->flags & 2)
			{
				mface->color = (long)OFFSET_DATA(mface->color, offset);
			}
		}

		for (d = 0; d < model->numSegments; d++)
		{
			struct _Segment* segment;
			struct _HInfo* hInfo;

			segment = &model->segmentList[d];

			segment->hInfo = (struct _HInfo*)OFFSET_DATA(segment->hInfo, offset);

			if (segment->hInfo != NULL)
			{
				hInfo = segment->hInfo;

				hInfo->hfaceList = (struct _HFace*)OFFSET_DATA(hInfo->hfaceList, offset);
				hInfo->hsphereList = (struct _HSphere*)OFFSET_DATA(hInfo->hsphereList, offset);
				hInfo->hboxList = (struct _HBox*)OFFSET_DATA(hInfo->hboxList, offset);
			}
		}

		if (model->aniTextures != NULL)
		{
			struct AniTexInfo* aniTexInfo;

			aniTexInfo = &model->aniTextures->aniTexInfo;

			for (d = 0; d < model->aniTextures->numAniTextues; d++, aniTexInfo++)
			{
				aniTexInfo->texture = (struct TextureMT3*)OFFSET_DATA(aniTexInfo->texture, offset);
			}
		}

		{
			struct MultiSpline* multiSpline;

			if (model->multiSpline != NULL)
			{
				multiSpline = model->multiSpline;

				multiSpline->positional = (struct Spline*)OFFSET_DATA(multiSpline->positional, offset);
				multiSpline->rotational = (struct RSpline*)OFFSET_DATA(multiSpline->rotational, offset);
				multiSpline->scaling = (struct Spline*)OFFSET_DATA(multiSpline->scaling, offset);
				multiSpline->color = (struct Spline*)OFFSET_DATA(multiSpline->color, offset);

				if (multiSpline->positional != NULL)
				{
					multiSpline->positional->key = (struct SplineKey*)OFFSET_DATA(multiSpline->positional->key, offset);
				}

				if (multiSpline->rotational != NULL)
				{
					multiSpline->rotational->key = (struct SplineRotKey*)OFFSET_DATA(multiSpline->rotational->key, offset);
				}

				if (multiSpline->scaling != NULL)
				{
					multiSpline->scaling->key = (struct SplineKey*)OFFSET_DATA(multiSpline->scaling->key, offset);
				}

				if (multiSpline->color != NULL)
				{
					multiSpline->color->key = (struct SplineKey*)OFFSET_DATA(multiSpline->color->key, offset);
				}
			}
		}
	}

	for (i = 0; i < object->numAnims; i++)
	{
		MEMPACK_RelocateG2AnimKeylistType(&object->animList[i], offset, (char*)oldObject, (char*)oldObject + sizeOfObject);
	}

	if (object->animList != NULL)
	{
		instance = gameTrackerX.instanceList->first;

		if (instance != NULL)
		{
			do
			{
				if (instance->object == oldObject)
				{
					instance->anim.modelData = (struct _Model*)OFFSET_DATA(instance->anim.modelData, offset);
				}

				for (j = 0; j < instance->anim.sectionCount; j++)
				{
					if (((unsigned int)instance->anim.section[j].keylist >= (unsigned int)oldObject) && ((unsigned int)((char*)oldObject + sizeOfObject) >= (unsigned int)instance->anim.section[j].keylist))
					{
						instance->anim.section[j].keylist = (struct _G2AnimKeylist_Type*)OFFSET_DATA(instance->anim.section[j].keylist, offset);
					}
				}

				instance = instance->next;
			} while (instance != NULL);
		}
	}

	STREAM_UpdateObjectPointer(oldObject, object, sizeOfObject);
}

void MEMPACK_RelocateCDMemory(struct MemHeader *newAddress, long offset, struct _BigFileDir *oldDir)
{ 
	struct _BigFileDir *newDir;
	newDir = (struct _BigFileDir*)(newAddress + 1);
	LOAD_UpdateBigFilePointers(oldDir, newDir);
}
