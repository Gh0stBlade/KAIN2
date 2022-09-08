#ifndef FX_H
#define FX_H

#include "CORE.H"

typedef void (*TDRFuncPtr__FX_Build5fxSetup)();
typedef void (*TDRFuncPtr__FX_Build6fxProcess)();
typedef void (*TDRFuncPtr__FX_BuildNonSegmentedSplinters6fxSetup)();
typedef void (*TDRFuncPtr__FX_BuildNonSegmentedSplinters7fxProcess)();
typedef void (*TDRFuncPtr__FX_BuildSegmentedSplinters6fxSetup)();
typedef void (*TDRFuncPtr__FX_BuildSegmentedSplinters7fxProcess)();
typedef void (*TDRFuncPtr__FX_BuildSingleFaceWithModel6fxSetup)();
typedef void (*TDRFuncPtr__FX_BuildSingleFaceWithModel7fxProcess)();
typedef void (*TDRFuncPtr__FX_BuildSplinters6fxSetup)();
typedef void (*TDRFuncPtr__FX_BuildSplinters7fxProcess)();
typedef void (*TDRFuncPtr_FX_Build5fxSetup)();
typedef void (*TDRFuncPtr_FX_Build6fxProcess)();
typedef void (*TDRFuncPtr_FX_BuildSingleFaceWithModel6fxSetup)();
typedef void (*TDRFuncPtr_FX_BuildSingleFaceWithModel7fxProcess)();
typedef void (*TDRFuncPtr_FX_SimpleQuadSetup1fxProcess)();

struct _FXGlowEffect {
  void *next; // size=0, offset=0
  void *continue_process; // size=0, offset=4
  struct _Instance *instance; // size=668, offset=8
  unsigned char effectType; // size=0, offset=12
  unsigned char type; // size=0, offset=13
  short lifeTime; // size=0, offset=14
  unsigned long diffTime; // size=0, offset=16
  long *colorArray; // size=0, offset=20
  long currentColor; // size=0, offset=24
  short numColors; // size=0, offset=28
  short colorBlendCycle; // size=0, offset=30
  short width; // size=0, offset=32
  short height; // size=0, offset=34
  unsigned char numSegments; // size=0, offset=36
  unsigned char SegmentInc; // size=0, offset=37
  short segment; // size=0, offset=38
  short fadein_time; // size=0, offset=40
  short fadeout_time; // size=0, offset=42
};

struct FXSplinter {
  short flags; // size=0, offset=0
  short soundFx; // size=0, offset=2
  short chunkVelXY; // size=0, offset=4
  short chunkVelZ; // size=0, offset=6
  short chunkVelRng; // size=0, offset=8
  short triVelRng; // size=0, offset=10
  short lifetime; // size=0, offset=12
  short faceLimit; // size=0, offset=14
  short rotRateRng; // size=0, offset=16
  short gravityZ; // size=0, offset=18
};

struct _FXGeneralEffect // hashcode: 0xC4783451 (dec: -998755247)
{
	void* next; // size=0, offset=0
	void* continue_process; // size=0, offset=4
	struct _Instance* instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
};

struct _FXRibbon // hashcode: 0xB11070B8 (dec: -1324322632)
{
	void* next; // size=0, offset=0
	void* continue_process; // size=0, offset=4
	struct _Instance* instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	short numberVerts; // size=0, offset=16
	short endIndex; // size=0, offset=18
	SVECTOR* vertexPool; // size=8, offset=20
	short startSegment; // size=0, offset=24
	short endSegment; // size=0, offset=26
	long startColor; // size=0, offset=28
	long endColor; // size=0, offset=32
	short colorStepValue; // size=0, offset=36
	short faceLifeTime; // size=0, offset=38
	short startFadeValue; // size=0, offset=40
	short fadeStep; // size=0, offset=42
};

