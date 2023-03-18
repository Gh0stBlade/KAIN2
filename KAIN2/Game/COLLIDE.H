#ifndef COLLIDE_H
#define COLLIDE_H

#include "Game/CORE.H"

typedef long (*TDRFuncPtr_COLLIDE_LineWithBoxFace6collideBoundFunc)();

struct SandHFScratch {
	struct _SVector normal; // size=8, offset=0
	MATRIX posMatrix; // size=32, offset=8
	struct _Vector dpv; // size=12, offset=40
	struct _SVector triPoint; // size=8, offset=52
	struct _SVector planePoint; // size=8, offset=60
};

struct PandTFScratch {
	MATRIX posMatrix; // size=32, offset=0
	struct _SVector planePoint; // size=8, offset=32
	struct _SVector newPos; // size=8, offset=40
	struct _SVector oldPos; // size=8, offset=48
	struct _SVector normal; // size=8, offset=56
	struct _Vector dpv; // size=12, offset=64
};


struct _CollideInfo // hashcode: 0xCA2DF5E9 (dec: -902957591)
{
	void* level; // size=0, offset=0
	char flags; // size=0, offset=4
	char segment; // size=0, offset=5
	char type0; // size=0, offset=6
	char type1; // size=0, offset=7
	void* prim0; // size=0, offset=8
	void* prim1; // size=0, offset=12
	void* inst0; // size=0, offset=16
	void* inst1; // size=0, offset=20
	SVECTOR point0; // size=8, offset=24
	SVECTOR point1; // size=8, offset=32
	struct _Position offset; // size=6, offset=40
	short bspID; // size=0, offset=46
};

struct IandTScratch 
{
	MATRIX posMatrix; // size=32, offset=0
	struct _SVector normal; // size=8, offset=32
	struct _Vector dpv; // size=12, offset=40
	struct _SVector planePoint; // size=8, offset=52
	struct _SVector line; // size=8, offset=60
	struct _SVector oldPos; // size=8, offset=68
	struct _SVector newPos; // size=8, offset=76
	int i; // size=0, offset=84
	long in_spectral; // size=0, offset=88
	short* normalList; // size=0, offset=92
	struct _TVertex* vertexList; // size=12, offset=96
};

struct _PCollideInfo 
{
  short type; // size=0, offset=0
  short segment; // size=0, offset=2
  void *prim; // size=0, offset=4
  struct _Instance *inst; // size=0, offset=8
  SVECTOR wNormal; // size=8, offset=12
  SVECTOR *newPoint; // size=8, offset=20
  SVECTOR *oldPoint; // size=8, offset=24
  SVECTOR cldPoint; // size=8, offset=28
  long collideType; // size=0, offset=36
  struct _Instance *instance; // size=0, offset=40
};

struct SandTScratch 
{
	MATRIX posMatrix; // size=32, offset=0
	struct _Vector dpv; // size=12, offset=32
	struct _HNormal* normalList; // size=8, offset=44
	struct _CollideInfo collideInfo; // size=48, offset=48
	struct _HFaceInfo hfaceInfo; // size=32, offset=96
	struct _SVector midPoint; // size=8, offset=128
	struct _SVector spherePos; // size=8, offset=136
	void (*collideFunc)(struct _Instance* instance, struct GameTracker* gameTracker); // size=0, offset=144
	struct _Instance* instance; // size=668, offset=148
	long edge; // size=0, offset=152
	long in_spectral; // size=0, offset=156
	struct _SVector oldPos; // size=8, offset=160
	void* prim; // size=0, offset=168
	struct _SVector normal; // size=8, offset=172
	struct _Sphere sphere; // size=12, offset=180
	short result; // size=0, offset=192
	short i; // size=0, offset=194
	long collide_ignoreAttr; // size=0, offset=196
	long collide_acceptAttr; // size=0, offset=200
	long midRadius; // size=0, offset=204
	struct _TVertex* vertexList; // size=12, offset=208
};

