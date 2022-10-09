#include "Game/CORE.H"

#if defined(PC_VERSION)
struct _MonsterStateChoice HUNTER_StateChoiceTable[] =
{
	{MONSTER_STATE_STUNNED,    HUMAN_StunnedEntry,     HUMAN_Stunned},
	{MONSTER_STATE_DEAD,       HUMAN_DeadEntry,        HUMAN_Dead},
	{MONSTER_STATE_EMBRACE,    HUMAN_EmbraceEntry,     HUMAN_Embrace},
	{MONSTER_STATE_PROJECTILE, HUNTER_ProjectileEntry, HUNTER_Projectile},
	{-1}
};

struct _MonsterFunctionTable HUNTER_FunctionTable =
{
	HUNTER_Init,
	HUNTER_CleanUp,
	0,
	HUMAN_Query,
	0,
	HUNTER_StateChoiceTable,
	__DATE__
};
#endif

//0001:00006f00       _FX_MakeHitFlame           00407f00 f   hunter.obj
void FX_MakeHitFlame(_Position* pos, short a2, int a3, int a4, int a5) {}	// unused
 //0001:00006ff0       _HUNTER_InitFlamethrow     00407ff0 f   hunter.obj
void HUNTER_InitFlamethrow(struct _Instance* instance){}	// unused
 //0001:00007020       _HUNTER_Flamethrow         00408020 f   hunter.obj
int HUNTER_Flamethrow(struct _Instance* instance, int damage, int a3, int segment) { return 0; }
 //0001:00007410       _HUNTER_Init               00408410 f   hunter.obj
void HUNTER_Init(struct _Instance* instance){}
 //0001:000074c0       _HUNTER_CleanUp            004084c0 f   hunter.obj
void HUNTER_CleanUp(struct _Instance* instance){}
 //0001:000074f0       _HUNTER_ProjectileEntry    004084f0 f   hunter.obj
void HUNTER_ProjectileEntry(struct _Instance* instance) {}
 //0001:00007520       _HUNTER_Projectile         00408520 f   hunter.obj
void HUNTER_Projectile(struct _Instance* instance) {}
