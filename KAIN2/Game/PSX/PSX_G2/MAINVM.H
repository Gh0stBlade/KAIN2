#ifndef MAINVM_H
#define MAINVM_H

#include "Game/CORE.H"

struct _G2AppDataVM_Type // hashcode: 0xC1BF7B11 (dec: -1044415727)
{
	int argc; // size=0, offset=0
	int* argv; // size=0, offset=4
};

#if !defined(UWP) && !defined(__EMSCRIPTEN__) && !defined(PLATFORM_NX) && !defined(PLATFORM_NX_ARM) && !defined(SN_TARGET_PSP2) && !defined(__ANDROID__) && !defined(__APPLE__) && !defined(_XBOX)
extern int main(); // 0x800962C0
#endif

#endif
