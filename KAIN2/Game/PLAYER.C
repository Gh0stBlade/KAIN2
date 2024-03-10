#include "CORE.H"
#include "PLAYER.H"
#include "Game/RAZIEL/RAZIEL.H"

void PLAYER_TurnHead(struct _Instance* instance, short* rotx, short* rotz, struct GameTracker* gameTracker)//Matching - 99.53%
{
	if ((INSTANCE_Query(instance, 0x1) & 0x1))
	{
		RAZIEL_TurnHead(instance, rotx, rotz, gameTracker);
	}
	else
	{
		MONAPI_TurnHead(instance, rotx, rotz, gameTracker);
	}
}

long PLAYER_OkToLookAround(struct _Instance* instance)
{
	if ((INSTANCE_Query(instance, 0x1) & 0x1))
	{
		return RAZIEL_OkToLookAround(instance);
	}
	else
	{
		return MONAPI_OkToLookAround(instance);
	}
}

void PLAYER_SetLookAround(struct _Instance* instance)
{
	if (INSTANCE_Query(instance, 0x1) & 0x1)
	{
		RAZIEL_SetLookAround(instance);
	}
	else
	{
		MONAPI_SetLookAround(instance);
	}
}

void PLAYER_ReSetLookAround(struct _Instance* instance) // Matching - 100%
{
	if(INSTANCE_Query(instance, 0x1) & 0x1)
	{
		RAZIEL_ResetLookAround(instance);
	}
	else
	{
		MONAPI_ResetLookAround(instance);
	}
}