#ifndef PHYSOBS_H
#define PHYSOBS_H

#include "COLLIDE.H"
#include "Game/PHYSICS.H"

struct PhysObSplinter 
{
    long numSplintData; // size=0, offset=0
    void* splinterData; // size=0, offset=4
};

struct PhysObWeaponAttributes 
{
    long Damage; // size=0, offset=0
    long AltDamage; // size=0, offset=4
    short knockBackDistance; // size=0, offset=8
    char knockBackFrames; // size=0, offset=10
    char dropSound; // size=0, offset=11
    char Class; // size=0, offset=12
    char ThrowSphere; // size=0, offset=13
    char LeftHandSphere; // size=0, offset=14
    char RightHandSphere; // size=0, offset=15
    struct _PhysObLight* Light; // size=0, offset=16
    struct PhysObSplinter splinter; // size=8, offset=20
};

struct LightTableEntry 
{
  short r; // size=0, offset=0
  short g; // size=0, offset=2
  short b; // size=0, offset=4
  short radius; // size=0, offset=6
};

struct SwitchData 
{
    short state; // size=0, offset=0
    short accumulator; // size=0, offset=2
};

struct PhysObProperties 
{
    unsigned short version; // size=0, offset=0
    unsigned short family; // size=0, offset=2
    unsigned short ID; // size=0, offset=4
    unsigned short Type; // size=0, offset=6
};

struct PhysObSwitchProperties 
{
    struct PhysObProperties Properties; // size=8, offset=0
    short Distance; // size=0, offset=8
    short Class; // size=0, offset=10
    unsigned char onAnim; // size=0, offset=12
    unsigned char offAnim; // size=0, offset=13
    unsigned char failedOnAnim; // size=0, offset=14
    unsigned char enableAnim; // size=0, offset=15
    unsigned char shutAnim; // size=0, offset=16
    unsigned char onWalkTimer; // size=0, offset=17
    unsigned char offWalkTimer; // size=0, offset=18
    unsigned char walkMode; // size=0, offset=19
    unsigned char razielOnAnim; // size=0, offset=20
    unsigned char razielOffAnim; // size=0, offset=21
    unsigned char razielFailedOnAnim; // size=0, offset=22
    unsigned char razielEnableAnim; // size=0, offset=23
    unsigned char startMode; // size=0, offset=24
    unsigned char startAnim; // size=0, offset=25
    unsigned short engageXYDistance; // size=0, offset=26
    char engageZMinDelta; // size=0, offset=28
    char engageZMaxDelta; // size=0, offset=29
    unsigned char engageYCone; // size=0, offset=30
    unsigned char engageZCone; // size=0, offset=31
};

struct PhysObInteractProperties 
{
    struct PhysObProperties Properties; // size=8, offset=0
    unsigned short conditions; // size=0, offset=8
    unsigned short auxConditions; // size=0, offset=10
    unsigned short action; // size=0, offset=12
    unsigned short auxAction; // size=0, offset=14
    unsigned char startAnim; // size=0, offset=16
    unsigned char endAnim; // size=0, offset=17
    unsigned char razielAnim; // size=0, offset=18
    unsigned char razielAuxAnim; // size=0, offset=19
    unsigned char frame; // size=0, offset=20
    unsigned char startAnimMode; // size=0, offset=21
    unsigned short distance; // size=0, offset=22
    unsigned short newType; // size=0, offset=24
    unsigned short newClass; // size=0, offset=26
    unsigned short mode; // size=0, offset=28
    unsigned short engageXYDistance; // size=0, offset=30
    short engageZMinDelta; // size=0, offset=32
    short engageZMaxDelta; // size=0, offset=34
    unsigned char engageYCone; // size=0, offset=36
    unsigned char engageZCone; // size=0, offset=37
    unsigned short pad; // size=0, offset=38
    struct PhysObWeaponAttributes* weapon; // size=28, offset=40
};

struct PhysObCollectibleProperties 
{
    struct PhysObProperties Properties; // size=8, offset=0
    short collectClass; // size=0, offset=8
    short collectValue; // size=0, offset=10
    unsigned long distance; // size=0, offset=12
    unsigned char idleAnim; // size=0, offset=16
    unsigned char collectAnim; // size=0, offset=17
    unsigned short spatialNode; // size=0, offset=18
};

struct PhysObData 
{
  long Mode; // size=0, offset=0
  short xForce; // size=0, offset=4
  short yForce; // size=0, offset=6
  struct _Instance *Force; // size=668, offset=8
  short LinkNode; // size=0, offset=12
  short px; // size=0, offset=14
  short py; // size=0, offset=16
  short pz; // size=0, offset=18
  short rx1; // size=0, offset=20
  short ry1; // size=0, offset=22
  short rz1; // size=0, offset=24
  short Segment1; // size=0, offset=26
  short rx2; // size=0, offset=28
  short ry2; // size=0, offset=30
  short rz2; // size=0, offset=32
  short Segment2; // size=0, offset=34
  short Steps; // size=0, offset=36
  short Step; // size=0, offset=38
  short PathNumber; // size=0, offset=40
  short Pad1; // size=0, offset=42
  struct _HInfo *RightCollision; // size=24, offset=44
  struct _HInfo *LeftCollision; // size=24, offset=48
  short burnAmpl; // size=0, offset=52
  short throwFlags; // size=0, offset=54
  short initialXRot; // size=0, offset=56
  short xRotVel; // size=0, offset=58
  short yRotVel; // size=0, offset=60
  short zRotVel; // size=0, offset=62
  struct _Instance *throwingInstance; // size=668, offset=64
  int physObTimer; // size=0, offset=68
};

