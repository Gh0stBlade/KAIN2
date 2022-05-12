#ifndef POOLMMG2_H
#define POOLMMG2_H

struct _G2PoolMemPool_Type // hashcode: 0x987744BB (dec: -1737014085)
{
	unsigned short blockSize; // size=0, offset=0
	unsigned short stackTop; // size=0, offset=2
	unsigned short stackSize; // size=0, offset=4
	unsigned short* stack; // size=0, offset=8
	struct _G2PoolMem_Type* blockPool; // size=0, offset=12
};

extern void G2PoolMem_InitPool(void *voidPool, int blockCount, int blockSize); // 0x80095D9C
extern void G2PoolMem_ResetPool(void *voidPool); // 0x80095E08
extern void * G2PoolMem_Allocate(void *voidPool); // 0x80095E44
extern void G2PoolMem_Free(void *voidPool, void *block); // 0x80095EA0

#endif
