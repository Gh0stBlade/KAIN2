#include "Game/CORE.H"
#include "Game/G2/POOLMMG2.H"
#include "Game/MEMPACK.H"

void G2PoolMem_InitPool(void *voidPool, int blockCount, int blockSize)//Matching - 100%
{
	((struct _G2PoolMemPool_Type*)voidPool)->blockPool = (struct _G2PoolMem_Type*)MEMPACK_Malloc(blockCount * blockSize, 0x19);
	((struct _G2PoolMemPool_Type*)voidPool)->stack = (unsigned short*)MEMPACK_Malloc(blockCount * 4, 0x19);
	((struct _G2PoolMemPool_Type*)voidPool)->blockSize = blockSize;
	((struct _G2PoolMemPool_Type*)voidPool)->stackSize = blockCount;

	G2PoolMem_ResetPool(voidPool);
}

void G2PoolMem_ResetPool(void* voidPool)//Matching - 100%
{ 
	struct _G2PoolMemPool_Type* pool;
	int blockIndex;

	pool = (struct _G2PoolMemPool_Type*)voidPool;

	blockIndex = 0;
	pool->stackTop = 0;

	if (pool->stackSize != 0)
	{
		do
		{
		
			pool->stack[blockIndex++] = blockIndex;
	
		} while (blockIndex < pool->stackSize);
	}
}

void* G2PoolMem_Allocate(void* voidPool)//Matching - 100%
{
	int blockIndex;

	if (((struct _G2PoolMemPool_Type*)voidPool)->stackTop >= ((struct _G2PoolMemPool_Type*)voidPool)->stackSize)
	{
		return NULL;
	}

	blockIndex = ((struct _G2PoolMemPool_Type*)voidPool)->stack[((struct _G2PoolMemPool_Type*)voidPool)->stackTop];
	((struct _G2PoolMemPool_Type*)voidPool)->stackTop++;
	return (char*)(((struct _G2PoolMemPool_Type*)voidPool)->blockPool) + (((struct _G2PoolMemPool_Type*)voidPool)->blockSize * blockIndex);
}

void G2PoolMem_Free(void *voidPool, void *block) // Matching - 100%
{
	int blockIndex;
	
	blockIndex = (((char*)block) - ((char*)((struct _G2PoolMemPool_Type*)voidPool)->blockPool)) / ((struct _G2PoolMemPool_Type*)voidPool)->blockSize;
	
	((struct _G2PoolMemPool_Type*)voidPool)->stack[--((struct _G2PoolMemPool_Type*)voidPool)->stackTop] = blockIndex;
}