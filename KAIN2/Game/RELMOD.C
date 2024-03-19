#include "CORE.H"
#include "RELMOD.H"

#if defined(PSXPC_VERSION)
void RELMOD_InitModulePointers(uintptr_t baseaddr, int* relocs)
#else
void RELMOD_InitModulePointers(int baseaddr, int* relocs) // Matching - 100%
#endif
{ 
    unsigned int* rel_addr;
    int temp;  // not from SYMDUMP

    while (*relocs != -1)
    {
        rel_addr = (unsigned int*)(baseaddr + (*relocs & ~0x3));

        switch (*relocs++ & 0x3)
        {
        case 0:
            temp = *rel_addr;

            if (temp >= 0)
            {
                *rel_addr += baseaddr;
            }

            break;
        case 1:
            *(short*)rel_addr = ((baseaddr + *relocs++) + 32768) >> 16;
            break;
        case 2:
            *(short*)rel_addr += baseaddr;
            break;
        case 3:
            *rel_addr += ((unsigned int)baseaddr << 4) >> 6;
            break;
        }
    }
}


void RELMOD_RelocModulePointers(int baseaddr, int offset, int* relocs) // Matching - 95.78%
{
	int oldbaseaddr;
	int* rel_addr;

	while (*relocs != -1)
	{
		oldbaseaddr = baseaddr - offset;

		rel_addr = (int*)(baseaddr + (*relocs & ~0x3));

		switch (*relocs++ & 0x3)
		{
		case 0:
			*rel_addr += offset;
			break;
		case 1:
			*(short*)rel_addr = (((baseaddr + *relocs++) + 32768) >> 16);
			break;
		case 2:
			*(short*)rel_addr += offset;
			break;
		case 3:
			*rel_addr = (*rel_addr - (((unsigned int)oldbaseaddr << 4) >> 6)) + ((unsigned int)(baseaddr << 4) >> 6);
			break;
		}
	}
}
