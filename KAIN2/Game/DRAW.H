#ifndef DRAW_H
#define DRAW_H

#include "VRAM.H"
#include "CORE.H"

struct UVTYPE 
{
	unsigned char u; // size=0, offset=0
	unsigned char v; // size=0, offset=1
	short pad; // size=0, offset=2
};

struct SP_SUBDIV_STRUCT 
{
	struct _SVector vertex0; // size=8, offset=0
	struct _SVector vertex1; // size=8, offset=8
	struct _SVector vertex2; // size=8, offset=16
	SVECTOR face_v0; // size=8, offset=24
	SVECTOR face_v1; // size=8, offset=32
	SVECTOR face_v2; // size=8, offset=40
	SVECTOR face_v01; // size=8, offset=48
	SVECTOR face_v12; // size=8, offset=56
	SVECTOR face_v20; // size=8, offset=64
	struct UVTYPE face_uv0; // size=4, offset=72
	struct UVTYPE face_uv1; // size=4, offset=76
	struct UVTYPE face_uv2; // size=4, offset=80
	struct UVTYPE face_uv01; // size=4, offset=84
	struct UVTYPE face_uv12; // size=4, offset=88
	struct UVTYPE face_uv20; // size=4, offset=92
	struct _ColorType color0; // size=4, offset=96
	struct _ColorType color1; // size=4, offset=100
	struct _ColorType color2; // size=4, offset=104
	struct _ColorType color01; // size=4, offset=108
	struct _ColorType color12; // size=4, offset=112
	struct _ColorType color20; // size=4, offset=116
	struct _Vector out[4]; // size=48, offset=120
	long color[4]; // size=16, offset=168
	long texinfo[3]; // size=12, offset=184
	DVECTOR sxy[4]; // size=16, offset=196
};

typedef struct POLY_F4_SEMITRANS 
{
#if defined(USE_32_BIT_ADDR)
	unsigned long tag;
#if defined(PGXP)
	unsigned short len;
	unsigned short pgxp_index;
#else
	unsigned long len;
#endif
#else
	unsigned long tag;
#endif

	unsigned long dr_tpage; // size=0, offset=4
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
	short x0; // size=0, offset=12
	short y0; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	short x2; // size=0, offset=20
	short y2; // size=0, offset=22
	short x3; // size=0, offset=24
	short y3; // size=0, offset=26
} POLY_F4_SEMITRANS;

extern short pause_redraw_flag; // offset 0x800d2fd4
extern void* pause_redraw_prim; // offset 0x800d2fd8
extern short RENDER_currentStreamUnitID;
extern int InStreamUnit;
extern unsigned long** hackOT;

extern void fDRAW_SPLIT_INTPL_XYZ(struct _SVector *newVertex, struct _SVector *pvb, struct _SVector *pvc); // 0x8002A5B0

extern void DRAW_InitShadow(); // 0x8002A62C

extern unsigned long * DRAW_DrawShadow(struct _PrimPool *primPool, struct _Model *model, unsigned long **ot, long fadeValue); // 0x8002A6F4

extern void DRAW_FlatQuad(CVECTOR *color, short x0, short y0, short x1, int y1, int x2, int y2, int x3, int y3, struct _PrimPool *primPool, unsigned long **ot); // 0x8002A8B4

extern void DRAW_TranslucentQuad(short x0, short y0, short x1, short y1, int x2, int y2, int x3, int y3, int r, int g, int b, int abr, struct _PrimPool *primPool, unsigned long **ot); // 0x8002A95C

extern void DRAW_DrawButton(struct _ButtonTexture *button, short x, short y, unsigned long **ot); // 0x8002AA40

extern void DRAW_LoadButton(long *addr, struct _ButtonTexture *button); // 0x8002ABA8

extern void DRAW_FreeButton(struct _ButtonTexture *button); // 0x8002AD48

extern void DRAW_RingLine(struct _PrimPool *primPool, unsigned long **ot, long color); // 0x8002AD6C

extern void DRAW_RingPoint(struct _PrimPool *primPool, unsigned long **ot, long color, struct _SVector *vel, struct _SVector *acc); // 0x8002AE9C

extern unsigned long * DRAW_DrawRingPoints(struct _Model *model, struct _VertexPool *vertexPool, MATRIX *pcTransform, struct _PrimPool *primPool, unsigned long **ot, long color, int ring_type); // 0x8002AFAC

extern void DRAW_GlowQuad(struct _PrimPool *primPool, unsigned long **ot, long otz, long color, struct _Vector *v0, struct _Vector *v1, struct _Vector *v2, struct _Vector *v3); // 0x8002B2C8

extern void DRAW_CreateAGlowingCircle(struct _Vector *f1, long z, struct _PrimPool *primPool, unsigned long **ot, long otz, long color, long w, long h, long angle); // 0x8002B52C

extern unsigned long * DRAW_DrawGlowPoints2(struct _Instance *instance, long seg1, long seg2, struct _PrimPool *primPool, unsigned long **ot, long color, long height); // 0x8002BAD8

extern unsigned long * DRAW_DrawGlowPoint(struct _Instance *instance, long seg1, struct _PrimPool *primPool, unsigned long **ot, long color, int width, int height); // 0x8002BC44

extern int DRAW_DisplayTFace_zclipped_C(SVECTOR*vertex0, SVECTOR*vertex1, SVECTOR*vertex2, struct UVTYPE *uv0, struct UVTYPE *uv1, struct UVTYPE *uv2, long color0, long color1, long color2, struct _PrimPool *primPool, unsigned long **ot, int ndiv); // 0x8002BD14

extern long * DRAW_Zclip_subdiv(POLY_GT3 *texture, unsigned long **ot, int ndiv); // 0x8002C554

extern void DRAW_LoadingMessage(); // 0x8002CA58


#endif
