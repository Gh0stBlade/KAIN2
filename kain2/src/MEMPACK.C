#include "CORE.H"
#include "MEMPACK.H"
#include "PSX/DRAWS.H"
#include "STREAM.H"
#include "DEBUG.H"
#include "PSX/AADLIB.H"
#include "LOAD3D.H"

#include <stddef.h>

static struct NewMemTracker newMemTracker;
unsigned long mem_used, mem_total;

#if defined(PSXPC_VERSION)
char memBuffer[ONE_MB];
void* overlayAddress = &memBuffer[0]; // 0x800CE194
#else
void* overlayAddress; // For PSX this is quite clearly set by the linker script maybe.
#endif

void MEMPACK_Init()
{
#if defined(PSXPC_VERSION)
	newMemTracker.totalMemory = sizeof(memBuffer);
	memset(overlayAddress, 0, ONE_MB);
#else
	newMemTracker.totalMemory = (BASE_ADDRESS + TWO_MB - (ONE_MB / 256)) - (unsigned int)overlayAddress;
#endif
	newMemTracker.rootNode = (struct MemHeader*)overlayAddress;
	newMemTracker.rootNode->magicNumber = DEFAULT_MEM_MAGIC;
	newMemTracker.rootNode->memStatus = 0;
	newMemTracker.rootNode->memType = 0;
	newMemTracker.rootNode->memSize = newMemTracker.totalMemory;
	newMemTracker.currentMemoryUsed = 0;
	newMemTracker.doingGarbageCollection = 0;
	newMemTracker.lastMemoryAddress = (char*)((DWORD)newMemTracker.rootNode + (DWORD)newMemTracker.totalMemory);
}

struct MemHeader * MEMPACK_GetSmallestBlockTopBottom(long allocSize)
{ 
	struct MemHeader *address;
	struct MemHeader *bestAddress;
	
	address = newMemTracker.rootNode;

	bestAddress = NULL;
	while ((char*)address != newMemTracker.lastMemoryAddress)
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

struct MemHeader * MEMPACK_GetSmallestBlockBottomTop(long allocSize)
{ 
	struct MemHeader* address;
	struct MemHeader* bestAddress;

	address = newMemTracker.rootNode;
	bestAddress = NULL;

	while ((char*)address != newMemTracker.lastMemoryAddress)
	{
		if (address->memStatus == 0)
		{
			if (address->memSize >= allocSize)
			{
				if (bestAddress == NULL || bestAddress < address)
				{
					bestAddress = address;
				}
			}
		}

		address = (struct MemHeader*)((char*)address + address->memSize);
	}
	
