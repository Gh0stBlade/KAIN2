#include "CORE.H"
#include "RESOLVE.H"

void RESOLVE_Pointers(struct RedirectList* redirectList, long* data, long* baseAddr)//Matching - 100%
{
	long* rdList;
	int i;
	long* handle;

	rdList = redirectList->data;

	for (i = redirectList->numPointers; i != 0; i--, rdList++)
	{
		handle = (long*)((char*)data + *rdList);
		*handle += (long)((char*)baseAddr);
	}
}
