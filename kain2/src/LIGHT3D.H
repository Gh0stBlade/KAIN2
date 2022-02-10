#ifndef LIGHT3D_H
#define LIGHT3D_H


extern void LIGHT_GetLightMatrix(struct _Instance *instance, struct Level *level, struct MATRIX *lightM, struct MATRIX *colorM); // 0x80035824

extern void LIGHT_PresetInstanceLight(struct _Instance *instance, short attenuate, struct MATRIX *lm); // 0x80035B98

extern void LIGHT_GetAmbient(struct _ColorType *color, struct _Instance *instance); // 0x80035D14

extern void LIGHT_CalcLightValue(struct _TFace *tface, struct _Instance *instance, struct _Terrain *terrain); // 0x80035D44

extern void LIGHT_SetAmbientInstance(struct _Instance *instance, struct Level *level); // 0x80036498

extern void LIGHT_SetMatrixForLightGroupInstance(struct _Instance *instance, struct Level *level); // 0x800364C8

extern void LIGHT_DrawShadow(struct MATRIX *wcTransform, struct _Instance *instance, struct _PrimPool *primPool, unsigned long **ot); // 0x80036644

extern void LIGHT_CalcShadowPositions(struct GameTracker *gameTracker); // 0x80036928

extern void LIGHT_Restore(struct LightInfo *lightInfo); // 0x80036C28

extern void LIGHT_CalcDQPTable(struct Level *level); // 0x80036C30


#endif
