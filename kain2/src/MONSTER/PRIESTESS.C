/* ========================================== */
/* Unused code from the PC version, it's from */
/* the prototype priestess and barely does a  */
/* thing in actuality.                        */
/* ========================================== */
#include <stdlib.h>
#include "CORE.H"

void __cdecl PRIESTS_Init(struct _Instance* instance)
{
#if defined(PC_VERSION)
	__int16* v1; // ebx
	struct _MonsterVars* mv; // ebp
	char* extra; // eax
	struct _Position* v4; // edi
	struct _StreamUnit* unit; // eax MAPDST

	v1 = (__int16*)*((DWORD*)instance->data + 1);
	MON_DefaultInit(instance);
	mv = (struct _MonsterVars*)instance->extraData;
	if (mv)
	{
		extra = MEMPACK_Malloc(0x1Cu, 0x25u);
		v4 = (struct _Position*)extra;
		if (!extra)
		{
			MON_Say(instance, "ERROR: Out of space for priests variables!\n");
			mv->extraVars = 0;
			mv->mvFlags = (mv->mvFlags & ~0x1000) | 0x2000;
			return;
		}
		mv->extraVars = extra;
		unit = STREAM_GetStreamUnitWithID(instance->currentStreamUnitID);
		if (v1)
		{
			PLANAPI_FindNodePositionInUnit(unit, v4, *v1, 5);
			PLANAPI_FindNodePositionInUnit(unit, v4 + 1, v1[1], 5);
		}
		mv->mvFlags = mv->mvFlags & ~0x3000 | 0x200;
	}
#endif
}
void __cdecl PRIESTS_CleanUp(struct _Instance* instance)
{
#if defined(PC_VERSION)
	struct _MonsterVars* mv; // eax
	char* v2; // eax

	mv = (struct _MonsterVars*)instance->extraData;
	if (mv)
	{
		v2 = (char*)mv->extraVars;
		if (v2)
			MEMPACK_Free(v2);
	}
	MON_CleanUp(instance);
#endif
}

u_long __cdecl PRIESTS_Query(struct _Instance* instance, unsigned long data)
{
#if defined(PC_VERSION)
	char result; // al
	int aux; // ecx

	if (data != 30)
		return MonsterQuery(instance, data);
	aux = ((struct _MonsterVars*)instance->extraData)->auxFlags;
	result = (aux & 1) != 0;
	if ((aux & 2) != 0)
		return result | 2;
	return result;
#else
	return 0;
#endif
}

void __cdecl PRIESTS_Message(struct _Instance* instance, unsigned int message, unsigned int data)
{
#if defined(PC_VERSION)
	struct _MonsterVars* mv; // edx
	int v4; // eax
	int v5; // eax
	int v6; // esi
	struct _MonsterAttributes* ma; // ecx

	mv = (struct _MonsterVars*)instance->extraData;
	if (message == 0x1000017)
	{
		switch (data)
		{
		case 0u:
		case 1u:
		case 3u:
		case 5u:
			v6 = (int)mv->extraVars;
			ma = (struct _MonsterAttributes*)instance->data;
			if (v6)
			{
				switch (data)
				{
				case 0u:
					MON_PlayAnimFromList(instance, ma->auxAnimList, 4, 2);
					*(WORD*)(v6 + 18) = 1;
					break;
				case 1u:
					MON_PlayAnimFromList(instance, ma->auxAnimList, 9, 1);
					*(WORD*)(v6 + 18) = 2;
					break;
				case 3u:
					MON_PlayAnimFromList(instance, ma->auxAnimList, 10, 1);
					*(WORD*)(v6 + 18) = 3;
					break;
				case 5u:
					MON_PlayAnimFromList(instance, ma->auxAnimList, 8, 1);
					*(WORD*)(v6 + 18) = 7;
					break;
				default:
					return;
				}
			}
			break;
		case 2u:
			v4 = mv->auxFlags;
			v4 = v4 & ~1;
			mv->auxFlags = v4;
			break;
		case 4u:
			v5 = mv->auxFlags;
			v5 = v5& ~2;
			mv->auxFlags = v5;
			break;
		case 6u:
			MON_SwitchStateDoEntry(instance, MONSTER_STATE_FLEE);
			break;
		default:
			return;
		}
	}
	else
	{
		MonsterMessage(instance, message, data);
	}
#endif
}

