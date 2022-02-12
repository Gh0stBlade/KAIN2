#ifndef MEMPACK_H
#define MEMPACK_H

#define ONE_MB 1048576
#define TWO_MB ONE_MB * 2
#define BASE_ADDRESS 0x80000000
#define PACK_MAGIC_USHORT(A, B, C, D) A << 12 | B << 8 | C << 4 | D << 0
#define DEFAULT_MEM_MAGIC PACK_MAGIC_USHORT(0xB, 0xA, 0xD, 0xE)

extern void MEMPACK_Init(); // 0x8004FAA4

extern struct MemHeader * MEMPACK_GetSmallestBlockTopBottom(long allocSize); // 0x8004FB0C

extern struct MemHeader * MEMPACK_GetSmallestBlockBottomTop(long allocSize); // 0x8004FB74

extern long MEMPACK_RelocatableType(long memType); // 0x8004FBE0

extern char * MEMPACK_Malloc(unsigned long allocSize, unsigned char memType); // 0x8004FC18

extern char * MEMPACK_MallocFailOk(unsigned long allocSize, unsigned char memType); // 0x8004FC94

extern void MEMORY_MergeAddresses(struct MemHeader *firstAddress, struct MemHeader *secondAddress); // 0x8004FE20

extern void MEMPACK_Return(char *address, long takeBackSize); // 0x8004FE68

extern void MEMPACK_Free(char *address); // 0x8004FEEC

extern void MEMPACK_FreeByType(unsigned char memType); // 0x8004FF9C

extern unsigned long MEMPACK_Size(char *address); // 0x80050034

extern unsigned long MEMPACK_ReportFreeMemory(); // 0x80050040

extern void MEMPACK_ReportMemory2(); // 0x80050050

extern void MEMPACK_ReportMemory(); // 0x80050080

extern void MEMPACK_SetMemoryBeingStreamed(char *address); // 0x8005011C

extern void MEMPACK_SetMemoryDoneStreamed(char *address); // 0x80050128

extern long MEMPACK_MemoryValidFunc(char *address); // 0x80050134

extern char * MEMPACK_GarbageCollectMalloc(unsigned long *allocSize, unsigned char memType, unsigned long *freeSize); // 0x80050168

extern void MEMPACK_GarbageSplitMemoryNow(unsigned long allocSize, struct MemHeader *bestAddress, long memType, unsigned long freeSize); // 0x800502CC

extern void MEMPACK_GarbageCollectFree(struct MemHeader *memAddress); // 0x800502F0

extern void MEMPACK_DoGarbageCollection(); // 0x800503A0

extern void MEMPACK_RelocateAreaType(struct MemHeader *newAddress, long offset, struct Level *oldLevel); // 0x800505BC

extern void MEMPACK_RelocateG2AnimKeylistType(struct _G2AnimKeylist_Type **pKeylist, int offset, char *start, char *end); // 0x80050EA0

extern void MEMPACK_RelocateObjectType(struct MemHeader *newAddress, long offset, struct Object *oldObject); // 0x80050F40

extern void MEMPACK_RelocateCDMemory(struct MemHeader *newAddress, long offset, struct _BigFileDir *oldDir); // 0x800514B0


#endif
