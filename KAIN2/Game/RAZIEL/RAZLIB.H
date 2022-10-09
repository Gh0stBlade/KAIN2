#ifndef RAZLIB_H
#define RAZLIB_H

#include "Game/CORE.H"
#include "Game/INSTANCE.H"
#include "Game/MONSTER/MONSTER.H"
#include "Game/PLAYER.H"

struct evPhysicsSwimData // hashcode: 0x947F2BE5 (dec: -1803605019)
{
	short CheckDepth; // size=0, offset=0
	short SwimDepth; // size=0, offset=2
	short WadeDepth; // size=0, offset=4
	short TreadDepth; // size=0, offset=6
	struct _SVector *iVelocity; // size=8, offset=8
	short Depth; // size=0, offset=12
	short WaterDepth; // size=0, offset=14
	short rc; // size=0, offset=16
	short pad; // size=0, offset=18
	int WaterLevel; // size=0, offset=20
};

struct evPhysicsWallCrawlData // hashcode: 0x27DFBF06 (dec: 668974854)
{
	short Segment; // size=0, offset=0
	short Length; // size=0, offset=2
	short ForwardOffset; // size=0, offset=4
	short NormalDistance; // size=0, offset=6
	struct _SVector DropNormal; // size=8, offset=8
	struct _SVector ForwardNormal; // size=8, offset=16
	_Position NewPosition; // size=6, offset=24
	struct _Rotation DropRotation; // size=8, offset=30
	short ForwardXRotation; // size=0, offset=38
	int rc; // size=0, offset=40
};

struct evActionLookAroundData // hashcode: 0x27BD6BB6 (dec: 666725302)
{
	short *rotx; // size=0, offset=0
	short *rotz; // size=0, offset=4
	short minx; // size=0, offset=8
	short maxx; // size=0, offset=10
	short minz; // size=0, offset=12
	short maxz; // size=0, offset=14
};
struct __VAnim // hashcode: 0xDC662191 (dec: -597286511)
{
	unsigned char anim0; // size=0, offset=0
	unsigned char anim1; // size=0, offset=1
	unsigned char anim2; // size=0, offset=2
	unsigned char frames; // size=0, offset=3
	unsigned char mode; // size=0, offset=4
	unsigned char alpha; // size=0, offset=5
	unsigned short frame; // size=0, offset=6
};

struct _SoundRamp // hashcode: 0xB5503009 (dec: -1253036023)
{
	unsigned long soundHandle; // size=0, offset=0
	long soundTimer; // size=0, offset=4
	long soundTotalTime; // size=0, offset=8
	unsigned long soundDistance; // size=0, offset=12
	short soundStartPitch; // size=0, offset=16
	short soundEndPitch; // size=0, offset=18
	short soundStartVolume; // size=0, offset=20
	short soundEndVolume; // size=0, offset=22
};

struct __razController 
{
	unsigned short segment; // size=0, offset=0
	unsigned short type; // size=0, offset=2
};

struct __EngagedInstance 
{
	struct _Instance* instance; // size=668, offset=0
	unsigned long distance; // size=0, offset=4
};

extern struct __Player Raziel;

extern void razAlignYMoveRot(struct _Instance *dest, short distance, _Position *position, struct _Rotation *rotation, int extraZ); // 0x800A51B0

extern void razAlignYRotMove(struct _Instance *dest, short distance, _Position *position, struct _Rotation *rotation, int extraZ); // 0x800A5274

extern void razAlignYRotMoveInterp(struct _Instance *source, struct _Instance *dest, short distance, unsigned char segNumber, int Frames, int extraZ); // 0x800A534C

extern void razAlignYRotInterp(struct _Instance *source, _Position *dest, unsigned char segNumber, int Frames); // 0x800A5440

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
