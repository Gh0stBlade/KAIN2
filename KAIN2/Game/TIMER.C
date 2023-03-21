#include "CORE.H"
#include "TIMER.H"
#include "PSX/MAIN.H"

long gameTimer; // 0x800CE188

#if defined(PSXPC_VERSION)
unsigned long long TIMER_GetTimeMS()
#else
unsigned long TIMER_GetTimeMS()
#endif
{
#if defined (PSXPC_VERSION)
	return (1000 * Emulator_GetPerformanceCounter()) / Emulator_GetPerformanceFrequency();
#elif defined(PSX_VERSION)
	unsigned long ticks;
	unsigned long mticks;

	EnterCriticalSection();
	ticks = GetRCnt(0xF2000000);
	mticks = gameTimer;
	ExitCriticalSection();
	return (mticks >> 16) * 126819 + (ticks & 0xFFFF | mticks << 16) / 33869;
#endif
}

#if defined(PSXPC_VERSION)
unsigned long long TIMER_TimeDiff(long long x)
#else
unsigned long TIMER_TimeDiff(unsigned long x)
#endif
{
#if defined(PSXPC_VERSION)
	return Emulator_GetPerformanceCounter() / (Emulator_GetPerformanceFrequency() / 1000000) - x;

#elif defined(PSX_VERSION)
	unsigned long intrs;
	unsigned long ticks;
	unsigned long prevIntrs;
	unsigned long prevTicks;
	unsigned long diffIntrs;
	unsigned long diffTicks;
	unsigned long timeDiff;

	ticks = GetRCnt(0xF2000000) & 0xFFFF;
	prevIntrs = x >> 16;
	intrs = gameTimer & 0xFFFF;
	prevTicks = x & 0xFFFF;

	if (intrs < prevIntrs)
	{
		diffIntrs = (intrs + 65536) - prevIntrs;
	}
	else
	{
		diffIntrs = intrs - prevIntrs;
	}

	if (ticks < prevTicks)
	{
		diffTicks = (ticks + 65535) - prevTicks;
		diffIntrs--;
	}
	else
	{
		diffTicks = ticks - prevTicks;
	}

	if (diffIntrs >= 2259)
	{
		timeDiff = 4293263;
	}
	else
	{
		timeDiff = ((diffTicks * 29) + (diffIntrs * 1900515)) / 1000;
	}
	
	if (gTimerEnabled == 0)
	{
		timeDiff = 0;
	}

	return timeDiff;
#endif
}
