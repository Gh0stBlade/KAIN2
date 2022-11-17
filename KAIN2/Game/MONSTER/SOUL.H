#ifndef SOUL_H
#define SOUL_H

#include "MONSTER.H"

struct SOUL_114fake // hashcode: 0x75056227 (dec: 1963287079)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: soul.o, soul.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: soul.o, soul.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: soul.o, soul.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: soul.o, soul.o
};

extern struct _MonsterFunctionTable SOUL_FunctionTable;

extern void SOUL_QueueHandler(struct _Instance *instance); // 0x8008D6D4
extern void SOUL_Physics(struct _Instance *instance, long time); // 0x8008D770
extern void SOUL_Fade(struct _Instance *instance); // 0x8008D7D0
extern void SOUL_MoveToDest(struct _Instance *instance, long maxAccel, long time); // 0x8008D884
extern void SOUL_MovePastWall(struct _Instance *instance, struct _Instance *sucker); // 0x8008D974
extern void SOUL_Init(struct _Instance *instance); // 0x8008DAE4
extern void SOUL_CleanUp(struct _Instance *instance); // 0x8008DBA8
extern void SOUL_BirthEntry(struct _Instance *instance); // 0x8008DBC8
extern void SOUL_Birth(struct _Instance *instance); // 0x8008DC1C
extern void SOUL_SoulSuckEntry(struct _Instance *instance); // 0x8008DC84
extern long SOUL_CalcAccel(long delta, long vel, long magnitude); // 0x8008DCAC
extern void SOUL_SoulSuck(struct _Instance *instance); // 0x8008DCEC
extern void SOUL_WanderEntry(struct _Instance *instance); // 0x8008E294
extern void SOUL_Wander(struct _Instance *instance); // 0x8008E304
extern void SOUL_FleeEntry(struct _Instance *instance); // 0x8008E494
extern void SOUL_Flee(struct _Instance *instance); // 0x8008E4C4
extern void SOUL_IdleEntry(struct _Instance *instance); // 0x8008E628
extern void SOUL_Idle(struct _Instance *instance); // 0x8008E6CC
extern void SOUL_ReanimateEntry(struct _Instance *instance); // 0x8008E7CC
extern void SOUL_Reanimate(struct _Instance *instance); // 0x8008E834
extern void SOUL_Effect(struct _Instance *instance, struct evFXHitData *data); // 0x8008E8EC

#endif