struct _FXParticle // hashcode: 0xDD6F143E (dec: -579922882)
{
	void* next; // size=0, offset=0
	void* continue_process; // size=0, offset=4
	struct _Instance* instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	struct TextureMT3* texture; // size=16, offset=16
	void* fxprim_modify_process; // size=0, offset=20
	void* fxprim_process; // size=0, offset=24
	short size; // size=0, offset=28
	short primLifeTime; // size=0, offset=30
	short birthRadius; // size=0, offset=32
	short scaleSpeed; // size=0, offset=34
	_Position direction; // size=6, offset=36
	unsigned short flag_bits; // size=0, offset=42
	_Position acceleration; // size=6, offset=44
	short startScale; // size=0, offset=50
	_Position offset; // size=6, offset=52
	char startSegment; // size=0, offset=58
	char z_undulate; // size=0, offset=59
	long startColor; // size=0, offset=60
	long endColor; // size=0, offset=64
	short colorStepValue; // size=0, offset=68
	char numberBirthParticles; // size=0, offset=70
	char endSegment; // size=0, offset=71
	short startFadeValue; // size=0, offset=72
	short fadeStep; // size=0, offset=74
};

struct _FXLightBeam // hashcode: 0xDC9CA53F (dec: -593713857)
{
	void* next; // size=0, offset=0
	void* continue_process; // size=0, offset=4
	struct _Instance* instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	short startSeg; // size=0, offset=16
	short endSeg; // size=0, offset=18
	long color; // size=0, offset=20
};

struct _FXBlastringEffect 
{
	void* next; // size=0, offset=0
	void* continue_process; // size=0, offset=4
	struct _Instance* instance; // size=668, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	struct _SVector position; // size=8, offset=16
	MATRIX matrix; // size=32, offset=24
	short segment; // size=0, offset=56
	short pad; // size=0, offset=58
	int radius; // size=0, offset=60
	int size1; // size=0, offset=64
	int size2; // size=0, offset=68
	int startRadius; // size=0, offset=72
	int endRadius; // size=0, offset=76
	int colorchange_radius; // size=0, offset=80
	int vel; // size=0, offset=84
	int accl; // size=0, offset=88
	int height1; // size=0, offset=92
	int height2; // size=0, offset=96
	int height3; // size=0, offset=100
	char predator_offset; // size=0, offset=104
	char predator_offset_orig; // size=0, offset=105
	char stay_in_place; // size=0, offset=106
	char sortInWorld; // size=0, offset=107
	long color; // size=0, offset=108
	long startColor; // size=0, offset=112
	long endColor; // size=0, offset=116
};


struct _FXForceFieldEffect // hashcode: 0x59105BC4 (dec: 1494244292)
{
	void* next; // size=0, offset=0
	void* continue_process; // size=0, offset=4
	struct _Instance* instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	struct _SVector offset; // size=8, offset=16
	short size; // size=0, offset=24
	short size_diff; // size=0, offset=26
	short size_change; // size=0, offset=28
	short deg; // size=0, offset=30
	short deg_change; // size=0, offset=32
	short start_fade; // size=0, offset=34
	short end_fade; // size=0, offset=36
	long color; // size=0, offset=40
};

struct _FXLightning // hashcode: 0x552145A3 (dec: 1428243875)
{
	void* next; // size=0, offset=0
	void* continue_process; // size=0, offset=4
	struct _Instance* instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	short deg; // size=0, offset=16
	short deg_inc; // size=0, offset=18
	_Position start_offset; // size=6, offset=20
	short startSeg; // size=0, offset=26
	struct _Instance* end_instance; // size=616, offset=28
	_Position end_offset; // size=6, offset=32
	short endSeg; // size=0, offset=38
	short matrixSeg; // size=0, offset=40
	short width; // size=0, offset=42
	short small_width; // size=0, offset=44
	short segs; // size=0, offset=46
	short sine_size; // size=0, offset=48
	short variation; // size=0, offset=50
	long color; // size=0, offset=52
	long glow_color; // size=0, offset=56
};

