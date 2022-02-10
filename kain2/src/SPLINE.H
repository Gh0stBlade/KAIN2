#ifndef SPLINE_H
#define SPLINE_H


extern void _SplineS2Pos(struct vecS *p, long s, struct SplineKey *key, struct SplineKey *key2); // 0x80040C18

extern void SplineSetDefDenom(struct Spline *spline, struct SplineDef *def, int denomFlag); // 0x80040D88

extern unsigned short SplineGetFrameNumber(struct Spline *spline, struct SplineDef *def); // 0x80040E4C

extern short SplineSetDef2FrameNumber(struct Spline *spline, struct SplineDef *def, unsigned short frame_number); // 0x80040EE0

extern short SplineIsWhere(struct Spline *spline, struct SplineDef *def); // 0x80041010

extern short SplineMultiIsWhere(struct MultiSpline *multi); // 0x80041078

extern struct _G2Quat_Type * SplineGetFirstRot(struct RSpline *rspline, struct SplineDef *def); // 0x80041104

extern struct _SVector * SplineGetFirstPoint(struct Spline *spline, struct SplineDef *def); // 0x8004112C

extern struct _SVector * SplineGetNextPoint(struct Spline *spline, struct SplineDef *def); // 0x80041154

extern struct _SVector * SplineGetPreviousPoint(struct Spline *spline, struct SplineDef *def); // 0x800411AC

extern struct _SVector * SplineGetNearestPoint(struct Spline *spline, struct _SVector *point, struct SplineDef *def); // 0x80041204

extern unsigned long SplineGetData(struct Spline *spline, struct SplineDef *def, void *p); // 0x800415C0

extern unsigned long SplineGetQuatData(struct Spline *spline, struct SplineDef *def, void *p); // 0x800417D0

extern unsigned long SplineGetNext(struct Spline *spline, struct SplineDef *def); // 0x80041904

extern unsigned long SplineGetPrev(struct Spline *spline, struct SplineDef *def); // 0x80041A20

extern unsigned long SplineGetOffsetNext(struct Spline *spline, struct SplineDef *def, long fracOffset); // 0x80041B4C

extern unsigned long SplineGetOffsetPrev(struct Spline *spline, struct SplineDef *def, long fracOffset); // 0x80041D30

extern struct _SVector * SplineGetOffsetNextPoint(struct Spline *spline, struct SplineDef *def, long offset); // 0x80041EA4

extern struct _SVector * SplineGetOffsetPreviousPoint(struct Spline *spline, struct SplineDef *def, long offset); // 0x80041EFC


#endif
