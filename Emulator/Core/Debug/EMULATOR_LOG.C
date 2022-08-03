#include "EMULATOR_LOG.H"

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"
#include "Core/Setup/Platform/EMULATOR_PLATFORM_INCLUDES.H"

#if defined(UWP) || defined(_WIN32)

#include <stdio.h>

void Emulator_Log(enum LOG_TYPE lt, const char* file, const char* func, int line, const char* fmt, ...) 
{
#if !defined(UWP) && (defined(_WIN32) || defined(_WIN64))
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, lt);
    printf("[F:%s:%s:L%d] - ", file, func, line, fmt);
    
    va_list arglist;
    va_start(arglist, fmt);
    vprintf(fmt, arglist);
    va_end(arglist);
#else
    char buff[1024];
    sprintf(buff, "[F:%s:%s:L%d] - %s\n", file, func, line, fmt);

    OutputDebugStringA(buff);

#endif
}

#endif