#ifndef LIGHT3D_H
#define LIGHT3D_H

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

#include "CORE.H"

struct CDLight 
{
	struct NodeType node; // size=8, offset=0
	unsigned char r; // size=0, offset=8
	unsigned char g; // size=0, offset=9
	unsigned char b; // size=0, offset=10
	unsigned char flags; // size=0, offset=11
	struct _Sphere sphere; // size=12, offset=12
};

extern void LIGHT_GetLightMatrix(struct _Instance *instance, struct Level *level, MATRIX *lightM, MATRIX *colorM); // 0x80035824
extern void LIGHT_PresetInstanceLight(struct _Instance *instance, short attenuate, MATRIX *lm); // 0x80035B98
extern void LIGHT_GetAmbient(struct _ColorType *color, struct _Instance *instance); // 0x80035D14
extern void LIGHT_CalcLightValue(struct _TFace *tface, struct _Instance *instance, struct _Terrain *terrain); // 0x80035D44
extern void LIGHT_SetAmbientInstance(struct _Instance *instance, struct Level *level); // 0x80036498
extern void LIGHT_SetMatrixForLightGroupInstance(struct _Instance *instance, struct Level *level); // 0x800364C8
extern void LIGHT_DrawShadow(MATRIX *wcTransform, struct _Instance *instance, struct _PrimPool *primPool, unsigned int **ot); // 0x80036644
extern void LIGHT_CalcShadowPositions(struct GameTracker *gameTracker); // 0x80036928
extern void LIGHT_Restore(struct LightInfo *lightInfo); // 0x80036C28
extern void LIGHT_CalcDQPTable(struct Level *level); // 0x80036C30

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