void __cdecl PRIESTS_IdleEntry(struct _Instance* instance)
{
#if defined(PC_VERSION)
	struct _MonsterVars* mv; // esi
	int zVel; // edi

	mv = (struct _MonsterVars*)instance->extraData;
	if (mv->extraVars)
	{
		if ((mv->mvFlags & 4) != 0)
		{
			MON_IdleEntry(instance);
		}
		else
		{
			zVel = (int)mv->extraVars;
			MON_PlayAnimID(instance, ((struct _MonsterAttributes*)instance->data)->idleList, 2);
			*(WORD*)(zVel + 18) = 0;
			mv->mode = 1;
		}
	}
#endif
}
// TODO: fill me
void __cdecl PRIESTS_Idle(struct _Instance* instance)
{}

void __cdecl PRIESTS_PursueEntry(struct _Instance* instance)
{
#if defined(PC_VERSION)
	if ((((struct _MonsterVars*)instance->extraData)->mvFlags & 4) != 0)
		MON_PursueEntry(instance);
#endif
}

void __cdecl PRIESTS_Pursue(struct _Instance* instance)
{
#if defined(PC_VERSION)
	if ((((struct _MonsterVars*)instance->extraData)->mvFlags & 4) != 0)
	{
		MON_Pursue(instance);
	}
	else
	{
		MON_SwitchState(instance, MONSTER_STATE_IDLE);
		MON_DefaultQueueHandler(instance);
	}
#endif
}

void __cdecl PRIESTS_FleeEntry(struct _Instance* instance)
{
#if defined(PC_VERSION)
	struct _MonsterVars* mv; // esi
	struct _MonsterAttributes* ma; // ecx
	WORD* extraVars; // edi

	mv = (struct _MonsterVars*)instance->extraData;
	ma = (struct _MonsterAttributes*)instance->data;
	extraVars = mv->extraVars;
	if (extraVars)
	{
		mv->mvFlags = mv->mvFlags & ~0x31000u | 0x21000;
		MON_PlayAnimFromList(instance, ma->auxAnimList, 6, 2);
		mv->mode = 4;
		extraVars[9] = 0;
	}
#endif
}

void __cdecl PRIESTS_Flee(struct _Instance* instance)
{
#if defined(PC_VERSION)
	struct _MonsterVars* mv; // eax
	struct _MonsterAttributes* ma; // ebx
	struct _Position* extraVars; // esi
	char* v5; // ebp
	int v6; // ebp
	int v7; // eax
	char* v8; // [esp+10h] [ebp-4h]
	struct _MonsterIR* ir; // [esp+18h] [ebp+4h]

	mv = (struct _MonsterVars*)instance->extraData;
	ma = (struct _MonsterAttributes*)instance->data;
	extraVars = (struct _Position*)mv->extraVars;
	v5 = (char*)ma->tunData;
	v8 = v5;
	ir = mv->enemy;
	if (extraVars && v5)
	{
		switch (extraVars[3].x)
		{
		case 0:
			if (MATH3D_LengthXY(instance->position.x - extraVars->x, instance->position.y - extraVars->y) >= *((__int16*)v5 + 2))
				goto LABEL_16;
			MON_PlayAnimID(instance, *(BYTE*)ma->idleList, 2);
			++extraVars[3].x;
			MON_DefaultQueueHandler(instance);
			return;
		case 1:
			if (MON_TurnToPosition(instance, extraVars + 1, mv->subAttr->speedFleeTurn))
			{
				MON_PlayAnimFromList(instance, ma->auxAnimList, 0, 1);
				++extraVars[3].x;
			}
			goto LABEL_8;
		case 2:
		LABEL_8:
			v6 = 1;
			if (ir
				&& MATH3D_LengthXY(
					instance->position.x - ir->instance->position.x,
					instance->position.y - ir->instance->position.y) < *((__int16*)v8 + 4))
			{
				MON_PlayAnimFromList(instance, ma->auxAnimList, 3, 1);
				++extraVars[3].x;
				v6 = 0;
			}
			if (!v6 || (instance->flags2 & 0x10) == 0)
				goto LABEL_16;
			v7 = rand();
			MON_PlayAnimFromList(instance, ma->auxAnimList, v7 % 3, 1);
			MON_DefaultQueueHandler(instance);
			break;
		case 3:
			if ((instance->flags2 & 0x10) != 0)
			{
				MON_PlayAnimFromList(instance, ma->auxAnimList, 5, 1);
				++extraVars[3].x;
			}
			goto LABEL_16;
		default:
		LABEL_16:
			MON_DefaultQueueHandler(instance);
			break;
		}
	}
#endif
}