struct evObjectData 
{
    short x; // size=0, offset=0
    short y; // size=0, offset=2
    short PathNumber; // size=0, offset=4
    short LinkNode; // size=0, offset=6
    struct _Instance* Force; // size=668, offset=8
    int rc; // size=0, offset=12
};

struct evObjectThrowData 
{
  short type; // size=0, offset=0
  short spinType; // size=0, offset=2
  union {
    struct _Instance *target; // size=668, offset=0
    struct _Position position; // size=6, offset=0
    struct _Rotation direction; // size=8, offset=0
    struct _Position throwVector; // size=6, offset=0
  } data; // size=8, offset=4
  struct _SVector angularVel; // size=8, offset=12
  unsigned short speed; // size=0, offset=20
  short gravity; // size=0, offset=22
  short initialXRot; // size=0, offset=24
  short zVel; // size=0, offset=26
};

struct _PhysObLight 
{
  unsigned char length; // size=0, offset=0
  unsigned char segment; // size=0, offset=1
  short speed; // size=0, offset=2
  struct LightTableEntry *lightTable; // size=8, offset=4
};

extern void PHYSOB_PlayDropSound(struct _Instance *instance); // 0x80068700

extern struct _Instance * PHYSOBS_IsAPushBlockAttached(struct _Instance *block); // 0x80068820

extern struct _Instance * PHYSOBS_IsAnythingAttached(struct _Instance *block); // 0x800688A8

extern int CheckPhysOb(struct _Instance *instance); // 0x800688F0

extern int CheckPhysObAbility(struct _Instance *instance, unsigned short ability); // 0x8006891C

extern int CheckPhysObFamily(struct _Instance *instance, unsigned short family); // 0x80068998

extern int GetPhysicalAbility(struct _Instance *instance); // 0x800689DC

extern int AnyBlocksInMotion(); // 0x80068A34

extern void SetThrowDirection(struct _Instance *instance, struct _Instance *parent, struct evObjectThrowData *throwData, struct PhysObData *Data); // 0x80068AD4

extern void ThrowPhysOb(struct _Instance *instance, struct evObjectThrowData *throwData); // 0x80068E20

extern int PushPhysOb(struct _Instance *instance, short x, short y, short PathNumber, struct _Instance *Force); // 0x80069200

extern void ResetSwitchPhysOb(struct _Instance *instance); // 0x8006935C

extern int SwitchPhysOb(struct _Instance *instance); // 0x800693F4

extern int InteractPhysOb(struct _Instance *instance, struct _Instance *Force, int LinkNode, int Action); // 0x800695D0

extern void ResetOrientation(struct _Instance *instance); // 0x80069930

extern void FinishPush(struct _Instance *instance); // 0x80069E4C

extern void PhysOb_AlignPush(struct _Instance *instance, int x, int y, int path, struct PhysObData *Data); // 0x80069E6C

extern int FlipPhysOb(struct _Instance *instance, short x, short y, struct _Instance *Force); // 0x80069FDC

extern int CanBePickedUp(struct _Instance *instance, struct _Instance *Force, int LinkNode); // 0x8006A120

extern int PickUpPhysOb(struct _Instance *instance, short Steps, struct _Instance *Force, int LinkNode); // 0x8006A2B8

extern struct _Instance * PHYSOB_BirthCollectible(struct _Instance *parent, int x, int y, int z, int type, int lifeTime); // 0x8006A3B8

extern struct evObjectBirthProjectileData * PHYSOB_BirthProjectile(struct _Instance *parent, int joint, int type); // 0x8006A468

extern struct _Instance * BirthProjectilePhysOb(struct _Instance *instance, int grabJoint, int type); // 0x8006A4C0

extern void PHYSOB_SetLightTable(struct _PhysObLight *pLight, struct LightInstance *li, short burnAmplitude); // 0x8006A694

extern void PHYSOB_EndLighting(struct _Instance *instance, struct _PhysObLight *pLight); // 0x8006A7FC

extern void PHYSOB_StartLighting(struct _Instance *instance, struct _PhysObLight *pLight); // 0x8006A820

extern void PHYSOB_StartBurnFX(struct _Instance *instance); // 0x8006A88C

extern void PHYSOB_StartBurning(struct _Instance *instance, struct _PhysObLight *pLight); // 0x8006A918

extern void PHYSOB_StopLighting(struct _Instance *instance, struct _PhysObLight *pLight); // 0x8006A954

