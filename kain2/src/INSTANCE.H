#ifndef INSTANCE_H
#define INSTANCE_H

#include "CORE.H"
#include "SPLINE.H"

typedef struct _Instance _Instance;

struct _HFace 
{
	short v0; // size=0, offset=0
	short v1; // size=0, offset=2
	short v2; // size=0, offset=4
	unsigned char attr; // size=0, offset=6
	char pad; // size=0, offset=7
	unsigned short normal; // size=0, offset=8
	unsigned short n0; // size=0, offset=10
};

struct _HSphere 
{
	long attr; // size=0, offset=0
	unsigned char id; // size=0, offset=4
	unsigned char rank; // size=0, offset=5
	short pad; // size=0, offset=6
	struct _Position position; // size=6, offset=8
	unsigned short radius; // size=0, offset=14
	unsigned long radiusSquared; // size=0, offset=16
};

struct _HBox 
{
	short flags; // size=0, offset=0
	unsigned char id; // size=0, offset=2
	unsigned char rank; // size=0, offset=3
	short minX; // size=0, offset=4
	short minY; // size=0, offset=6
	short minZ; // size=0, offset=8
	short maxX; // size=0, offset=10
	short maxY; // size=0, offset=12
	short maxZ; // size=0, offset=14
	short refMinX; // size=0, offset=16
	short refMinY; // size=0, offset=18
	short refMinZ; // size=0, offset=20
	short refMaxX; // size=0, offset=22
	short refMaxY; // size=0, offset=24
	short refMaxZ; // size=0, offset=26
};

struct _HInfo
{
	long numHFaces; // size=0, offset=0
	struct _HFace* hfaceList; // size=12, offset=4
	long numHSpheres; // size=0, offset=8
	struct _HSphere* hsphereList; // size=20, offset=12
	long numHBoxes; // size=0, offset=16
	struct _HBox* hboxList; // size=28, offset=20
};

struct _Segment 
{
	long flags; // size=0, offset=0
	short firstTri; // size=0, offset=4
	short lastTri; // size=0, offset=6
	short firstVertex; // size=0, offset=8
	short lastVertex; // size=0, offset=10
	short px; // size=0, offset=12
	short py; // size=0, offset=14
	short pz; // size=0, offset=16
	short parent; // size=0, offset=18
	struct _HInfo* hInfo; // size=24, offset=20
};

struct DrMoveAniTex 
{
	long numAniTextues; // size=0, offset=0
	struct DrMoveAniTexDestInfo* aniTexInfo; // size=40, offset=4
};

struct TextureMT3 
{
	unsigned char u0; // size=0, offset=0
	unsigned char v0; // size=0, offset=1
	unsigned short clut; // size=0, offset=2
	unsigned char u1; // size=0, offset=4
	unsigned char v1; // size=0, offset=5
	unsigned short tpage; // size=0, offset=6
	unsigned char u2; // size=0, offset=8
	unsigned char v2; // size=0, offset=9
	char pad1; // size=0, offset=10
	char sortPush; // size=0, offset=11
	long color; // size=0, offset=12
};

struct _Model 
{
	long numVertices; // size=0, offset=0
	struct _MVertex* vertexList; // size=8, offset=4
	long numNormals; // size=0, offset=8
	struct _SVectorNoPad* normalList; // size=6, offset=12
	long numFaces; // size=0, offset=16
	struct _MFace* faceList; // size=12, offset=20
	long numSegments; // size=0, offset=24
	struct _Segment* segmentList; // size=24, offset=28
	struct AniTex* aniTextures; // size=16, offset=32
	short maxRad; // size=0, offset=36
	short pad; // size=0, offset=38
	long maxRadSq; // size=0, offset=40
	struct MultiSpline* multiSpline; // size=0, offset=44
	struct TextureMT3* startTextures; // size=16, offset=48
	struct TextureMT3* endTextures; // size=16, offset=52
};

struct _HPrim 
{
	unsigned char hpFlags; // size=0, offset=0
	unsigned char withFlags; // size=0, offset=1
	unsigned char type; // size=0, offset=2
	unsigned char segment; // size=0, offset=3
	union {
		struct _HSphere* hsphere; // size=20, offset=0
		struct _HBox* hbox; // size=28, offset=0
		struct _HFace* hface; // size=12, offset=0
	} data; // size=4, offset=4
};

