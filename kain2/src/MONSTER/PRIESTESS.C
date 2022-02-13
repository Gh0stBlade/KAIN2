/* ========================================== */
/* Unused code from the PC version, it's from */
/* the prototype priestess and barely does a  */
/* thing in actuality.                        */
/* ========================================== */

#include "KAIN2.H"

void __cdecl PRIESTS_Init(struct _Instance* instance)
{
	__int16* v1; // ebx
	int* extraData; // ebp
	char* v3; // eax
	struct _Position* v4; // edi
	int v5; // ecx
	struct _StreamUnit* StreamUnitWithID; // eax
	struct _StreamUnit* v7; // esi
	int v8; // ecx

	v1 = (__int16*)*((DWORD*)instance->data + 1);
	MON_DefaultInit(instance);
	extraData = (int*)instance->extraData;
	if (extraData)
	{
		v3 = MEMPACK_Malloc(0x1Cu, 0x25u);
		v4 = (struct _Position*)v3;
		if (!v3)
		{
			MON_Say();
			v5 = *extraData;
			v5 = *extraData & ~0x1000;
			extraData[87] = 0;
			v5 |= 0x2000u;
			*extraData = v5;
			return;
		}
		extraData[87] = (int)v3;
		StreamUnitWithID = STREAM_GetStreamUnitWithID(instance->currentStreamUnitID);
		v7 = StreamUnitWithID;
		if (v1)
		{
			PLANAPI_FindNodePositionInUnit(StreamUnitWithID, v4, *v1, 5);
			PLANAPI_FindNodePositionInUnit(v7, v4 + 1, v1[1], 5);
		}
	}
	v8 = *extraData;
	v8 = *extraData & ~0x3000 | 0x200;
	*extraData = v8;
}
void __cdecl PRIESTS_CleanUp(struct _Instance* instance)
{
	void* extraData; // eax
	char* v2; // eax

	extraData = instance->extraData;
	if (extraData)
	{
		v2 = (char*)*((DWORD*)extraData + 87);
		if (v2)
			MEMPACK_Free(v2);
	}
	MON_CleanUp(instance);
}
char __cdecl PRIESTS_Query(struct _Instance* instance, struct evFXHitData* data)
{
	char result; // al
	int v3; // ecx

	if (data != (struct evFXHitData*)30)
		return MonsterQuery(instance, data);
	v3 = *((DWORD*)instance->extraData + 1);
	result = (v3 & 1) != 0;
	if ((v3 & 2) != 0)
		return result | 2;
	return result;
}
void __cdecl PRIESTS_Message(struct _Instance* instance, unsigned int message, unsigned int data)
{
	DWORD* extraData; // edx
	int v4; // eax
	int v5; // eax
	int v6; // esi
	char** v7; // ecx

	extraData = instance->extraData;
	if (message == 0x1000017)
	{
		switch (data)
		{
		case 0u:
		case 1u:
		case 3u:
		case 5u:
			v6 = extraData[87];
			v7 = (char**)instance->data;
			if (v6)
			{
				switch (data)
				{
				case 0u:
					MON_PlayAnimFromList(instance, v7[2], 4, 2);
					*(WORD*)(v6 + 18) = 1;
					break;
				case 1u:
					MON_PlayAnimFromList(instance, v7[2], 9, 1);
					*(WORD*)(v6 + 18) = 2;
					break;
				case 3u:
					MON_PlayAnimFromList(instance, v7[2], 10, 1);
					*(WORD*)(v6 + 18) = 3;
					break;
				case 5u:
					MON_PlayAnimFromList(instance, v7[2], 8, 1);
					*(WORD*)(v6 + 18) = 7;
					break;
				default:
					return;
				}
			}
			break;
		case 2u:
			v4 = extraData[1];
			v4 = v4 & ~1;
			extraData[1] = v4;
			break;
		case 4u:
			v5 = extraData[1];
			v5 = v5& ~2;
			extraData[1] = v5;
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
}

void __cdecl PRIESTS_IdleEntry(struct _Instance* instance)
{
	DWORD* extraData; // esi
	int zVel; // edi

	extraData = (DWORD*)instance->extraData;
	if (extraData[87])
	{
		if ((*(BYTE*)extraData & 4) != 0)
		{
			MON_IdleEntry(instance);
		}
		else
		{
			zVel = extraData[87];
			MON_PlayAnimID(instance, **((char**)instance->data + 0x11), 2);
			*(WORD*)(zVel + 18) = 0;
			extraData[61] = 1;
		}
	}
}
void __cdecl PRIESTS_Idle(struct _Instance* instance)
{}
void __cdecl PRIESTS_PursueEntry(struct _Instance* instance)
{
	if ((*(BYTE*)instance->extraData & 4) != 0)
		MON_PursueEntry(instance);
}
void __cdecl PRIESTS_Pursue(struct _Instance* instance)
{
	if ((*(BYTE*)instance->extraData & 4) != 0)
	{
		MON_Pursue(instance);
	}
	else
	{
		MON_SwitchState(instance, MONSTER_STATE_IDLE);
		MON_DefaultQueueHandler(instance);
	}
}
void __cdecl PRIESTS_FleeEntry(struct _Instance* instance)
{
	DWORD* extraData; // esi
	char** data; // ecx
	int v3; // edi

	extraData = instance->extraData;
	data = (char**)instance->data;
	v3 = extraData[87];
	if (v3)
	{
		*extraData = *extraData & ~0x31000u | 0x21000;
		MON_PlayAnimFromList(instance, data[2], 6, 2);
		extraData[61] = 4;
		*(WORD*)(v3 + 18) = 0;
	}
}
void* __cdecl PRIESTS_Flee(struct _Instance* instance)
{
	DWORD* extraData; // eax
	char** data; // ebx
	struct _Position* v4; // esi
	char* v5; // ebp
	int v6; // ebp
	int v7; // eax
	char* v8; // [esp+10h] [ebp-4h]
	struct _Instance* instancea; // [esp+18h] [ebp+4h]

	extraData = instance->extraData;
	data = (char**)instance->data;
	v4 = (struct _Position*)extraData[87];
	v5 = data[1];
	v8 = v5;
	instancea = (struct _Instance*)extraData[49];
	if (v4 && v5)
	{
		switch (v4[3].x)
		{
		case 0:
			if (MATH3D_LengthXY(instance->position.x - v4->x, instance->position.y - v4->y) >= *((__int16*)v5 + 2))
				goto LABEL_16;
			MON_PlayAnimID(instance, *data[17], 2);
			++v4[3].x;
			MON_DefaultQueueHandler(instance);
			return;
		case 1:
			if (MON_TurnToPosition(instance, v4 + 1, *(WORD*)(extraData[85] + 34)))
			{
				MON_PlayAnimFromList(instance, data[2], 0, 1);
				++v4[3].x;
			}
			goto LABEL_8;
		case 2:
		LABEL_8:
			v6 = 1;
			if (instancea
				&& MATH3D_LengthXY(
					instance->position.x - SLOWORD(instancea->node.next[11].next),
					instance->position.y - SHIWORD(instancea->node.next[11].next)) < *((__int16*)v8 + 4))
			{
				MON_PlayAnimFromList(instance, data[2], 3, 1);
				++v4[3].x;
				v6 = 0;
			}
			if (!v6 || (instance->flags2 & 0x10) == 0)
				goto LABEL_16;
			v7 = rand();
			MON_PlayAnimFromList(instance, data[2], v7 % 3, 1);
			MON_DefaultQueueHandler(instance);
			break;
		case 3:
			if ((instance->flags2 & 0x10) != 0)
			{
				MON_PlayAnimFromList(instance, data[2], 5, 1);
				++v4[3].x;
			}
			goto LABEL_16;
		default:
		LABEL_16:
			MON_DefaultQueueHandler(instance);
			break;
		}
	}
}

extern void HUMAN_DeadEntry(struct _Instance* instance);
extern void HUMAN_Dead(struct _Instance* instance);

struct _MonsterStateChoice PRIESTS_StateChoiceTable[] =
{
	{2, PRIESTS_IdleEntry, PRIESTS_Idle},
	{1, PRIESTS_PursueEntry, PRIESTS_Pursue},
	{0x13, PRIESTS_FleeEntry, PRIESTS_Flee},
	{0x17, HUMAN_DeadEntry, HUMAN_Dead},
	{-1}
};

struct _MonsterFunctionTable PRIESTS_FunctionTable =
{
	PRIESTS_Init, PRIESTS_CleanUp, 0, __DATE__,  PRIESTS_Query, PRIESTS_Message, PRIESTS_StateChoiceTable, __DATE__
};