	return bestAddress;
}

long MEMPACK_RelocatableType(long memType)
{
	if (memType - 1 < 2 || memType == 0x2C || memType == 0x2F || memType == 0x4)
	{
		return 1;
	}

	return 0;
}

char * MEMPACK_Malloc(unsigned long allocSize, unsigned char memType)
{
#ifdef PSX_VERSION
	struct MemHeader* bestAddress;
	long relocatableMemory;
	int curMem;
	struct MemHeader* address;
	long topOffset;

	relocatableMemory = MEMPACK_RelocatableType(memType);
	
	allocSize = ((allocSize + 11) >> 2) << 2;

	if (newMemTracker.doingGarbageCollection == 0)
	{
		if (relocatableMemory != 0)
		{
			MEMPACK_DoGarbageCollection();
		}

		if (relocatableMemory != 0)
		{
			bestAddress = MEMPACK_GetSmallestBlockTopBottom(allocSize);
		}
		else
		{
			bestAddress = MEMPACK_GetSmallestBlockBottomTop(allocSize);
		}
	}
	else
	{
		if (relocatableMemory != 0)
		{
			bestAddress = MEMPACK_GetSmallestBlockTopBottom(allocSize);
		}
		else
		{
			bestAddress = MEMPACK_GetSmallestBlockBottomTop(allocSize);
		}
	}

	if (bestAddress == NULL)
	{
		curMem = newMemTracker.currentMemoryUsed;
		STREAM_TryAndDumpANonResidentObject();
	
		if (curMem == newMemTracker.currentMemoryUsed)
		{
			if (memType != 16)
			{
				MEMPACK_ReportMemory2();
				DEBUG_FatalError("Trying to fit memory size %d Type = %d\nAvalible memory : used = % d, free = % d\n", allocSize, memType, newMemTracker.currentMemoryUsed, newMemTracker.totalMemory - newMemTracker.currentMemoryUsed);
			}
		}
	}
	
	topOffset = bestAddress->memSize;
	if (topOffset - allocSize < 8)
	{
		allocSize = topOffset;
	}

	if (allocSize != topOffset)
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

			bestAddress = address;

			newMemTracker.currentMemoryUsed += allocSize;
		}
		else
		{
			topOffset -= allocSize;
			
			address = (struct MemHeader*)((char*)bestAddress + topOffset);
			address->magicNumber = DEFAULT_MEM_MAGIC;
			address->memStatus = 1;
			address->memType = memType;
			address->memSize = allocSize;

			newMemTracker.currentMemoryUsed += allocSize;

			bestAddress->magicNumber = DEFAULT_MEM_MAGIC;
			bestAddress->memStatus = 0;
			bestAddress->memType = 0;
			bestAddress->memSize = topOffset;

			bestAddress = address;
		}
	}
	return (char*)(bestAddress + 1);

#else
	char* result; // eax

	result = (char*)MEMPACK_MallocFailOk(allocSize, memType);
	if (!result)
	{
		GXFilePrint("---------------------\n");
		MEMPACK_ReportMemory2();
		DEBUG_FatalError(
			"Trying to fit memory size %d Type = %d\nAvailable memory : used = %d, free = %d\n",
			allocSize,
			memType,
			mem_used,
			mem_total - mem_used);
	}
	return result;
#endif
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

	return null;
}

void MEMORY_MergeAddresses(struct MemHeader *firstAddress, struct MemHeader *secondAddress)
{
	if (firstAddress->memStatus == 0 && secondAddress->memType == 0)
	{
		firstAddress->memSize += secondAddress->memSize;
		secondAddress->magicNumber = 0;
		secondAddress->memStatus = 1;
	}
}

void MEMPACK_Return(char *address, long takeBackSize)
{
	struct MemHeader *memAddress;
	struct MemHeader *nextAddress;
	
	memAddress = (struct MemHeader*)address;

	if (((takeBackSize >> 2) << 2) >= 8)
	{
		memAddress--;
		memAddress->memSize -= ((takeBackSize >> 2) << 2);
		newMemTracker.currentMemoryUsed -= ((takeBackSize >> 2) << 2);

		memAddress = (struct MemHeader*)(char*)memAddress + memAddress->memSize;
		memAddress->magicNumber = DEFAULT_MEM_MAGIC;
		memAddress->memStatus = 0;
		memAddress->memType = 0;
		memAddress->memSize = ((takeBackSize >> 2) << 2);
		nextAddress = (struct MemHeader*)(char*)memAddress + ((takeBackSize >> 2) << 2);

		if ((char*)nextAddress != newMemTracker.lastMemoryAddress)
		{
			MEMORY_MergeAddresses(memAddress, nextAddress);
		}
	}
}

void MEMPACK_Free(char *address)
{ 
	struct MemHeader *memAddress;
	struct MemHeader *secondAddress;
	
	memAddress = (struct MemHeader*)&address[-8];
	memAddress->memStatus = 0;
	memAddress->memType = 0;
	newMemTracker.currentMemoryUsed -= memAddress->memSize;

	secondAddress = (struct MemHeader*)(char*)memAddress + memAddress->memSize;

	if ((char*)secondAddress != newMemTracker.lastMemoryAddress)
	{
		MEMORY_MergeAddresses(memAddress, secondAddress);
	}

	secondAddress = memAddress;
	memAddress = newMemTracker.rootNode;

	if ((char*)memAddress != newMemTracker.lastMemoryAddress)
	{
		do
		{
			if ((char*)memAddress + memAddress->memSize == (char*)secondAddress)
			{
				MEMORY_MergeAddresses(memAddress, (struct MemHeader*)(char*)memAddress + memAddress->memSize);
				break;
			}
			else
			{
				memAddress = (struct MemHeader*)(char*)memAddress + memAddress->memSize;
			}

		} while ((char*)memAddress + memAddress->memSize != newMemTracker.lastMemoryAddress);
	}
}

