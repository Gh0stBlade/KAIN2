#ifndef CONTROL_H
#define CONTROL_H

#include "Game/PLAYER.H"

struct __Force 
{
	int Type; // size=0, offset=0
	struct _Vector LinearForce; // size=12, offset=4
	short Friction; // size=0, offset=16
};

extern struct __Force ExternalForces[];

extern void SetPhysics(struct _Instance *instance, short gravity, long x, long y, long z); // 0x8009A1B0

extern void ResetPhysics(struct _Instance *instance, short gravity); // 0x8009A224

extern void SetDampingPhysics(struct _Instance *instance, int damping); // 0x8009A2E0

extern void SetImpulsePhysics(struct _Instance *instance, struct __Player *player); // 0x8009A388

extern void SetDropPhysics(struct _Instance *instance, struct __Player *player); // 0x8009A4B8

extern void InitExternalForces(struct __Force *Forces, int MaxForces); // 0x8009A4F0

extern void SetExternalForce(struct __Force *In, short x, short y, short z, int Space, int Friction); // 0x8009A530

extern void SetExternalTransitionForce(struct __Force *in, struct _Instance *instance, int time, int x, int y, int z); // 0x8009A570

extern void ProcessPhysics(struct __Player *player, struct __CharacterState *In, int CurrentSection, int Mode); // 0x8009A5D8

extern void ApplyExternalLocalForces(struct __Player *player, struct _Instance *instance, struct __Force *Forces, int MaxForces, struct _Vector *Out); // 0x8009A7D0

extern int PhysicsMode;

#endif
