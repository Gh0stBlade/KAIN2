#ifndef SLUAGH_H
#define SLUAGH_H

struct SLUAGH_114fake // hashcode: 0x17BD51C9 (dec: 398283209)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: sluagh.o, sluagh.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: sluagh.o, sluagh.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: sluagh.o, sluagh.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: sluagh.o, sluagh.o
};

extern unsigned long SLUAGH_Query(struct _Instance *instance, unsigned long query); // 0x8008D04C
extern void SLUAGH_DamageEffect(struct _Instance *instance, struct evFXHitData *data); // 0x8008D0E0
extern void SLUAGH_Init(struct _Instance *instance); // 0x8008D2D4
extern void SLUAGH_DeathEntry(struct _Instance *instance); // 0x8008D390
extern void SLUAGH_Death(struct _Instance *instance); // 0x8008D454
extern void SLUAGH_AttackEntry(struct _Instance *instance); // 0x8008D4DC
extern void SLUAGH_Attack(struct _Instance *instance); // 0x8008D540

#endif