void MEMPACK_FreeByType(unsigned char memType)
{ 
	struct MemHeader *address;
	int freed;
	
	do
	{
		freed = 0;
		address = newMemTracker.rootNode;
		if ((char*)address != newMemTracker.lastMemoryAddress)
		{
			do
			{
				if (address->memStatus == 1 && address->memType == memType)
				{
					address++;
					freed = 1;
					MEMPACK_Free((char*)address);
					break;
				}

				address = (struct MemHeader*)(char*)address + address->memSize;

			} while ((char*)address != newMemTracker.lastMemoryAddress);
		}
	} while (freed == 1);
}


// autogenerated function stub: 
// unsigned long /*$ra*/ MEMPACK_Size(char *address /*$a0*/)
unsigned long MEMPACK_Size(char *address)
{ // line 611, offset 0x80050034
	return *((DWORD*)address - 1) - 8;
}


// autogenerated function stub: 
// unsigned long /*$ra*/ MEMPACK_ReportFreeMemory()
unsigned long MEMPACK_ReportFreeMemory()
{ // line 621, offset 0x80050040
	return mem_total - mem_used;
}

int dword_C550A8, dword_C550B4;

// autogenerated function stub: 
// void /*$ra*/ MEMPACK_ReportMemory2()
void MEMPACK_ReportMemory2()
{ // line 689, offset 0x80050050

#if defined(PC_VERSION)
	int i;
	GXFilePrint("----- Memory Map -----\n");
	for (i = dword_C550A8; i != dword_C550B4; i += *(DWORD*)(i + 4))
	{
		if (*(BYTE*)(i + 2))
		{
			if (*(BYTE*)(i + 3) == 1)
			{
				GXFilePrint("CLOSED) addr %x size=%d type=OBJECT : %s\n", i, *(DWORD*)(i + 4), *(DWORD*)(i + 44));
			}
			else if (*(BYTE*)(i + 3) == 2)
			{
				GXFilePrint("CLOSED) addr %x size=%d type=AREA : %s\n", i, *(DWORD*)(i + 4), *(DWORD*)(i + 160));
			}
			else
			{
				GXFilePrint("CLOSED) addr %x size=%d type=%d\n", i, *(DWORD*)(i + 4), *(unsigned __int8*)(i + 3));
			}
		}
		else
		{
			GXFilePrint("OPEN) addr %x size=%d\n", i, *(DWORD*)(i + 4));
		}
	}
	GXFilePrint("Total Memory Used = %d, Total Memory Free = %d\n", mem_used, mem_total - mem_used);
#endif
}

void MEMPACK_ReportMemory()
{
	struct MemHeader* address;
	long i;
	long firstTime;

	address = newMemTracker.rootNode;

	while ((char*)address != newMemTracker.lastMemoryAddress)
	{
		address = (struct MemHeader*)(char*)address + address->memSize;
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

			address = (struct MemHeader*)(char*)address + address->memSize;
		}
	}
}

void MEMPACK_SetMemoryBeingStreamed(char *address)
{
	address[-6] = 2;
}

void MEMPACK_SetMemoryDoneStreamed(char *address)
{ 
	address[-6] = 1;
}


// autogenerated function stub: 
// long /*$ra*/ MEMPACK_MemoryValidFunc(char *address /*$a0*/)
long MEMPACK_MemoryValidFunc(char *address)
{ // line 826, offset 0x80050134
	return address != (char*)0xFAFBFCFD && address && *(address - 6) == 1;
}


