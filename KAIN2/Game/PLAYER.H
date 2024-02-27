#ifndef PLAYER_H
#define PLAYER_H

#include "RAZIEL/RAZLIB.H"
#include "MONSTER/MONSTER.H"

struct __FitPoint 
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
};

struct __CharacterState 
{
	struct _Instance* CharacterInstance; // size=668, offset=0
	int TotalSections; // size=0, offset=4
	struct __State SectionList[3]; // size=852, offset=8
};

struct __SensoryData 
{
	int Flags; // size=0, offset=0
	struct _SVector ForwardNormal; // size=8, offset=4
	struct _SVector AboveNormal; // size=8, offset=12
	struct _SVector BelowNormal; // size=8, offset=20
	struct _SVector BelowPoint; // size=8, offset=28
	struct _SVector Delta; // size=8, offset=36
	struct _Instance* HitMonster; // size=668, offset=44
	struct _Instance* CurrentAutoFace; // size=668, offset=48
	struct _Instance* LastAutoFace; // size=668, offset=52
	struct _Instance* Portal; // size=668, offset=56
	struct __EngagedInstance* EngagedList; // size=8, offset=60
	unsigned long EngagedMask; // size=0, offset=64
	struct _Vector lookAtPoint; // size=12, offset=68
	unsigned long heldClass; // size=0, offset=80
};

struct __AttackItem 
{
	unsigned char anim; // size=0, offset=0
	unsigned char framesIn; // size=0, offset=1
	unsigned char alphaIn; // size=0, offset=2
	unsigned char framesOut; // size=0, offset=3
	unsigned char switchDelay; // size=0, offset=4
	unsigned char ignoreDelay; // size=0, offset=5
	unsigned char segmentToGlow; // size=0, offset=6
	unsigned char startCollisionFrame; // size=0, offset=7
	unsigned char handsToCollide; // size=0, offset=8
	unsigned char endCollisionFrame; // size=0, offset=9
	unsigned char ribbonStartFrame; // size=0, offset=10
	unsigned char ribbonStartSegment; // size=0, offset=11
	unsigned char ribbonEndSegment; // size=0, offset=12
	unsigned char ribbonLifeTime; // size=0, offset=13
	unsigned char ribbonFaceLifeTime; // size=0, offset=14
	unsigned char knockBackFrames; // size=0, offset=15
	unsigned char glowFadeInFrames; // size=0, offset=16
	unsigned char glowFadeOutFrames; // size=0, offset=17
	unsigned short ribbonStartOpacity; // size=0, offset=18
	unsigned long ribbonStartColor; // size=0, offset=20
	unsigned long ribbonEndColor; // size=0, offset=24
	unsigned long glowColor; // size=0, offset=28
	unsigned short knockBackDistance; // size=0, offset=32
	unsigned short hitPowerScale; // size=0, offset=34
};

struct __ThrowItem 
{
	unsigned char prepAnim; // size=0, offset=0
	unsigned char prepAnimFramesIn; // size=0, offset=1
	unsigned char prepAnimAlphaIn; // size=0, offset=2
	unsigned char coilAnim; // size=0, offset=3
	unsigned char coilAnimFramesIn; // size=0, offset=4
	unsigned char coilAnimAlphaIn; // size=0, offset=5
	unsigned char throwAnim; // size=0, offset=6
	unsigned char throwAnimFramesInFactor; // size=0, offset=7
	unsigned char throwFrameToLaunch; // size=0, offset=8
	unsigned char framesOut; // size=0, offset=9
	short launchPointX; // size=0, offset=10
	short launchPointY; // size=0, offset=12
	short launchPointZ; // size=0, offset=14
	short maxXRotation; // size=0, offset=16
	short minXRotation; // size=0, offset=18
	short maxZRotation; // size=0, offset=20
	short minZRotation; // size=0, offset=22
	short velocity; // size=0, offset=24
	short gravity; // size=0, offset=26
	short coilRot; // size=0, offset=28
	short selfAdjustingFlag; // size=0, offset=30
};

