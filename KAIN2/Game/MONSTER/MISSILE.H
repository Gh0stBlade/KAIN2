#ifndef MISSILE_H
#define MISSILE_H

#include "MONSTER.H"

struct MissileGraphics 
{
	unsigned char object; // size=0, offset=0
	unsigned char model; // size=0, offset=1
};

extern void MISSILE_Process(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007D7BC
extern void MISSILE_Collide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007D814
extern struct _Instance * MISSILE_Find(struct _Instance *instance, struct _MonsterMissile *missiledef); // 0x8007D850
extern struct _Instance * MISSILE_Birth(struct _Instance *instance, struct _MonsterMissile *missiledef); // 0x8007D8CC
extern struct _Instance * MISSILE_Fire(struct _Instance *instance, struct _MonsterMissile *missiledef, void *target, int type); // 0x8007D9A0
extern struct _Instance * MISSILE_FireAtInstance(struct _Instance *instance, struct _MonsterMissile *missiledef, struct _Instance *target); // 0x8007DA60

#endif