char * MEMPACK_GarbageCollectMalloc(unsigned long *allocSize, unsigned char memType, unsigned long *freeSize)
{
	struct MemHeader* bestAddress;

	allocSize[0] = ((allocSize[0] + 11) >> 2) << 2;
	bestAddress = MEMPACK_GetSmallestBlockTopBottom(allocSize[0]);
	
	if (bestAddress == NULL)
	{
		STREAM_DumpNonResidentObjects();
		bestAddress = MEMPACK_GetSmallestBlockTopBottom(allocSize[0]);

		if (bestAddress == NULL)
		{
			if (memType == 16)
			{
				return NULL;
			}

			MEMPACK_ReportMemory();
			DEBUG_FatalError("Trying to fit memory size %d Type = %d\nAvailable memory : used = %d, free = %d", allocSize[0], memType, newMemTracker.currentMemoryUsed, newMemTracker.totalMemory - newMemTracker.currentMemoryUsed);
		}
	}

	if (allocSize[0] - bestAddress->memSize < 8)
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

	return (char*)bestAddress + 1;
}

void MEMPACK_GarbageSplitMemoryNow(unsigned long allocSize, struct MemHeader *bestAddress, long memType, unsigned long freeSize)
{
	struct MemHeader* address = (struct MemHeader*)(char*)bestAddress + allocSize;

	if (freeSize != 0)
	{
		address->magicNumber = DEFAULT_MEM_MAGIC;
		address->memStatus = 0;
		address->memType = 0;
		address->memSize = freeSize;
	}
}

void MEMPACK_GarbageCollectFree(struct MemHeader *memAddress)
{
	struct MemHeader* secondAddress;

	memAddress->memStatus = 0;
	memAddress->memType = 0;

	newMemTracker.currentMemoryUsed -= memAddress->memSize;
	secondAddress = (struct MemHeader*)(char*)memAddress + memAddress->memSize;

	if ((char*)secondAddress != newMemTracker.lastMemoryAddress)
	{
		MEMORY_MergeAddresses(memAddress, secondAddress);
	}

	if ((char*)newMemTracker.rootNode != newMemTracker.lastMemoryAddress)
	{
		do
		{
			if ((char*)newMemTracker.rootNode + newMemTracker.rootNode->memSize == (char*)memAddress)
			{
				MEMORY_MergeAddresses(newMemTracker.rootNode, (struct MemHeader*)(char*)newMemTracker.rootNode + newMemTracker.rootNode->memSize);
			}

		} while ((unsigned long)&newMemTracker != (unsigned long)newMemTracker.rootNode + newMemTracker.rootNode->memSize);
	}
}

void MEMPACK_DoGarbageCollection()
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
		relocateAddress = newMemTracker.rootNode;
		foundOpening = 0;

		while ((char*)relocateAddress != newMemTracker.lastMemoryAddress)
		{
			if (relocateAddress->memStatus != 0)
			{
				if (MEMPACK_RelocatableType(relocateAddress->memType) != 0 && foundOpening == 1 &&
					relocateAddress->memStatus != 2)
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
			addressSize = relocateAddress->memSize - 8;
			MEMPACK_GarbageCollectFree(relocateAddress);
			holdSize = addressSize;
			newAddress = MEMPACK_GarbageCollectMalloc((unsigned long*)&holdSize, addressMemType, (unsigned long*)&freeSize);
			relocateAddress++;

			if (newAddress != NULL)
			{
				if (addressMemType == 2)
				{
					RemoveIntroducedLights((struct Level*)relocateAddress);

				}
				else if (addressMemType == 4)
				{
					aadRelocateMusicMemoryBegin();
				}

				memcpy(newAddress, (char*)relocateAddress, addressSize);

				if (addressMemType == 4)
				{
					MEMPACK_RelocateAreaType(relocateAddress - 1, newAddress - (char*)relocateAddress, (struct Level*)relocateAddress);
				}
				else if (addressMemType == 1)
				{
					MEMPACK_RelocateObjectType((struct MemHeader*)newAddress - 8, newAddress - (char*)relocateAddress, (struct Object*)relocateAddress);
				}
				else if (addressMemType == 14)
				{
					STREAM_UpdateInstanceCollisionInfo((struct _HModel*)relocateAddress, (struct _HModel*)newAddress);
				}
				else if (addressMemType == 44)
				{
					MEMPACK_RelocateCDMemory((struct MemHeader*)newAddress - 8, newAddress - (char*)relocateAddress, (struct _BigFileDir*)relocateAddress);
				}
				else if (addressMemType == 4)
				{
					aadRelocateMusicMemoryEnd((struct MemHeader*)relocateAddress, newAddress - (char*)relocateAddress, NULL);
				}
				else if (addressMemType == 47)
				{
					aadRelocateSfxMemory(relocateAddress, newAddress - (char*)relocateAddress);
				}

				MEMPACK_GarbageSplitMemoryNow(holdSize, (struct MemHeader*)newAddress - 8, addressMemType, freeSize);
			}
		}
		else
		{
			done = 1;
		}
	}

	newMemTracker.doingGarbageCollection = 0;
}


