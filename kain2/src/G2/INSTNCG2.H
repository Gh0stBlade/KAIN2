#ifndef INSTNCG2_H
#define INSTNCG2_H


extern void G2Instance_BuildTransformsForList(struct _Instance *listHead); // 0x80094D90

extern void G2Instance_BuildTransforms(struct _Instance *instance); // 0x80094F0C

extern void G2Instance_RebuildTransforms(struct _Instance *instance); // 0x80094F68

extern struct _G2AnimKeylist_Type * G2Instance_GetKeylist(struct _Instance *instance, int id); // 0x80094FC4

extern void _G2Instance_RebuildAnimatedTransforms(struct _Instance *instance); // 0x80094FE4

extern void G2Instance_ClearMatrices(struct _Instance *instance); // 0x80095418

extern void _G2Instance_BuildAnimatedTransforms(struct _Instance *instance); // 0x8009546C

extern void _G2Instance_RebuildNonAnimatedTransforms(struct _Instance *instance); // 0x80095528

extern void _G2Instance_BuildDeactivatedTransforms(struct _Instance *instance); // 0x800957B4

extern void _G2Instance_BuildNonAnimatedTransforms(struct _Instance *instance); // 0x80095934

extern void _G2Instance_BuildFacadeTransforms(struct _Instance *instance, struct _Segment *segment, struct MATRIX *segMatrix, struct MATRIX *matrixPool, long scale_flag); // 0x800959F0


#endif
