#include "CORE.H"
#include "MEMPACK.H"
#include "PSX/DRAWS.H"
#include "STREAM.H"
#include "DEBUG.H"
#include "PSX/AADLIB.H"
#include "LOAD3D.H"

#include <stddef.h>

#ifdef PC_VERSION
#pragma warning(disable: 4101)
#endif

extern void GXFilePrint(const char* fmt, ...);

struct NewMemTracker newMemTracker;
unsigned long mem_used, mem_total;

#if defined(PSXPC_VERSION) || defined(PC_VERSION)
char memBuffer[/*0x11F18C */ (ONE_MB * 8)];
void* overlayAddress = memBuffer; // 0x800CE194
#else
void* overlayAddress; // For PSX this is quite clearly set by the linker script maybe.
#endif

void MEMPACK_Init()
{
#if defined(PSX_VERSION)
#if defined(PSXPC_VERSION)
	newMemTracker.totalMemory = sizeof(memBuffer);
	memset(&memBuffer[0], 0, sizeof(memBuffer));
	newMemTracker.rootNode = (struct MemHeader*)&memBuffer[0];
#else
	newMemTracker.totalMemory = ((TWO_MB - (ONE_MB / 256)) + BASE_ADDRESS) - ((long*)overlayAddress)[0];
	newMemTracker.rootNode = (struct MemHeader*)((long*)overlayAddress)[0];
#endif
#endif
	
	newMemTracker.rootNode->magicNumber = DEFAULT_MEM_MAGIC;
	newMemTracker.rootNode->memStatus = 0;
	newMemTracker.rootNode->memType = 0;
	newMemTracker.rootNode->memSize = newMemTracker.totalMemory;
	newMemTracker.currentMemoryUsed = 0;
	newMemTracker.doingGarbageCollection = 0;
	newMemTracker.lastMemoryAddress = (char*)newMemTracker.rootNode + newMemTracker.totalMemory;
}

struct MemHeader * MEMPACK_GetSmallestBlockTopBottom(long allocSize)
{ 
	struct MemHeader* address;
	struct MemHeader* bestAddress;
	
	address = newMemTracker.rootNode;
	
	bestAddress = NULL;
	
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

struct MemHeader * MEMPACK_GetSmallestBlockBottomTop(long allocSize)
{ 
	struct MemHeader* address;
	struct MemHeader* bestAddress;

	address = newMemTracker.rootNode;

	bestAddress = NULL;

	while ((char*)address != (char*)newMemTracker.lastMemoryAddress)
	{
		if (address->memStatus == 0 && address->memSize >= allocSize && (bestAddress == NULL || (char*)bestAddress < (char*)address))
		{
			bestAddress = address;
			break;
		}

		address = (struct MemHeader*)((char*)address + address->memSize);
	}
	
	return bestAddress;
}

long MEMPACK_RelocatableType(long memType)
{
	if ((unsigned int)(memType - 1) < 2 || memType == 0x2C || memType == 0x2F || memType == 0x4)
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

	while (1)
	{
		if (newMemTracker.doingGarbageCollection == 0)
		{
			if (relocatableMemory != 0)
			{
				MEMPACK_DoGarbageCollection();
#
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
					DEBUG_FatalError("Trying to fit memory size %d Type = %d\nAvailable memory : used = % d, free = % d", allocSize, memType, newMemTracker.currentMemoryUsed, newMemTracker.totalMemory - newMemTracker.currentMemoryUsed);
				}

				return NULL;
			}
		}
		else
		{
			break;
		}
	}

	if ((unsigned int)bestAddress->memSize - allocSize < 8)
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
			
			bestAddress->magicNumber = 1;
			bestAddress->memStatus = 1;
			bestAddress->memType = memType;
			bestAddress->memSize = allocSize;

			newMemTracker.currentMemoryUsed += allocSize;
		}
		else
		{
			address = (struct MemHeader*)((char*)bestAddress + (bestAddress->memSize - allocSize));

			address->magicNumber = DEFAULT_MEM_MAGIC;
			address->memStatus = 1;
			address->memType = memType;
			address->memSize = allocSize;

			newMemTracker.currentMemoryUsed += allocSize;

			bestAddress->magicNumber = DEFAULT_MEM_MAGIC;
			bestAddress->memStatus = 0;
			bestAddress->memType = 0;
			bestAddress->memSize = (bestAddress->memSize - allocSize);

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
					UNIMPLEMENTED();
	return null;
}

void MEMORY_MergeAddresses(struct MemHeader *firstAddress, struct MemHeader *secondAddress)
{
	if (firstAddress->memStatus == 0 && secondAddress->memStatus == 0)
	{
		firstAddress->memSize += secondAddress->memSize;
		secondAddress->magicNumber = 0;
		secondAddress->memStatus = 1;
	}
}

void MEMPACK_Return(char *address, long takeBackSize)
{
	struct MemHeader* memAddress;
	struct MemHeader* nextAddress;
	
	takeBackSize = (takeBackSize >> 2) << 2;

	if (takeBackSize >= sizeof(struct MemHeader))
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

void MEMPACK_Free(char *address)
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
#if defined(PC_VERSION)
	return *((DWORD*)address - 1) - 8;
#else
	UNIMPLEMENTED();
	return 0;
#endif
}


// autogenerated function stub: 
// unsigned long /*$ra*/ MEMPACK_ReportFreeMemory()
unsigned long MEMPACK_ReportFreeMemory()
{ // line 621, offset 0x80050040
#if defined(PC_VERSION)
	return mem_total - mem_used;
#else
	UNIMPLEMENTED();
	return 0;
#endif
}

int dword_C550A8, dword_C550B4;

void MEMPACK_ReportMemory2()
{
#if defined(PSX_VERSION)
	struct MemHeader* address;

	address = newMemTracker.rootNode;

	while ((char*)address != newMemTracker.lastMemoryAddress)
	{
		address = (struct MemHeader*)((char*)address + address->memSize);
	}

#elif defined(PC_VERSION)
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
#if defined(PC_VERSION)
	return address != (char*)0xFAFBFCFD && address && *(address - 6) == 1;
#else
	UNIMPLEMENTED();
	return 0;
#endif
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

	if ((unsigned int)bestAddress->memSize - allocSize[0] < 8)
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

void MEMPACK_GarbageSplitMemoryNow(unsigned long allocSize, struct MemHeader *bestAddress, long memType, unsigned long freeSize)
{
	struct MemHeader* address = (struct MemHeader*)((char*)bestAddress + allocSize);

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

	secondAddress = (struct MemHeader*)((char*)memAddress + memAddress->memSize);

	if ((char*)secondAddress != (char*)newMemTracker.lastMemoryAddress)
	{
		MEMORY_MergeAddresses(memAddress, secondAddress);
	}

	secondAddress = memAddress;

	if ((char*)newMemTracker.rootNode != (char*)newMemTracker.lastMemoryAddress)
	{
		do
		{
			if (((char*)memAddress + memAddress->memSize) == (char*)secondAddress)
			{
				MEMORY_MergeAddresses(memAddress, (struct MemHeader*)((char*)memAddress + memAddress->memSize));
				break;
			}

			memAddress = (struct MemHeader*)((char*)memAddress + memAddress->memSize);
		
		} while ((char*)memAddress != (char*)newMemTracker.lastMemoryAddress);
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
					UNIMPLEMENTED();
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
#else
	UNIMPLEMENTED();
#endif
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
