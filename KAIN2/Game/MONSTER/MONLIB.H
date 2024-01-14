#ifndef MONLIB_H
#define MONLIB_H

#include "MONSTER.H"

struct evMonsterHitData 
{
	struct _Instance* sender; // size=668, offset=0
	struct _Instance* lastHit; // size=668, offset=4
	short knockBackDistance; // size=0, offset=8
	short knockBackDuration; // size=0, offset=10
	long power; // size=0, offset=12
};

struct evFXHitData 
{
	struct _SVector location; // size=8, offset=0
	struct _SVector velocity; // size=8, offset=8
	short amount; // size=0, offset=16
	short type; // size=0, offset=18
};

extern void MON_TurnOffWeaponSpheres(struct _Instance *instance); // 0x8007F3E4

extern void MON_TurnOnWeaponSpheres(struct _Instance *instance); // 0x8007F4DC

extern void MON_TurnOnWeaponSphere(struct _Instance *instance, int segment); // 0x8007F594

extern void MON_TurnOffBodySpheres(struct _Instance *instance); // 0x8007F69C

extern void MON_TurnOnBodySpheres(struct _Instance *instance); // 0x8007F744

extern void MON_TurnOffAllSpheres(struct _Instance *instance); // 0x8007F7F8

extern void MON_TurnOnAllSpheres(struct _Instance *instance); // 0x8007F824

extern void MON_SwitchState(struct _Instance *instance, MonsterState state); // 0x8007F850

extern void MON_SwitchStateDoEntry(struct _Instance *instance, MonsterState state); // 0x8007F94C

extern int MON_TransNodeAnimation(struct _Instance *instance); // 0x8007F9B4

extern struct _MonsterAnim * MON_GetAnim(struct _Instance *instance, char *animList, int index); // 0x8007F9DC

extern void MON_PlayAnimID(struct _Instance *instance, int index, int mode); // 0x8007FA10

extern void MON_PlayAnimFromList(struct _Instance *instance, char *animList, int animtype, int mode); // 0x8007FBC8

extern int MON_AnimIDPlaying(struct _Instance *instance, int index); // 0x8007FBF0

extern void MON_PlayAnimIDIfNotPlaying(struct _Instance *instance, int index, int mode); // 0x8007FC14

extern int MON_AnimPlayingFromList(struct _Instance *instance, char *animList, int animtype); // 0x8007FC64

extern void MON_PlayAnimFromListIfNotPlaying(struct _Instance *instance, char *animList, int animtype, int mode); // 0x8007FC8C

extern void MON_PlayAnim(struct _Instance *instance, enum MonsterAnim animtype, int mode); // 0x8007FCE4

extern int MON_AnimPlaying(struct _Instance *instance, enum MonsterAnim animtype); // 0x8007FD18

extern void MON_PlayAnimIfNotPlaying(struct _Instance *instance, enum MonsterAnim animtype, int mode); // 0x8007FD4C

extern long MON_AnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, void *data); // 0x8007FD80

extern void MON_AnimInit(struct _Instance *instance); // 0x8007FE30

extern short MON_FacingOffset(struct _Instance *instance, struct _Instance *target); // 0x8007FFA8

extern int MON_CheckConditions(struct _Instance *instance, struct _MonsterIR *mir, char *probArray); // 0x8007FFE4

extern int MON_ShouldIAttackInstance(struct _Instance *instance, struct _Instance *ei); // 0x800800BC

extern int MON_ShouldIAttack(struct _Instance *instance, struct _MonsterIR *enemy, struct _MonsterAttackAttributes *attack); // 0x800801E0

extern struct _MonsterAttackAttributes * MON_ChooseAttack(struct _Instance *instance, struct _MonsterIR *enemy); // 0x80080388

extern int MON_ShouldIEvade(struct _Instance *instance); // 0x800804E8

extern int MON_ChooseLeftOrRight(struct _Instance *instance, struct _MonsterIR *enemy); // 0x80080594

extern int MON_ChooseEvadeMove(struct _Instance *instance); // 0x80080638

extern int MON_ChooseCombatMove(struct _Instance *instance, int reason); // 0x80080694

extern void MON_PlayRandomIdle(struct _Instance *instance, int mode); // 0x80080ADC

extern void MON_PlayCombatIdle(struct _Instance *instance, int mode); // 0x80080C80

extern void MON_GetRandomPoint(_Position *out, _Position *in, short r); // 0x80080CC0

extern int MON_GetRandomDestinationInWorld(struct _Instance *instance, _Position *in, short r); // 0x80080D6C

extern void MON_MoveForward(struct _Instance *instance); // 0x80080F54

extern int MON_TurnToPosition(struct _Instance *instance, _Position *position, short turnspeed); // 0x80080FA4

extern void MON_MoveToPosition(struct _Instance *instance, _Position *position, short turnSpeed); // 0x80081014

extern int MON_OnGround(struct _Instance *instance); // 0x80081058

