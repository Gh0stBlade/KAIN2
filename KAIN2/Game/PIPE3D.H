#ifndef PIPE3D_H
#define PIPE3D_H

#include "FX.H"
#include "G2/MAING2.H"

struct _Mirror // hashcode: 0x6161DBD4 (dec: 1633803220)
{
	short a; // size=0, offset=0
	short b; // size=0, offset=2
	short c; // size=0, offset=4
	short d; // size=0, offset=6
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
extern void PIPE3D_InstanceTransformAndDraw(struct _Instance *instance, struct _CameraCore_Type *cameraCore, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned int **ot, struct _Mirror *mirror); // 0x8003AADC
extern void PIPE3D_InstanceListTransformAndDrawFunc(struct _StreamUnit *unit, unsigned long **ot, struct _CameraCore_Type *cameraCore, struct _Instance *instance); // 0x8003ADD0
extern void PIPE3D_InstanceListTransformAndDraw(struct _StreamUnit *unit, struct GameTracker *gameTracker, unsigned int **ot, struct _CameraCore_Type *cameraCore); // 0x8003B120
extern void PIPE3D_TransformFromZAxis(MATRIX * transform, struct _SVector *normal); // 0x8003B218
extern void PIPE3D_CalcWorldToSplitPlaneTransform(MATRIX * wpTransform, struct _SVector *normal, struct _SVector *translation); // 0x8003B408
extern long PIPE3D_TransformAnimatedSplitInstanceVertices(_MVertex *vertexList, struct _PVertex *poolVertex, struct _Model *model, MATRIX * wcTransform, MATRIX * matrixPool, struct _Mirror *mirror, MATRIX * lm, CVECTOR *vertexColor, CVECTOR *vertexSrcCol); // 0x8003B4DC
extern void PIPE3D_TransformSplitInstanceVertices(_MVertex *vertexList, struct _PVertex *pvertex, struct _Model *model, MATRIX * wpTransform, MATRIX * matrixPool, struct _Mirror *mirror); // 0x8003BAA8
extern void PIPE3D_AnimateTextures(struct AniTex *aniTextures, long req_frame); // 0x8003BC1C
extern void PIPE3D_AnimateTerrainTextures(struct DrMoveAniTex *aniTextures, long req_frame, struct _PrimPool *primPool, unsigned int **drawot); // 0x8003BCBC
extern void PIPE3D_HalvePlaneInstanceTransformAndDraw(struct _Instance *instance, MATRIX * wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned int **ot, struct _Mirror *mirror); // 0x8003BE54
extern void PIPE3D_HalvePlaneGetRingPoints(struct _Instance *instance, MATRIX * wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _FXHalvePlane *ring); // 0x8003C048
extern void PIPE3D_DoGlow(struct _Instance *instance, MATRIX * wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _FXGlowEffect *glow); // 0x8003C218
extern long PIPE3D_Segment2ScreenPt(struct _Instance *instance, MATRIX * wcTransform, int segIndex, _Position *pos); // 0x8003C5F4

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
