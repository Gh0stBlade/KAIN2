#ifndef EMULATOR_ARCHITECTURE_SETUP_H
#define EMULATOR_ARCHITECTURE_SETUP_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#if INTPTR_MAX == INT64_MAX
#define GAME_X64
#define VALKYRIE_X64
#define GAME_ARCH_NAME "64-bit"
#elif INTPTR_MAX == INT32_MAX
#define GAME_X86
#define VALKYRIE_X86
#define GAME_ARCH_NAME "32-bit"
#else
#error Unknown error occurred!
#endif

#endif