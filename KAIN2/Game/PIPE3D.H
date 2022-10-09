#ifndef PIPE3D_H
#define PIPE3D_H

#include "FX.H"
#include "G2/MAING2.H"

union PIPE3D_71fake // Hashcode: 0x1EBB5F62 (dec: 515596130)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: pipe3d.o, pipe3d.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: pipe3d.o, pipe3d.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: pipe3d.o, pipe3d.o
};

union PIPE3D_72fake // Hashcode: 0x17C0E47B (dec: 398517371)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct _Mirror // hashcode: 0x6161DBD4 (dec: 1633803220)
{
	short a; // size=0, offset=0
	short b; // size=0, offset=2
	short c; // size=0, offset=4
	short d; // size=0, offset=6
};

struct PIPE3D_109fake // hashcode: 0xEADE4BF5 (dec: -354530315)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: pipe3d.o, pipe3d.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: pipe3d.o, pipe3d.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: pipe3d.o, pipe3d.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: pipe3d.o, pipe3d.o
};

union PIPE3D_199fake // Hashcode: 0x416C539D (dec: 1097618333)
{
	struct _Instance* target; // size=616, offset=0
	_Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	_Position throwVector; // size=6, offset=0
};

struct PIPE3D_239fake // hashcode: 0x11DBF281 (dec: 299627137)
{
	long m[3]; // size=12, offset=0
};

struct PIPE3D_240fake // hashcode: 0xB73CD4A8 (dec: -1220750168)
{
	long m[5]; // size=20, offset=0
};

struct AniTexInfo 
{
	struct TextureMT3* texture; // size=16, offset=0
	long numFrames; // size=0, offset=4
	long speed; // size=0, offset=8
};

struct AniTex 
{
	long numAniTextues; // size=0, offset=0
	struct AniTexInfo aniTexInfo; // size=12, offset=4
};

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern void PIPE3D_AspectAdjustMatrix(MATRIX * matrix); // 0x8003A1F0
extern void PIPE3D_CalculateWCTransform(struct _CameraCore_Type *cameraCore); // 0x8003A278
extern void PIPE3D_InvertTransform(MATRIX * target, MATRIX * source); // 0x8003A5F4
extern long PIPE3D_MatrixColumnLength(MATRIX * transform, long column); // 0x8003A688
extern void PIPE3D_NormalizeMatrix(MATRIX * target, MATRIX * source); // 0x8003A6C4
extern void PIPE3D_TransformVerticesToWorld(struct _Instance *instance, struct _SVector *poolVertex, long *vtxSegment, struct _Vector *Average); // 0x8003A7B0
extern void PIPE3D_InstanceTransformAndDraw(struct _Instance *instance, struct _CameraCore_Type *cameraCore, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _Mirror *mirror); // 0x8003AADC
extern void PIPE3D_InstanceListTransformAndDrawFunc(struct _StreamUnit *unit, unsigned long **ot, struct _CameraCore_Type *cameraCore, struct _Instance *instance); // 0x8003ADD0
extern void PIPE3D_InstanceListTransformAndDraw(struct _StreamUnit *unit, struct GameTracker *gameTracker, unsigned long **ot, struct _CameraCore_Type *cameraCore); // 0x8003B120
extern void PIPE3D_TransformFromZAxis(MATRIX * transform, struct _SVector *normal); // 0x8003B218
extern void PIPE3D_CalcWorldToSplitPlaneTransform(MATRIX * wpTransform, struct _SVector *normal, struct _SVector *translation); // 0x8003B408
extern long PIPE3D_TransformAnimatedSplitInstanceVertices(_MVertex *vertexList, struct _PVertex *poolVertex, struct _Model *model, MATRIX * wcTransform, MATRIX * matrixPool, struct _Mirror *mirror, MATRIX * lm, CVECTOR *vertexColor, CVECTOR *vertexSrcCol); // 0x8003B4DC
extern void PIPE3D_TransformSplitInstanceVertices(_MVertex *vertexList, struct _PVertex *pvertex, struct _Model *model, MATRIX * wpTransform, MATRIX * matrixPool, struct _Mirror *mirror); // 0x8003BAA8
extern void PIPE3D_AnimateTextures(struct AniTex *aniTextures, long req_frame); // 0x8003BC1C
extern void PIPE3D_AnimateTerrainTextures(struct DrMoveAniTex *aniTextures, long req_frame, struct _PrimPool *primPool, unsigned long **drawot); // 0x8003BCBC
extern void PIPE3D_HalvePlaneInstanceTransformAndDraw(struct _Instance *instance, MATRIX * wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _Mirror *mirror); // 0x8003BE54
extern void PIPE3D_HalvePlaneGetRingPoints(struct _Instance *instance, MATRIX * wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _FXHalvePlane *ring); // 0x8003C048
extern void PIPE3D_DoGlow(struct _Instance *instance, MATRIX * wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _FXGlowEffect *glow); // 0x8003C218
extern long PIPE3D_Segment2ScreenPt(struct _Instance *instance, MATRIX * wcTransform, int segIndex, _Position *pos); // 0x8003C5F4

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
