#ifndef RAZLIB_H
#define RAZLIB_H

#include "CORE.H"
#include "INSTANCE.H"
#include "MONSTER/MONSTER.H"

struct __EngagedInstance 
{
	struct _Instance* instance; // size=668, offset=0
	unsigned long distance; // size=0, offset=4
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

struct __CharacterState 
{
	struct _Instance* CharacterInstance; // size=668, offset=0
	int TotalSections; // size=0, offset=4
	struct __State SectionList[3]; // size=852, offset=8
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
	void* returnState(); // size=0, offset=1288
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


extern void razAlignYMoveRot(struct _Instance *dest, short distance, struct _Position *position, struct _Rotation *rotation, int extraZ); // 0x800A51B0

extern void razAlignYRotMove(struct _Instance *dest, short distance, struct _Position *position, struct _Rotation *rotation, int extraZ); // 0x800A5274

extern void razAlignYRotMoveInterp(struct _Instance *source, struct _Instance *dest, short distance, unsigned char segNumber, int Frames, int extraZ); // 0x800A534C

extern void razAlignYRotInterp(struct _Instance *source, struct _Position *dest, unsigned char segNumber, int Frames); // 0x800A5440

extern int razConstrictAngle(struct _Instance *instance); // 0x800A546C

extern void razRotateUpperBody(struct _Instance *instance, struct evActionLookAroundData *data); // 0x800A5580

extern void razSetFadeEffect(short source, short dest, int steps); // 0x800A5750

extern int razPlaneShift(struct _Instance *instance); // 0x800A5774

extern void razSpectralShift(); // 0x800A57D0

extern void razMaterialShift(); // 0x800A5908

extern int RAZIEL_OkToShift(); // 0x800A59D0

extern int razPickupAndGrab(struct __CharacterState *In, int CurrentSection); // 0x800A5A44

extern int razZeroAxis(long *x, long *y, int radius); // 0x800A5C54

extern int razAdjustSpeed(struct _Instance *instance, int minSpeed); // 0x800A5CB4

extern void razLaunchForce(struct _Instance *instance); // 0x800A5D50

extern struct _Instance * razGetHeldItem(); // 0x800A5DE0

extern struct _Instance * razGetHeldWeapon(); // 0x800A5E18

extern void razReaverBladeOff(); // 0x800A5E78

extern void razReaverBladeOn(); // 0x800A5EA8

extern int razReaverOff(); // 0x800A5EE8

extern int razReaverOn(); // 0x800A5F70

extern void razReaverPickup(struct _Instance *instance, struct _Instance *soulReaver); // 0x800A6054

extern void razReaverImbue(int reaverType); // 0x800A6110

extern int razGetReaverFromMask(int reaverMask); // 0x800A6168

extern void razReaverScale(int scale); // 0x800A618C

extern void razGetForwardNormal(struct _Instance *inst, struct _Instance *target); // 0x800A61F4

extern void razGetRotFromNormal(struct _SVector *normal, struct _Rotation *rot); // 0x800A62D8

extern void razCenterWithBlock(struct _Instance *inst, struct _Instance *target, int dist); // 0x800A6324

extern void razSetPauseTranslation(struct _Instance *instance); // 0x800A64F4

extern void razResetPauseTranslation(struct _Instance *instance); // 0x800A6568

extern void razSelectMotionAnim(struct __CharacterState *In, int CurrentSection, int Frames, int *Anim); // 0x800A65C0

extern int razApplyMotion(struct __CharacterState *In, int CurrentSection); // 0x800A6C30

extern void razResetMotion(struct _Instance *instance); // 0x800A6DC0

extern void razSetDampingPhysics(struct _Instance *instance); // 0x800A6E14

extern void razEnterWater(struct __CharacterState *In, int CurrentSection, struct evPhysicsSwimData *SwimData); // 0x800A6E4C

extern void razSetSwimVelocity(struct _Instance *instance, int vel, int accl); // 0x800A70F0

extern void razSetWallCrawlNodes(struct _Instance *instance, struct evPhysicsWallCrawlData *data); // 0x800A7144

extern int razSwitchVAnimCharacterGroup(struct _Instance *instance, int animGroup, int *frame, int *frames); // 0x800A7230

extern int razSwitchVAnimGroup(struct _Instance *instance, int section, int animGroup, int frame, int frames); // 0x800A72EC

extern void razSwitchVAnimCharacterSingle(struct _Instance *instance, int anim, int *frame, int *frames); // 0x800A7348

extern void razSwitchVAnimSingle(struct _Instance *instance, int section, int anim, int frame, int frames); // 0x800A7400

extern void razSwitchVAnim(struct _Instance *instance, int section, struct __VAnim *vAnim, int frame, int frames); // 0x800A7434

extern int razProcessSAnim(struct _Instance *instance, int mode); // 0x800A7500

extern void razSwitchStringAnimation(struct _Instance *instance, int anim); // 0x800A7678

extern int CheckStringAnimation(struct _Instance *instance, int mode); // 0x800A7740

extern void razLaunchBubbles(int segments, int count, int type); // 0x800A77A0

extern void razSetCowlNoDraw(int mode); // 0x800A7838

extern void razAttachControllers(); // 0x800A78E8

extern void razSetPlayerEvent(); // 0x800A7950

extern void razClearPlayerEvent(); // 0x800A7C5C

extern void razSetPlayerEventHistory(unsigned long event); // 0x800A7C94

extern int razSideMoveSpiderCheck(struct _Instance *instance, int x); // 0x800A7CAC

extern struct _Instance * RAZIEL_QueryEngagedInstance(int index); // 0x800A7D78

extern int razUpdateSoundRamp(struct _Instance *instance, struct _SoundRamp *sound); // 0x800A7DB0

extern void razSetupSoundRamp(struct _Instance *instance, struct _SoundRamp *sound, int sfx, int startPitch, int endPitch, int startVolume, int endVolume, int timer, int distance); // 0x800A7EB4

extern void RAZIEL_SetInteractiveMusic(int modifier, int action); // 0x800A7F58

extern void RAZIEL_DebugHurtRaziel(); // 0x800A7FD4

extern void RAZIEL_StartNewGame(); // 0x800A7FF4

extern int razInBaseArea(char *name, int length); // 0x800A801C

extern void razPrepGlyph(); // 0x800A8078


#endif