struct _LCollideInfo // hashcode: 0xCC1DB336 (dec: -870468810)
{
	struct _TFace* tface; // size=12, offset=0
	struct _Terrain* terrain; // size=84, offset=4
	int curTree; // size=0, offset=8
};

struct SCollideInfo // hashcode: 0x0DEAD21F (dec: 233493023)
{
	struct _Sphere* sphere; // size=12, offset=0
	SVECTOR* oldPos; // size=8, offset=4
	void (*collideFunc)(struct _Instance* instance, struct GameTracker* gameTracker); // size=0, offset=8
	long id; // size=0, offset=12
	long segment; // size=0, offset=16
	struct _Instance* instance; // size=616, offset=20
	void* prim; // size=0, offset=24
};

struct PandTScratch // hashcode: 0x53B2A743 (dec: 1404217155)
{
	MATRIX posMatrix; // size=32, offset=0
	struct _SVector normal; // size=8, offset=32
	struct _Vector dpv; // size=12, offset=40
	struct _SVector planePoint; // size=8, offset=52
	long ignore_attr; // size=0, offset=60
	long accept_attr; // size=0, offset=64
	short* backface_flag; // size=0, offset=68
	struct _SVector newPos; // size=8, offset=72
	struct _SVector oldPos; // size=8, offset=80
	int flags; // size=0, offset=88
	int i; // size=0, offset=92
	int slack; // size=0, offset=96
	long in_spectral; // size=0, offset=100
	short* normalList; // size=0, offset=104
	struct _TVertex* vertexList; // size=12, offset=108
	struct _TFace* result; // size=12, offset=112
	struct _SVector line; // size=8, offset=116
};

struct COLLIDE_258fake // hashcode: 0x1D802093 (dec: 494936211)
{
	long m[3]; // size=12, offset=0
};

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern void G2Quat_ToMatrix(struct _G2EulerAngles_Type* a1, struct _G2Matrix_Type* a2);

