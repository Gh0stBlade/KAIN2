#ifndef MATH3D_H
#define MATH3D_H

typedef struct _G2EulerAngles_Type G2EulerAngles;

enum MATH3D_AXIS : int 
{
	AXIS_X = 0,
	AXIS_Y = 1,
	AXIS_Z = 2,
	AXIS_NEG_X = 3,
	AXIS_NEG_Y = 4,
	AXIS_NEG_Z = 5,
};

#define compare(a,b) ( a-b<0?b-a:a-b )

#define POW2(x) ((x) * (x))

#define MATH3D_AddPosToPos(pos0, pos1) \
{ \
    short _x0, _y0, _z0; \
    short _x1, _y1, _z1; \
    struct _Position* _v; \
    _v = pos0; \
	_x0 = _v->x; \
	_y0 = _v->y; \
	_z0 = _v->z; \
	_x1 = (short)pos1->x; \
	_y1 = (short)pos1->y; \
	_z1 = (short)pos1->z; \
	_v->x = _x0 + _x1; \
	_v->y = _y0 + _y1; \
	_v->z = _z0 + _z1; \
}

#define ADD_VEC(A, B, C) {\
short _x0;\
short _y0;\
short _z0;\
short _x1;\
short _y1;\
short _z1;\
struct _SVector *_v;\
struct _Position *_v0;\
struct _Position *_v1;\
\
_v0 = B;\
_v1 = C;\
_v = A;\
\
_x0 = _v0->x;\
_y0 = _v0->y;\
_z0 = _v0->z;\
\
_x1 = _v1->x;\
_y1 = _v1->y;\
_z1 = _v1->z;\
\
_v->x = _x0 + _x1;\
_v->y = _y0 + _y1;\
_v->z = _z0 + _z1;\
}

#define SUB_VEC(A, B, C) {\
short _x0;\
short _y0;\
short _z0;\
short _x1;\
short _y1;\
short _z1;\
struct _SVector *_v;\
struct _Position *_v0;\
struct _Position *_v1;\
\
_v0 = B;\
_v1 = C;\
_v = A;\
\
_x0 = _v0->x;\
_y0 = _v0->y;\
_z0 = _v0->z;\
\
_x1 = _v1->x;\
_y1 = _v1->y;\
_z1 = _v1->z;\
\
_v->x = _x0 - _x1;\
_v->y = _y0 - _y1;\
_v->z = _z0 - _z1;\
}

#define SET_VEC(A, B) {\
short _x0;\
short _y0;\
short _z0;\
struct _SVector *_v;\
struct _Position *_v0;\
\
_v0 = B;\
_v = A;\
\
_x0 = _v0->x;\
_y0 = _v0->y;\
_z0 = _v0->z;\
\
\
_v->x = _x0;\
_v->y = _y0;\
_v->z = _z0;\
}

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern void MATH3D_Sort3VectorCoords(long *a, long *b, long *c); // 0x80039478
extern long MATH3D_LengthXYZ(long x, long y, long z); // 0x800394FC
extern long MATH3D_LengthXY(long x, long y); // 0x800395CC
extern void MATH3D_Normalize(struct _Normal *normal); // 0x8003962C
extern short MATH3D_FastAtan2(long y, long x); // 0x800396BC
extern long MATH3D_FastSqrt(long square); // 0x800397D8
extern long MATH3D_FastSqrt0(long square); // 0x800398B8
extern long MATH3D_DistanceBetweenPositions(_Position *pos1, _Position *pos2); // 0x80039948
extern short MATH3D_AngleBetweenVectors(struct _SVector *vector1, struct _SVector *vector2); // 0x80039994
extern void MATH3D_RotMatAboutVec(struct _SVector *vec, MATRIX *mat, short angle); // 0x80039A80
extern void MATH3D_SetUnityMatrix(MATRIX *mat); // 0x80039B50
extern void AngleMoveToward(short *current_ptr, short destination, short step); // 0x80039B6C
extern short AngleDiff(short current, short destination); // 0x80039C18
extern short MATH3D_AngleFromPosToPos(_Position *from, _Position *to); // 0x80039C3C
extern void MATH3D_ZYXtoXYZ(struct _Rotation *rot); // 0x80039C74
extern short MATH3D_ElevationFromPosToPos(_Position *from, _Position *to); // 0x80039CD0
extern void MATH3D_RotationFromPosToPos(_Position *from, _Position *to, struct _Rotation *rot); // 0x80039D54
extern int MATH3D_veclen2(int ix, int iy); // 0x80039DA8
extern void MATH3D_RotateAxisToVector(MATRIX *dest, MATRIX *src, struct _SVector *vec, enum MATH3D_AXIS axis); // 0x80039E08
extern int MATH3D_ConeDetect(struct _SVector *pos, int arc, int elevation); // 0x8003A060
extern void MATH3D_CrossProduct(struct _SVector *t, struct _SVector *r, struct _SVector *s); // 0x8003A118
extern unsigned long MATH3D_SquareLength(long x, long y, long z); // 0x8003A1BC

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
