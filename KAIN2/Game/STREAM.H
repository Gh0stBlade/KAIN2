#ifndef STREAM_H
#define STREAM_H

#include "SIGNAL.H"
#include "Game/INSTANCE.H"
#include "PLAN/PLANAPI.H"

struct evPositionData // hashcode: 0x853C21E0 (dec: -2059656736)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct _VMOffset {
	short dx; // size=0, offset=0
	short dy; // size=0, offset=2
	short dz; // size=0, offset=4
};
struct _VMColorOffset {
	char dr; // size=0, offset=0
	char dg; // size=0, offset=1
	char db; // size=0, offset=2
};
struct _VMInterpolated {
	short tvIdx; // size=0, offset=0
	short startIdx; // size=0, offset=2
	short endIdx; // size=0, offset=4
	short time; // size=0, offset=6
};
struct _VMOffsetTable {
	long numVMOffsets; // size=0, offset=0
	union {
		struct _VMOffset moveOffsets; // size=6, offset=0
		struct _VMColorOffset colorOffsets; // size=3, offset=0
	} offsets; // size=6, offset=4
};
struct _VMObject {
	unsigned short flags; // size=0, offset=0
	short bspIdx; // size=0, offset=2
	short materialIdx; // size=0, offset=4
	short spectralIdx; // size=0, offset=6
	short currentIdx; // size=0, offset=8
	short timeScale; // size=0, offset=10
	long timer; // size=0, offset=12
	struct _Position position; // size=6, offset=16
	short radius; // size=0, offset=22
	long radiusSquared; // size=0, offset=24
	long numVMOffsetTables; // size=0, offset=28
	struct _VMOffsetTable** vmoffsetTableList; // size=12, offset=32
	struct _VMOffsetTable* curVMOffsetTable; // size=12, offset=36
	long numVMVertices; // size=0, offset=40
	void* vmvertexList; // size=0, offset=44
	long numVMInterpolated; // size=0, offset=48
	struct _VMInterpolated* vminterpolatedList; // size=8, offset=52
	char* name; // size=0, offset=56
};

struct WarpGateLoadInfo
{
	int warpgate_in_use; // size=0, offset=0
	char loading; // size=0, offset=4
	char blocked; // size=0, offset=5
	short fadeValue; // size=0, offset=6
	long curTime; // size=0, offset=8
	long maxTime; // size=0, offset=12
	struct _Instance* warpFaceInstance; // size=668, offset=16
};

struct WarpRoom 
{
	char name[16]; // size=16, offset=0
	struct _StreamUnit* streamUnit; // size=64, offset=16
};

struct TClassAttr 
{
	short flags; // size=0, offset=0
	unsigned short sound; // size=0, offset=2
};

struct _SFXMkr 
{
	unsigned char* soundData; // size=0, offset=0
	long uniqueID; // size=0, offset=4
	struct SoundInstance sfxTbl[4]; // size=12, offset=8
	_Position pos; // size=6, offset=20
	short pad; // size=0, offset=26
	long livesInOnePlace; // size=0, offset=28
	long inSpectral; // size=0, offset=32
};

struct _BGObject 
{
	short flags; // size=0, offset=0
	short numFaces; // size=0, offset=2
	struct _BVertex* vertexList; // size=8, offset=4
	struct _BFace* faceList; // size=12, offset=8
	short x; // size=0, offset=12
	short y; // size=0, offset=14
	short z; // size=0, offset=16
	short radius; // size=0, offset=18
	long numVertices; // size=0, offset=20
};

struct LightGroup 
{
	MATRIX lightMatrix; // size=32, offset=0
	MATRIX colorMatrix; // size=32, offset=32
};
struct LightList 
{
	CVECTOR ambient; // size=4, offset=0
	long numLightGroups; // size=0, offset=4
	struct LightGroup* lightGroupList; // size=64, offset=8
};

