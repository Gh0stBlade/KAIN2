#ifndef MISSILE_H
#define MISSILE_H

struct MISSILE_114fake // hashcode: 0xA1EFD7AB (dec: -1578117205)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: missile.o, missile.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: missile.o, missile.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: missile.o, missile.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: missile.o, missile.o
};

struct MissileGraphics // hashcode: 0xD28383EB (dec: -763132949)
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