struct __BubbleParams {
  short DisperseFrames; // size=0, offset=0
  short KillScale; // size=0, offset=2
  short MinSplashSize; // size=0, offset=4
  short MaxSpeed; // size=0, offset=6
  short MaxSpeedRange; // size=0, offset=8
  short ScaleRate; // size=0, offset=10
  short ScaleRateRange; // size=0, offset=12
  short StartScale; // size=0, offset=14
  short StartScaleRange; // size=0, offset=16
  short UniqueBubbles; // size=0, offset=18
};

struct _FXFlash // hashcode: 0xD3DE17A3 (dec: -740419677)
{
	void* next; // size=0, offset=0
	void* continue_process; // size=0, offset=4
	struct _Instance* instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	long timeToColor; // size=0, offset=16
	long timeAtColor; // size=0, offset=20
	long timeFromColor; // size=0, offset=24
	long currentTime; // size=0, offset=28
	long color; // size=0, offset=32
};

struct GenericFXObject // hashcode: 0xD22FD65C (dec: -768616868)
{
	struct _GenericParticleParams* ParticleList; // size=48, offset=0
	struct _GenericRibbonParams* RibbonList; // size=20, offset=4
	struct _GenericGlowParams* GlowList; // size=24, offset=8
	struct _GenericLightningParams* LightningList; // size=40, offset=12
	struct _GenericBlastringParams* BlastList; // size=48, offset=16
	struct _GenericFlashParams* FlashList; // size=12, offset=20
	long* ColorList; // size=0, offset=24
};

union FX_204fake // Hashcode: 0x83EDAC95 (dec: -2081575787)
{
	struct _Instance* target; // size=616, offset=0
	_Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	_Position throwVector; // size=6, offset=0
};

struct _FX_MATRIX // hashcode: 0x13190ADD (dec: 320408285)
{
	struct NodeType node; // size=8, offset=0
	long flags; // size=0, offset=8
#if !defined(PSX_VERSION)//Undefined
	MATRIX lwTransform; // size=32, offset=12
#endif
};

struct _FX_PRIM // hashcode: 0x16BF818E (dec: 381649294)
{
	struct NodeType node; // size=8, offset=0
	struct TextureMT3* PTR_32 texture; // size=16, offset=8
	long flags; // size=0, offset=12
	short timeToLive; // size=0, offset=16
	short curFrame; // size=0, offset=18
	void* PTR_32 process; // size=0, offset=20
	struct _FX_MATRIX* PTR_32 matrix; // size=44, offset=24
	long color; // size=0, offset=28
	long startColor; // size=0, offset=32
	long endColor; // size=0, offset=36
	short colorFadeValue; // size=0, offset=40
	short colorFadeStep; // size=0, offset=42
	_Position position; // size=6, offset=44
	short fadeStep; // size=0, offset=50
	short fadeValue[4]; // size=8, offset=52
	_Position v0; // size=6, offset=60
	short work0; // size=0, offset=66
	_Position v1; // size=6, offset=68
	short work1; // size=0, offset=74
	_Position v2; // size=6, offset=76
	short work2; // size=0, offset=82
	_Position v3; // size=6, offset=84
	short work3; // size=0, offset=90
	union {
		struct {
			struct _Instance* PTR_32 parent; // size=616, offset=0
			long segment; // size=0, offset=4
		} flame; // size=8, offset=0, found in object files: debug.o, debug.o
		struct {
			short xVel; // size=0, offset=0
			short yVel; // size=0, offset=2
			short zVel; // size=0, offset=4
			short xAccl; // size=0, offset=6
			short yAccl; // size=0, offset=8
			short zAccl; // size=0, offset=10
		} phys; // size=12, offset=0, found in object files: debug.o, debug.o
	} duo; // size=12, offset=92, found in object files: debug.o, debug.o
};