struct Level // hashcode: 0x012B0198 (dec: 19595672)
{
	struct _Terrain* PTR_32 terrain; // size=84, offset=0
	struct LightList* PTR_32 lightList; // size=12, offset=4
	long numVMObjects; // size=0, offset=8
	struct _VMObject* PTR_32 vmobjectList; // size=60, offset=12
	long numSpotLights; // size=0, offset=16
	struct SpotLight* PTR_32 spotLightList; // size=40, offset=20
	long numPointLights; // size=0, offset=24
	struct PointLight* PTR_32 pointLightList; // size=24, offset=28
	long numSpotSpecturalLights; // size=0, offset=32
	struct SpotLight* PTR_32 spotSpecturalLightList; // size=40, offset=36
	long numPointSpecturalLights; // size=0, offset=40
	struct PointLight* PTR_32 pointSpecturalLightList; // size=24, offset=44
	long numBGObjects; // size=0, offset=48
	struct _BGObject* PTR_32 bgObjectList; // size=24, offset=52
	long waterZLevel; // size=0, offset=56
	unsigned char backColorR; // size=0, offset=60
	unsigned char backColorG; // size=0, offset=61
	unsigned char backColorB; // size=0, offset=62
	unsigned char cpad1; // size=0, offset=63
	unsigned char specturalColorR; // size=0, offset=64
	unsigned char specturalColorG; // size=0, offset=65
	unsigned char specturalColorB; // size=0, offset=66
	unsigned char cpad2; // size=0, offset=67
	unsigned short fogFar; // size=0, offset=68
	unsigned short fogNear; // size=0, offset=70
	unsigned short spectralFogFar; // size=0, offset=72
	unsigned short spectralFogNear; // size=0, offset=74
	unsigned short waterFogFar; // size=0, offset=76
	unsigned short waterFogNear; // size=0, offset=78
	long depthQBlendStart; // size=0, offset=80
	struct LightList* PTR_32 spectrallightList; // size=12, offset=84
	long numCameras; // size=0, offset=88
	void* PTR_32 cameraList; // size=0, offset=92
	long bspPlaneError; // size=0, offset=96
	long numVGroups; // size=0, offset=100
	struct _VGroup* PTR_32 vGroupList; // size=12, offset=104
	long deathZ; // size=0, offset=108
	long flags; // size=0, offset=112
	struct _MultiSignal* PTR_32 startSignal; // size=904, offset=116
	long numIntros; // size=0, offset=120
	struct Intro* PTR_32 introList; // size=76, offset=124
	struct DrMoveAniTex* PTR_32 bgAniList; // size=8, offset=128
	long numHotSpots; // size=0, offset=132
	struct _HotSpot* PTR_32 hotSpotList; // size=8, offset=136
	void* PTR_32 objectNameList; // size=0, offset=140
	long depthQFogStart; // size=0, offset=144
	long morphLastStep; // size=0, offset=148
	char* PTR_32 worldName; // size=0, offset=152
	struct VramSize vramSize; // size=8, offset=156
	unsigned short holdFogFar; // size=0, offset=164
	unsigned short holdFogNear; // size=0, offset=166
	long numberEnemyUnits; // size=0, offset=168
	char* PTR_32 * PTR_32 enemyUnitsNames; // size=0, offset=172
	long* PTR_32 * PTR_32 timesSignalList; // size=0, offset=176
	struct _MultiSignal* PTR_32 spectralSignal; // size=904, offset=180
	struct _MultiSignal* PTR_32 materialSignal; // size=904, offset=184
	struct _MultiSignal* PTR_32 startUnitLoadedSignal; // size=904, offset=188
	struct _MultiSignal* PTR_32 startUnitMainSignal; // size=904, offset=192
	struct _MultiSignal* PTR_32 startGoingIntoWaterSignal; // size=904, offset=196
	struct _MultiSignal* PTR_32 startGoingOutOfWaterSignal; // size=904, offset=200
	long unitFlags; // size=0, offset=204
	struct _MultiSignal* PTR_32 SignalListStart; // size=904, offset=208
	struct _MultiSignal* PTR_32 SignalListEnd; // size=904, offset=212
	long numBSPLeaves; // size=0, offset=216
	struct EventPointers* PTR_32 PuzzleInstances; // size=0, offset=220
	long NumberOfPlanMarkers; // size=0, offset=224
	struct _PlanMkr* PTR_32 PlanMarkerList; // size=8, offset=228
	long NumberOfSFXMarkers; // size=0, offset=232
	struct _SFXMkr* PTR_32 SFXMarkerList; // size=36, offset=236
	unsigned long versionNumber; // size=0, offset=240
	char* PTR_32 dynamicMusicName; // size=0, offset=244
	long streamUnitID; // size=0, offset=248
	struct TClassAttr tClassAttr[16]; // size=64, offset=252
	short TODLighting[4][3]; // size=24, offset=316
	unsigned char TODbackColor[4][3]; // size=12, offset=340
	unsigned short TODfogNearFar[4][2]; // size=16, offset=352
	short TODRedScale; // size=0, offset=368
	short TODGrnScale; // size=0, offset=370
	short TODBluScale; // size=0, offset=372
	short TODfogNear; // size=0, offset=374
	short TODfogFar; // size=0, offset=376
	short short_pad; // size=0, offset=378
	unsigned char TODbackColorR; // size=0, offset=380
	unsigned char TODbackColorG; // size=0, offset=381
	unsigned char TODbackColorB; // size=0, offset=382
	unsigned char cpad3; // size=0, offset=383
	struct LightGroup* PTR_32 razielLightGroup; // size=64, offset=384
	struct LightGroup* PTR_32 razielSpectralLightGroup; // size=64, offset=388
};