struct _HModel 
{
	struct _HPrim* hPrimList; // size=8, offset=0
	long numHPrims; // size=0, offset=4
};

struct LightInstance // hashcode: 0xB8D5E61F (dec: -1193941473)
{
	struct _Instance* lightInstance; // size=0, offset=0
	long r; // size=0, offset=4
	long g; // size=0, offset=8
	long b; // size=0, offset=12
	short radius; // size=0, offset=16
	unsigned char segment; // size=0, offset=18
	unsigned char flags; // size=0, offset=19
};

struct SoundInstance // hashcode: 0xFF235679 (dec: -14461319)
{
	unsigned char channel; // size=0, offset=0
	unsigned char state; // size=0, offset=1
	unsigned char delay; // size=0, offset=2
};

struct EventAliasCommandStruct // hashcode: 0xB6EF2AF6 (dec: -1225839882)
{
	struct _Instance* hostInstance; // size=0, offset=0
	short newanim; // size=0, offset=4
	short newframe; // size=0, offset=6
	short interpframes; // size=0, offset=8
	short speed; // size=0, offset=10
};

struct INICommand // hashcode: 0x04B02DE1 (dec: 78654945)
{
	short command; // size=0, offset=0
	short numParameters; // size=0, offset=2
	long parameter[1]; // size=0, offset=4
};

struct _Instance // hashcode: 0x699A5B3D (dec: 1771723581)
{
	struct NodeType node; // size=8, offset=0
	struct _Instance* next; // size=616, offset=8
	struct _Instance* prev; // size=616, offset=12
	long instanceID; // size=0, offset=16
	long flags; // size=0, offset=20
	long flags2; // size=0, offset=24
	struct Object* object; // size=76, offset=28
	struct Intro* intro; // size=0, offset=32
	void* data; // size=0, offset=36 _MonsterAttributes*
	void* introData; // size=0, offset=40
	struct _Instance* parent; // size=616, offset=44
	long introNum; // size=0, offset=48
	long birthStreamUnitID; // size=0, offset=52
	long currentStreamUnitID; // size=0, offset=56
	long introUniqueID; // size=0, offset=60
	MATRIX* matrix; // size=32, offset=64
	MATRIX* oldMatrix; // size=32, offset=68
	struct SoundInstance soundInstanceTbl[4]; // size=12, offset=72
	unsigned long maxCheckDistance; // size=0, offset=84
	unsigned long checkMask; // size=0, offset=88
	_Position position; // size=6, offset=92
	unsigned char lightGroup; // size=0, offset=98
	_Position oldPos; // size=6, offset=100
	unsigned char spectralLightGroup; // size=0, offset=106
	short currentTextureAnimFrame; // size=0, offset=108
	_Position initialPos; // size=6, offset=110
	_Rotation rotation; // size=8, offset=116
	_Rotation oldRotation; // size=8, offset=124
	_SVector scale; // size=8, offset=132
	long attachedID; // size=0, offset=140
	long attachedSegment; // size=0, offset=144
#if !defined(PSX_VERSION)
	MATRIX relativeMatrix; // size=32, offset=148//Disabled by Gh0stBlade why is this saying incomplete type?
#endif
	struct _TFace* tface; // size=12, offset=180
	struct _TFace* oldTFace; // size=12, offset=184
	void* tfaceLevel; // size=0, offset=188
	void* collideInfo; // size=0, offset=192
	long bspTree; // size=0, offset=196
	short cachedTFace; // size=0, offset=200
	short cachedBSPTree; // size=0, offset=202
	void* cachedTFaceLevel; // size=0, offset=204
	struct _Normal oldWNormal; // size=8, offset=208
	struct _Normal wNormal; // size=8, offset=216
	_SVector offset; // size=8, offset=224
	struct _HModel* hModelList; // size=8, offset=232
	short fadeValue; // size=0, offset=236
	short lightMatrix; // size=0, offset=238
	void (*processFunc)(); // size=0, offset=240
	void (*collideFunc)(struct _Instance* instance, struct GameTracker* gameTracker); // size=0, offset=244
	void (*additionalCollideFunc)(); // size=0, offset=248
	unsigned long (*queryFunc)(); // size=0, offset=252
	void (*messageFunc)(); // size=0, offset=256
	_Position shadowPosition; // size=6, offset=260
	short currentModel; // size=0, offset=266
	struct _TFace* waterFace; // size=12, offset=268
	struct _TFace* oldWaterFace; // size=12, offset=272
	struct _Terrain* waterFaceTerrain; // size=0, offset=276
	struct _PlaneConstants halvePlane; // size=12, offset=280
	_SVector splitPoint; // size=8, offset=292
	struct _Instance* LinkParent; // size=616, offset=300
	struct _Instance* LinkChild; // size=616, offset=304
	struct _Instance* LinkSibling; // size=616, offset=308
	long ParentLinkNode; // size=0, offset=312
	void* InstanceState; // size=0, offset=316
	char introName[12]; // size=12, offset=320
	void* extraData; // size=0, offset=332 _MonsterVars*
	short constrictAngle; // size=0, offset=336
	short petrifyValue; // size=0, offset=338
	long xVel; // size=0, offset=340
	long yVel; // size=0, offset=344
	long zVel; // size=0, offset=348
	long xAccl; // size=0, offset=352
	long yAccl; // size=0, offset=356
	long zAccl; // size=0, offset=360
	long maxXVel; // size=0, offset=364
	long maxYVel; // size=0, offset=368
	long maxZVel; // size=0, offset=372
	long currentMainState; // size=0, offset=376
	long work0; // size=0, offset=380
	long work1; // size=0, offset=384
	long work2; // size=0, offset=388
	long work3; // size=0, offset=392
	long work4; // size=0, offset=396
	long work5; // size=0, offset=400
	struct _G2Anim_Type anim; // size=180, offset=404
	enum _G2Bool_Enum(*rebuildCallback)(); // size=4, offset=584
	short splineFlags; // size=0, offset=588
	short targetFrame; // size=0, offset=590
	short clipBeg; // size=0, offset=592
	short clipEnd; // size=0, offset=594
	struct EventAliasCommandStruct aliasCommand; // size=12, offset=596
	long light_color; // size=0, offset=608
	CVECTOR* perVertexColor; // size=4, offset=612
};

