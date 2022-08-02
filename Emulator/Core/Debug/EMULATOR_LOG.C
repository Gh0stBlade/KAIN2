#include "EMULATOR_LOG.H"

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#if defined(UWP)

#include <stdio.h>


void Emulator_Log(const char* file, const char* func, int line, const char* fmt, ...) 
{
    char buff[1024];
    sprintf(buff, "[F:%s:%s:L%d] - %s", file, func, line, fmt);

#if 0
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int k = 1; k < 255; k++)
    {
        SetConsoleTextAttribute(hConsole, k);
        printf("%3d  %s\n", k, "I want to be nice today!");
    }
#endif

    OutputDebugStringA(buff);
}

#endif