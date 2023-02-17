#include "EMULATOR_UTILITY_D3D12.H"
#include "Core/Debug/EMULATOR_LOG.H"
#include "Core/Utility/EMULATOR_UTILITY_COMMON.H"
#include <stdio.h>
#include <stdlib.h>

#if defined(D3D12)

int64_t Emulator_GetTicks()
{
	return SDL_GetTicks();
}

#endif