// ------------------ unlinked code ------------------
struct _Instance* __cdecl PRIESTS_InstanceToPossess(struct _Instance* instance)
{
#if defined(PC_VERSION)
	struct _Instance* nextinst; // esi
	struct _MonsterVars* mv; // eax

	nextinst = (struct _Instance*)*((DWORD*)gameTrackerX.instanceList + 1);
	if (!nextinst)
		return 0;
	while (1)
	{
		if ((INSTANCE_Query(nextinst, 1) & 8) != 0 && instance != nextinst)
		{
			mv = (struct _MonsterVars*)nextinst->extraData;
			if (mv)
			{
				if ((mv->mvFlags & 0x20000000) == 0)
					break;
			}
		}
		nextinst = nextinst->next;
		if (!nextinst)
			return 0;
	}
	return nextinst;
#else
	return NULL;
#endif
}

void __cdecl PRIESTS_DoAttackAnim(_Instance* instance, int a2)
{
#if defined(PC_VERSION)
	struct _MonsterAttributes* ma; // ecx
	WORD* extraVars; // esi

	ma = (struct _MonsterAttributes*)instance->data;
	extraVars = ((struct _MonsterVars*)instance->extraData)->extraVars;
	if (extraVars)
	{
		switch (a2)
		{
		case 0:
			MON_PlayAnimFromList(instance, ma->auxAnimList, 4, 2);
			extraVars[9] = 1;
			break;
		case 1:
			MON_PlayAnimFromList(instance, ma->auxAnimList, 9, 1);
			extraVars[9] = 2;
			break;
		case 3:
			MON_PlayAnimFromList(instance, ma->auxAnimList, 10, 1);
			extraVars[9] = 3;
			break;
		case 5:
			MON_PlayAnimFromList(instance, ma->auxAnimList, 8, 1);
			extraVars[9] = 7;
			break;
		default:
			return;
		}
	}
#endif
}

extern void HUMAN_DeadEntry(struct _Instance* instance);
extern void HUMAN_Dead(struct _Instance* instance);

#if defined(PC_VERSION)
struct _MonsterStateChoice PRIESTS_StateChoiceTable[] =
{
	{MONSTER_STATE_IDLE,   PRIESTS_IdleEntry,   PRIESTS_Idle},
	{MONSTER_STATE_PURSUE, PRIESTS_PursueEntry, PRIESTS_Pursue},
	{MONSTER_STATE_FLEE,   PRIESTS_FleeEntry,   PRIESTS_Flee},
	{MONSTER_STATE_DEAD,   HUMAN_DeadEntry,     HUMAN_Dead},
	{-1}
};

_MonsterFunctionTable PRIESTS_FunctionTable =
{
	PRIESTS_Init,
	PRIESTS_CleanUp,
	0,
	PRIESTS_Query,
	PRIESTS_Message,
	PRIESTS_StateChoiceTable,
	__DATE__
};
#endif