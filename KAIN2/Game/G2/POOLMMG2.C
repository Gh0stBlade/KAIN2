#include "CORE.H"
#include "POOLMMG2.H"
#include "MEMPACK.H"

void G2PoolMem_InitPool(void *voidPool, int blockCount, int blockSize)
{
#if defined(PSX_VERSION)

	((struct _G2PoolMemPool_Type*)voidPool)->blockPool = (struct _G2PoolMem_Type*)MEMPACK_Malloc(blockCount * blockSize, 0x19);
	((struct _G2PoolMemPool_Type*)voidPool)->stack = (unsigned short*)MEMPACK_Malloc(blockCount * 4, 0x19);
	((struct _G2PoolMemPool_Type*)voidPool)->blockSize = 0;
	((struct _G2PoolMemPool_Type*)voidPool)->stackSize = blockCount;

	G2PoolMem_ResetPool(voidPool);

#elif defined(PC_VERSION)
	int v3; // eax
	struct _G2PoolMemPool_Type* pool = (struct _G2PoolMemPool_Type*)voidPool; // $a0

	pool->blockPool = (struct _G2PoolMem_Type*)MEMPACK_Malloc(blockCount * blockSize, 0x19u);
	pool->stack = (unsigned __int16*)MEMPACK_Malloc(2 * blockCount, 0x19u);
	v3 = 0;
	pool->blockSize = blockSize;
	pool->stackSize = blockCount;
	pool->stackTop = 0;
	if ((WORD)blockCount)
	{
		do
		{
			pool->stack[v3] = v3;
			++v3;
		} while (v3 < pool->stackSize);
	}
#endif
}

void G2PoolMem_ResetPool(void *voidPool)
{ 
#if defined(PSX_VERSION)

	struct _G2PoolMemPool_Type* pool;
	int blockIndex;

	pool = (struct _G2PoolMemPool_Type*)voidPool;

	blockIndex = 0;
	pool->stackTop = 0;

	if (pool->stackSize != 0)
	{
		do
		{
		
			pool->stack[blockIndex++] = 0;
	
		} while (blockIndex < pool->stackSize);
	}

#elif defined(PC_VERSION)
	struct _G2PoolMemPool_Type *pool = (struct _G2PoolMemPool_Type*)voidPool; // $a0

	int v1; // eax
	bool v2; // zf

	v1 = 0;
	v2 = pool->stackSize == 0;
	pool->stackTop = 0;
	if (!v2)
	{
		do
		{
			pool->stack[v1] = v1;
			++v1;
		} while (v1 < pool->stackSize);
	}
#endif
}

void * G2PoolMem_Allocate(void *voidPool)
{
#if defined(PSX_VERSION)
	int blockIndex;

	if (((struct _G2PoolMemPool_Type*)voidPool)->stackTop < ((struct _G2PoolMemPool_Type*)voidPool)->stackSize)
	{
		blockIndex = ((struct _G2PoolMemPool_Type*)voidPool)->stack[((struct _G2PoolMemPool_Type*)voidPool)->stackTop];
		((struct _G2PoolMemPool_Type*)voidPool)->stackTop++;
		return (char*)(((struct _G2PoolMemPool_Type*)voidPool)->blockPool) + (((struct _G2PoolMemPool_Type*)voidPool)->blockSize * blockIndex);
	}
	
	return NULL;

#elif defined(PC_VERSION)

	struct _G2PoolMemPool_Type* pool = (struct _G2PoolMemPool_Type*)voidPool;
	unsigned __int16 stackTop; // ax
	unsigned __int16 v3; // dx

	stackTop = pool->stackTop;
	if (stackTop >= pool->stackSize)
		return 0;
	v3 = pool->stack[stackTop];
	pool->stackTop = stackTop + 1;
	return (char*)pool->blockPool + v3 * pool->blockSize;
#endif
}

void G2PoolMem_Free(void *voidPool, void *block)
{
#if defined(PSX_VERSION)
	int blockIndex;
	
	blockIndex = (((char*)block) - ((char*)((struct _G2PoolMemPool_Type*)voidPool)->blockPool)) / ((struct _G2PoolMemPool_Type*)voidPool)->blockSize;
	
	((struct _G2PoolMemPool_Type*)voidPool)->stack[--((struct _G2PoolMemPool_Type*)voidPool)->stackTop] = blockIndex;

#elif defined(PC_VERSION)
	struct _G2PoolMemPool_Type* pool = (struct _G2PoolMemPool_Type*)voidPool;
	int v2; // eax

	v2 = (signed int)((int)block - (unsigned int)pool->blockPool) / pool->blockSize;
	pool->stack[--pool->stackTop] = v2;
#endif
}