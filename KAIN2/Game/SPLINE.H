#ifndef SPLINE_H
#define SPLINE_H

#include "GEX2.H"

struct SplineKey 
{
	short count; // size=0, offset=0
	struct vecS point; // size=6, offset=2
	struct vecL dd; // size=12, offset=8
	struct vecL ds; // size=12, offset=20
};

struct Spline 
{
	struct SplineKey* key; // size=32, offset=0
	short numkeys; // size=0, offset=4
	unsigned char type; // size=0, offset=6
	unsigned char flags; // size=0, offset=7
};

struct SplineRotKey 
{
	short count; // size=0, offset=0
	struct _G2Quat_Type q; // size=8, offset=2
};

struct RSpline 
{
	struct SplineRotKey* key; // size=10, offset=0
	short numkeys; // size=0, offset=4
	unsigned char type; // size=0, offset=6
	unsigned char flags; // size=0, offset=7
};

struct SplineDef 
{
	short currkey; // size=0, offset=0
	unsigned short denomFlag; // size=0, offset=2
	long fracCurr; // size=0, offset=4
};

struct MultiSpline 
{
	struct Spline* positional; // size=8, offset=0
	struct RSpline* rotational; // size=8, offset=4
	struct Spline* scaling; // size=8, offset=8
	struct Spline* color; // size=8, offset=12
	struct SplineDef curPositional; // size=8, offset=16
	struct SplineDef curRotational; // size=8, offset=24
	struct SplineDef curScaling; // size=8, offset=32
	struct SplineDef curColor; // size=8, offset=40
	//struct MATRIX curRotMatrix; // size=32, offset=48
	MATRIX curRotMatrix; // size=32, offset=48
};

struct InstanceSpline 
{
	struct _Instance* instance; // size=668, offset=0
	struct MultiSpline* spline; // size=80, offset=4
	long attribute; // size=0, offset=8
	long isParent; // size=0, offset=12
	long isClass; // size=0, offset=16
	long splineFlags; // size=0, offset=20
};

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
