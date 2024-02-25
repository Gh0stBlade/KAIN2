#ifndef MBMISS_H
#define MBMISS_H

struct _walbossAttributes // hashcode: 0x300B2C35 (dec: 806038581)
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
	struct {
		short plusDelta; // size=0, offset=0
		short minusDelta; // size=0, offset=2
		short validAtHitPoint; // size=0, offset=4
	} attackDeltas; // size=0, offset=28, found in object files: mbmiss.o, mbmiss.o
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