struct __Player 
{
	int footPrint; // size=0, offset=0
	struct __CharacterState State; // size=860, offset=4
	int Mode; // size=0, offset=864
	int Anim; // size=0, offset=868
	int RotationSegment; // size=0, offset=872
	struct _SVector iVelocity; // size=8, offset=876
	int Magnitude; // size=0, offset=884
	int input; // size=0, offset=888
	int lastInput; // size=0, offset=892
	int ZDirection; // size=0, offset=896
	short Bearing; // size=0, offset=900
	short LastBearing; // size=0, offset=902
	int steeringMode; // size=0, offset=904
	short steeringVelocity; // size=0, offset=908
	short steeringAcceleration; // size=0, offset=910
	int steeringLockRotation; // size=0, offset=912
	short movementMinRate; // size=0, offset=916
	short movementMaxRate; // size=0, offset=918
	short movementMinAnalog; // size=0, offset=920
	short movementMaxAnalog; // size=0, offset=922
	struct __SensoryData Senses; // size=84, offset=924
	struct _Instance* attachedPlatform; // size=668, offset=1008
	short dropOffHeight; // size=0, offset=1012
	short fallZVelocity; // size=0, offset=1014
	short HealthScale; // size=0, offset=1016
	short HealthBalls; // size=0, offset=1018
	int HitPoints; // size=0, offset=1020
	int DamageFrequency; // size=0, offset=1024
	int invincibleTimer; // size=0, offset=1028
	int Abilities; // size=0, offset=1032
	struct _Instance* GlyphSystem; // size=668, offset=1036
	unsigned short GlyphManaBalls; // size=0, offset=1040
	unsigned short GlyphManaMax; // size=0, offset=1042
	struct __FitPoint* constrictData; // size=4, offset=1044
	short constrictIndex; // size=0, offset=1048
	short constrictWaitIndex; // size=0, offset=1050
	struct _SVector constrictCenter; // size=8, offset=1052
	short constrictFlag; // size=0, offset=1060
	short constrictGoodCircle; // size=0, offset=1062
	unsigned long constrictXTotal; // size=0, offset=1064
	unsigned long constrictYTotal; // size=0, offset=1068
	struct _Instance* soulReaver; // size=668, offset=1072
	unsigned long currentSoulReaver; // size=0, offset=1076
	int CurrentPlane; // size=0, offset=1080
	int slipSlope; // size=0, offset=1084
	unsigned long idleCount; // size=0, offset=1088
	struct _Instance* idleInstance; // size=668, offset=1092
	struct __AttackItem* attack; // size=36, offset=1096
	int attackFlags; // size=0, offset=1100
	int currentAttack; // size=0, offset=1104
	struct _Instance* attackCurrentHit; // size=668, offset=1108
	struct _Instance* attackLastHit; // size=668, offset=1112
	struct _FXGlowEffect* glowEffect; // size=44, offset=1116
	struct _Instance* attackedBy; // size=668, offset=1120
	struct __ThrowItem* throwData; // size=32, offset=1124
	int throwMode; // size=0, offset=1128
	struct _Position throwTarget; // size=6, offset=1132
	struct _Instance* throwInstance; // size=668, offset=1140
	short throwXRot; // size=0, offset=1144
	short throwZRot; // size=0, offset=1146
	int throwReturnRot; // size=0, offset=1148
	struct __MessageQueue padCommands; // size=136, offset=1152
	void (*returnState)(struct __CharacterState*, int, int); // size=0, offset=1288
	unsigned long passedMask; // size=0, offset=1292
	struct _G2SVector3_Type extraRot; // size=6, offset=1296
	long blankPad; // size=0, offset=1304
	int effectsFlags; // size=0, offset=1308
	short effectsFadeSource; // size=0, offset=1312
	short effectsFadeDest; // size=0, offset=1314
	int effectsFadeStep; // size=0, offset=1316
	int effectsFadeSteps; // size=0, offset=1320
	short autoFaceZone; // size=0, offset=1324
	short autoFaceAngle; // size=0, offset=1326
	int autoFaceLastRootAngle; // size=0, offset=1328
	int autoFaceRootAngle; // size=0, offset=1332
	int autoFaceTrueAngle; // size=0, offset=1336
	int autoFaceAnim; // size=0, offset=1340
	int autoFaceLastAnim; // size=0, offset=1344
	int nothingCounter; // size=0, offset=1348
	int timeAccumulator; // size=0, offset=1352
	struct _Position puppetMoveToPoint; // size=6, offset=1356
	struct _Position puppetRotToPoint; // size=6, offset=1362
	int soundModifier; // size=0, offset=1368
	short alarmTable; // size=0, offset=1372
	short terminator; // size=0, offset=1374
	short swimSpeedAdjustment; // size=0, offset=1376
	short swimTargetSpeed; // size=0, offset=1378
	unsigned long soundHandle; // size=0, offset=1380
	long soundTimer; // size=0, offset=1384
	long soundTotalTime; // size=0, offset=1388
	unsigned long soundDistance; // size=0, offset=1392
	short soundStartPitch; // size=0, offset=1396
	short soundEndPitch; // size=0, offset=1398
	short soundStartVolume; // size=0, offset=1400
	short soundEndVolume; // size=0, offset=1402
	unsigned long soundHandle2; // size=0, offset=1404
	long soundTimer2; // size=0, offset=1408
	long soundTotalTime2; // size=0, offset=1412
	unsigned long soundDistance2; // size=0, offset=1416
	short soundStartPitch2; // size=0, offset=1420
	short soundEndPitch2; // size=0, offset=1422
	short soundStartVolume2; // size=0, offset=1424
	short soundEndVolume2; // size=0, offset=1426
	long soundTimerNext; // size=0, offset=1428
	unsigned long soundTimerData; // size=0, offset=1432
	unsigned long forcedGlideSpeed; // size=0, offset=1436
	struct _SVector collisionEdgeOffset; // size=8, offset=1440
	struct __SAnim* currentSAnim; // size=16, offset=1448
	unsigned long playerEvent; // size=0, offset=1452
	unsigned long playerEventHistory; // size=0, offset=1456
	int currentHint; // size=0, offset=1460
};

extern struct __Player Raziel;

extern void PLAYER_TurnHead(struct _Instance *instance, short *rotx, short *rotz, struct GameTracker *gameTracker); // 0x80012D5C
extern long PLAYER_OkToLookAround(struct _Instance *instance); // 0x80012DDC
extern void PLAYER_SetLookAround(struct _Instance *instance); // 0x80012E28
extern void PLAYER_ReSetLookAround(struct _Instance *instance); // 0x80012E74

#endif
