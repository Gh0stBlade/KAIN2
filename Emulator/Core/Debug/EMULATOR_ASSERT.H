#ifndef EMULATOR_ASSERT_H
#define EMULATOR_ASSERT_H
	#if defined(_DEBUG)
		#include <assert.h>
		#define eassert assert
	#else
		#define eassert(x)
	#endif
#endif