struct _FXHalvePlane // hashcode: 0x10EBCD5A (dec: 283888986)
{
	void* next; // size=0, offset=0
	void* continue_process; // size=0, offset=4
	struct _Instance* instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	long diffTime; // size=0, offset=16
	struct _PlaneConstants ringPlane; // size=12, offset=20
	long currentColor; // size=0, offset=32
	long* colorArray; // size=0, offset=36
	short numColors; // size=0, offset=40
	short colorBlendLife; // size=0, offset=42
};

struct _FXTracker // hashcode: 0xCC65A064 (dec: -865755036)
{
	struct _FX_MATRIX matrixPool[32]; // size=1408, offset=0
	struct NodeType usedMatrixList; // size=8, offset=1408
	struct NodeType freeMatrixList; // size=8, offset=1416
	struct _FX_PRIM primPool[256]; // size=26624, offset=1424
	struct NodeType usedPrimList; // size=8, offset=28048
	struct NodeType usedPrimListSprite; // size=8, offset=28056
	struct NodeType freePrimList; // size=8, offset=28064
};


extern struct _FXBlastringEffect* fx_blastring;
extern short fx_going;

extern void FX_Init(struct _FXTracker *fxTracker); // 0x80041F54
extern void FX_Die(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80042034
extern struct _FX_MATRIX * FX_GetMatrix(struct _FXTracker *fxTracker); // 0x800420A4
extern struct _FX_PRIM * FX_GetPrim(struct _FXTracker *fxTracker); // 0x800420F4
extern struct _FXParticle * FX_GetParticle(struct _Instance *instance, short startSegment); // 0x80042164
extern void FX_AniTexSetup(struct _FX_PRIM *fxPrim, struct _MFace *mface, struct _Model *model, struct _FXTracker *fxTracker); // 0x80042200
extern void FX_StandardProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x8004227C
extern void FX_ShatterProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x8004229C
extern void FX_DFacadeProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80042544
extern struct _FX_PRIM * _FX_BuildSingleFaceWithModel(struct _Model *model, struct _MFace *mface, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct _FXTracker *fxTracker, TDRFuncPtr__FX_BuildSingleFaceWithModel6fxSetup fxSetup, TDRFuncPtr__FX_BuildSingleFaceWithModel7fxProcess fxProcess, struct _FX_MATRIX *fxMatrix, int timeToLive); // 0x80042718
extern struct _FX_PRIM * FX_BuildSingleFaceWithModel(struct _Model *model, struct _MFace *mface, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct _FXTracker *fxTracker, TDRFuncPtr_FX_BuildSingleFaceWithModel6fxSetup fxSetup, TDRFuncPtr_FX_BuildSingleFaceWithModel7fxProcess fxProcess, int timeToLive); // 0x80042814
extern void _FX_SetupLighting(struct _Instance *instance); // 0x80042864
extern long _FX_DoLighting(struct _MFace *mface); // 0x800428B4
extern void _FX_BuildSegmentedSplinters(struct _Instance *instance, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, TDRFuncPtr__FX_BuildSegmentedSplinters6fxSetup fxSetup, TDRFuncPtr__FX_BuildSegmentedSplinters7fxProcess fxProcess, int shardFlags); // 0x800429CC
extern void _FX_BuildNonSegmentedSplinters(struct _Instance *instance, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, TDRFuncPtr__FX_BuildNonSegmentedSplinters6fxSetup fxSetup, TDRFuncPtr__FX_BuildNonSegmentedSplinters7fxProcess fxProcess, int shardFlags); // 0x80043450
extern void _FX_BuildSplinters(struct _Instance *instance, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, TDRFuncPtr__FX_BuildSplinters6fxSetup fxSetup, TDRFuncPtr__FX_BuildSplinters7fxProcess fxProcess, int shardFlags); // 0x8004403C
extern void _FX_Build(struct _Instance *instance, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct _FXTracker *fxTracker, TDRFuncPtr__FX_Build5fxSetup fxSetup, TDRFuncPtr__FX_Build6fxProcess fxProcess, int shardFlags); // 0x80044190
extern void FX_Build(struct _Instance *instance, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct _FXTracker *fxTracker, TDRFuncPtr_FX_Build5fxSetup fxSetup, TDRFuncPtr_FX_Build6fxProcess fxProcess); // 0x800442A4
extern void FX_UpdatePos(struct _FX_PRIM *fxPrim, struct _SVector *offset, int spriteflag); // 0x800442DC
extern void FX_Relocate(struct _SVector *offset); // 0x80044430
extern void FX_UpdateTexturePointers(struct _FX_PRIM *fxPrim, struct Object *oldObject, long sizeOfObject, long offset); // 0x800445E0
extern void FX_RelocateFXPointers(struct Object *oldObject, struct Object *newObject, long sizeOfObject); // 0x80044640
extern void FX_ProcessList(struct _FXTracker *fxTracker); // 0x80044710
extern void FX_DrawReaver(struct _PrimPool *primPool, unsigned long **ot, MATRIX *wcTransform); // 0x8004490C
extern void FX_DrawList(struct _FXTracker *fxTracker, struct GameTracker *gameTracker, unsigned long **ot, MATRIX *wcTransform); // 0x80044958
extern void FX_SimpleQuadSetup(struct _FX_PRIM *fxPrim, TDRFuncPtr_FX_SimpleQuadSetup1fxProcess fxProcess, struct _FX_MATRIX *fxMatrix, struct _Instance *instance, struct _MFace *mface, struct _MVertex *vertexList, SVECTOR*center, SVECTOR*vel, SVECTOR*accl, struct _FXTracker *fxTracker, int timeToLive); // 0x80045B60
extern void FX_WaterRingProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80045D9C
extern void FX_WaterBubbleProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80045E18
extern void FX_Sprite_Insert(struct NodeType *list, struct _FX_PRIM *fxPrim); // 0x80046030
extern struct TextureMT3 * FX_GetTextureObject(struct Object *object, int modelnum, int texnum); // 0x80046068
extern void FX_MakeWaterBubble(struct _SVector *position, struct _SVector *vel, struct _SVector *accl, long splashZ, struct __BubbleParams *BP); // 0x800460A4
extern void FX_DrawScreenPoly(int transtype, unsigned long color, int zdepth); // 0x80046254
extern POLY_GT4 * FX_SetupPolyGT4(int x1, int y1, int x2, int y2, int otz, struct TextureMT3 *texture, long color0, long color1, long color2, long color3); // 0x80046300
extern void FX_MakeWarpArrow(int x, int y, int xsize, int ysize, int fade); // 0x80046404
extern void FX_MakeMannaIcon(int x, int y, int xsize, int ysize); // 0x800464E4
extern void FX_MakeGlyphIcon(_Position *position, struct Object *glyphObject, int size, int glyphnum, int enabled); // 0x80046690
extern void FX_SoulDustProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80046854
extern void FX_MakeSoulDust(struct _Instance *instance, short segment); // 0x80046A08
extern void FX_WaterTrailProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80046C48
extern void FX_MakeWaterTrail(struct _Instance *instance, int depth); // 0x80046D60
extern struct _FXRibbon * FX_StartRibbon(struct _Instance *instance, short startSegment, short endSegment, short type, int ribbonLifeTime, int faceLifeTime, int startFadeValue, long startColor, long endColor); // 0x80046F94
extern void FX_RibbonProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80047264
extern void FX_ConstrictProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80047358
extern void FX_StartConstrict(struct _Instance *instance, struct _SVector *constrict_point, short startSegment, short endSegment); // 0x800475FC
extern void FX_EndConstrict(int ConstrictEnemyFlag, struct _Instance *instance); // 0x8004768C
extern void FX_SubDividePrim(struct _FX_PRIM *fxPrim1, struct _FX_PRIM *fxPrim2); // 0x800476F8
extern void FX_ContinueRibbon(struct _FXRibbon *ribbon, struct _FXTracker *fxTracker); // 0x800477BC
extern void FX_StandardFXPrimProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80047D8C
extern void FX_AttachedParticlePrimProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x800480FC
extern void FX_FlamePrimProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80048190
extern void FX_DFacadeParticleSetup(struct _FX_PRIM *fxPrim, SVECTOR*center, short halveWidth, short halveHeight, long color, SVECTOR*vel, SVECTOR*accl, struct _FXTracker *fxTracker, int timeToLive); // 0x80048364
extern struct _FX_PRIM * FX_Dot(struct _SVector *location, struct _SVector *vel, struct _SVector *accel, int scale_speed, long color, long size, int lifetime, int texture_num); // 0x8004844C
extern void FX_Blood(struct _SVector *location, struct _SVector *input_vel, struct _SVector *accel, int amount, long color, long size); // 0x800485B8
extern void FX_Blood2(struct _SVector *location, struct _SVector *input_vel, struct _SVector *accel, int amount, long color, long dummyCrapShouldRemove); // 0x800486F4
extern void FX_Blood_Impale(struct _Instance *locinst, short locseg, struct _Instance *instance, short segment); // 0x80048720
extern struct _FXParticle * FX_BloodCone(struct _Instance *instance, short startSegment, long time); // 0x800488EC
extern struct _FXParticle * FX_GetTorchParticle(struct _Instance *instance, short startSegment, int tex, int birthRadius, int num); // 0x800489B8
extern struct _FXParticle * FX_TorchFlame(struct _Instance *instance, short startSegment); // 0x80048A9C
extern int FX_GetMorphFadeVal(); // 0x80048BF8
extern void FX_ConvertCamPersToWorld(SVECTOR*campos, SVECTOR*worldpos); // 0x80048C38
extern void FX_GetRandomScreenPt(SVECTOR*point); // 0x80048D50
extern void FX_ProcessSnow(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x80048DDC
extern void FX_ContinueSnow(struct _FXTracker *fxTracker); // 0x80048F10
extern void FX_UpdateWind(struct _FXTracker *fxTracker); // 0x80049068
extern void FX_ProcessRain(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker); // 0x800491E0
extern void FX_ContinueRain(struct _FXTracker *fxTracker); // 0x800492B0
extern void FX_MakeSpark(struct _Instance *instance, struct _Model *model, int segment); // 0x800495E8
extern void FX_ContinueParticle(struct _FXParticle *currentParticle, struct _FXTracker *fxTracker); // 0x800496A4
extern void FX_UpdraftPrimModify(struct _FX_PRIM *fxPrim, struct _Instance *instance, struct _FXParticle *particle, struct _FXTracker *fxTracker); // 0x80049EE4
extern void FX_MakeParticleTexFX(struct _FX_PRIM *fxPrim, struct _SVector *position, struct Object *object, int modelnum, int texnum, struct _SVector *vel, struct _SVector *accl, long color, int size, int life); // 0x8004A028
extern void FX_MakeHitFX(struct _SVector *position); // 0x8004A130
extern void FX_ContinueLightning(struct _FXLightning *zap, struct _FXTracker *fxTracker); // 0x8004A1C4
extern void FX_SetReaverInstance(struct _Instance *instance); // 0x8004A224
extern void FX_SoulReaverBlade(struct _Instance *instance, unsigned long **drawot); // 0x8004A230
extern void FX_ReaverBladeInit(); // 0x8004A620
extern void FX_SoulReaverWinding(struct _Instance *instance, struct _PrimPool *primPool, unsigned long **ot, MATRIX *wcTransform); // 0x8004A628
extern void FX_UpdateInstanceWaterSplit(struct _Instance *instance); // 0x8004A878
extern void FX_GetPlaneEquation(struct _SVector *normal, struct _SVector *poPlane, struct _PlaneConstants *plane); // 0x8004A948
extern void FX_DoInstancePowerRing(struct _Instance *instance, long atuTime, long *color, long numColors, int follow_halveplane); // 0x8004A9BC
extern void FX_UpdatePowerRing(struct _FXHalvePlane *ring); // 0x8004AB3C
extern void FX_UpdateInstanceSplitRing(struct _FXHalvePlane *ring, struct _FXTracker *fxTracker); // 0x8004ADC0
extern void FX_UpdateGlowEffect(struct _FXGlowEffect *effect, struct _FXTracker *fxTracker); // 0x8004AE50
extern void FX_InsertGeneralEffect(void *ptr); // 0x8004AE80
extern void FX_DeleteGeneralEffect(struct _FXGeneralEffect *effect); // 0x8004AE90
extern struct _FXGlowEffect * FX_DoInstanceOneSegmentGlow(struct _Instance *instance, long segment, long *color, long numColors, long atuColorCycleRate, long width, long height); // 0x8004AF2C
extern void FX_SetGlowFades(struct _FXGlowEffect *glowEffect, int fadein, int fadeout); // 0x8004B08C
extern struct _FXGlowEffect * FX_DoInstanceTwoSegmentGlow(struct _Instance *instance, long segment, long segmentEnd, long *color, long numColors, long atuColorCycleRate, long height); // 0x8004B0A8
extern struct _FXGlowEffect * FX_DoInstanceManySegmentGlow(struct _Instance *instance, long segment, long numSegments, long *color, long numColors, long atuColorCycleRate, long height); // 0x8004B100
extern struct _FXGlowEffect * FX_DoInstanceOneSegmentGlowWithTime(struct _Instance *instance, long segment, long *color, long numColors, long atuColorCycleRate, long width, long height, long ATULifeTime); // 0x8004B148
extern void FX_StopAllGlowEffects(struct _Instance *instance, int fadeout_time); // 0x8004B190
extern void FX_StopGlowEffect(struct _FXGlowEffect *glowEffect, int fadeout_time); // 0x8004B224
extern void FX_DrawLightning(struct _FXLightning *zap, MATRIX *wcTransform, unsigned long **ot); // 0x8004B2AC
extern void FX_DrawAllGeneralEffects(MATRIX *wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot); // 0x8004B560
extern void FX_ContinueBlastRing(struct _FXBlastringEffect *blast, struct _FXTracker *fxTracker); // 0x8004B738
extern struct _FXBlastringEffect * FX_DoBlastRing(struct _Instance *instance, struct _SVector *position, MATRIX *mat, int segment, int radius, int endRadius, int colorChangeRadius, int size1, int size2, int vel, int accl, int height1, int height2, int height3, long startColor, long endColor, int pred_offset, int lifeTime, int sortInWorld); // 0x8004B94C
extern void FX_DrawBlastring(MATRIX *wcTransform, struct _FXBlastringEffect *blast); // 0x8004BB20
extern void FX_ContinueFlash(struct _FXFlash *flash, struct _FXTracker *fxTracker); // 0x8004BCEC
extern void FX_DrawFlash(struct _FXFlash *flash); // 0x8004BD3C
extern void FX_RelocateGeneric(struct Object *object, long offset); // 0x8004BE50
extern struct _FXParticle * FX_StartGenericParticle(struct _Instance *instance, int num, int segOverride, int lifeOverride, int InitFlag); // 0x8004BEEC
extern void FX_StartGenericRibbon(struct _Instance *instance, int num, int segOverride, int endOverride, int InitFlag); // 0x8004C23C
extern void FX_StartGenericGlow(struct _Instance *instance, int num, int segOverride, int seg2Override, int InitFlag); // 0x8004C2F8
extern struct _FXLightning * FX_CreateLightning(struct _Instance *instance, int lifeTime, int deg, int deg_inc, int width, int small_width, int segs, int sine_size, int variation, unsigned long color, unsigned long glow_color); // 0x8004C4A0
extern void FX_SetLightingPos(struct _FXLightning *zap, struct _Instance *startInstance, int startSeg, _Position *startOffset, struct _Instance *endInstance, int endSeg, _Position *endOffset, int matrixSeg); // 0x8004C5A8
extern struct _FXLightning * FX_StartGenericLightning(struct _Instance *instance, int num, int segOverride, int endSegOverride); // 0x8004C658
extern struct _FXBlastringEffect * FX_StartGenericBlastring(struct _Instance *instance, int num, int segOverride, int matrixSegOverride); // 0x8004C7FC
extern struct _FXFlash * FX_StartGenericFlash(struct _Instance *instance, int num); // 0x8004CA04
extern long FX_GetHealthColor(int currentHealth); // 0x8004CAE4
extern void FX_Start_Snow(int percent); // 0x8004CB38
extern void FX_Start_Rain(int percent); // 0x8004CB6C
extern void FX_StartLightbeam(struct _Instance *instance, int startSeg, int endSeg, int color_num); // 0x8004CBA8
extern void FX_StartInstanceEffect(struct _Instance *instance, struct ObjectEffect *effect, int InitFlag); // 0x8004CC4C
extern void FX_EndInstanceEffects(struct _Instance *instance); // 0x8004D06C
extern void FX_EndInstanceParticleEffects(struct _Instance *instance); // 0x8004D0C0
extern void FX_GetSpiralPoint(int radius, int deg, int *x, int *y); // 0x8004D130
extern void FX_GetLinePoint(int radius, int next_radius, int deg, int next_deg, int *pntx, int *pnty, int part); // 0x8004D204
extern void FX_CalcSpiral(int degchange); // 0x8004D2D0
extern void FX_Spiral(struct _PrimPool *primPool, unsigned long **ot); // 0x8004D5C8
extern void FX_Health_Spiral(int number, int current_health, int max_health); // 0x8004D9E8
extern void FX_Spiral_Init(); // 0x8004DB34
extern void FX_DrawModel(struct Object *object, int model_num, struct _SVector *rotation, struct _SVector *position, struct _SVector *offset, int transflag); // 0x8004DB74
extern void fx_calc_points(struct _SVector *points, int degrees, int radius, int radius2, int radius3); // 0x8004DDEC
extern long fx_get_startz(struct _SVector *position); // 0x8004DEFC
extern void FX_DrawRing(MATRIX *wcTransform, struct _SVector *position, MATRIX *matrix, int radius, int radius2, int radius3, int z1, int z2, int z3, long color, int sortInWorld); // 0x8004DF54
extern void fx_setTex(DVECTOR *x, unsigned char *uv, int tx, int offset); // 0x8004E200
extern void FX_DrawRing2(MATRIX *wcTransform, struct _SVector *position, MATRIX *matrix, int radius, int radius2, int radius3, int z1, int z2, int z3, long offset, int sortInWorld); // 0x8004E244
extern void FX_DrawFField(MATRIX *wcTransform, struct _FXForceFieldEffect *field); // 0x8004E61C
extern struct _FXForceFieldEffect * FX_StartFField(struct _Instance *instance, int size, _Position *offset, int size_diff, int size_change, int deg_change, long color); // 0x8004E810
extern void FX_EndFField(struct _Instance *instance); // 0x8004E8E4
extern void FX_Draw_Glowing_Line(unsigned long **ot, long otz, DVECTOR *sxy0, DVECTOR *sxy1, DVECTOR *xy0, DVECTOR *xy1, long color, long color2); // 0x8004E964
extern void FX_Lightning(MATRIX *wcTransform, unsigned long **ot, MATRIX *mat, short deg, struct _SVector *start, struct _SVector *end, int width, int small_width, int segs, int sine_size, int variation, long color, long glow_color); // 0x8004EB00
extern void FX_LightHouse(MATRIX *wcTransform, unsigned long **ot, struct _Instance *instance, int startSeg, int endSeg, int segs, long beam_color); // 0x8004EFF0
extern void FX_StartPassthruFX(struct _Instance *instance, struct _SVector *normal, struct _SVector *point); // 0x8004F46C
extern void FX_EndPassthruFX(struct _Instance *instance); // 0x8004F548

#endif
