#ifndef HUMAN_H
#define HUMAN_H

struct HUMAN_114fake // hashcode: 0xC7C43F22 (dec: -943440094)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: human.o, human.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: human.o, human.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: human.o, human.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: human.o, human.o
};

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void HUMAN_WaitForWeapon(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8007CB40
extern struct _Instance * HUMAN_CreateWeapon(struct _Instance *instance, int weaponid, int segment); // 0x8007CBB0
extern void HUMAN_Init(struct _Instance *instance); // 0x8007CC84
extern void HUMAN_CleanUp(struct _Instance *instance); // 0x8007CD7C
extern unsigned long HUMAN_Query(struct _Instance *instance, unsigned long query); // 0x8007CD9C
extern void HUMAN_DeadEntry(struct _Instance *instance); // 0x8007CE8C
extern void HUMAN_Dead(struct _Instance *instance); // 0x8007CF04
extern void HUMAN_StunnedEntry(struct _Instance *instance); // 0x8007CFEC
extern void HUMAN_Stunned(struct _Instance *instance); // 0x8007D064
extern void HUMAN_EmbraceEntry(struct _Instance *instance); // 0x8007D10C
extern void HUMAN_Embrace(struct _Instance *instance); // 0x8007D168
extern void HUMAN_IdleEntry(struct _Instance *instance); // 0x8007D330
extern void HUMAN_Idle(struct _Instance *instance); // 0x8007D378
extern void HUMAN_Flee(struct _Instance *instance); // 0x8007D530
extern void HUMAN_GetAngry(); // 0x8007D688
extern int HUMAN_TypeOfHuman(struct _Instance *instance); // 0x8007D750

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