struct evCollideInstanceStatsData // hashcode: 0xD8198216 (dec: -669416938)
{
	struct _Instance* instance; // size=616, offset=0
	unsigned long distance; // size=0, offset=4
	_SVector relativePosition; // size=8, offset=8
	long xyDistance; // size=0, offset=16
	long zDelta; // size=0, offset=20
};

struct _InstancePool // hashcode: 0xC591AB92 (dec: -980309102)
{
	long nextInstanceID; // size=0, offset=0
	long numFreeInstances; // size=0, offset=4
	struct _Instance instance[62]; // size=38192, offset=8
	struct _Instance* first_free; // size=616, offset=38200
};

struct _InstanceList // hashcode: 0x7594B1C7 (dec: 1972679111)
{
	long numInstances; // size=0, offset=0
	struct _Instance* first; // size=616, offset=4
	struct _InstancePool* pool; // size=38204, offset=8
	struct NodeType group[32]; // size=256, offset=12
};

union INSTANCE_270fake // Hashcode: 0xB44B3EBC (dec: -1270137156)
{
	struct Intro* intro; // size=76, offset=0
	struct _SFXMkr* sfxMarker; // size=36, offset=0
};

union INSTANCE_77fake // Hashcode: 0x2031C6D4 (dec: 540133076)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union INSTANCE_76fake // Hashcode: 0x868EC9CC (dec: -2037462580)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: instance.o, instance.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: instance.o, instance.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: instance.o, instance.o
};

