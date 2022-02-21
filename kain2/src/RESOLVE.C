#include "CORE.H"
#include "RESOLVE.H"

void RESOLVE_Pointers(struct RedirectList *redirectList, long *data, long *baseAddr)
{
#if defined (PSX_VERSION)
	long* rdList;
	int i;
	long* handle;

	i = redirectList->numPointers;
	rdList = redirectList->data;

	if (i != 0)
	{
		do
		{
			handle = (long*)((char*)data + *rdList++);
			*handle = (long)(handle[0] + (char*)baseAddr);
		} while (--i != 0);
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
