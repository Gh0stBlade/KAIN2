#ifndef STREAM_H
#define STREAM_H


extern void STREAM_FillOutFileNames(char *baseAreaName, char *dramName, char *vramName, char *sfxName); // 0x800584E4

extern void STREAM_AbortAreaLoad(char *baseAreaName); // 0x800585A4

extern void STREAM_Init(); // 0x800585EC

extern int FindObjectName(char *name); // 0x80058618

extern struct _ObjectTracker * FindObjectInTracker(struct Object *object); // 0x80058684

extern struct _StreamUnit * FindStreamUnitFromLevel(struct Level *level); // 0x800586D4

extern void STREAM_LoadObjectReturn(void *loadData, void *data, void *data2); // 0x80058724

extern void STREAM_DumpMonster(struct _ObjectTracker *dumpee); // 0x8005882C

extern int STREAM_InList(char *name, char **nameList); // 0x800588A0

extern int STREAM_IsSpecialMonster(char *name); // 0x80058908

extern void STREAM_DumpSomeMonsters(); // 0x8005892C

extern void STREAM_NoMonsters(); // 0x800589BC

extern void STREAM_YesMonsters(); // 0x800589EC

extern int STREAM_IsMonster(char *name); // 0x80058A08

extern int STREAM_TryAndDumpANonResidentObject(); // 0x80058A2C

extern int InsertGlobalObject(char *name, struct GameTracker *gameTracker); // 0x80058A7C

extern struct _ObjectTracker * STREAM_GetObjectTracker(char *name); // 0x80058C18

extern void LoadLevelObjects(struct _StreamUnit *stream); // 0x80058C64

extern long STREAM_IsAnInstanceUsingObject(struct Object *object); // 0x80058D88

extern void STREAM_StreamLoadObjectAbort(void *loadData, void *data, void *data2); // 0x80058DC8

extern void STREAM_DumpLoadingObjects(); // 0x80058DF8

extern void STREAM_DumpObject(struct _ObjectTracker *objectTracker); // 0x80058E58

extern int STREAM_IsObjectInAnyUnit(struct _ObjectTracker *tracker); // 0x80058F40

extern void STREAM_RemoveAllObjectsNotInUse(); // 0x80059000

extern void RemoveAllObjects(struct GameTracker *gameTracker); // 0x800592C0

extern struct Level * STREAM_GetLevelWithID(long id); // 0x80059318

extern struct _StreamUnit * STREAM_GetStreamUnitWithID(long id); // 0x80059370

extern struct _StreamUnit * STREAM_GetStreamUnitWithName(char *name); // 0x800593C0

extern void STREAM_CalculateWaterLevel(struct Level *level); // 0x80059450

extern int STREAM_IsMorphInProgress(); // 0x80059580

extern long STREAM_GetWaterZLevel(struct Level *level, struct _Instance *instance); // 0x80059594

extern void STREAM_SetMainFog(struct _StreamUnit *streamUnit); // 0x80059614

extern void STREAM_SetStreamFog(struct _StreamUnit *streamUnit, short fogNear, short fogFar); // 0x80059668

extern void STREAM_ConnectStream(struct _StreamUnit *streamUnit); // 0x800596F8

extern void STREAM_StreamLoadLevelAbort(void *loadData, void *data, void *data2); // 0x80059B04

extern void STREAM_DoObjectLoadAndDump(struct _StreamUnit *streamUnit); // 0x80059B3C

extern void STREAM_FinishLoad(struct _StreamUnit *streamUnit); // 0x80059B84

extern void STREAM_LoadLevelReturn(void *loadData, void *data, void *data2); // 0x80059CD0

extern void STREAM_StreamLoadLevelReturn(void *loadData, void *data, void *data2); // 0x80059D30

extern void STREAM_UpdateLevelPointer(struct Level *oldLevel, struct Level *newLevel, long sizeOfLevel); // 0x80059E18

extern struct _StreamUnit * STREAM_WhichUnitPointerIsIn(void *pointer); // 0x8005A108

extern void STREAM_UpdateObjectPointer(struct Object *oldObject, struct Object *newObject, long sizeOfObject); // 0x8005A1B4

extern void STREAM_UpdateInstanceCollisionInfo(struct _HModel *oldHModel, struct _HModel *newHModel); // 0x8005A454

extern void STREAM_LoadMainVram(struct GameTracker *gameTracker, char *baseAreaName, struct _StreamUnit *streamUnit); // 0x8005A498

extern void STREAM_MoveIntoNewStreamUnit(); // 0x8005A540