extern void PHYSOB_StopBurning(struct _Instance *instance, struct _PhysObLight *pLight); // 0x8006A974

extern void PHYSOB_EndBurning(struct _Instance *instance, struct _PhysObLight *pLight); // 0x8006A998

extern void StopPhysOb(struct _Instance *instance); // 0x8006A9F8

extern void InitPhysicalObject(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8006AA34

extern void ProcessPhysicalObject(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8006B2B0

extern unsigned long PhysicalObjectQuery(struct _Instance *instance, unsigned long Query); // 0x8006BEE4

extern void PhysicalObjectPost(struct _Instance *instance, unsigned long Message, unsigned long Data); // 0x8006C3D4

extern long PhysobAnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, void *data); // 0x8006CCC4

extern void CheckForceCollision(struct _Instance *instance, struct _Instance *hitinst, struct _TFace *tface, struct _CollideInfo *collideInfo, _SVector *dir, int obliqueFlg); // 0x8006CD68

extern int GetObliqueDirection(struct _Instance *instance, struct _SVector *dir); // 0x8006CFD4

extern void CollidePhysicalObject(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8006D0B0

extern struct PhysObWeaponAttributes * PhysObGetWeapon(struct _Instance *instance); // 0x8006D884

extern struct _PhysObLight * PhysObGetLight(struct _Instance *instance); // 0x8006D928

extern struct PhysObSplinter * PhysObGetSplinter(struct _Instance *instance); // 0x8006D960

extern void TurnOnCollisionPhysOb(struct _Instance *instance, int coll); // 0x8006D994

extern void TurnOffCollisionPhysOb(struct _Instance *instance, int coll); // 0x8006DA38

extern int GetPhysObCollisionType(struct _Instance *instance); // 0x8006DAC0

extern void ExecuteThrow(struct _Instance *instance); // 0x8006DDFC

extern void ExecuteDrag(struct _Instance *instance); // 0x8006DF0C

extern void ExecuteSlideToStop(struct _Instance *instance); // 0x8006DFB4

extern void ExecuteFlip(struct _Instance *instance); // 0x8006E074

extern int CheckSlope(int Znormal, int Slope, int Slop); // 0x8006E164

extern int CheckBlockSlope(struct _Instance *instance, int x, int y, int hOff, int vOff, int dH, int dV); // 0x8006E184

extern int PHYSOBS_FigureDragForSlope(struct _Instance *instance, int pathNumber, int *result); // 0x8006E244

extern int PHYSOB_CheckThrownLineCollision(struct _Instance *instance, struct _Instance *parent); // 0x8006E2C4

extern int PHYSOB_CheckDroppedLineCollision(struct _Instance *instance, struct _Instance *parent); // 0x8006E7BC

extern int PHYSOB_CheckDirectedLineCollision(struct _Instance *instance, int xoffset, int yoffset, int startZOffset); // 0x8006EC78

extern long PHYSOBS_CheckForStackedForwardHits(struct _Instance *block, long xoffset, long yoffset); // 0x8006ECEC

extern void PHYSOBS_SetNewAnim(struct _Instance *instance, struct PhysObData *Data, unsigned long modeBits, int path, int resetflg); // 0x8006ED68

extern long PHYSOB_CheckForEnemyInBlkSpot(struct _Instance *instance, int dx, int dy); // 0x8006EDE0

extern long PHYSOBS_CheckForValidMove(struct _Instance *instance); // 0x8006EF14

extern void ExecuteGravitate(struct _Instance *instance); // 0x8006F460

extern void ExecuteFollow(struct _Instance *instance); // 0x8006F7D4

extern void PHYSOB_Normalize(struct _SVector *v); // 0x8006F82C

extern int PHYSOB_MoveTowardsAlign(struct _Instance *instance, struct _SVector *orgVec, struct _SVector *endVec); // 0x8006F8C8

extern int PHYSOB_ReAlignFalling(struct _Instance *instance, int zEndOff); // 0x8006FBA0

extern void DropPhysOb(struct _Instance *instance, int flags); // 0x800700C4

extern void PhysicalRelocateTune(struct Object *object, long offset); // 0x800701C0

extern int PHYSOBS_CheckObjectAxisAlignment(MATRIX *m0, MATRIX *m1, struct _SVector *axis); // 0x80070358

extern int PHYSOB_CheckSlide(struct _Instance *instance, int x, int y, struct evPhysicsSlideData **data); // 0x80070428

extern int PHYSOB_CheckSlide2(struct _Instance *instance, int x, int y, struct evPhysicsSlideData **data); // 0x800704B8

extern int PHYSOB_CheckDropOnSlope(struct _Instance *instance, int x, int y, struct evPhysicsSlideData **data); // 0x80070548

extern int PHYSOB_CheckSlideNormal(struct _Instance *instance, int x, int y, struct evPhysicsSlideData **data); // 0x800705DC

extern void PHYSOB_NormalToAxis(short *x, short *y); // 0x80070630


#endif
