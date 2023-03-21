#include "CORE.H"
#include "RESOLVE.H"

void RESOLVE_Pointers(struct RedirectList *redirectList, int *data, int *baseAddr)//Matching - 100%
{
	int* rdList;
	int i;
	uintptr_t* handle;

	rdList = redirectList->data;

	for (i = redirectList->numPointers; i != 0; i--, rdList++)
	{
		handle = (uintptr_t*)((char*)data + *rdList);
		*handle += (uintptr_t)((char*)baseAddr);
	}
}