extern struct _StreamUnit * STREAM_LoadLevel(char *baseAreaName, struct StreamUnitPortal *streamPortal, int loadnext); // 0x8005A5D4

extern void RemoveIntroducedLights(struct Level *level); // 0x8005A7F8

extern void STREAM_RemoveInstancesWithIDInInstanceList(struct _InstanceList *list, long id, struct Level *level); // 0x8005A8DC

extern void STREAM_MarkUnitNeeded(long streamID); // 0x8005A988

extern void STREAM_DumpUnit(struct _StreamUnit *streamUnit, long doSave); // 0x8005A9D4

extern void STREAM_DumpAllUnitsNotNeeded(); // 0x8005ABB8

extern void STREAM_DumpAllLevels(long IDNoRemove, int DoSave); // 0x8005AC20

extern void STREAM_LoadCurrentWarpRoom(struct StreamUnitPortal *streamPortal, struct _StreamUnit *mainStreamUnit); // 0x8005AC9C

extern void WARPGATE_RelocateLoadedWarpRooms(struct _StreamUnit *mainUnit, struct StreamUnitPortal *streamPortal); // 0x8005AD58

extern long WARPGATE_GetWarpRoomIndex(char *name); // 0x8005AE00

extern void WARPGATE_UpdateAddToArray(struct _StreamUnit *streamUnit); // 0x8005AE60

extern void WARPGATE_RemoveFromArray(struct _StreamUnit *streamUnit); // 0x8005AEAC

extern void WARPGATE_Init(); // 0x8005AEDC

extern void WARPGATE_StartUsingWarpgate(); // 0x8005AF24

extern void WARPGATE_EndUsingWarpgate(); // 0x8005AF5C

extern int WARPGATE_IsWarpgateInUse(); // 0x8005AF94

extern int WARPGATE_IsWarpgateActive(); // 0x8005AFA0

extern int WARPGATE_IsWarpgateUsable(); // 0x8005AFAC

extern int WARPGATE_IsWarpgateReady(); // 0x8005AFC0

extern int WARPGATE_IsWarpgateSpectral(); // 0x8005AFD4

extern int WARPGATE_IsObjectOnWarpSide(struct _Instance *instance); // 0x8005B010

extern void WARPGATE_IsItActive(struct _StreamUnit *streamUnit); // 0x8005B074

extern long WARPGATE_IsUnitWarpRoom(struct _StreamUnit *streamUnit); // 0x8005B13C

extern void WARPGATE_FixUnit(struct _StreamUnit *streamUnit); // 0x8005B194

extern void STREAM_MarkWarpUnitsNeeded(); // 0x8005B1C8

extern long WARPGATE_IncrementIndex(); // 0x8005B208

extern void WARPGATE_CalcWarpFade(int timeInc); // 0x8005B320

extern long WARPGATE_DecrementIndex(); // 0x8005B390

extern void PreloadAllConnectedUnits(struct _StreamUnit *streamUnit, struct _SVector *offset); // 0x8005B468

extern void RelocateLevel(struct Level *level, struct _SVector *offset); // 0x8005B62C

extern void RelocateCameras(struct _CameraKey *cameraList, long numCameras, struct _SVector *offset); // 0x8005B76C

extern void RelocateSavedCameras(struct Camera *camera, struct Level *level, struct _SVector *offset); // 0x8005B800

extern void RelocateLevelWithInstances(struct Level *level, struct _SVector *offset); // 0x8005B938

extern void RelocateTerrain(struct _Terrain *terrain, struct _SVector *offset); // 0x8005B970

extern void RelocateVMObjects(struct _VMObject *vobjectlist, long numvmobjs, struct _SVector *offset); // 0x8005BA60

extern void RelocateBGObjects(struct _BGObject *BGObjList, long numBGObjs, struct _SVector *offset); // 0x8005BAB8

extern void RelocatePlanPool(struct PlanningNode *planPool, struct _SVector *offset); // 0x8005BB5C

extern void RelocatePlanMarkers(struct _PlanMkr *planMkrList, int numPlanMkrs, struct _SVector *offset); // 0x8005BC10

extern void RelocateSFXMarkers(struct _SFXMkr *sfxMkrList, int numSFXMkrs, struct _SVector *offset); // 0x8005BC64

extern void STREAM_AdjustMultiSpline(struct MultiSpline *multi, struct _SVector *offset); // 0x8005BCB0

extern void STREAM_RelocateInstance(struct _Instance *instance, struct _SVector *offset); // 0x8005BD60

extern void STREAM_OffsetInstancePosition(struct _Instance *instance, struct _SVector *offset, int streamSignalFlag); // 0x8005BDA8

