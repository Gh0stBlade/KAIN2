#include "EMULATOR_UTILITY_OGLES.H"
#include "Core/Debug/EMULATOR_LOG.H"
#include "Core/Utility/EMULATOR_UTILITY_COMMON.H"
#include <stdio.h>
#include <stdlib.h>

#if defined(OGLES)

int64_t Emulator_GetTicks()
{
	return  SDL_GetTicks();
}

#endif