struct _StreamUnit // hashcode: 0xAC366E40 (dec: -1405718976)
{
	long StreamUnitID; // size=0, offset=0
	short used; // size=0, offset=4
	short flags; // size=0, offset=6
	struct Level* level; // size=392, offset=8
	char baseAreaName[16]; // size=16, offset=12
	short toSignal; // size=0, offset=28
	short fromSignal; // size=0, offset=30
	long FrameCount; // size=0, offset=32
	short eventVariables[6]; // size=12, offset=36
	long FogColor; // size=0, offset=48
	short UnitFogFar; // size=0, offset=52
	short UnitFogNear; // size=0, offset=54
	short TargetFogFar; // size=0, offset=56
	short TargetFogNear; // size=0, offset=58
	short sfxFileHandle; // size=0, offset=60
	short pad; // size=0, offset=62
};

struct StreamUnitPortal // hashcode: 0x7C52D8D9 (dec: 2085804249)
{
	char tolevelname[16]; // size=16, offset=0
	long MSignalID; // size=0, offset=16
	long streamID; // size=0, offset=20
	short minx; // size=0, offset=24
	short miny; // size=0, offset=26
	short minz; // size=0, offset=28
	short flags; // size=0, offset=30
	short maxx; // size=0, offset=32
	short maxy; // size=0, offset=34
	short maxz; // size=0, offset=36
	short pad2; // size=0, offset=38
	struct _StreamUnit* PTR_32 toStreamUnit; // size=64, offset=40
	struct _SVector t1[3]; // size=24, offset=44
	struct _SVector t2[3]; // size=24, offset=68
};

struct _BVertex {
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};
struct _BFace 
{
	short v0; // size=0, offset=0
	short v1; // size=0, offset=2
	short v2; // size=0, offset=4
	short pad; // size=0, offset=6
	struct TextureFT3* texture; // size=12, offset=8
};

struct STracker // hashcode: 0xAED5E069 (dec: -1361715095)
{
	struct _StreamUnit StreamList[16]; // size=1024, offset=0
};

typedef struct StreamUnitPortal StreamUnitPortal;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif
extern struct STracker StreamTracker; // offset 0x800D1920

