#ifndef VWRAITH_H
#define VWRAITH_H

struct VWRAITH_114fake // hashcode: 0x99946622 (dec: -1718327774)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: vwraith.o, vwraith.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: vwraith.o, vwraith.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: vwraith.o, vwraith.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: vwraith.o, vwraith.o
};

extern void VWRAITH_MoveVertical(struct _Instance *instance, long targetZ, int velocity); // 0x8008E8F4
extern void VWRAITH_Init(struct _Instance *instance); // 0x8008E94C
extern int VWRAITH_ShouldISwoop(struct _Instance *instance); // 0x8008EA00
extern void VWRAITH_PursueEntry(struct _Instance *instance); // 0x8008EA84
extern void VWRAITH_Pursue(struct _Instance *instance); // 0x8008EB1C
extern void VWRAITH_VerticalMove(struct _Instance *instance); // 0x8008ECEC
extern void VWRAITH_CombatEntry(struct _Instance *instance); // 0x8008ED70
extern void VWRAITH_Combat(struct _Instance *instance); // 0x8008ED90
extern void VWRAITH_EmbraceEntry(struct _Instance *instance); // 0x8008EDBC
extern void VWRAITH_Embrace(struct _Instance *instance); // 0x8008EDDC

#endif