extern void INSTANCE_ClearTfaces(struct _Instance *instance); // 0x80031ED8
extern void INSTANCE_Deactivate(struct _Instance *instance); // 0x80031F04
extern void INSTANCE_Reactivate(struct _Instance *instance); // 0x80031FC0
extern void INSTANCE_ForceActive(struct _Instance *instance); // 0x80032058
extern void INSTANCE_DeactivatedProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8003208C
extern void INSTANCE_DeactivateFarInstances(struct GameTracker *gameTracker); // 0x80032094
extern void INSTANCE_InitInstanceList(struct _InstanceList *list, struct _InstancePool *pool); // 0x80032310
extern struct _Instance * INSTANCE_NewInstance(struct _InstanceList *list); // 0x800323B0
extern long INSTANCE_InstanceGroupNumber(struct _Instance *instance); // 0x8003243C
extern void INSTANCE_InsertInstanceGroup(struct _InstanceList *list, struct _Instance *instance); // 0x80032558
extern void INSTANCE_ReallyRemoveInstance(struct _InstanceList *list, struct _Instance *instance, long reset); // 0x800325A0
extern void INSTANCE_CleanUpInstanceList(struct _InstanceList *list, long reset); // 0x800327E4
extern long INSTANCE_Introduced(struct Intro *intro, short streamUnitID); // 0x800328A0
extern struct INICommand * INSTANCE_GetIntroCommand(struct INICommand *command, int cmd); // 0x80032958
extern struct INICommand * INSTANCE_FindIntroCommand(struct _Instance *instance, int cmd); // 0x800329A8
extern void INSTANCE_ProcessIntro(struct _Instance *instance); // 0x800329CC
extern void INSTANCE_InitEffects(struct _Instance *instance, struct Object *object); // 0x80032A48
extern struct _Instance * INSTANCE_IntroduceInstance(struct Intro *intro, short streamUnitID); // 0x80032AD0
extern void INSTANCE_AdditionalCollideFunctions(struct _InstanceList *instanceList); // 0x800331F0
extern long INSTANCE_GetSplineFrameNumber(struct _Instance *instance, struct MultiSpline *spline); // 0x80033318
extern void INSTANCE_ProcessFunctions(struct _InstanceList *instanceList); // 0x80033350
extern struct _Instance * INSTANCE_BirthObject(struct _Instance *parent, struct Object *object, int modelNum); // 0x80033BFC
extern void INSTANCE_BuildStaticShadow(struct _Instance *instance); // 0x80033E6C
extern void INSTANCE_DefaultInit(struct _Instance *instance, struct Object *object, int modelNum); // 0x80033E74
extern void INSTANCE_PlainDeath(struct _Instance *instance); // 0x80034230
extern void INSTANCE_KillInstance(struct _Instance *instance); // 0x80034360
extern unsigned long INSTANCE_Query(struct _Instance *Inst, int Query); // 0x80034394
extern void INSTANCE_Post(struct _Instance *Inst, int Message, int Data); // 0x800343D0
extern void INSTANCE_Broadcast(struct _Instance *sender, long whatAmIMask, int Message, int Data); // 0x80034434
extern int INSTANCE_InPlane(struct _Instance *instance, int plane); // 0x800344E0
extern long INSTANCE_FindWithID(long uniqueID); // 0x8003453C
extern struct _Instance * INSTANCE_FindWithName(long areaID, char *instanceName, struct _Instance *startInstance); // 0x8003457C
extern struct Intro * INSTANCE_FindIntro(long areaID, long introUniqueID); // 0x80034650
extern struct _Instance * INSTANCE_Find(long introUniqueID); // 0x800346D4
extern struct _Instance * INSTANCE_IntroduceSavedInstance(struct _SavedIntro *savedIntro, struct _StreamUnit *streamUnit, int *deleted); // 0x80034714
extern struct _Instance * INSTANCE_IntroduceSavedInstanceWithIntro(struct _SavedIntroWithIntro *savedIntro, struct _StreamUnit *streamUnit, int *deleted); // 0x80034B8C
extern void INSTANCE_SpatialRelationships(struct _InstanceList *instanceList); // 0x80035000
extern int INSTANCE_SetStatsData(struct _Instance *instance, struct _Instance *checkee, struct _Vector *checkPoint, struct evCollideInstanceStatsData *data, MATRIX *mat); // 0x80035164
extern void INSTANCE_LinkToParent(struct _Instance *instance, struct _Instance *parent, int node); // 0x80035274
extern void INSTANCE_UnlinkFromParent(struct _Instance *instance); // 0x80035330
extern void INSTANCE_UnlinkChildren(struct _Instance *instance); // 0x80035494
extern void INSTANCE_UpdateFamilyStreamUnitID(struct _Instance *instance); // 0x80035500
extern void INSTANCE_ReallyRemoveAllChildren(struct _Instance *instance); // 0x80035558
extern struct _Instance * INSTANCE_GetChildLinkedToSegment(struct _Instance *instance, int segment); // 0x800355B4
extern int INSTANCE_Linked(struct _Instance *instance1, struct _Instance *instance2); // 0x800355EC
extern int INSTANCE_GetFadeValue(struct _Instance *instance); // 0x8003563C
extern unsigned long INSTANCE_DefaultAnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, struct _Instance *instance); // 0x80035720

#endif
