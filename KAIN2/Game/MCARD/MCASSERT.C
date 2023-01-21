#include "Game/CORE.H"
#include "MCASSERT.H"


void mcassert(char* exp, char* file, int line)
{
    if (exp != 0)
    {
        printf("%s:%ld: %s\n", file, line, exp);
    }
    else
    {
        printf("%s:%ld: assertion failure\n", file, line);
    }
    
#if defined(PSXPC_VERSION)
    while (1)
    {

    }
#elif defined(PSX_VERSION)
    _break(1u, 7u);
#endif
}




