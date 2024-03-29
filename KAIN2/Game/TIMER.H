#ifndef TIMER_H
#define TIMER_H

extern volatile int gameTimer; // 0x800D05F0


#if defined(PSXPC_VERSION)
extern unsigned long long TIMER_GetTimeMS(); // 0x8003D7B0
extern unsigned long long TIMER_TimeDiff(long long x); // 0x8003D840
#else
extern unsigned long TIMER_GetTimeMS(); // 0x8003D7B0
extern unsigned long TIMER_TimeDiff(unsigned long x); // 0x8003D840
#endif

#endif
