#include "Game/CORE.H"
#include "TIMERG2.H"

short G2Timer_GetFrameTime()
{
#if defined(PSX_VERSION)
	short atime;

	if (gameTrackerX.timeMult == 0)
	{
		atime = 0x64;
	}
	else 
	{
		atime = (gameTrackerX.timeMult * 25) >> 10;
	}
	
	if (atime <= 0)
	{
		atime = 1;
	}

	return atime;

#elif defined(PC_VERSION)
	unsigned int result; // eax

  if ( gameTrackerX.timeMult )
	result = (100 * gameTrackerX.timeMult) >> 12;
  else
	result = 100;
  if ( (__int16)result <= 0 )
	return 1;
  return result;
#endif
}
