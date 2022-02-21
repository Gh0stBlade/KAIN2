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
#if 0
		addu    $v0, $a2
		bnez    $a3, loc_8003CC94
		sw      $v0, 0($v1)

		locret_8003CCB4:
	jr      $ra
		nop
#endif

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
