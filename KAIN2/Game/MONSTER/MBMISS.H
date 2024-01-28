#ifndef MBMISS_H
#define MBMISS_H

struct MBMISS_114fake // hashcode: 0x8D708B2D (dec: -1922004179)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: mbmiss.o, mbmiss.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: mbmiss.o, mbmiss.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: mbmiss.o, mbmiss.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: mbmiss.o, mbmiss.o
};

struct _walbossAttributes 
{
	unsigned char segmentToAttach; // size=0, offset=0
	unsigned char frameToBirth; // size=0, offset=1
	unsigned char frameToExpel; // size=0, offset=2
	unsigned char maxEggHits; // size=0, offset=3
	short maxAngle; // size=0, offset=4
	short timeForHitAgony; // size=0, offset=6
	short segmentForBodyTwist; // size=0, offset=8
	short timeToThornPrep; // size=0, offset=10
	short timeToThornPoke; // size=0, offset=12
	short timeToEggThrob; // size=0, offset=14
	short timeToEggExplode; // size=0, offset=16
	short razielStunTime; // size=0, offset=18
	short eggIgniteDist; // size=0, offset=20
	short angleForShortAttack; // size=0, offset=22
	short closeAttackStart; // size=0, offset=24
	short junk; // size=0, offset=26
	struct
	{
		short plusDelta; // size=0, offset=0
	    short minusDelta; // size=0, offset=2
	    short validAtHitPoint; // size=0, offset=4
	}
	attackDeltas[0]; // size=0, offset=28
};

extern void WCBEGG_Message(struct _Instance *instance, unsigned long message, unsigned long data); // 0x8008EFA8
extern int WCBEGG_ShouldIgniteEgg(struct _Instance *egg, struct _walbossAttributes *wa); // 0x8008F04C
extern void WCBEGG_Process(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F144
extern void WCBEGG_ExplodeProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F338
extern void WCBEGG_SplitProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F478
extern void WCBEGG_CommonPostProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F574
extern void WCBEGG_CommonPostProcess2(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F604
extern void WCBEGG_ExplodeCollide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F678
extern void WCBEGG_Collide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F7FC
extern long WALBOSC_AnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, struct _Instance *instance); // 0x8008F884
extern void WALBOSC_Collide(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8008F904
extern void WALBOSC_Message(struct _Instance *instance, unsigned long message, unsigned long data); // 0x8008F9C4

#endif
