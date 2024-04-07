#include "Game/CORE.H"
#include "MONSTER.H"
#include "HUMAN.H"
#include <Game/SAVEINFO.H>
#include <Game/STATE.H>
#include <Game/SOUND.H>
#include <Game/GAMEPAD.H>
#include <Game/OBTABLE.H>

struct _MonsterStateChoice HUMAN_StateChoiceTable[] =
{
	{9, HUMAN_StunnedEntry, HUMAN_Stunned},
	{0x17, HUMAN_DeadEntry, HUMAN_Dead},
	{0x1B, HUMAN_EmbraceEntry, HUMAN_Embrace },
	{2, HUMAN_IdleEntry, HUMAN_Idle},
	{0x13, MON_FleeEntry, HUMAN_Flee},
	{-1}
};

struct _MonsterFunctionTable HUMAN_FunctionTable =
{
	HUMAN_Init,
	HUMAN_CleanUp,
	0,
	HUMAN_Query,
	0,
	HUMAN_StateChoiceTable,
	__DATE__
};

typedef void (*TDRFuncPtr_MONTABLE_GetInitFunc)();
extern void MonsterProcess(struct _Instance* instance, struct GameTracker* gameTracker);
extern void MON_FleeEntry(struct _Instance* instance);

void HUMAN_WaitForWeapon(struct _Instance* instance, struct GameTracker* gameTracker) // Matching - 100%
{
	typedef void (*MONTABLE_InitFunc)(struct _Instance*); // not from SYMDUMP

	((MONTABLE_InitFunc)MONTABLE_GetInitFunc(instance))(instance);

	if (instance->LinkChild != NULL)
	{
		instance->processFunc = MonsterProcess;
		instance->flags &= ~0x800;
		instance->flags2 &= ~0x20000000;
		instance->flags2 &= ~0x80;
	}
}


struct _Instance* HUMAN_CreateWeapon(struct _Instance* instance, int weaponid, int segment) // Matching - 100%
{
	struct Object* weapon;
	struct _Instance* iweapon;

	weapon = (struct Object*)objectAccess[weaponid].object;

	if (weapon != NULL)
	{
		iweapon = INSTANCE_BirthObject(instance, weapon, 0);

		if (iweapon != NULL)
		{
			INSTANCE_Post(iweapon, 0x800002, SetObjectData(0, 0, 0, instance, segment));

			iweapon->flags2 |= 0x20000;

			return iweapon;
		}
	}

	instance->processFunc = HUMAN_WaitForWeapon;
	instance->flags |= 0x800;
	instance->flags2 |= 0x20000080;

	return NULL;
}


void HUMAN_Init(struct _Instance* instance)  // Matching - 100%
{
	struct _MonsterVars* mv;
	struct _MonsterAttributes* ma;

	mv = (struct _MonsterVars*)instance->extraData;
	ma = (struct _MonsterAttributes*)instance->data;

	if (!(ma->whatAmI & 0x2000))
	{
		int opinion;
		struct _MonsterAllegiances* allegiances;

		allegiances = mv->subAttr->allegiances;
		if (GlobalSave->humanOpinionOfRaziel > 0)
		{
			allegiances->enemies &= ~0x1;
			allegiances->gods |= 0x1;
			allegiances->allies |= 0x1;
		}
		else
		{
			allegiances->gods &= ~0x1;
			allegiances->allies &= ~0x1;
			allegiances->enemies |= 0x1;
		}
	}
	if (strcmpi(instance->object->name, "vlgrb___") == 0)
	{
		mv->auxFlags |= 0x20;
	}
	MON_DefaultInit(instance);
	mv->soulJuice = 16384;
	mv->mvFlags |= 0x2000;
}

void HUMAN_CleanUp(struct _Instance* instance)  // Matching - 100%
{
	MON_CleanUp(instance);
}


unsigned long HUMAN_Query(struct _Instance* instance, unsigned long query)  // Matching - 100%
{
	struct _MonsterVars* mv;
	struct _MonsterAttributes* ma;
	unsigned long ret;

	ma = (struct _MonsterAttributes*)instance->data;
	mv = (struct _MonsterVars*)instance->extraData;

	if (ma == NULL)
	{
		return 0;
	}

	switch (query)
	{
	case 0:
		if ((mv->mvFlags & 0x200))
		{
			ret = 0x40000000;
			break;
		}

		ret = 0x12000000;
		if ((instance->currentMainState != 30))
		{
			ret = ((mv->mvFlags & 0x100) != 0) << 29;
			if (!(mv->mvFlags & 0x200000))
			{
				if ((mv->hitPoints <= 0x1000) || (mv->auxFlags & 3))
				{
					ret |= 0x08000000;
				}
			}
		}
		break;
	case 37:
		if (!(ma->whatAmI & 0x8000))
		{
			ret = mv->subAttr->allegiances->enemies & 1;
		}
		else
		{
			ret = 0;
		}
		break;
	default:
		ret = MonsterQuery(instance, query);
	}
	return ret;
}


