#include "CORE.H"
#include "MEMPACK.H"
#include "PSX/DRAWS.H"
#include "STREAM.H"
#include "DEBUG.H"
#include "PSX/AADLIB.H"
#include "LOAD3D.H"
#include "Game/EVENT.H"

#include <stddef.h>

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
		if (address->memStatus == 0 && address->memSize >= allocSize && bestAddress == NULL)
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
		if (address->memStatus == 0 && address->memSize >= allocSize && (bestAddress == NULL || (char*)bestAddress < (char*)address))
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

// autogenerated function stub: 
// char * /*$ra*/ MEMPACK_MallocFailOk(unsigned long allocSize /*$s1*/, unsigned char memType /*$s3*/)
char * MEMPACK_MallocFailOk(unsigned long allocSize, unsigned char memType)
{ // line 349, offset 0x8004fc94
	/* begin block 1 */
		// Start line: 350
		// Start offset: 0x8004FC94
		// Variables:
			struct MemHeader *bestAddress; // $a1
			long relocatableMemory; // $s2

		/* begin block 1.1 */
			// Start line: 378
			// Start offset: 0x8004FD18
			// Variables:
				int curMem; // $s0
		/* end block 1.1 */
		// End offset: 0x8004FD34
		// End Line: 390

		/* begin block 1.2 */
			// Start line: 404
			// Start offset: 0x8004FD60
			// Variables:
				struct MemHeader *address; // $a2

			/* begin block 1.2.1 */
				// Start line: 424
				// Start offset: 0x8004FD94
				// Variables:
					long topOffset; // $a0
			/* end block 1.2.1 */
			// End offset: 0x8004FD94
			// End Line: 426
		/* end block 1.2 */
		// End offset: 0x8004FD94
		// End Line: 426
	/* end block 1 */
	// End offset: 0x8004FE04
	// End Line: 457

	/* begin block 2 */
		// Start line: 733
	/* end block 2 */
	// End Line: 734
					UNIMPLEMENTED();
	return null;
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
			newAddress = MEMPACK_GarbageCollectMalloc((unsigned long*)&holdSize, addressMemType, (unsigned long*)&freeSize);
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


void MEMPACK_RelocateAreaType(struct MemHeader* newAddress, long offset, struct Level* oldLevel)
{
	int v5; // $a1
	struct Level* level; // $s0
	int v7; // $v1
	int v8; // $s2
	int memSize; // $v0
	struct LightList* v10; // $v1
	int v11; // $v0
	struct LightGroup* v12; // $a0
	struct LightGroup* razielSpectralLightGroup; // $v0
	struct LightGroup* v14; // $v1
	struct _VMObject* vmobjectList; // $v0
	struct _VMObject* v16; // $a0
	struct SpotLight* spotLightList; // $v0
	struct SpotLight* v18; // $v1
	struct PointLight* pointLightList; // $v0
	struct PointLight* v20; // $a0
	struct SpotLight* spotSpecturalLightList; // $v0
	struct SpotLight* v22; // $v1
	struct PointLight* pointSpecturalLightList; // $v0
	struct PointLight* v24; // $a0
	struct _BGObject* bgObjectList; // $v0
	struct _BGObject* v26; // $v1
	int cameraList; // $v0
	int v28; // $a0
	struct _VGroup* vGroupList; // $v0
	struct _VGroup* v30; // $v1
	struct _MultiSignal* startSignal; // $v0
	struct _MultiSignal* msignal; // $a0
	struct Intro* introList; // $v0
	struct Intro* v34; // $v1
	struct DrMoveAniTex* bgAniList; // $v0
	struct DrMoveAniTex* v36; // $a0
	struct _HotSpot* hotSpotList; // $v0
	struct _HotSpot* v38; // $v1
	int objectNameList; // $v0
	int v40; // $a0
	int v41; // $v0
	int v42; // $v1
	int v43; // $v0
	int v44; // $a0
	int timesSignalList; // $v0
	int v46; // $v1
	struct _MultiSignal* spectralSignal; // $v0
	struct _MultiSignal* v48; // $a0
	struct _MultiSignal* materialSignal; // $v0
	struct _MultiSignal* v50; // $v1
	struct _MultiSignal* startUnitLoadedSignal; // $v0
	struct _MultiSignal* v52; // $a0
	struct _MultiSignal* startUnitMainSignal; // $v0
	struct _MultiSignal* v54; // $v1
	struct _MultiSignal* startGoingIntoWaterSignal; // $v0
	struct _MultiSignal* v56; // $a0
	struct _MultiSignal* startGoingOutOfWaterSignal; // $v0
	struct _MultiSignal* v58; // $v1
	struct _MultiSignal* SignalListStart; // $v0
	struct _MultiSignal* v60; // $a0
	struct _MultiSignal* SignalListEnd; // $v0
	struct _MultiSignal* v62; // $v1
	struct EventPointers* PuzzleInstances; // $v0
	struct EventPointers* v64; // $a0
	struct _PlanMkr* PlanMarkerList; // $v0
	struct _PlanMkr* v66; // $v1
	struct _SFXMkr* SFXMarkerList; // $v0
	struct _SFXMkr* v68; // $a0
	int NumberOfSFXMarkers; // $v0
	int i; // $t0 MAPDST
	struct _SFXMkr* v72; // $a0
	int v73; // $v1
	int v74; // $v0
	int v75; // $v1
	struct LightList* spectrallightList; // $v0
	struct LightList* v77; // $a0
	struct _Terrain* terrain_r; // $v0
	int v79; // dc
	struct _Terrain* terrain; // $t1
	struct Intro* intro; // $v0
	struct Intro* v82; // $v1
	struct _TVertex* vertexList; // $v0
	struct _TVertex* v84; // $a0
	struct _TFace* faceList; // $v0
	struct _TFace* v86; // $v1
	struct _Normal* normalList; // $v0
	struct _Normal* v88; // $a0
	struct DrMoveAniTex* aniList; // $v0
	struct DrMoveAniTex* v90; // $v1
	int StreamUnits; // $v0
	int v92; // $a0
	struct TextureFT3* StartTextureList; // $v0
	struct TextureFT3* v94; // $v1
	struct TextureFT3* EndTextureList; // $v0
	struct TextureFT3* v96; // $a0
	struct _MorphVertex* MorphDiffList; // $v0
	struct _MorphVertex* v98; // $v1
	struct _MorphColor* MorphColorList; // $v0
	struct _MorphColor* v100; // $a0
	struct BSPTree* BSPTreeArray; // $v0
	struct BSPTree* v102; // $v1
	int v103; // $v0
	int v104; // $a0
	struct _MultiSignal* signals; // $v0
	struct _MultiSignal* v106; // $v1
	int numIntros; // $v0
	struct Intro* v110; // $v0
	int data; // $v1
	int v112; // $a1
	struct MultiSpline* v1155; // $a0
	struct MultiSpline* v114; // $v1
	struct MultiSpline* multiSpline; // $a2
	struct Spline* positional; // $v1
	struct Spline* v117; // $a0
	struct RSpline* rotational; // $v1
	struct RSpline* v119; // $a1
	struct Spline* scaling; // $v1
	struct Spline* v121; // $a3
	struct Spline* color; // $v1
	struct Spline* v123; // $a0
	struct Spline* v124; // $v1
	int v125; // $a0
	struct RSpline* v126; // $v1
	int v127; // $a0
	struct Spline* v128; // $v1
	int v129; // $a0
	struct Spline* v130; // $v1
	int v131; // $a0
	int dsignal; // $v1
	int v133; // $a0
	int* p_numAniTextues; // $v0
	int v135; // $v1
	int v136; // $a3
	int* v137; // $v0
	int* v138; // $a0
	int v139; // $v0
	struct LightList* lightList; // $v0
	struct LightGroup* lightGroupList; // $a0
	struct LightGroup* v142; // $v1
	struct LightList* v143; // $v0
	struct LightGroup* v144; // $a0
	struct LightGroup* v145; // $v1
	struct _VMObject* vmo; // $a1
	struct _VMOffsetTable* vmoffsetTableList; // $v0
	struct _VMOffsetTable* v150; // $v1
	int currentIdx; // $v0
	struct _VMOffsetTable** v152; // $v0
	struct _VMOffsetTable* curVMOffsetTable; // $v1
	struct _VMOffsetTable* v154; // $v0
	int d; // $a3
	struct _VMOffsetTable* v156; // $v0
	int v157; // $v1
	int vmvertexList; // $v0
	int v159; // $v1
	struct _VMInterpolated* vminterpolatedList; // $v0
	struct _VMInterpolated* v161; // $a0
	int v162; // $v0
	int v163; // $v1
	struct BSPTree* bsp; // $t2
	int v165; // $a3
	int v167; // $v0
	struct _BSPLeaf* v168; // $v0
	unsigned int v169; // $a0
	unsigned int v170; // $v1
	unsigned int v171; // $v0
	struct _BSPNode* node; // $a2
	int* v173; // $a0
	int v174; // $v0
	struct _BSPLeaf* leaf; // $a1
	int v176; // $v1
	int v177; // $v0
	unsigned int v178; // $a1
	int* v179; // $a0
	int v180; // $v0
	struct _MultiSignal* j; // $a0

	v5 = 0;
	level = (struct Level*)&newAddress[1];
	v7 = *(int*)&newAddress[1].magicNumber;
	v8 = newAddress->memSize - 8;
	if (v7)
		v5 = v7 + offset;
	*(int*)&newAddress[1].magicNumber = v5;
	memSize = newAddress[1].memSize;
	v10 = 0;
	if (memSize)
		v10 = (struct LightList*)(memSize + offset);
	v11 = *(int*)&newAddress[49].magicNumber;
	v12 = 0;
	level->lightList = v10;
	if (v11)
		v12 = (struct LightGroup*)(v11 + offset);
	razielSpectralLightGroup = level->razielSpectralLightGroup;
	v14 = 0;
	level->razielLightGroup = v12;
	if (razielSpectralLightGroup)
		v14 = (struct LightGroup*)((char*)razielSpectralLightGroup + offset);
	vmobjectList = level->vmobjectList;
	v16 = 0;
	level->razielSpectralLightGroup = v14;
	if (vmobjectList)
		v16 = (struct _VMObject*)((char*)vmobjectList + offset);
	spotLightList = level->spotLightList;
	v18 = 0;
	level->vmobjectList = v16;
	if (spotLightList)
		v18 = (struct SpotLight*)((char*)spotLightList + offset);
	pointLightList = level->pointLightList;
	v20 = 0;
	level->spotLightList = v18;
	if (pointLightList)
		v20 = (struct PointLight*)((char*)pointLightList + offset);
	spotSpecturalLightList = level->spotSpecturalLightList;
	v22 = 0;
	level->pointLightList = v20;
	if (spotSpecturalLightList)
		v22 = (struct SpotLight*)((char*)spotSpecturalLightList + offset);
	pointSpecturalLightList = level->pointSpecturalLightList;
	v24 = 0;
	level->spotSpecturalLightList = v22;
	if (pointSpecturalLightList)
		v24 = (struct PointLight*)((char*)pointSpecturalLightList + offset);
	bgObjectList = level->bgObjectList;
	v26 = 0;
	level->pointSpecturalLightList = v24;
	if (bgObjectList)
		v26 = (struct _BGObject*)((char*)bgObjectList + offset);
	cameraList = (int)level->cameraList;
	v28 = 0;
	level->bgObjectList = v26;
	if (cameraList)
		v28 = cameraList + offset;
	vGroupList = level->vGroupList;
	v30 = 0;
	level->cameraList = (void*)v28;
	if (vGroupList)
		v30 = (struct _VGroup*)((char*)vGroupList + offset);
	startSignal = level->startSignal;
	msignal = 0;
	level->vGroupList = v30;
	if (startSignal)
		msignal = (struct _MultiSignal*)((char*)startSignal + offset);
	introList = level->introList;
	v34 = 0;
	level->startSignal = msignal;
	if (introList)
		v34 = (struct Intro*)((char*)introList + offset);
	bgAniList = level->bgAniList;
	v36 = 0;
	level->introList = v34;
	if (bgAniList)
		v36 = (struct DrMoveAniTex*)((char*)bgAniList + offset);
	hotSpotList = level->hotSpotList;
	v38 = 0;
	level->bgAniList = v36;
	if (hotSpotList)
		v38 = (struct _HotSpot*)((char*)hotSpotList + offset);
	objectNameList = (int)level->objectNameList;
	v40 = 0;
	level->hotSpotList = v38;
	if (objectNameList)
		v40 = objectNameList + offset;
	v41 = *(int*)level->worldName;
	v42 = 0;
	level->objectNameList = (void*)v40;
	if (v41)
		v42 = v41 + offset;
	v44 = 0;
	*(int*)level->worldName = v42;
	if (level->enemyUnitsNames)
		v44 = (int)level->enemyUnitsNames + offset;
	timesSignalList = (int)level->timesSignalList;
	v46 = 0;
	level->enemyUnitsNames = (char**)v44;
	if (timesSignalList)
		v46 = timesSignalList + offset;
	spectralSignal = level->spectralSignal;
	v48 = 0;
	level->timesSignalList = (long**)v46;
	if (spectralSignal)
		v48 = (struct _MultiSignal*)((char*)spectralSignal + offset);
	materialSignal = level->materialSignal;
	v50 = 0;
	level->spectralSignal = v48;
	if (materialSignal)
		v50 = (struct _MultiSignal*)((char*)materialSignal + offset);
	startUnitLoadedSignal = level->startUnitLoadedSignal;
	v52 = 0;
	level->materialSignal = v50;
	if (startUnitLoadedSignal)
		v52 = (struct _MultiSignal*)((char*)startUnitLoadedSignal + offset);
	startUnitMainSignal = level->startUnitMainSignal;
	v54 = 0;
	level->startUnitLoadedSignal = v52;
	if (startUnitMainSignal)
		v54 = (struct _MultiSignal*)((char*)startUnitMainSignal + offset);
	startGoingIntoWaterSignal = level->startGoingIntoWaterSignal;
	v56 = 0;
	level->startUnitMainSignal = v54;
	if (startGoingIntoWaterSignal)
		v56 = (struct _MultiSignal*)((char*)startGoingIntoWaterSignal + offset);
	startGoingOutOfWaterSignal = level->startGoingOutOfWaterSignal;
	v58 = 0;
	level->startGoingIntoWaterSignal = v56;
	if (startGoingOutOfWaterSignal)
		v58 = (struct _MultiSignal*)((char*)startGoingOutOfWaterSignal + offset);
	SignalListStart = level->SignalListStart;
	v60 = 0;
	level->startGoingOutOfWaterSignal = v58;
	if (SignalListStart)
		v60 = (struct _MultiSignal*)((char*)SignalListStart + offset);
	SignalListEnd = level->SignalListEnd;
	v62 = 0;
	level->SignalListStart = v60;
	if (SignalListEnd)
		v62 = (struct _MultiSignal*)((char*)SignalListEnd + offset);
	PuzzleInstances = level->PuzzleInstances;
	v64 = 0;
	level->SignalListEnd = v62;
	if (PuzzleInstances)
		v64 = (struct EventPointers*)((char*)PuzzleInstances + offset);
	PlanMarkerList = level->PlanMarkerList;
	v66 = 0;
	level->PuzzleInstances = v64;
	if (PlanMarkerList)
		v66 = (struct _PlanMkr*)((char*)PlanMarkerList + offset);
	SFXMarkerList = level->SFXMarkerList;
	v68 = 0;
	level->PlanMarkerList = v66;
	if (SFXMarkerList)
		v68 = (struct _SFXMkr*)((char*)SFXMarkerList + offset);
	NumberOfSFXMarkers = level->NumberOfSFXMarkers;
	i = 0;
	level->SFXMarkerList = v68;
	if (NumberOfSFXMarkers > 0)
	{
		i = 0;
		do
		{
			v72 = (struct _SFXMkr*)((char*)level->SFXMarkerList + i);
			v73 = 0;
			if (*(int*)v72->soundData)
				v73 = *(int*)v72->soundData + offset;
			*(int*)v72->soundData = v73;
			++i;
			i += 36;
		} while (i < level->NumberOfSFXMarkers);
	}
	v74 = *(int*)level->dynamicMusicName;
	v75 = 0;
	if (v74)
		v75 = v74 + offset;
	spectrallightList = level->spectrallightList;
	v77 = 0;
	*(int*)level->dynamicMusicName = v75;
	if (spectrallightList)
		v77 = (struct LightList*)((char*)spectrallightList + offset);
	terrain_r = level->terrain;
	v79 = level->terrain == 0;
	level->spectrallightList = v77;
	if (!v79)
	{
		terrain = terrain_r;
		intro = terrain_r->introList;
		v82 = 0;
		if (intro)
			v82 = (struct Intro*)((char*)intro + offset);
		vertexList = terrain->vertexList;
		v84 = 0;
		terrain->introList = v82;
		if (vertexList)
			v84 = (struct _TVertex*)((char*)vertexList + offset);
		faceList = terrain->faceList;
		v86 = 0;
		terrain->vertexList = v84;
		if (faceList)
			v86 = (struct _TFace*)((char*)faceList + offset);
		normalList = terrain->normalList;
		v88 = 0;
		terrain->faceList = v86;
		if (normalList)
			v88 = (struct _Normal*)((char*)normalList + offset);
		aniList = terrain->aniList;
		v90 = 0;
		terrain->normalList = v88;
		if (aniList)
			v90 = (struct DrMoveAniTex*)((char*)aniList + offset);
		StreamUnits = (int)terrain->StreamUnits;
		v92 = 0;
		terrain->aniList = v90;
		if (StreamUnits)
			v92 = StreamUnits + offset;
		StartTextureList = terrain->StartTextureList;
		v94 = 0;
		terrain->StreamUnits = (void*)v92;
		if (StartTextureList)
			v94 = (struct TextureFT3*)((char*)StartTextureList + offset);
		EndTextureList = terrain->EndTextureList;
		v96 = 0;
		terrain->StartTextureList = v94;
		if (EndTextureList)
			v96 = (struct TextureFT3*)((char*)EndTextureList + offset);
		MorphDiffList = terrain->MorphDiffList;
		v98 = 0;
		terrain->EndTextureList = v96;
		if (MorphDiffList)
			v98 = (struct _MorphVertex*)((char*)MorphDiffList + offset);
		MorphColorList = terrain->MorphColorList;
		v100 = 0;
		terrain->MorphDiffList = v98;
		if (MorphColorList)
			v100 = (struct _MorphColor*)((char*)MorphColorList + offset);
		BSPTreeArray = terrain->BSPTreeArray;
		v102 = 0;
		terrain->MorphColorList = v100;
		if (BSPTreeArray)
			v102 = (struct BSPTree*)((char*)BSPTreeArray + offset);
		v103 = *(int*)terrain->morphNormalIdx;
		v104 = 0;
		terrain->BSPTreeArray = v102;
		if (v103)
			v104 = v103 + offset;
		signals = terrain->signals;
		v106 = 0;
		*(int*)terrain->morphNormalIdx = v104;
		if (signals)
			v106 = (struct _MultiSignal*)((char*)signals + offset);
		numIntros = terrain->numIntros;
		i = 0;
		terrain->signals = v106;
		if (numIntros > 0)
		{
			i = 0;
			do
			{
				v110 = (struct Intro*)((char*)terrain->introList + i);
				data = (int)v110->data;
				v112 = 0;
				if (data)
					v112 = data + offset;
				v1155 = v110->multiSpline;
				v114 = 0;
				v110->data = (void*)v112;
				if (v1155)
					v114 = (struct MultiSpline*)((char*)v1155 + offset);
				v110->multiSpline = v114;
				if (v114)
				{
					multiSpline = v114;
					positional = v114->positional;
					v117 = 0;
					if (positional)
						v117 = (struct Spline*)((char*)positional + offset);
					rotational = multiSpline->rotational;
					v119 = 0;
					multiSpline->positional = v117;
					if (rotational)
						v119 = (struct RSpline*)((char*)rotational + offset);
					scaling = multiSpline->scaling;
					v121 = 0;
					multiSpline->rotational = v119;
					if (scaling)
						v121 = (struct Spline*)((char*)scaling + offset);
					color = multiSpline->color;
					v123 = 0;
					multiSpline->scaling = v121;
					if (color)
						v123 = (struct Spline*)((char*)color + offset);
					v124 = multiSpline->positional;
					v79 = multiSpline->positional == 0;
					multiSpline->color = v123;
					if (!v79)
					{
						v125 = 0;
						if (v124->key)
							v125 = (int)v124->key + offset;
						v124->key = (struct SplineKey*)v125;
					}
					v126 = multiSpline->rotational;
					if (v126)
					{
						v127 = 0;
						if (v126->key)
							v127 = (int)v126->key + offset;
						v126->key = (struct SplineRotKey*)v127;
					}
					v128 = multiSpline->scaling;
					if (v128)
					{
						v129 = 0;
						if (v128->key)
							v129 = (int)v128->key + offset;
						v128->key = (struct SplineKey*)v129;
					}
					v130 = multiSpline->color;
					if (v130)
					{
						v131 = 0;
						if (v130->key)
							v131 = (int)v130->key + offset;
						v130->key = (struct SplineKey*)v131;
					}
				}
				dsignal = (int)v110->dsignal;
				v133 = 0;
				if (dsignal)
					v133 = dsignal + offset;
				v110->dsignal = (void*)v133;
				++i;
				i += 76;
			} while (i < terrain->numIntros);
		}
		if (terrain->aniList->numAniTextues)
		{
			v135 = *p_numAniTextues;
			if (terrain->aniList->numAniTextues > 0)
			{
				v136 = 0;
				v137 = p_numAniTextues + 1;
				if (v135 > 0)
				{
					v138 = v137;
					do
					{
						v139 = 0;
						if (*v138)
							v139 = *v138 + offset;
						*v138 = v139;
						++v136;
						++v138;
					} while (v136 < terrain->aniList->numAniTextues);
				}
			}
		}
		lightList = level->lightList;
		if (lightList)
		{
			lightGroupList = lightList->lightGroupList;
			v142 = 0;
			if (lightGroupList)
				v142 = (struct LightGroup*)((char*)lightGroupList + offset);
			lightList->lightGroupList = v142;
		}
		v143 = level->spectrallightList;
		if (v143)
		{
			v144 = v143->lightGroupList;
			v145 = 0;
			if (v144)
				v145 = (struct LightGroup*)((char*)v144 + offset);
			v143->lightGroupList = v145;
		}
		i = 0;
		if (level->numVMObjects > 0)
		{
			i = 0;
			do
			{
				vmo = (struct _VMObject*)((char*)level->vmobjectList + i);
				vmoffsetTableList = (struct _VMOffsetTable*)vmo->vmoffsetTableList;
				v150 = 0;
				if (vmoffsetTableList)
					v150 = (struct _VMOffsetTable*)((char*)vmoffsetTableList + offset);
				currentIdx = vmo->currentIdx;
				vmo->vmoffsetTableList = (struct _VMOffsetTable**)v150;
				v152 = (struct _VMOffsetTable**)(&v150->numVMOffsets + currentIdx);
				curVMOffsetTable = vmo->curVMOffsetTable;
				if (curVMOffsetTable == *v152)
				{
					v154 = 0;
					if (curVMOffsetTable)
						v154 = (struct _VMOffsetTable*)((char*)curVMOffsetTable + offset);
					vmo->curVMOffsetTable = v154;
				}
				for (d = 0; d < vmo->numVMOffsetTables; ++d)
				{
					v156 = vmo->vmoffsetTableList[d];
					v157 = 0;
					if (v156->numVMOffsets)
						v157 = v156->numVMOffsets + offset;
					v156->numVMOffsets = v157;
				}
				vmvertexList = (int)vmo->vmvertexList;
				v159 = 0;
				if (vmvertexList)
					v159 = vmvertexList + offset;
				vminterpolatedList = vmo->vminterpolatedList;
				v161 = 0;
				vmo->vmvertexList = (void*)v159;
				if (vminterpolatedList)
					v161 = (struct _VMInterpolated*)((char*)vminterpolatedList + offset);
				v162 = *(int*)vmo->name;
				v163 = 0;
				vmo->vminterpolatedList = v161;
				if (v162)
					v163 = v162 + offset;
				*(int*)vmo->name = v163;
				++i;
				i += 60;
			} while (i < level->numVMObjects);
		}
		bsp = terrain->BSPTreeArray;
		v165 = 0;
		if (terrain->numBSPTrees > 0)
		{
			do
			{
				v167 = 0;
				if (bsp->bspRoot)
					v167 = (int)bsp->bspRoot + offset;
				bsp->bspRoot = (struct _BSPNode*)v167;
				v168 = bsp->startLeaves;
				v169 = 0;
				if (v168)
					v169 = (unsigned int)v168 + offset;
				v170 = (unsigned int)bsp->endLeaves;
				v171 = 0;
				v79 = bsp->endLeaves == 0;
				bsp->startLeaves = (struct _BSPLeaf*)v169;
				if (!v79)
					v171 = v170 + offset;
				bsp->endLeaves = (struct _BSPLeaf*)v171;
				node = bsp->bspRoot;
				if (bsp->bspRoot < (struct _BSPNode*)bsp->startLeaves)
				{
					v173 = (int*)&node->back;
					do
					{
						v174 = *(v173 - 1);
						leaf = 0;
						if (v174)
							leaf = (struct _BSPLeaf*)(v174 + offset);
						v176 = *v173;
						v177 = 0;
						v79 = *v173 == 0;
						*(v173 - 1) = (int)leaf;
						if (!v79)
							v177 = v176 + offset;
						*v173 = v177;
						++node;
						v173 += 11;
					} while ((struct _BSPLeaf*)node < bsp->startLeaves);
				}
				v178 = (unsigned int)bsp->startLeaves;
				if ((struct _BSPLeaf*)v178 < bsp->endLeaves)
				{
					v179 = (int*)(v178 + 8);
					do
					{
						v180 = 0;
						if (*v179)
							v180 = *v179 + offset;
						*v179 = v180;
						v178 += 48;
						v179 += 12;
					} while ((struct _BSPLeaf*)v178 < bsp->endLeaves);
				}
				++v165;
				++bsp;
			} while (v165 < terrain->numBSPTrees);
		}
	}
	//for (j = level->SignalListStart; j < level->SignalListEnd; j = SIGNAL_RelocateSignal(j, offset))
		;
	EVENT_UpdatePuzzlePointers(level->PuzzleInstances, offset);
	STREAM_UpdateLevelPointer(oldLevel, level, v8);
	EVENT_RelocateInstanceList(oldLevel, level, v8);
}

// autogenerated function stub: 
// void /*$ra*/ MEMPACK_RelocateG2AnimKeylistType(struct _G2AnimKeylist_Type **pKeylist /*$a0*/, int offset /*$a1*/, char *start /*$a2*/, char *end /*$a3*/)
void MEMPACK_RelocateG2AnimKeylistType(struct _G2AnimKeylist_Type **pKeylist, int offset, char *start, char *end)
{ // line 1432, offset 0x80050ea0
	UNIMPLEMENTED();
}

void MEMPACK_RelocateObjectType(struct MemHeader* newAddress, long offset, struct Object* oldObject)
{
	struct _Instance* instance; // $a1
	struct Object* object; // $s1
	int i; // $s2
	int j; // $a2
	int d; // $a3
	int sizeOfObject; // $s4
	struct _Model* model; // $t0
	struct _MFace* mface; // $a0
	struct _Segment* segment; // $v0
	struct _HInfo* hInfo; // $v1
	struct AniTexInfo* aniTexInfo; // $a0
	struct MultiSpline* multiSpline; // $v0
	char* p;//$a1

	//s0 = offset
	//s3 = oldObject
	p = NULL;
	object = (struct Object*)(newAddress + 1);

	sizeOfObject = newAddress->memSize - sizeof(struct MemHeader);

	if (object->modelList != NULL)
	{
		p = ((char*)object->modelList + offset);
	}
	//loc_80051540

	//v0 = object->animList
	p = NULL;
	object->modelList = (struct _Model**)p;

	if (object->animList != NULL)
	{
		p = ((char*)object->animList + offset);
	}
	//loc_80051554
	//v0 = object->soundData

	object->animList = (struct _G2AnimKeylist_Type**)p;

	p = NULL;

	if (object->soundData != NULL)
	{
		p = (char*)object->soundData + offset;
	}
	//loc_80051568
#if 0
		loc_80051568 :
		lw      $v0, 0x1C($s1)
		move    $v1, $zero
		beqz    $v0, loc_8005157C
		sw      $a1, 0x28($s1)
		addu    $v1, $v0, $s0

		loc_8005157C :
	lw      $v0, 0x20($s1)
		move    $a1, $zero
		beqz    $v0, loc_80051590
		sw      $v1, 0x1C($s1)
		addu    $a1, $v0, $s0

		loc_80051590 :
	lw      $v0, 0x24($s1)
		move    $a2, $zero
		beqz    $v0, loc_800515A4
		sw      $a1, 0x20($s1)
		addu    $a2, $v0, $s0

		loc_800515A4 :
	lw      $v1, 0x38($s1)
		move    $v0, $zero
		beqz    $v1, loc_800515B8
		sw      $a2, 0x24($s1)
		addu    $v0, $v1, $s0

		loc_800515B8 :
	sw      $v0, 0x38($s1)
		lw      $v0, 8($a0)
		lui     $v1, 0x800
		and $v0, $v1
		beqz    $v0, loc_800515FC
		nop
		lw      $v0, 0x3C($s1)
		nop
		beqz    $v0, loc_800515E4
		move    $a0, $zero
		addu    $a0, $v0, $s0

		loc_800515E4 :
	lw      $v1, 0x40($s1)
		move    $v0, $zero
		beqz    $v1, loc_800515F8
		sw      $a0, 0x3C($s1)
		addu    $v0, $v1, $s0

		loc_800515F8 :
	sw      $v0, 0x40($s1)

		loc_800515FC :
		lh      $v0, 8($s1)
		nop
		blez    $v0, loc_80051948
		move    $s2, $zero
		move    $t2, $s2

		loc_80051610 :
	lw      $v0, 0xC($s1)
		nop
		addu    $a0, $t2, $v0
		lw      $v1, 0($a0)
		nop
		beqz    $v1, loc_80051630
		move    $v0, $zero
		addu    $v0, $v1, $s0

		loc_80051630 :
	sw      $v0, 0($a0)
		lw      $v0, 0xC($s1)
		nop
		addu    $v0, $t2, $v0
		lw      $t0, 0($v0)
		nop
		lw      $v0, 4($t0)
		nop
		beqz    $v0, loc_8005165C
		move    $v1, $zero
		addu    $v1, $v0, $s0

		loc_8005165C :
	lw      $v0, 0xC($t0)
		move    $a0, $zero
		beqz    $v0, loc_80051670
		sw      $v1, 4($t0)
		addu    $a0, $v0, $s0

		loc_80051670 :
	lw      $v0, 0x14($t0)
		move    $v1, $zero
		beqz    $v0, loc_80051684
		sw      $a0, 0xC($t0)
		addu    $v1, $v0, $s0

		loc_80051684 :
	lw      $v0, 0x1C($t0)
		move    $a0, $zero
		beqz    $v0, loc_80051698
		sw      $v1, 0x14($t0)
		addu    $a0, $v0, $s0

		loc_80051698 :
	lw      $v0, 0x20($t0)
		move    $v1, $zero
		beqz    $v0, loc_800516AC
		sw      $a0, 0x1C($t0)
		addu    $v1, $v0, $s0

		loc_800516AC :
	lw      $v0, 0x2C($t0)
		move    $a0, $zero
		beqz    $v0, loc_800516C0
		sw      $v1, 0x20($t0)
		addu    $a0, $v0, $s0

		loc_800516C0 :
	lw      $v0, 0x30($t0)
		move    $v1, $zero
		beqz    $v0, loc_800516D4
		sw      $a0, 0x2C($t0)
		addu    $v1, $v0, $s0

		loc_800516D4 :
	lw      $v0, 0x34($t0)
		move    $a0, $zero
		beqz    $v0, loc_800516E8
		sw      $v1, 0x30($t0)
		addu    $a0, $v0, $s0

		loc_800516E8 :
	lw      $v0, 0x10($t0)
		move    $a3, $zero
		blez    $v0, loc_80051748
		sw      $a0, 0x34($t0)
		move    $a1, $a3

		loc_800516FC :
	lw      $v0, 0x14($t0)
		nop
		addu    $a0, $v0, $a1
		lbu     $v0, 7($a0)
		nop
		andi    $v0, 2
		beqz    $v0, loc_80051734
		nop
		lw      $v1, 8($a0)
		nop
		beqz    $v1, loc_80051730
		move    $v0, $zero
		addu    $v0, $v1, $s0

		loc_80051730 :
	sw      $v0, 8($a0)

		loc_80051734 :
		lw      $v0, 0x10($t0)
		addiu   $a3, 1
		slt     $v0, $a3, $v0
		bnez    $v0, loc_800516FC
		addiu   $a1, 0xC

		loc_80051748 :
		lw      $v0, 0x18($t0)
		nop
		blez    $v0, loc_800517DC
		move    $a3, $zero
		move    $t1, $a3

		loc_8005175C :
	lw      $v0, 0x1C($t0)
		nop
		addu    $v0, $t1
		lw      $v1, 0x14($v0)
		nop
		beqz    $v1, loc_8005177C
		move    $a2, $zero
		addu    $a2, $v1, $s0

		loc_8005177C :
	beqz    $a2, loc_800517C8
		sw      $a2, 0x14($v0)
		move    $v1, $a2
		lw      $v0, 4($v1)
		nop
		beqz    $v0, loc_8005179C
		move    $a0, $zero
		addu    $a0, $v0, $s0

		loc_8005179C :
	lw      $v0, 0xC($v1)
		move    $a1, $zero
		beqz    $v0, loc_800517B0
		sw      $a0, 4($v1)
		addu    $a1, $v0, $s0

		loc_800517B0 :
	lw      $a0, 0x14($v1)
		move    $v0, $zero
		beqz    $a0, loc_800517C4
		sw      $a1, 0xC($v1)
		addu    $v0, $a0, $s0

		loc_800517C4 :
	sw      $v0, 0x14($a2)

		loc_800517C8 :
		lw      $v0, 0x18($t0)
		addiu   $a3, 1
		slt     $v0, $a3, $v0
		bnez    $v0, loc_8005175C
		addiu   $t1, 0x18

		loc_800517DC :
		lw      $v0, 0x20($t0)
		nop
		beqz    $v0, loc_80051834
		nop
		addiu   $a0, $v0, 4
		lw      $v0, 0($v0)
		nop
		blez    $v0, loc_80051834
		move    $a3, $zero

		loc_80051800 :
	lw      $v1, 0($a0)
		nop
		beqz    $v1, loc_80051814
		move    $v0, $zero
		addu    $v0, $v1, $s0

		loc_80051814 :
	sw      $v0, 0($a0)
		lw      $v0, 0x20($t0)
		addiu   $a3, 1
		lw      $v0, 0($v0)
		nop
		slt     $v0, $a3, $v0
		bnez    $v0, loc_80051800
		addiu   $a0, 0xC

		loc_80051834:
	lw      $v0, 0x2C($t0)
		nop
		beqz    $v0, loc_80051934
		nop
		lw      $v1, 0($v0)
		nop
		beqz    $v1, loc_80051858
		move    $a0, $zero
		addu    $a0, $v1, $s0

		loc_80051858 :
	lw      $v1, 4($v0)
		move    $a1, $zero
		beqz    $v1, loc_8005186C
		sw      $a0, 0($v0)
		addu    $a1, $v1, $s0

		loc_8005186C :
	lw      $v1, 8($v0)
		move    $a2, $zero
		beqz    $v1, loc_80051880
		sw      $a1, 4($v0)
		addu    $a2, $v1, $s0

		loc_80051880 :
	lw      $v1, 0xC($v0)
		move    $a0, $zero
		beqz    $v1, loc_80051894
		sw      $a2, 8($v0)
		addu    $a0, $v1, $s0

		loc_80051894 :
	lw      $v1, 0($v0)
		nop
		beqz    $v1, loc_800518BC
		sw      $a0, 0xC($v0)
		lw      $a1, 0($v1)
		nop
		beqz    $a1, loc_800518B8
		move    $a0, $zero
		addu    $a0, $a1, $s0

		loc_800518B8 :
	sw      $a0, 0($v1)

		loc_800518BC :
		lw      $v1, 4($v0)
		nop
		beqz    $v1, loc_800518E4
		nop
		lw      $a1, 0($v1)
		nop
		beqz    $a1, loc_800518E0
		move    $a0, $zero
		addu    $a0, $a1, $s0

		loc_800518E0 :
	sw      $a0, 0($v1)

		loc_800518E4 :
		lw      $v1, 8($v0)
		nop
		beqz    $v1, loc_8005190C
		nop
		lw      $a1, 0($v1)
		nop
		beqz    $a1, loc_80051908
		move    $a0, $zero
		addu    $a0, $a1, $s0

		loc_80051908 :
	sw      $a0, 0($v1)

		loc_8005190C :
		lw      $v0, 0xC($v0)
		nop
		beqz    $v0, loc_80051934
		nop
		lw      $a0, 0($v0)
		nop
		beqz    $a0, loc_80051930
		move    $v1, $zero
		addu    $v1, $a0, $s0

		loc_80051930 :
	sw      $v1, 0($v0)

		loc_80051934 :
		lh      $v0, 8($s1)
		addiu   $s2, 1
		slt     $v0, $s2, $v0
		bnez    $v0, loc_80051610
		addiu   $t2, 4

		loc_80051948 :
		lh      $v0, 0xA($s1)
		nop
		blez    $v0, loc_80051988
		move    $s2, $zero
		move    $a1, $s0

		loc_8005195C :
	move    $a2, $s3
		sll     $a0, $s2, 2
		lw      $v0, 0x10($s1)
		addu    $a3, $s3, $s4
		jal     sub_8005145C
		addu    $a0, $v0, $a0
		lh      $v0, 0xA($s1)
		addiu   $s2, 1
		slt     $v0, $s2, $v0
		bnez    $v0, loc_8005195C
		move    $a1, $s0

		loc_80051988 :
	lw      $v0, 0x10($s1)
		nop
		beqz    $v0, loc_80051A3C
		nop
		lw      $v0, -0x4204($gp)//gameTrackerX.instanceList//gameTrackerX.instanceList
		nop
		lw      $a1, 4($v0)
		nop
		beqz    $a1, loc_80051A3C
		addu    $a3, $s3, $s4

		loc_800519B0 :
	lw      $v0, 0x1C($a1)
		nop
		bne     $v0, $s3, loc_800519D8
		nop
		lw      $v1, 0x1D8($a1)
		nop
		beqz    $v1, loc_800519D4
		move    $v0, $zero
		addu    $v0, $v1, $s0

		loc_800519D4 :
	sw      $v0, 0x1D8($a1)

		loc_800519D8 :
		lbu     $v0, 0x1C8($a1)
		nop
		beqz    $v0, loc_80051A2C
		move    $a2, $zero
		move    $a0, $a1

		loc_800519EC :
	lw      $v1, 0x210($a0)
		nop
		sltu    $v0, $v1, $s3
		bnez    $v0, loc_80051A18
		sltu    $v0, $a3, $v1
		bnez    $v0, loc_80051A18
		nop
		beqz    $v1, loc_80051A14
		move    $v0, $zero
		addu    $v0, $v1, $s0

		loc_80051A14 :
	sw      $v0, 0x210($a0)

		loc_80051A18 :
		lbu     $v0, 0x1C8($a1)
		addiu   $a2, 1
		slt     $v0, $a2, $v0
		bnez    $v0, loc_800519EC
		addiu   $a0, 0x30  # '0'

		loc_80051A2C :
		lw      $a1, 8($a1)
		nop
		bnez    $a1, loc_800519B0
		nop

		loc_80051A3C :
	move    $a0, $s3
		move    $a1, $s1
		jal     sub_8005A67C
		move    $a2, $s4
		lw      $ra, 0x10 + var_s14($sp)
		lw      $s4, 0x10 + var_s10($sp)
		lw      $s3, 0x10 + var_sC($sp)
		lw      $s2, 0x10 + var_s8($sp)
		lw      $s1, 0x10 + var_s4($sp)
		lw      $s0, 0x10 + var_s0($sp)
		jr      $ra
		addiu   $sp, 0x28
#endif

}

void MEMPACK_RelocateCDMemory(struct MemHeader *newAddress, long offset, struct _BigFileDir *oldDir)
{ 
	struct _BigFileDir *newDir;
	newDir = (struct _BigFileDir*)(newAddress + 1);
	LOAD_UpdateBigFilePointers(oldDir, newDir);
}