extern int COLLIDE_PointInTriangle(struct _SVector *v0, struct _SVector *v1, struct _SVector *v2, struct _SVector *point, struct _SVector *normal); // 0x8001E460
extern int COLLIDE_PointInTriangle2DPub(short *v0, short *v1, short *v2, short *point); // 0x8001E704
extern long COLLIDE_GetNormal(short nNum, short *nrmlArray, struct _SVector *nrml); // 0x8001E750
extern void COLLIDE_MakeNormal(struct _Terrain *terrain, struct _TFace *tface, struct _SVector *normal); // 0x8001E7E8
extern void COLLIDE_UpdateAllTransforms(struct _Instance *instance, SVECTOR*offset); // 0x8001E9C0
extern void COLLIDE_MoveAllTransforms(struct _Instance *instance, _Position *offset); // 0x8001EA90
extern long COLLIDE_WithinYZBounds(struct _SVector *point, struct _HBox *hbox); // 0x8001EB60
extern long COLLIDE_WithinXZBounds(struct _SVector *point, struct _HBox *hbox); // 0x8001EBBC
extern long COLLIDE_WithinXYBounds(struct _SVector *point, struct _HBox *hbox); // 0x8001EC18
extern void COLLIDE_LineWithBoxFace(short startDist, long lineDist, short planeDist, struct _SVector *start, struct _Vector *line, struct _HBox *hbox, TDRFuncPtr_COLLIDE_LineWithBoxFace6collideBoundFunc collideBoundFunc, struct _SVector *normal); // 0x8001EC74
extern long COLLIDE_IntersectLineAndBox(struct _SVector *point0, struct _SVector *normal0, struct _SVector *point1, struct _SVector *normal1, struct _SVector *end, struct _SVector *start, struct _HBox *hbox); // 0x8001EE20
extern struct _TFace* COLLIDE_PointAndTerrain(struct _Terrain *terrain, struct _PCollideInfo *pcollideInfo, struct _LCollideInfo *lcol); // 0x8001F09C
extern struct _TFace* COLLIDE_PointAndTerrainFunc(struct _Terrain *terrain, struct _PCollideInfo *pCollideInfo, int Flags, short *Backface_Flag, long ignoreAttr, long acceptAttr, struct _LCollideInfo *lcolinfo); // 0x8001F0CC
extern int COLLIDE_PointAndHFace(struct _SVector *newPos, struct _SVector *oldPos, struct _HFace *hface, struct _Model *model, struct _SVector *hfNormal); // 0x8001FC64
extern void COLLIDE_PointAndInstance(struct _PCollideInfo *pcollideInfo, struct _Instance *instance); // 0x8001FEAC
extern void COLLIDE_PointAndInstanceTrivialReject(struct _PCollideInfo *pcollideInfo, struct _Instance *instance); // 0x800206E0
extern void COLLIDE_PointAndWorld(struct _PCollideInfo *pcollideInfo, struct Level *level); // 0x80020864
extern long COLLIDE_ClosestPointInBoxToPoint(_Position *boxPoint, struct _HBox *hbox, struct _SVector *point); // 0x80020C4C
extern long COLLIDE_SphereAndPoint(struct _Sphere *sphere, struct _SVector *point, struct _SVector *normal); // 0x80020D20
extern long COLLIDE_SphereAndHBox(struct _HBox *hbox, struct _Sphere *sphere, _Position *oldPos, struct _SVector *normal); // 0x80020F64
extern void COLLIDE_Instance1SpheresToInstance2(struct _Instance *instance1, struct _Instance *instance2, long sphereToSphere); // 0x800213DC
extern void COLLIDE_Instances(struct _Instance *instance1, struct _Instance *instance2); // 0x8002212C
extern void COLLIDE_InstanceList(struct _InstanceList *instanceList); // 0x80022274
extern long COLLIDE_SphereAndHFace(struct _Sphere *sphere, _Position *oldPos, struct _HFaceInfo *hfaceInfo, struct _SVector *intersect, long *edge); // 0x8002251C
extern long COLLIDE_SAndT(struct SCollideInfo *scollideInfo, struct Level *level); // 0x80022BF8
extern long COLLIDE_SphereAndTerrain(struct SCollideInfo *scollideInfo, struct Level *level); // 0x800236E4
extern void COLLIDE_InstanceTerrain(struct _Instance *instance, struct Level *level); // 0x800237CC
extern long COLLIDE_LineWithSignals(struct _SVector *startPoint, struct _SVector *endPoint, struct _MultiSignal **signalList, long maxSignals, struct Level *level); // 0x80023A38
extern void COLLIDE_InstanceTerrainSignal(struct _Instance *instance, struct Level *level); // 0x800241FC
extern struct _StreamUnit * COLLIDE_CameraWithStreamSignals(struct Camera *camera); // 0x80024444
extern void COLLIDE_InstanceListWithSignals(struct _InstanceList *instanceList); // 0x8002472C
extern void COLLIDE_InstanceListTerrain(struct _InstanceList *instanceList); // 0x800247A0
extern void COLLIDE_SegmentCollisionOn(struct _Instance *instance, int segment); // 0x80024850
extern void COLLIDE_SegmentCollisionOff(struct _Instance *instance, int segment); // 0x800248C8
extern long COLLIDE_FindCollisionFaceNormal(struct _CollideInfo *collideInfo, struct _Normal *normal); // 0x80024964
extern short * COLLIDE_GetBSPTreeFlag(struct _CollideInfo *collideInfo); // 0x80024AA0
extern void COLLIDE_SetBSPTreeFlag(struct _CollideInfo *collideInfo, short flag); // 0x80024AC8
extern int COLLIDE_PointAndTfaceFunc(struct _Terrain *terrain, struct BSPTree *bsp, struct _SVector *orgNewPos, struct _SVector *orgOldPos, struct _TFace *tface, long ignoreAttr, long flags); // 0x80024AFC

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