extern void MON_ApplyPhysics(struct _Instance *instance); // 0x80081148

extern void MON_ChangeBehavior(struct _Instance *instance, int behavior); // 0x8008116C

extern void MON_CheckEnvironment(struct _Instance *instance); // 0x800811F4

extern void MON_CheckTerrainAndRespond(struct _Instance *instance, struct BSPTree *bsp, struct _TFace *tface); // 0x80081628

extern unsigned long MON_CheckTerrain(struct _Instance *instance, struct BSPTree *bsp, struct _TFace *tface); // 0x800816AC

extern int MON_CheckPointSuitability(struct _Instance *instance, _Position *origin, _Position *destination); // 0x8008176C

extern unsigned long MON_GetTime(struct _Instance *instance); // 0x80081844

extern void MON_BirthSoul(struct _Instance *instance, int link); // 0x80081898

extern void MON_ProcessIntro(struct _Instance *instance); // 0x800819FC

extern void MON_SetDefaults(struct _Instance *instance); // 0x80081F28

extern void MON_GetPlanSlot(struct _MonsterVars *mv); // 0x80082128

extern int MON_DefaultPlanMovement(struct _Instance *instance, int anim, long distance); // 0x8008216C

extern void MON_DropAllObjects(struct _Instance *instance); // 0x8008240C

extern void MON_EnableHeadMove(struct _Instance *instance); // 0x8008247C

extern void MON_DisableHeadMove(struct _Instance *instance); // 0x80082524

extern void MON_LookInDirection(struct _Instance *instance, short tx, short tz); // 0x800825B0

extern void MON_LookAtPos(struct _Instance *instance, _Position *position); // 0x80082724

extern void MON_ProcessLookAt(struct _Instance *instance); // 0x80082800

extern int MON_TakeDamage(struct _Instance *instance, int damage, int type); // 0x800828F4

extern void MON_SetUpSaveInfo(struct _Instance *instance, struct _MonsterSaveInfo *saveData); // 0x800829A0

extern void MON_GetSaveInfo(struct _Instance *instance, struct _MonsterSaveInfo *saveData); // 0x80082B0C

extern void MON_KillMonster(struct _Instance *instance); // 0x80082D50

extern int MON_ShouldIAmbushEnemy(struct _Instance *instance); // 0x80082F28

extern int MON_ShouldIFireAtTarget(struct _Instance *instance, struct _MonsterIR *target); // 0x80083094

extern int MON_ShouldIFlee(struct _Instance *instance); // 0x80083184

extern void MON_RelocateCoords(struct _Instance *instance, struct _SVector *offset); // 0x80083284

extern int MON_ValidUnit(struct _Instance *instance, unsigned long unitId); // 0x80083310

extern int MON_ValidPosition(struct _Instance *instance); // 0x8008335C

extern void MON_SphereWorldPos(MATRIX *mat, struct _HSphere *sphere, _Position *ret); // 0x800833B4

extern struct _HPrim * MON_FindSphereForTerrain(struct _Instance *instance); // 0x80083424

extern struct Intro * MON_FindNearestImpalingIntro(int unitID, _Position *position); // 0x800834B0

extern struct Intro * MON_TestForTerrainImpale(struct _Instance *instance, struct _Terrain *terrain); // 0x800835E0

extern void MON_MoveInstanceToImpalePoint(struct _Instance *instance); // 0x800836DC

extern int MON_ReachableIntro(struct _Instance *instance, _Position *pos, _Position *introPos, struct _Rotation *introRot, int checkOrientation); // 0x800837C0

extern int MON_SetVelocityTowardsImpalingObject(struct _Instance *instance, int checkOrientation); // 0x80083BAC

extern void MON_TurnOffSphereCollisions(struct _Instance *instance); // 0x80083D20

extern void MON_TurnOnSphereCollisions(struct _Instance *instance); // 0x80083D7C

extern void MON_ProcessSpecialFade(struct _Instance *instance); // 0x80083DD8

extern void MON_StartSpecialFade(struct _Instance *instance, int fadeLevel, int fadeTime); // 0x80084014

extern void MON_UnlinkFromRaziel(struct _Instance *instance); // 0x8008407C

extern void MON_BurnInAir(struct _Instance *instance, int currentState); // 0x8008411C

extern void MON_BirthMana(struct _Instance *instance); // 0x800841D8

extern void MON_SoulSucked(struct _Instance *instance); // 0x80084290

extern int MON_SetUpKnockBack(struct _Instance *instance, struct _Instance *enemy, struct evMonsterHitData *data); // 0x80084308

extern void MON_DoDrainEffects(struct _Instance *instance, struct _Instance *ei); // 0x8008440C

extern void MON_SetFXHitData(struct _Instance *instance, struct evFXHitData *data, int type, int amount); // 0x80084698

extern void MON_LaunchMonster(struct _Instance *instance, int zDirection, int power, int loft); // 0x80084724


#endif
