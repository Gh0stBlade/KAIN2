#ifndef SLUAGH_H
#define SLUAGH_H

#include "MONSTER.H"

extern struct _MonsterFunctionTable SLUAGH_FunctionTable;
extern unsigned long SLUAGH_Query(struct _Instance *instance, unsigned long query); // 0x8008D04C
extern void SLUAGH_DamageEffect(struct _Instance *instance, struct evFXHitData *data); // 0x8008D0E0
extern void SLUAGH_Init(struct _Instance *instance); // 0x8008D2D4
extern void SLUAGH_DeathEntry(struct _Instance *instance); // 0x8008D390
extern void SLUAGH_Death(struct _Instance *instance); // 0x8008D454
extern void SLUAGH_AttackEntry(struct _Instance *instance); // 0x8008D4DC
extern void SLUAGH_Attack(struct _Instance *instance); // 0x8008D540

#endif