// autogenerated function stub: 
// void /*$ra*/ MEMPACK_RelocateAreaType(struct MemHeader *newAddress /*$a0*/, long offset /*$s1*/, struct Level *oldLevel /*$s3*/)
void MEMPACK_RelocateAreaType(struct MemHeader *newAddress, long offset, struct Level *oldLevel)
{ // line 1163, offset 0x800505bc
	/* begin block 1 */
		// Start line: 1164
		// Start offset: 0x800505BC
		// Variables:
			struct Level *level; // $s0
			struct _MultiSignal *msignal; // $a0
			long sizeOfLevel; // $s2
			long i; // $t0
			long d; // $a3

		/* begin block 1.1 */
			// Start line: 1229
			// Start offset: 0x800508DC
			// Variables:
				struct _Terrain *terrain; // $t1

			/* begin block 1.1.1 */
				// Start line: 1257
				// Start offset: 0x800509F8
				// Variables:
					struct Intro *intro; // $v0

				/* begin block 1.1.1.1 */
					// Start line: 1267
					// Start offset: 0x80050A34
					// Variables:
						struct MultiSpline *multiSpline; // $a2
				/* end block 1.1.1.1 */
				// End offset: 0x80050B28
				// End Line: 1290
			/* end block 1.1.1 */
			// End offset: 0x80050B3C
			// End Line: 1292

			/* begin block 1.1.2 */
				// Start line: 1310
				// Start offset: 0x80050B64
				// Variables:
					struct DrMoveAniTexDestInfo **dest; // $v0
			/* end block 1.1.2 */
			// End offset: 0x80050BB8
			// End Line: 1320

			/* begin block 1.1.3 */
				// Start line: 1363
				// Start offset: 0x80050C1C
				// Variables:
					struct _VMObject *vmo; // $a1
			/* end block 1.1.3 */
			// End offset: 0x80050CF4
			// End Line: 1374

			/* begin block 1.1.4 */
				// Start line: 1361
				// Start offset: 0x80050D0C
				// Variables:
					struct BSPTree *bsp; // $t2
					struct _BSPNode *node; // $a2
					struct _BSPLeaf *leaf; // $a1
			/* end block 1.1.4 */
			// End offset: 0x80050E24
			// End Line: 1401
		/* end block 1.1 */
		// End offset: 0x80050E24
		// End Line: 1403
	/* end block 1 */
	// End offset: 0x80050E58
	// End Line: 1421

	/* begin block 2 */
		// Start line: 2348
	/* end block 2 */
	// End Line: 2349

}


