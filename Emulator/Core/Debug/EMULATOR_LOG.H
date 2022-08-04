#ifndef EMULATOR_LOG_H
#define EMULATOR_LOG_H

#include "Core/Setup/Platform/EMULATOR_PLATFORM_SETUP.H"

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

enum LOG_TYPE
{
	LT_DEFAULT = 7,
	LT_WARNING = 6,
	LT_ERROR = 4,
	LT_INFO = 11
};

extern void Emulator_Log(enum LOG_TYPE lt, const char* file, const char* func, int line, const char* fmt, ...);

#if defined(UWP)
	#define eprint(c, fmt, ...) Emulator_Log(c, __FILE__, FUNCNAME, __LINE__, fmt, __VA_ARGS__)
	#define eprintf(fmt, ...) eprint(LT_DEFAULT, fmt, ##__VA_ARGS__);
	#define eprintinf(fmt, ...) eprint(LT_INFO, fmt, ##__VA_ARGS__);
	#define eprintwarn(fmt, ...) eprint(LT_WARNING, fmt, ##__VA_ARGS__);
	#define eprinterr(fmt, ...) eprint(LT_ERROR, fmt, ##__VA_ARGS__);
#else
	#define eprint(c, fmt, ...) Emulator_Log(c, __FILE__, FUNCNAME, __LINE__, fmt, __VA_ARGS__)
	#define eprintf(fmt, ...) eprint(LT_DEFAULT, fmt, ##__VA_ARGS__);
	#define eprintinf(fmt, ...) eprint(LT_INFO, fmt, ##__VA_ARGS__);
	#define eprintwarn(fmt, ...) eprint(LT_WARNING, fmt, ##__VA_ARGS__);
	#define eprinterr(fmt, ...) eprint(LT_ERROR, fmt, ##__VA_ARGS__);
#endif

#else
#define eprint(...)
#define eprintf(...)
#define eprintinf(...)
#define eprintwarn(...)
#define eprinterr(...)
#endif
#endif

#if defined(__EMSCRIPTEN__) || !defined(DEBUG_PRINTS)
#define UNIMPLEMENTED()
#else
#if defined(_WIN32) && defined(_DEBUG)

#include "Core/Debug/BOUNTY_LIST.H"

#define UNIMPLEMENTED() eprintwarn("Unimplemented!\n"); \
		Emulator_AddBounty(FUNCNAME);
#else
#define UNIMPLEMENTED() eprinterr("Unimplemented!\n");
#endif
#endif

#endif