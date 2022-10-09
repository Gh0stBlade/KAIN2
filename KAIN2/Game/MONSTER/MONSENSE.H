#ifndef MONSENSE_H
#define MONSENSE_H

#include "Game/INSTANCE.H"
#include "MONSTER.H"

extern struct _MonsterIR * MONSENSE_FindIR(struct _MonsterVars *mv, struct _Instance *instance); // 0x80085AA4

extern int MONSENSE_See(struct _Instance *instance, struct evCollideInstanceStatsData *data); // 0x80085ADC

extern int MONSENSE_Hear(struct _Instance *instance, struct evCollideInstanceStatsData *data); // 0x80085C78

extern int MONSENSE_Smell(struct _Instance *instance, struct evCollideInstanceStatsData *data); // 0x80085D08

extern struct _MonsterIR * MONSENSE_FirstSense(struct _Instance *instance, struct _Instance *sensed); // 0x80085D30

extern void MONSENSE_SetupMIR(struct _Instance *instance, struct evCollideInstanceStatsData *data, int flags); // 0x80085ED4

extern void MONSENSE_SenseInstance(struct _Instance *instance, struct evCollideInstanceStatsData *data); // 0x80086300

extern void MONSENSE_StartMonsterIRList(struct _Instance *instance); // 0x800864F4

extern struct _MonsterIR * MONSENSE_GetMonsterIR(struct _MonsterVars *mv); // 0x80086608

extern void MONSENSE_InitIRList(struct _MonsterVars *mv, struct _MonsterIR *list, int num); // 0x80086630

extern struct _MonsterIR * MONSENSE_SetEnemy(struct _Instance *instance, struct _Instance *newenemy); // 0x8008665C

extern void MONSENSE_ProcessIRList(struct _Instance *instance); // 0x8008677C

extern void MONSENSE_SetupSenses(struct _Instance *instance); // 0x8008699C

extern void MONSENSE_RemoveSenses(struct _Instance *instance); // 0x80086B08

extern void MONSENSE_Radar(struct _Instance *instance); // 0x80086BA8

extern short MONSENSE_GetClosestFreeDirection(struct _Instance *instance, short angle, long range); // 0x80086DE4

extern int MONSENSE_GetDistanceInDirection(struct _Instance *instance, short angle); // 0x80086EE8

extern void MONSENSE_DoSenses(struct _Instance *instance); // 0x80086F20

extern void MONSENSE_AdjustRadarFromObjects(struct _Instance *instance); // 0x80087018


#endif
