#ifndef INSTANCE_H
#define INSTANCE_H


extern void INSTANCE_ClearTfaces(struct _Instance *instance); // 0x80031ED8

extern void INSTANCE_Deactivate(struct _Instance *instance); // 0x80031F04

extern void INSTANCE_Reactivate(struct _Instance *instance); // 0x80031FC0

extern void INSTANCE_ForceActive(struct _Instance *instance); // 0x80032058

extern void INSTANCE_DeactivatedProcess(struct _Instance *instance, struct GameTracker *gameTracker); // 0x8003208C

extern void INSTANCE_DeactivateFarInstances(struct GameTracker *gameTracker); // 0x80032094

extern void INSTANCE_InitInstanceList(struct _InstanceList *list, struct _InstancePool *pool); // 0x80032310

extern struct _Instance * INSTANCE_NewInstance(struct _InstanceList *list); // 0x800323B0

extern long INSTANCE_InstanceGroupNumber(struct _Instance *instance); // 0x8003243C

extern void INSTANCE_InsertInstanceGroup(struct _InstanceList *list, struct _Instance *instance); // 0x80032558

extern void INSTANCE_ReallyRemoveInstance(struct _InstanceList *list, struct _Instance *instance, long reset); // 0x800325A0

extern void INSTANCE_CleanUpInstanceList(struct _InstanceList *list, long reset); // 0x800327E4

extern long INSTANCE_Introduced(struct Intro *intro, short streamUnitID); // 0x800328A0

extern struct INICommand * INSTANCE_GetIntroCommand(struct INICommand *command, int cmd); // 0x80032958

extern struct INICommand * INSTANCE_FindIntroCommand(struct _Instance *instance, int cmd); // 0x800329A8

extern void INSTANCE_ProcessIntro(struct _Instance *instance); // 0x800329CC

extern void INSTANCE_InitEffects(struct _Instance *instance, struct Object *object); // 0x80032A48

extern struct _Instance * INSTANCE_IntroduceInstance(struct Intro *intro, short streamUnitID); // 0x80032AD0

extern void INSTANCE_AdditionalCollideFunctions(struct _InstanceList *instanceList); // 0x800331F0

extern long INSTANCE_GetSplineFrameNumber(struct _Instance *instance, struct MultiSpline *spline); // 0x80033318

extern void INSTANCE_ProcessFunctions(struct _InstanceList *instanceList); // 0x80033350

extern struct _Instance * INSTANCE_BirthObject(struct _Instance *parent, struct Object *object, int modelNum); // 0x80033BFC

extern void INSTANCE_BuildStaticShadow(struct _Instance *instance); // 0x80033E6C

extern void INSTANCE_DefaultInit(struct _Instance *instance, struct Object *object, int modelNum); // 0x80033E74

extern void INSTANCE_PlainDeath(struct _Instance *instance); // 0x80034230

extern void INSTANCE_KillInstance(struct _Instance *instance); // 0x80034360

extern unsigned long INSTANCE_Query(struct _Instance *Inst, int Query); // 0x80034394

extern void INSTANCE_Post(struct _Instance *Inst, int Message, int Data); // 0x800343D0

extern void INSTANCE_Broadcast(struct _Instance *sender, long whatAmIMask, int Message, int Data); // 0x80034434

extern int INSTANCE_InPlane(struct _Instance *instance, int plane); // 0x800344E0

extern long INSTANCE_FindWithID(long uniqueID); // 0x8003453C

extern struct _Instance * INSTANCE_FindWithName(long areaID, char *instanceName, struct _Instance *startInstance); // 0x8003457C

extern struct Intro * INSTANCE_FindIntro(long areaID, long introUniqueID); // 0x80034650

extern struct _Instance * INSTANCE_Find(long introUniqueID); // 0x800346D4

extern struct _Instance * INSTANCE_IntroduceSavedInstance(struct _SavedIntro *savedIntro, struct _StreamUnit *streamUnit, int *deleted); // 0x80034714

extern struct _Instance * INSTANCE_IntroduceSavedInstanceWithIntro(struct _SavedIntroWithIntro *savedIntro, struct _StreamUnit *streamUnit, int *deleted); // 0x80034B8C

extern void INSTANCE_SpatialRelationships(struct _InstanceList *instanceList); // 0x80035000

extern int INSTANCE_SetStatsData(struct _Instance *instance, struct _Instance *checkee, struct _Vector *checkPoint, struct evCollideInstanceStatsData *data, struct MATRIX *mat); // 0x80035164

extern void INSTANCE_LinkToParent(struct _Instance *instance, struct _Instance *parent, int node); // 0x80035274

extern void INSTANCE_UnlinkFromParent(struct _Instance *instance); // 0x80035330

extern void INSTANCE_UnlinkChildren(struct _Instance *instance); // 0x80035494

extern void INSTANCE_UpdateFamilyStreamUnitID(struct _Instance *instance); // 0x80035500

extern void INSTANCE_ReallyRemoveAllChildren(struct _Instance *instance); // 0x80035558

extern struct _Instance * INSTANCE_GetChildLinkedToSegment(struct _Instance *instance, int segment); // 0x800355B4

extern int INSTANCE_Linked(struct _Instance *instance1, struct _Instance *instance2); // 0x800355EC

extern int INSTANCE_GetFadeValue(struct _Instance *instance); // 0x8003563C

extern unsigned long INSTANCE_DefaultAnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, struct _Instance *instance); // 0x80035720


#endif
