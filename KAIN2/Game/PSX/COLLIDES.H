#ifndef COLLIDES_H
#define COLLIDES_H

#include "Game/CAMERA.H"

#include "Game/STREAM.H"

#include "DRAWS.H"

#include "Game/BSP.H"

struct _VMVertex
{
	short count;
	short timer;
};

struct _BGObject_Face
{
	short v0;
	short v1;
	short v2;
	short unknown2;
};

struct _VM_Face
{
	short v0;
	short v1;
	short v2;
	short v3;
	short v4;
};

extern void COLLIDE_NearestPointOnLine_S(struct _SVector* linePoint, SVECTOR* oldPoint, SVECTOR* newPoint, struct _Position* position) ; // 0x80077F3C

extern long COLLIDE_IntersectLineAndPlane_S(struct _SVector* planePoint, struct _Position* oldPos, struct _Position* position, struct _SVector* normal, long z) ; // 0x8007804C

extern void COLLIDE_NearestPointOnPlane_S(struct _SVector* planePoint, struct _SVector* normal, long constant, struct _SVector* point); // 0x80078124

extern void VM_ProcessVMObjectSetColor_S(struct Level* level, struct _VMObject* vmobject); // 0x800786A8

extern void VM_ProcessVMObjectList_S(struct Level* level, struct Camera* camera); // 0x8007871C

extern long BSP_SphereIntersectsViewVolume_S(struct _Sphere* sphere, struct Camera* camera); // 0x800787E0

extern void RotMatrixZYX(SVECTOR* r, MATRIX* m); // 0x800788B8

extern void rmzyx_1() ; // 0x800788F4

extern void rmzyx_2() ; // 0x80078928

extern void rmzyx_3() ; // 0x80078950

extern void rmzyx_onlyz() ; // 0x80078A20

//extern void RotMatrix() ; // 0x80078A4C

extern void rm_1() ; // 0x80078A88

extern void rm_2() ; // 0x80078ABC

extern void rm_3() ; // 0x80078AE4

extern void RotMatrixX(long r, MATRIX* m);
extern void RotMatrixY(long r, MATRIX* m);
extern void RotMatrixZ(long r, MATRIX* m);

extern void RMX_0() ; // 0x80078BE8

extern void RMY_0() ; // 0x80078D1C

extern void RMZ_0() ; // 0x80078E50

extern void ApplyMatrix_S() ; // 0x80078F60

//extern void ScaleMatrix() ; // 0x80078FB0

extern short rsin(long a); // 0x80079098

extern void rsin_exit() ; // 0x800790C0

extern short rcos(long a); // 0x800790C8

extern void rcos_exit() ; // 0x800790F0

#if 0
extern long MATH3D_racos_S(long a); // 0x800790F8
#else
extern short MATH3D_racos_S(short cos_ang_dest_act); // 0x800790F8
#endif

extern void SetGp() ; // 0x80079158

//extern void memcpy() ; // 0x80079160

extern void BSP_MarkVisibleLeaves_S(struct BSPTree* bsp, struct Camera* camera, struct _PolytopeList* polytopeList, unsigned int** drawot, int curTree, struct _Instance* savedLightInstance, struct _Terrain* terrain, struct GameTracker* gameTracker, struct _StreamUnit* currentUnit); // 0x80079218

extern void G2Quat_FromMatrix_S() ; // 0x800797E8

extern void G2Quat_ToMatrix_S(struct _G2Quat_Type* quat, struct _G2Matrix_Type* matrix); // 0x800799F4

extern void G2Quat_FromEuler_S(struct _G2Quat_Type* quatInfo, struct _G2EulerAngles_Type* preQuat) ; // 0x80079AE4

extern void eul_1() ; // 0x80079B50

extern void eul_2() ; // 0x80079B78

extern void eul_3() ; // 0x80079BA0

extern void G2EulerAngles_ToMatrix_S(struct _G2Quat_Type* quat, struct _G2Matrix_Type* matrix);

#endif
