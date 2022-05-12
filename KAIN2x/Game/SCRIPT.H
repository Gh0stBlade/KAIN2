#ifndef SCRIPT_H
#define SCRIPT_H


extern void SCRIPT_CombineEulerAngles(struct _Rotation *combinedRotation, struct _Rotation *inputRotation1, struct _Rotation *inputRotation2); // 0x8003C6B0

extern void SCRIPT_InstanceSplineInit(struct _Instance *instance); // 0x8003C730

extern short SCRIPTCountFramesInSpline(struct _Instance *instance); // 0x8003C980

extern struct Spline * ScriptGetPosSpline(struct _Instance *instance); // 0x8003CAD0

extern struct RSpline * ScriptGetRotSpline(struct _Instance *instance); // 0x8003CB08

extern int SCRIPT_GetSplineFrameNumber(struct _Instance *instance, struct SplineDef *splineDef); // 0x8003CB40

extern struct MultiSpline * SCRIPT_GetMultiSpline(struct _Instance *instance, unsigned long *isParent, unsigned long *isClass); // 0x8003CBA0

extern struct SplineDef * SCRIPT_GetPosSplineDef(struct _Instance *instance, struct MultiSpline *multi, unsigned long isParent, unsigned long isClass); // 0x8003CC4C

extern struct SplineDef * SCRIPT_GetRotSplineDef(struct _Instance *instance, struct MultiSpline *multi, unsigned long isParent, unsigned long isClass); // 0x8003CC78

extern struct SplineDef * SCRIPT_GetScaleSplineDef(struct _Instance *instance, struct MultiSpline *multi, unsigned long isParent, unsigned long isClass); // 0x8003CCA4

extern void SCRIPT_RelativisticSpline(struct _Instance *instance, struct _SVector *point); // 0x8003CCD0

extern void SCRIPT_InstanceSplineSet(struct _Instance *instance, short frameNum, struct SplineDef *splineDef, struct SplineDef *rsplineDef, struct SplineDef *ssplineDef); // 0x8003CEC0

extern long SCRIPT_SplineProcess(struct _Instance *instance, struct MultiSpline *multi, struct SplineDef *splineDef, struct SplineDef *rsplineDef, struct SplineDef *ssplineDef, int direction, int isClass); // 0x8003D198

extern long SCRIPT_InstanceSplineProcess(struct _Instance *instance, struct SplineDef *splineDef, struct SplineDef *rsplineDef, struct SplineDef *ssplineDef, int direction); // 0x8003D480

extern void SCRIPT_FadeOutProcess(struct _Instance *instance); // 0x8003D588

extern void ScriptKillInstance(struct _Instance *instance, int effect); // 0x8003D5D0


#endif
