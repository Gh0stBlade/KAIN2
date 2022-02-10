#ifndef MONSTER_H
#define MONSTER_H


extern void MON_DoCombatTimers(struct _Instance *instance); // 0x800871FC

extern void MON_ChangeHumanOpinion(struct _Instance *instance); // 0x800873E4

extern void MON_CutOut_Monster(struct _Instance *instance, int fade_amount, int startseg, int endseg); // 0x8008748C

extern void MON_DeadEntry(struct _Instance *instance); // 0x800876E0

extern void MON_Dead(struct _Instance *instance); // 0x800877D0

extern void MON_MissileHitEntry(struct _Instance *instance); // 0x80087F04

extern void MON_MissileHit(struct _Instance *instance); // 0x80087FD0

extern void MON_BirthEntry(struct _Instance *instance); // 0x800880B0

extern void MON_Birth(struct _Instance *instance); // 0x800880B8

extern void MON_ParryEntry(struct _Instance *instance); // 0x800880D8

extern void MON_Parry(struct _Instance *instance); // 0x80088128

extern void MON_LandOnFeetEntry(struct _Instance *instance); // 0x8008816C

extern void MON_LandOnFeet(struct _Instance *instance); // 0x80088230

extern void MON_LandInWaterEntry(struct _Instance *instance); // 0x800883BC

extern void MON_LandInWater(struct _Instance *instance); // 0x800883D0

extern void MON_BreakHoldEntry(struct _Instance *instance); // 0x800883F0

extern void MON_BreakHold(struct _Instance *instance); // 0x8008845C

extern void MON_ImpactEntry(struct _Instance *instance); // 0x80088638

extern void MON_Impact(struct _Instance *instance); // 0x8008871C

extern void MON_FallEntry(struct _Instance *instance); // 0x8008879C

extern void MON_Fall(struct _Instance *instance); // 0x8008885C

extern void MON_ThrownEntry(struct _Instance *instance); // 0x800889D8

extern void MON_Thrown(struct _Instance *instance); // 0x80088A74

extern void MON_ImpaleDeathEntry(struct _Instance *instance); // 0x80088BCC

extern void MON_ImpaleDeath(struct _Instance *instance); // 0x80088C4C

extern void MON_TerrainImpaleDeathEntry(struct _Instance *instance); // 0x80088E54

extern void MON_TerrainImpaleDeath(struct _Instance *instance); // 0x80088FA8

extern void MON_SurprisedEntry(struct _Instance *instance); // 0x80089190

extern void MON_Surprised(struct _Instance *instance); // 0x800891F0

extern void MON_StunnedEntry(struct _Instance *instance); // 0x800892B4

extern void MON_Stunned(struct _Instance *instance); // 0x80089464

extern void MON_GrabbedEntry(struct _Instance *instance); // 0x80089574

extern void MON_Grabbed(struct _Instance *instance); // 0x80089658

extern void MON_HitEntry(struct _Instance *instance); // 0x80089870

extern void MON_Hit(struct _Instance *instance); // 0x80089914

extern void MON_AttackEntry(struct _Instance *instance); // 0x800899FC

extern void MON_Attack(struct _Instance *instance); // 0x80089A70

extern void MON_CombatEntry(struct _Instance *instance); // 0x80089C50

extern void MON_Combat(struct _Instance *instance); // 0x80089C7C

extern void MON_ProjectileEntry(struct _Instance *instance); // 0x80089F64

extern void MON_Projectile(struct _Instance *instance); // 0x80089FBC

extern void MON_IdleEntry(struct _Instance *instance); // 0x8008A100

extern void MON_Idle(struct _Instance *instance); // 0x8008A1B8

extern void MON_FleeEntry(struct _Instance *instance); // 0x8008A3F4

extern void MON_Flee(struct _Instance *instance); // 0x8008A448

extern void MON_PursueEntry(struct _Instance *instance); // 0x8008A58C

extern void MON_Pursue(struct _Instance *instance); // 0x8008A61C

extern void MON_WanderEntry(struct _Instance *instance); // 0x8008AA54

extern void MON_Wander(struct _Instance *instance); // 0x8008AAFC

extern void MON_HideEntry(struct _Instance *instance); // 0x8008AD74

extern void MON_Hide(struct _Instance *instance); // 0x8008AD94

extern void MON_SurpriseAttackEntry(struct _Instance *instance); // 0x8008AF10

extern void MON_SurpriseAttack(struct _Instance *instance); // 0x8008AFA4

extern void MON_EnvironmentDamageEntry(struct _Instance *instance); // 0x8008AFC4

extern void MON_EnvironmentDamage(struct _Instance *instance); // 0x8008AFCC

extern void MON_MonsterGlow(struct _Instance *instance, long color, int glowtime, int glowin, int glowfade); // 0x8008AFD4

extern void MON_GeneralDeathEntry(struct _Instance *instance); // 0x8008B160

extern void MON_GeneralDeath(struct _Instance *instance); // 0x8008B3AC

extern void MON_NoticeEntry(struct _Instance *instance); // 0x8008B524

extern void MON_Notice(struct _Instance *instance); // 0x8008B5DC

extern void MON_PupateEntry(struct _Instance *instance); // 0x8008B67C

extern void MON_Pupate(struct _Instance *instance); // 0x8008B6DC

extern void MON_EmbraceEntry(struct _Instance *instance); // 0x8008BA1C

extern void MON_Embrace(struct _Instance *instance); // 0x8008BA7C

extern void MON_PetrifiedEntry(struct _Instance *instance); // 0x8008BBFC

extern void MON_Petrified(struct _Instance *instance); // 0x8008BC38

extern int MONSTER_CalcDamageIntensity(int hp, int maxHp); // 0x8008BD44

extern void MONSTER_ProcessClosestVerts(struct _Instance *instance, struct _SVector *location, TDRFuncPtr_MONSTER_ProcessClosestVerts2processVert_cb processVert_cb, void *cb_data); // 0x8008BD5C

extern void ProcessBloodyMess(struct _Instance *instance, int vertidx, int segidx, int dist, void *cb_data); // 0x8008BF24

extern void MONSTER_InitVertexColors(struct _Instance *instance, struct _Model *model); // 0x8008C030

extern int MONSTER_StartVertexBlood(struct _Instance *instance, struct _SVector *location, int amount); // 0x8008C098

extern void MONSTER_VertexBlood(struct _Instance *instance, int vert, int amount); // 0x8008C220

extern void ProcessBurntMess(struct _Instance *instance, int vertidx, int segidx, int dist, void *cb_data); // 0x8008C3EC

extern int MONSTER_StartVertexBurnt(struct _Instance *instance, struct _SVector *location, struct burntTuneType *burntTune); // 0x8008C498

extern void MONSTER_VertexBurnt(struct _Instance *instance, struct burntTuneType *burntTune); // 0x8008C618

extern void MON_DamageEffect(struct _Instance *instance, struct evFXHitData *data); // 0x8008C6C4

extern void MON_DefaultInit(struct _Instance *instance); // 0x8008CB28

extern void MON_CleanUp(struct _Instance *instance); // 0x8008CD74


typedef void (*TDRFuncPtr_MONSTER_ProcessClosestVerts2processVert_cb)();




#endif
