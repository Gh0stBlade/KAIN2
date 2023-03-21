#include "Game/CORE.H"
#include "TIMERG2.H"

short G2Timer_GetFrameTime()
{
	short atime;

	if (gameTrackerX.timeMult == 0)
	{
		atime = 100;
	}
	else 
	{
		atime = (gameTrackerX.timeMult * 100) >> 12;
	}
	
	if (atime <= 0)
	{
		atime = 1;
	}

	return atime;
}