extern void STREAM_FillOutFileNames(char *baseAreaName, char *dramName, char *vramName, char *sfxName); // 0x800584E4
extern void STREAM_AbortAreaLoad(char *baseAreaName); // 0x800585A4
extern void STREAM_Init(); // 0x800585EC
extern int FindObjectName(char *name); // 0x80058618
extern struct _ObjectTracker * FindObjectInTracker(struct Object *object); // 0x80058684
extern struct _StreamUnit * FindStreamUnitFromLevel(struct Level *level); // 0x800586D4
extern void STREAM_LoadObjectReturn(void *loadData, void* PTR_32 data, void *data2); // 0x80058724
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
extern void RelocatePlanPool(struct PlanningNode *planPool, _SVector *offset); // 0x8005BB5C
extern void RelocatePlanMarkers(struct _PlanMkr *planMkrList, int numPlanMkrs, struct _SVector *offset); // 0x8005BC10
extern void RelocateSFXMarkers(struct _SFXMkr *sfxMkrList, int numSFXMkrs, struct _SVector *offset); // 0x8005BC64
extern void STREAM_AdjustMultiSpline(struct MultiSpline *multi, struct _SVector *offset); // 0x8005BCB0
extern void STREAM_RelocateInstance(struct _Instance *instance, struct _SVector *offset); // 0x8005BD60
extern void STREAM_OffsetInstancePosition(struct _Instance *instance, struct _SVector *offset, int streamSignalFlag); // 0x8005BDA8
extern void STREAM_SetInstancePosition(struct _Instance *instance, struct evPositionData *data); // 0x8005BEC0
extern void RelocateInstances(struct _SVector *offset); // 0x8005BF18
extern void RelocateStreamPortals(struct StreamUnitPortal *StreamUnitList, int NumStreamUnits, struct _SVector *offset); // 0x8005BF74
extern void STREAM_PackVRAMObject(struct _ObjectTracker* PTR_32 objectTracker); // 0x8005C0A4
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
extern int AddVertex(VECTOR *v0, PSX_RECT *rect); // 0x8005DFEC
extern int GetPlaneDist(int k, int j, int i, VECTOR *v); // 0x8005E178
extern void CalcVert(VECTOR *v, VECTOR *v1, VECTOR *v2, int dist1, int dist2, int k, int j); // 0x8005E204
extern int AddClippedTri(SVECTOR*iv, PSX_RECT *cliprect, int *minz); // 0x8005E28C
extern int STREAM_GetClipRect(struct StreamUnitPortal *portal, PSX_RECT *rect); // 0x8005E534
extern long GetFogColor(struct StreamUnitPortal *portal, struct _StreamUnit *mainStreamUnit, struct Level *mainLevel); // 0x8005EAFC
extern void DrawFogRectangle(PSX_RECT *cliprect, struct _PrimPool *primPool, int otzpos, unsigned long **drawot, long color); // 0x8005EE20
extern void STREAM_RenderAdjacantUnit(unsigned long **curOT, struct StreamUnitPortal *curStreamPortal, struct _StreamUnit *toStreamUnit, struct _StreamUnit *mainStreamUnit, PSX_RECT *cliprect); // 0x8005EF0C
extern struct BSPTree * STREAM_GetBspTree(struct _StreamUnit *streamUnit, long bspNumber); // 0x8005EFB8
extern void WARPGATE_BlockWarpGateEntrance(struct _StreamUnit *streamUnit, long collideOn); // 0x8005F018
extern void WARPGATE_DrawWarpGateRim(struct _StreamUnit *streamUnit, long drawOn); // 0x8005F088
extern void WARPGATE_HideAllCloudCovers(); // 0x8005F0E0
extern struct _Instance * WARPGATE_UnHideCloudCoverInUnit(long streamUnitID); // 0x8005F140
extern void STREAM_RenderWarpGate(unsigned long **mainOT, struct StreamUnitPortal *curStreamPortal, struct _StreamUnit *mainStreamUnit, PSX_RECT *cliprect); // 0x8005F1B8
extern void WARPGATE_RenderWarpUnit(unsigned long **mainOT, struct StreamUnitPortal *curStreamPortal, struct _StreamUnit *mainStreamUnit, PSX_RECT *cliprect); // 0x8005F4F0
extern void STREAM_DumpNonResidentObjects(); // 0x8005F8C8
extern int STREAM_TryAndDumpNonResident(struct _ObjectTracker *otr); // 0x8005F978

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