void HUMAN_DeadEntry(struct _Instance* instance) // Matching - 100%
{
	struct _MonsterVars* mv;

	mv = (struct _MonsterVars*)instance->extraData;

	mv->soulJuice /= 4;

	if (mv->soulJuice >= 4097)
	{
		mv->soulJuice = 4096;
	}

	MON_DeadEntry(instance);

	mv->damageTimer = MON_GetTime(instance);

	MON_BirthMana(instance);
}


void HUMAN_Dead(struct _Instance* instance) // Matching - 100%
{
	struct _MonsterVars* mv;

	mv = (struct _MonsterVars*)instance->extraData;

	instance->fadeValue = (short)(MON_GetTime(instance) - mv->damageTimer);

	if (mv->causeOfDeath == 6)
	{
		MON_Dead(instance);
		return;
	}

	if (instance->fadeValue >= 4096)
	{
		MON_KillMonster(instance);
	}

	if (((mv->mvFlags & 0x400000)) && (mv->effectTimer < MON_GetTime(instance)))
	{
		mv->mvFlags &= ~0x400000;
	}

	if (!(mv->mvFlags & 0x2))
	{
		MON_ApplyPhysics(instance);
	}

	do
	{

	} while (DeMessageQueue(&mv->messageQueue) != NULL);
}


void HUMAN_StunnedEntry(struct _Instance* instance) // Matching - 100%
{
	struct _MonsterVars* mv;

	mv = (struct _MonsterVars*)instance->extraData;

	if ((mv->auxFlags & 0x10))
	{
		mv->generalTimer = MON_GetTime(instance) + 60000;

		MON_PlayAnim(instance, MONSTER_ANIM_SOULSUCK, 1);
	}
	else
	{
		MON_StunnedEntry(instance);
	}
}


void HUMAN_Stunned(struct _Instance* instance) // Matching - 100%
{
	struct _MonsterVars* mv;

	mv = (struct _MonsterVars*)instance->extraData;

	if ((mv->auxFlags & 0x10))
	{
		if ((instance->flags2 & 0x10))
		{
			MON_PlayAnim(instance, MONSTER_ANIM_STUNNED, 2);
		}

		if (mv->generalTimer < MON_GetTime(instance))
		{
			mv->soulJuice = 16384;
			mv->auxFlags &= ~0x10;
		}

		MON_DefaultQueueHandler(instance);
		return;
	}

	MON_Stunned(instance);
}


void HUMAN_EmbraceEntry(struct _Instance* instance) // Matching - 100%
{
	struct _MonsterVars* mv;

	mv = (struct _MonsterVars*)instance->extraData;

	MON_PlayAnim(instance, MONSTER_ANIM_EMBRACE, 1);

	MON_TurnOffBodySpheres(instance);

	mv->generalTimer = mv->soulJuice / 4096;
}


void HUMAN_Embrace(struct _Instance* instance) // Matching - 100%
{
	struct _MonsterVars* mv;
	struct __Event* message;
	int letgo;
	int juice;

	letgo = 0;

	mv = (struct _MonsterVars*)instance->extraData;

	MON_TurnToPosition(instance, &gameTrackerX.playerInstance->position, 4096);

	while (message = DeMessageQueue(&mv->messageQueue), message != NULL)
	{
		if (message->ID == 0x1000014)
		{
			letgo = 1;
		}
		else
		{
			MON_DefaultMessageHandler(instance, message);
		}
	}

	juice = (mv->generalTimer * gameTrackerX.timeMult * 33) / 5000;

	INSTANCE_Post(gameTrackerX.playerInstance, 0x1000016, juice);

	do
	{

	} while (0); // garbage code for reodering

	if (mv->soulJuice < juice)
	{
		mv->soulJuice = 0;
	}
	else
	{
		mv->soulJuice -= juice;
	}

	GAMEPAD_Shock1(128 - ((mv->soulJuice << 7) / (mv->generalTimer << 12)), 61440);

	if (mv->soulJuice == 0)
	{
		mv->damageType = 0;

		MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);

		INSTANCE_Post(gameTrackerX.playerInstance, 0x1000006, (int)instance);

		mv->soulJuice = 0;

		SOUND_Play3dSound(&instance->position, 8, -450, 80, 3500);
	}
	else if (letgo != 0)
	{
		mv->auxFlags |= 0x10;

		MON_SwitchState(instance, MONSTER_STATE_STUNNED);

		MON_TurnOnBodySpheres(instance);
	}
	else if (instance->currentMainState != 27)
	{
		INSTANCE_Post(gameTrackerX.playerInstance, 0x1000006, (int)instance);

		MON_TurnOnBodySpheres(instance);
	}
}