extern void STREAM_SetInstancePosition(struct _Instance *instance, struct evPositionData *data); // 0x8005BEC0

extern void RelocateInstances(struct _SVector *offset); // 0x8005BF18

extern void RelocateStreamPortals(struct StreamUnitPortal *StreamUnitList, int NumStreamUnits, struct _SVector *offset); // 0x8005BF74

extern void STREAM_PackVRAMObject(struct _ObjectTracker *objectTracker); // 0x8005C0A4

extern void MORPH_SetupInstanceFlags(struct _Instance *instance); // 0x8005C1A4

extern void MORPH_SetupInstanceListFlags(); // 0x8005C2D8

extern void MORPH_InMorphInstanceListFlags(); // 0x8005C320

extern void MORPH_InMorphDoFadeValues(); // 0x8005C388

extern void MORPH_UpdateTimeMult(); // 0x8005C3E0

extern void MORPH_UpdateNormals(struct Level *BaseLevel); // 0x8005C554

extern void MORPH_BringBackNormals(struct Level *BaseLevel); // 0x8005CA08

extern void MORPH_AddOffsets(struct Level *BaseLevel, int time); // 0x8005CE78

extern void MORPH_SubtractOffsets(struct Level *BaseLevel, int time); // 0x8005D244

extern void MORPH_GetComponentsForTrackingPoint(struct _TFace *face, struct Level *level); // 0x8005D618

extern void MORPH_AveragePoint(struct _SVector *start, struct _SVector *end, int interp, struct _SVector *out); // 0x8005D8F8

extern void MORPH_UpdateTrackingPoint(struct _TFace *face, struct Level *level); // 0x8005D970

extern void MORPH_ToggleMorph(); // 0x8005DB14

extern void MORPH_DoStep(struct _StreamUnit *streamUnit, char *baseAreaName); // 0x8005DC60

extern void MORPH_SetFog(struct _StreamUnit *streamUnit, int mainUnitFlag); // 0x8005DD3C

extern void MORPH_UpdateTextures(); // 0x8005DE10

extern void MORPH_Continue(); // 0x8005DE68

extern void STREAM_MORPH_Relocate(); // 0x8005DFDC

extern int AddVertex(struct VECTOR *v0, struct RECT *rect); // 0x8005DFEC

extern int GetPlaneDist(int k, int j, int i, struct VECTOR *v); // 0x8005E178

extern void CalcVert(struct VECTOR *v, struct VECTOR *v1, struct VECTOR *v2, int dist1, int dist2, int k, int j); // 0x8005E204

extern int AddClippedTri(struct SVECTOR *iv, struct RECT *cliprect, int *minz); // 0x8005E28C

extern int STREAM_GetClipRect(struct StreamUnitPortal *portal, struct RECT *rect); // 0x8005E534

extern long GetFogColor(struct StreamUnitPortal *portal, struct _StreamUnit *mainStreamUnit, struct Level *mainLevel); // 0x8005EAFC

extern void DrawFogRectangle(struct RECT *cliprect, struct _PrimPool *primPool, int otzpos, unsigned long **drawot, long color); // 0x8005EE20

extern void STREAM_RenderAdjacantUnit(unsigned long **curOT, struct StreamUnitPortal *curStreamPortal, struct _StreamUnit *toStreamUnit, struct _StreamUnit *mainStreamUnit, struct RECT *cliprect); // 0x8005EF0C

extern struct BSPTree * STREAM_GetBspTree(struct _StreamUnit *streamUnit, long bspNumber); // 0x8005EFB8

extern void WARPGATE_BlockWarpGateEntrance(struct _StreamUnit *streamUnit, long collideOn); // 0x8005F018

extern void WARPGATE_DrawWarpGateRim(struct _StreamUnit *streamUnit, long drawOn); // 0x8005F088

extern void WARPGATE_HideAllCloudCovers(); // 0x8005F0E0

extern struct _Instance * WARPGATE_UnHideCloudCoverInUnit(long streamUnitID); // 0x8005F140

extern void STREAM_RenderWarpGate(unsigned long **mainOT, struct StreamUnitPortal *curStreamPortal, struct _StreamUnit *mainStreamUnit, struct RECT *cliprect); // 0x8005F1B8

extern void WARPGATE_RenderWarpUnit(unsigned long **mainOT, struct StreamUnitPortal *curStreamPortal, struct _StreamUnit *mainStreamUnit, struct RECT *cliprect); // 0x8005F4F0

extern void STREAM_DumpNonResidentObjects(); // 0x8005F8C8

extern int STREAM_TryAndDumpNonResident(struct _ObjectTracker *otr); // 0x8005F978


#endif
