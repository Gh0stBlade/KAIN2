#ifndef ANMCTRLR_H
#define ANMCTRLR_H

#include "Game/GEX2.H"
#include "ANIMG2.H"

extern void G2Anim_AttachControllerToSeg(struct _G2Anim_Type *anim, int segNumber, int type); // 0x80090318
extern void G2Anim_DetachControllerFromSeg(struct _G2Anim_Type *anim, int segNumber, int type); // 0x80090350
extern void G2Anim_EnableController(struct _G2Anim_Type *anim, int segNumber, int type); // 0x80090418
extern void G2Anim_DisableController(struct _G2Anim_Type *anim, int segNumber, int type); // 0x800904F4
extern void G2Anim_InterpDisableController(struct _G2Anim_Type *anim, int segNumber, int type, short duration); // 0x80090534
extern enum _G2Bool_Enum G2Anim_IsControllerActive(struct _G2Anim_Type *anim, int segNumber, int type); // 0x80090654
extern enum _G2Bool_Enum G2Anim_IsControllerInterpolating(struct _G2Anim_Type *anim, int segNumber, int type); // 0x800906A0
extern void G2Anim_GetControllerCurrentInterpVector(struct _G2Anim_Type *anim, int segNumber, int type, struct _G2SVector3_Type *vector); // 0x800906F4
extern void G2Anim_SetControllerCallbackData(struct _G2Anim_Type *anim, int segNumber, int type, void *callbackData); // 0x80090854
extern void G2Anim_SetControllerAngleOrder(struct _G2Anim_Type *anim, int segNumber, int type, int order); // 0x8009087C
extern void G2Anim_SetController_Vector(struct _G2Anim_Type *anim, int segNumber, int type, struct _G2SVector3_Type *vector); // 0x800908B4
extern void G2Anim_SetInterpController_Vector(struct _G2Anim_Type *anim, int segNumber, int type, struct _G2SVector3_Type *vector, int duration); // 0x800908FC
extern void G2Anim_SetInterpController_Quat(struct _G2Anim_Type *anim, int segNumber, int type, struct _G2Quat_Type *quat, int duration); // 0x80090A60
extern void _G2Anim_ApplyControllersToStoredFrame(struct _G2Anim_Type *anim); // 0x80090ADC
extern void _G2Anim_BuildTransformsWithControllers(struct _G2Anim_Type *anim); // 0x80090B3C
extern void _G2Anim_BuildSegTransformWithControllers(struct _G2Matrix_Type *segMatrix, struct _G2Matrix_Type *parentMatrix, struct _G2AnimController_Type *controller, enum _G2Bool_Enum bRootTransUpdated, int segIndex); // 0x80090D1C
extern unsigned long _G2AnimController_ApplyToSegValue(struct _G2AnimController_Type *controller, struct _G2AnimSegValue_Type *segValue, struct _G2Matrix_Type *segMatrix, struct _G2Matrix_Type *parentMatrix); // 0x80091028
extern void _G2Anim_UpdateControllers(struct _G2Anim_Type *anim); // 0x800913D0
extern void _G2Anim_CopyVectorWithOrder(struct _G2SVector3_Type *sourceVector, struct _G2EulerAngles_Type *destVector, int order); // 0x8009155C
extern void _G2AnimSection_ApplyControllersToStoredFrame(struct _G2AnimSection_Type *section); // 0x800915C0
extern unsigned long _G2AnimController_ApplyWorldToParentMatrix(struct _G2AnimController_Type *controller, struct _G2Matrix_Type *parentMatrix); // 0x80091834
extern void _G2AnimController_GetMatrix(struct _G2AnimController_Type *controller, struct _G2Matrix_Type *matrix); // 0x80091988
extern void _G2AnimController_GetVector(struct _G2AnimController_Type *controller, struct _G2SVector3_Type *vector); // 0x80091A60
extern struct _G2AnimController_Type * _G2Anim_FindController(struct _G2Anim_Type *anim, int segNumber, int type); // 0x80091AE8
extern struct _G2AnimController_Type * _G2AnimController_Create(int segNumber, int type); // 0x80091B44
extern struct _G2AnimController_Type * _G2AnimController_Destroy(struct _G2AnimController_Type *controller); // 0x80091BAC
extern void _G2AnimController_InsertIntoList(struct _G2AnimController_Type *controller, unsigned short *listPtr); // 0x80091C10
extern void _G2AnimController_GetCurrentInterpQuat(struct _G2AnimController_Type *controller, struct _G2Anim_Type *anim, struct _G2Quat_Type *quat); // 0x80091D40
extern void _G2AnimController_GetSimpleWorldRotQuat(struct _G2AnimController_Type *controller, struct _G2Anim_Type *anim, struct _G2Quat_Type *quat); // 0x80091E7C
extern struct _G2AnimController_Type * _G2AnimControllerST_FindInList(int segNumber, int type, unsigned short *listPtr); // 0x80091EFC
extern struct _G2AnimController_Type * _G2AnimControllerST_FindPtrInList(int segNumber, int type, unsigned short **listPtrPtr); // 0x80091F6C
extern struct _G2AnimController_Type * _G2AnimControllerST_RemoveFromList(int segNumber, int type, unsigned short *listPtr); // 0x8009200C

#endif