void HUMAN_IdleEntry(struct _Instance* instance)//Matching - 100%
{
	struct _MonsterVars* mv;

	mv = (struct _MonsterVars*)instance->extraData;

	MON_IdleEntry(instance);

	mv->auxFlags &= 0xFFFFFFFD;
	mv->auxFlags &= 0xFFFFFFFB;
	mv->auxFlags &= 0xFFFFFFFE;
}


void HUMAN_Idle(struct _Instance* instance) // Matching - 100%
{
	struct _MonsterVars* mv;
	struct _MonsterAttributes* ma;
	struct _MonsterIR* ally;
	struct _MonsterIR* enemy;

	mv = (struct _MonsterVars*)instance->extraData;

	ma = (struct _MonsterAttributes*)instance->data;

	ally = mv->ally;

	enemy = mv->enemy;

	if ((!(mv->mvFlags & 0x4)) && (ally != NULL) && ((ally->mirFlags & 0x4)))
	{
		if ((mv->auxFlags & 0x2))
		{
			MON_TurnToPosition(instance, &ally->instance->position, mv->subAttr->speedPivotTurn);

			if ((instance->flags2 & 0x2))
			{
				mv->auxFlags = ((mv->auxFlags & ~0x2) | 0x1);
			}

			MON_DefaultQueueHandler(instance);
			return;
		}

		if ((mv->auxFlags & 0x1))
		{
			if ((ally->distance >= 2000) || ((enemy != NULL) && (enemy->distance < 2000)))
			{
				mv->auxFlags = ((mv->auxFlags & ~0x1) | 0x4);

				MON_PlayAnimFromList(instance, ma->auxAnimList, 1, 1);
			}

			MON_DefaultQueueHandler(instance);
			return;
		}

		if ((mv->auxFlags & 0x4))
		{
			if ((instance->flags2 & 0x10))
			{
				mv->auxFlags &= ~0x4;

				MON_PlayRandomIdle(instance, 2);
			}

			MON_DefaultQueueHandler(instance);
			return;
		}

		if ((ally->distance < 2000) && ((enemy == NULL) || (enemy->distance >= 2000)))
		{
			mv->auxFlags |= 0x2;

			MON_PlayAnimFromList(instance, ma->auxAnimList, 0, 2);

			MON_DefaultQueueHandler(instance);
			return;
		}
	}

	MON_Idle(instance);
}


void HUMAN_Flee(struct _Instance* instance) // Matching - 100%
{
	struct _MonsterVars* mv;
	struct _MonsterIR* enemy;

	mv = (struct _MonsterVars*)instance->extraData;

	enemy = mv->enemy;

	if ((enemy != NULL) && (enemy->distance < 640))
	{
		if (!(mv->auxFlags & 0x8))
		{
			struct _MonsterAttributes* ma;

			ma = (struct _MonsterAttributes*)instance->data;

			MON_PlayAnimFromList(instance, ma->auxAnimList, 2, 2);

			mv->auxFlags |= 0x8;

			if ((mv->auxFlags & 0x20))
			{
				SOUND_Play3dSound(&instance->position, 459, 0, 88, 3500);
			}
			else
			{
				SOUND_Play3dSound(&instance->position, 458, -100, 92, 3500);
			}
		}

		MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);

		MON_DefaultQueueHandler(instance);
	}
	else if ((mv->auxFlags & 0x8))
	{
		MON_SwitchState(instance, MONSTER_STATE_FLEE);
	}
	else
	{
		MON_Flee(instance);
	}

	if (((mv->auxFlags & 0x8)) && ((mv->mvFlags & 0x1)))
	{
		mv->auxFlags &= ~0x8;
	}
}


void HUMAN_GetAngry() // Matching - 100%
{
	struct _Instance* instance;

	instance = gameTrackerX.instanceList->first;

	while (instance != NULL)
	{
		if ((INSTANCE_Query(instance, 1) & 0xC000))
		{
			struct _MonsterVars* mv;
			struct _MonsterAllegiances* allegiances;
			struct _MonsterIR* mir;

			mv = (struct _MonsterVars*)instance->extraData;

			allegiances = mv->subAttr->allegiances;

			do
			{

			} while (0); // garbage code for reodering

			allegiances->gods &= ~0x1;
			allegiances->allies &= ~0x1;
			allegiances->enemies |= 0x1;

			mir = MONSENSE_SetEnemy(instance, gameTrackerX.playerInstance);

			if (mir != NULL)
			{
				mir->mirFlags &= ~0x6;
			}
		}

		instance = instance->next;
	};
}


int HUMAN_TypeOfHuman(struct _Instance* instance) // Matching - 100%
{
	int type;
	struct _MonsterVars* mv;

	type = INSTANCE_Query(instance, 1);

	mv = (struct _MonsterVars*)instance->extraData;

	if ((type & 0x4000))
	{
		return 1;
	}

	if ((type & 0x2000))
	{
		return 4;
	}

	if (!(type & 0x8000))
	{
		return 0;
	}

	if ((mv->auxFlags & 0x20))
	{
		return 3;
	}

	return 2;
}