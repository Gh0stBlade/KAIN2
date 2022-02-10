#ifndef MATH3D_H
#define MATH3D_H


extern void MATH3D_Sort3VectorCoords(long *a, long *b, long *c); // 0x80039478

extern long MATH3D_LengthXYZ(long x, long y, long z); // 0x800394FC

extern long MATH3D_LengthXY(long x, long y); // 0x800395CC

extern void MATH3D_Normalize(struct _Normal *normal); // 0x8003962C

extern short MATH3D_FastAtan2(long y, long x); // 0x800396BC

extern long MATH3D_FastSqrt(long square); // 0x800397D8

extern long MATH3D_FastSqrt0(long square); // 0x800398B8

extern long MATH3D_DistanceBetweenPositions(struct _Position *pos1, struct _Position *pos2); // 0x80039948

extern short MATH3D_AngleBetweenVectors(struct _SVector *vector1, struct _SVector *vector2); // 0x80039994

extern void MATH3D_RotMatAboutVec(struct _SVector *vec, struct MATRIX *mat, short angle); // 0x80039A80

extern void MATH3D_SetUnityMatrix(struct MATRIX *mat); // 0x80039B50

extern void AngleMoveToward(short *current_ptr, short destination, short step); // 0x80039B6C

extern short AngleDiff(short current, short destination); // 0x80039C18

extern short MATH3D_AngleFromPosToPos(struct _Position *from, struct _Position *to); // 0x80039C3C

extern void MATH3D_ZYXtoXYZ(struct _Rotation *rot); // 0x80039C74

extern short MATH3D_ElevationFromPosToPos(struct _Position *from, struct _Position *to); // 0x80039CD0

extern void MATH3D_RotationFromPosToPos(struct _Position *from, struct _Position *to, struct _Rotation *rot); // 0x80039D54

extern int MATH3D_veclen2(int ix, int iy); // 0x80039DA8

extern void MATH3D_RotateAxisToVector(struct MATRIX *dest, struct MATRIX *src, struct _SVector *vec, enum MATH3D_AXIS axis); // 0x80039E08

extern int MATH3D_ConeDetect(struct _SVector *pos, int arc, int elevation); // 0x8003A060

extern void MATH3D_CrossProduct(struct _SVector *t, struct _SVector *r, struct _SVector *s); // 0x8003A118

extern unsigned long MATH3D_SquareLength(long x, long y, long z); // 0x8003A1BC


#endif
