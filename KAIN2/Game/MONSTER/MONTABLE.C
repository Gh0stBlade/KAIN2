#include "Game/CORE.H"

#include "Game/MONSTER/MONSTER.H"
#include "Game/MONSTER/SOUL.H"
#include "Game/MONSTER/SLUAGH.H"
#include "Game/MONSTER/VWRAITH.H"
#include "Game/MONSTER/WORSHIP.H"
#include "Game/MONSTER/HUMAN.H"

struct _MonsterFunctionTable DefaultFunctionTable;
static struct _MonsterState DefaultStateTable[31]; // offset 0x800CC504

struct 
{
	long whatAmI; // size=0, offset=0
	struct _MonsterFunctionTable* table; // size=32, offset=4
} functionChoiceTable[6] = {

	4,      &SOUL_FunctionTable,
	2050,   &SLUAGH_FunctionTable,
	4098,   &VWRAITH_FunctionTable,
	8320,   &WORSHIP_FunctionTable,
	32776,  &HUMAN_FunctionTable,
	0,      NULL,
};

#if 0
struct _MonsterState monstate_tbl[] =
{
	MON_BirthEntry, MON_Birth,
	MON_PursueEntry, MON_Pursue,
	MON_IdleEntry, MON_Idle,
	MON_MissileHitEntry, MON_MissileHit,
	MON_FallEntry, MON_Fall,
	MON_WanderEntry, MON_Wander,
	MON_AttackEntry, MON_Attack,
	MON_ImpaleDeathEntry, MON_ImpaleDeath,
	MON_HitEntry, MON_Hit,
	MON_StunnedEntry, MON_Stunned,
	MON_GrabbedEntry, MON_Grabbed,
	MON_ThrownEntry, MON_Thrown,
	MON_ImpactEntry, MON_Impact,
	MON_CombatEntry, MON_Combat,
	MON_BreakHoldEntry, MON_BreakHold,
	MON_LandOnFeetEntry, MON_LandOnFeet,
	MON_GeneralDeathEntry, MON_GeneralDeath,
	MON_EnvironmentDamageEntry, MON_EnvironmentDamage,
	MON_LandInWaterEntry, MON_LandInWater,
	MON_FleeEntry, MON_Flee,
	MON_HideEntry, MON_Hide,
	MON_SurpriseAttackEntry, MON_SurpriseAttack,
	MON_ParryEntry, MON_Parry,
	MON_DeadEntry, MON_Dead,
	MON_SurprisedEntry, MON_Surprised,
	MON_NoticeEntry, MON_Notice,
	MON_PupateEntry, MON_Pupate,
	MON_EmbraceEntry, MON_Embrace,
	MON_ProjectileEntry, MON_Projectile,
	MON_TerrainImpaleDeathEntry, MON_TerrainImpaleDeath,
	MON_PetrifiedEntry, MON_Petrified
};

struct _MonsterFunctionTable mondef_tbl =
{
	MON_DefaultInit,
	MON_CleanUp,
	MON_DamageEffect,
	MonsterQuery,
	MonsterMessage, 0,
	__DATE__
};

struct MONTABLE_207fake // hashcode: 0x35A35A66 (dec: 899897958)
{
	long whatAmI; // size=0, offset=0
	struct _MonsterFunctionTable* table; // size=32, offset=4
};

struct MONTABLE_207fake mon_tables[] =
{
	//{4,        SOUL_FunctionTable},
	//{0x802,    SLUAGH_FunctionTable},
	//{0x1002,   VWRAITH_FunctionTable },
	//{0x2008,   WORSHIP_FunctionTable},
	//{0x8008,   HUMAN_FunctionTable},
	{0x4008,   &HUNTER_FunctionTable},
	//{0x42,     SKINNER_FunctionTable},
	//{0x82,     ALUKA_FunctionTable},
	//{0x102,    WALLCR_FunctionTable},
	//{0x10002,  KAIN_FunctionTable},
	//{0x10042,  SKINBOS_FunctionTable},
	//{0x10202,  RONINBSS_FunctionTable},
	//{0x10082,  ALUKABSS_FunctionTable},
	//{0x10102,  WALBOSS_FunctionTable},
	//{0x410002, WALBOSB_FunctionTable},
	{0}
};
#endif

void MONTABLE_SetupTablePointer(struct Object* object)  // Matching - 100%
{
	long whatAmI;
	int i;  // not from SYMDUMP

	whatAmI = ((struct _MonsterAttributes*)(object->data))->whatAmI;

	for (i = 0; functionChoiceTable[i].whatAmI != 0; i++)
	{
		if (whatAmI == functionChoiceTable[i].whatAmI)
		{
			object->relocModule = functionChoiceTable[i].table;
		}
	}
}

struct _MonsterState* MONTABLE_GetStateFuncs(struct _Instance* instance, int state)  // Matching - 100%
{
	struct _MonsterFunctionTable* ft;

	ft = (struct _MonsterFunctionTable*)instance->object->relocModule;

	if (ft != NULL)
	{
		struct _MonsterStateChoice* choice;

		choice = ft->stateFuncs;

		while (choice->state != -1)
		{
			if (state == choice->state)
			{
				return (struct _MonsterState*)&choice->functions.entryFunction;
			}

			choice++;
		}
	}
	return (struct _MonsterState*)&DefaultStateTable[state].entryFunction;
}

void* MONTABLE_GetDamageEffectFunc(struct _Instance* instance)  // Matching - 100%
{
	struct _MonsterFunctionTable* ft;

	ft = (struct _MonsterFunctionTable*)instance->object->relocModule;

	if (ft != NULL && ft->damageEffectFunc != NULL)
	{
		return (void*)ft->damageEffectFunc;
	}

	return (void*)DefaultFunctionTable.damageEffectFunc;
}

void* MONTABLE_GetInitFunc(struct _Instance* instance)  // Matching - 100%
{
	struct _MonsterFunctionTable* ft;

	ft = (struct _MonsterFunctionTable*)instance->object->relocModule;

	if (ft != NULL && ft->initFunc != NULL)
	{
		return (void*)ft->initFunc;
	}

	return (void*)DefaultFunctionTable.initFunc;
}

void* MONTABLE_GetCleanUpFunc(struct _Instance* instance)  // Matching - 100%
{
	struct _MonsterFunctionTable* ft;

	ft = (struct _MonsterFunctionTable*)instance->object->relocModule;

	if (ft != NULL && ft->cleanUpFunc != NULL)
	{
		return (void*)ft->cleanUpFunc;
	}

	return (void*)DefaultFunctionTable.cleanUpFunc;
}

void MONTABLE_SetQueryFunc(struct _Instance* instance)  // Matching - 100%
{
	struct _MonsterFunctionTable* ft;

	ft = (struct _MonsterFunctionTable*)instance->object->relocModule;

	if (ft != NULL && ft->queryFunc != NULL)
	{
		instance->queryFunc = ft->queryFunc;
	}
}

void MONTABLE_SetMessageFunc(struct _Instance* instance)  // Matching - 100%
{
	struct _MonsterFunctionTable* ft;

	ft = (struct _MonsterFunctionTable*)instance->object->relocModule;

	if (ft != NULL && ft->messageFunc != NULL)
	{
		instance->messageFunc = ft->messageFunc;
	}
}