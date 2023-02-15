#include "CORE.H"
#include "RESOLVE.H"

void RESOLVE_Pointers(struct RedirectList *redirectList, int *data, int *baseAddr)//Matching - 100%
{
#if defined (PSX_VERSION)
	int* rdList;
	int i;
	uintptr_t* handle;

	rdList = redirectList->data;

	for (i = redirectList->numPointers; i != 0; i--, rdList++)
	{
		handle = (uintptr_t*)((char*)data + *rdList);
		*handle += (uintptr_t)((char*)baseAddr);
	}

#elif defined(PC_VERSION)
	__int32 numPointers; // edx
	__int32* i; // ecx
	long* v5; // eax

	numPointers = redirectList->numPointers;
	for (i = redirectList->data; numPointers; *v5 += (long)baseAddr)
	{
		v5 = (int*)((char*)data + *i++);
		--numPointers;
	}
#endif
}
