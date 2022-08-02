#ifndef EMULATOR_LOG_H
#define EMULATOR_LOG_H

#ifdef _MSC_VER
#define FUNCNAME __FUNCTION__
#else
#define FUNCNAME __func__
#endif

#if defined(__ANDROID__)
#include <android/log.h>
#define LOG_TAG_EMU "[EMU] "

#if defined(_DEBUG)
#define eprintf(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG_EMU, fmt, ##__VA_ARGS__)
#define eprinterr(fmt, ...) eprintf("[%s] - " fmt, FUNCNAME, ##__VA_ARGS__);
#else
#define eprintf(fmt, ...) (0)
#define eprinterr(fmt, ...) (0)
#endif

#else
#if defined(DEBUG_PRINTS)
#if defined(UWP)
	extern void Emulator_Log(const char* file, const char* func, int line, const char* fmt, ...);
	#define eprintf(fmt, ...) Emulator_Log(__FILE__, FUNCNAME, __LINE__, fmt, __VA_ARGS__)
	#define eprinterr(fmt, ...) eprintf(fmt, ##__VA_ARGS__);
#else
	#define eprintf(fmt, ...) printf("[EMU] " fmt, ##__VA_ARGS__)
	#define eprinterr(fmt, ...) eprintf("[%s] - " fmt, FUNCNAME, ##__VA_ARGS__);
#endif

#else
#define eprintf(...) 
#define eprinterr(...)
#endif
#endif

#if defined(__EMSCRIPTEN__) || !defined(DEBUG_PRINTS)
#define UNIMPLEMENTED()
#else
#if defined(_WIN32) && defined(_DEBUG)

#include "Core/Debug/BOUNTY_LIST.H"

#define UNIMPLEMENTED() eprinterr("Unimplemented!\n"); \
		Emulator_AddBounty(FUNCNAME);
#else
#define UNIMPLEMENTED() eprinterr("Unimplemented!\n");
#endif
#endif

#endif