// autogenerated function stub: 
// void /*$ra*/ MEMPACK_RelocateG2AnimKeylistType(struct _G2AnimKeylist_Type **pKeylist /*$a0*/, int offset /*$a1*/, char *start /*$a2*/, char *end /*$a3*/)
void MEMPACK_RelocateG2AnimKeylistType(struct _G2AnimKeylist_Type **pKeylist, int offset, char *start, char *end)
{ // line 1432, offset 0x80050ea0
#if defined(PC_VERSION)
	struct _G2AnimKeylist_Type* v4; // eax
	struct _G2AnimKeylist_Type* v5; // esi
	struct _G2AnimFxHeader_Type* fxList; // eax
	int v7; // edx
	unsigned __int16** sectionData; // eax
	unsigned __int16* v9; // ecx

	v4 = *pKeylist;
	if (*pKeylist >= (struct _G2AnimKeylist_Type*)start && v4 < (struct _G2AnimKeylist_Type*)end)
	{
		v5 = v4 ? (struct _G2AnimKeylist_Type*)((char*)v4 + offset) : 0;
		*pKeylist = v5;
		if (*(DWORD*)&v5->sectionCount != 0xFACE0FF)
		{
			fxList = v5->fxList;
			if (fxList)
				fxList = (struct _G2AnimFxHeader_Type*)((char*)fxList + offset);
			v5->fxList = fxList;
			v7 = 0;
			if (v5->sectionCount)
			{
				sectionData = v5->sectionData;
				do
				{
					v9 = *sectionData;
					if (*sectionData)
						v9 = (unsigned __int16*)((char*)v9 + offset);
					*sectionData = v9;
					++v7;
					++sectionData;
				} while (v7 < v5->sectionCount);
			}
		}
	}
#endif
}


// autogenerated function stub: 
// void /*$ra*/ MEMPACK_RelocateObjectType(struct MemHeader *newAddress /*$a0*/, long offset /*$s0*/, struct Object *oldObject /*$s3*/)
void MEMPACK_RelocateObjectType(struct MemHeader *newAddress, long offset, struct Object *oldObject)
{ // line 1455, offset 0x80050f40
	/* begin block 1 */
		// Start line: 1456
		// Start offset: 0x80050F40
		// Variables:
			struct _Instance *instance; // $a1
			struct Object *object; // $s1
			int i; // $s2
			int j; // $a2
			int d; // $a3
			int sizeOfObject; // $s4
			struct _Model *model; // $t0

		/* begin block 1.1 */
			// Start line: 1502
			// Start offset: 0x80051140
			// Variables:
				struct _MFace *mface; // $a0
		/* end block 1.1 */
		// End offset: 0x80051178
		// End Line: 1507

		/* begin block 1.2 */
			// Start line: 1511
			// Start offset: 0x800511A0
			// Variables:
				struct _Segment *segment; // $v0

			/* begin block 1.2.1 */
				// Start line: 1515
				// Start offset: 0x800511C8
				// Variables:
					struct _HInfo *hInfo; // $v1
			/* end block 1.2.1 */
			// End offset: 0x8005120C
			// End Line: 1519
		/* end block 1.2 */
		// End offset: 0x8005120C
		// End Line: 1520

		/* begin block 1.3 */
			// Start line: 1522
			// Start offset: 0x80051230
			// Variables:
				struct AniTexInfo *aniTexInfo; // $a0
		/* end block 1.3 */
		// End offset: 0x80051278
		// End Line: 1531

		/* begin block 1.4 */
			// Start line: 1535
			// Start offset: 0x80051288
			// Variables:
				struct MultiSpline *multiSpline; // $v0
		/* end block 1.4 */
		// End offset: 0x80051378
		// End Line: 1557
	/* end block 1 */
	// End offset: 0x80051480
	// End Line: 1607

	/* begin block 2 */
		// Start line: 3084
	/* end block 2 */
	// End Line: 3085

}

void MEMPACK_RelocateCDMemory(struct MemHeader *newAddress, long offset, struct _BigFileDir *oldDir)
{ 
	struct _BigFileDir *newDir;
	newDir = (struct _BigFileDir*)(newAddress + 1);
	LOAD_UpdateBigFilePointers(oldDir, newDir);
}
