#ifndef POOLMMG2_H
#define POOLMMG2_H

extern void G2PoolMem_InitPool(void *voidPool, int blockCount, int blockSize); // 0x80095D9C
extern void G2PoolMem_ResetPool(void *voidPool); // 0x80095E08
extern void * G2PoolMem_Allocate(void *voidPool); // 0x80095E44
extern void G2PoolMem_Free(void *voidPool, void *block); // 0x80095EA0

#endif
