#include "EMULATOR_LOG.H"

#if defined(UWP) || defined(_WIN32) && !defined(SN_TARGET_PSP2)

#include <Windows.h>
#include <stdio.h>

void Emulator_Log(enum LOG_TYPE lt, const char* file, const char* func, int line, const char* fmt, ...) 
{
#if !defined(UWP) && (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG_PRINTS)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, lt);

    if (lt != LT_INFO)
    {
        printf("[F:%s:%s:L%d] - ", file, func, line);
    }

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