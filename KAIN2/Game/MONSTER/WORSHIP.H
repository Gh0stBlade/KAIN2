#ifndef WORSHIP_H
#define WORSHIP_H

#include "Game/MONSTER/MONSTER.H"

extern struct _MonsterFunctionTable WORSHIP_FunctionTable;

extern void WORSHIP_Init(struct _Instance *instance); // 0x8008EE08
extern void WORSHIP_CombatEntry(struct _Instance *instance); // 0x8008EEA8

#endif
