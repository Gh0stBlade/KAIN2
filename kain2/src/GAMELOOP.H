#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "CORE.H"

typedef struct _PrimPool // hashcode: 0x9529B283 (dec: -1792429437)
{
	long numPrims; // size=0, offset=0
	unsigned long* nextPrim; // size=0, offset=4
	unsigned long* lastPrim; // size=0, offset=8
	unsigned long prim[24000]; // size=96000, offset=12
} _PrimPool;

typedef struct GameTracker // hashcode: 0x4EB3EFC5 (dec: 1320415173)
{
	struct _GameData_Type gameData; // size=32, offset=0
	struct menu_t* menu; // size=0, offset=32
	struct memcard_t* memcard; // size=0, offset=36
	struct Level* level; // size=392, offset=40
	struct _Instance* playerInstance; // size=616, offset=44
	long drawPage; // size=0, offset=48
	struct _InstanceList* instanceList; // size=268, offset=52
	struct _InstancePool* instancePool; // size=38204, offset=56
	struct _VertexPool* vertexPool; // size=6336, offset=60
	struct _PrimPool* primPool; // size=96012, offset=64
	struct _ObjectTracker* GlobalObjects; // size=36, offset=68
	long controlCommand[2][5]; // size=40, offset=72
	long controlData[2][5]; // size=40, offset=112
	long overrideData[2][5]; // size=40, offset=152
	long debugFlags; // size=0, offset=192
	long debugFlags2; // size=0, offset=196
	CVECTOR wipeColor; // size=4, offset=200
	short wipeTime; // size=0, offset=204
	short maxWipeTime; // size=0, offset=206
	short wipeType; // size=0, offset=208
	short numGSignals; // size=0, offset=210
	void(*gSignal[16]); // size=64, offset=212
	struct LightInfo* lightInfo; // size=1148, offset=276
	void* reqDisp; // size=0, offset=280
	long* drawTimerReturn; // size=0, offset=284
	long usecsStartDraw; // size=0, offset=288
	void* disp; // size=0, offset=292
	unsigned long displayFrameCount; // size=0, offset=296
	unsigned long frameCount; // size=0, offset=300
	unsigned long fps30Count; // size=0, offset=304
	unsigned long vblFrames; // size=0, offset=308
	unsigned long vblCount; // size=0, offset=312
	long numMatrices; // size=0, offset=316
	long gameFlags; // size=0, offset=320
	long streamFlags; // size=0, offset=324
	void* drawNonAnimatedSegmentFunc; // size=0, offset=328
	void* drawAnimatedModelFunc; // size=0, offset=332
	void* drawDisplayPolytopeListFunc; // size=0, offset=336
	void* drawBgFunc; // size=0, offset=340
	struct Level* mainDrawUnit; // size=392, offset=344
	char baseAreaName[16]; // size=16, offset=348
	short levelDone; // size=0, offset=364
	short levelChange; // size=0, offset=366
	short hideBG; // size=0, offset=368
	short gameMode; // size=0, offset=370
	long currentHotSpot; // size=0, offset=372
	long StreamUnitID; // size=0, offset=376
	short SwitchToNewStreamUnit; // size=0, offset=380
	short SwitchToNewWarpIndex; // size=0, offset=382
	char S_baseAreaName[16]; // size=16, offset=384
	short toSignal; // size=0, offset=400
	short fromSignal; // size=0, offset=402
	char LastSignal; // size=0, offset=404
	short StreamNormalA; // size=0, offset=406
	short StreamNormalB; // size=0, offset=408
	short StreamNormalC; // size=0, offset=410
	long StreamNormalD; // size=0, offset=412
	long moveRazielToStreamID; // size=0, offset=416
	struct _ColorType animObjLine; // size=4, offset=420
	struct _ColorType animObjShade; // size=4, offset=424
	long maxIntroFXTime; // size=0, offset=428
	struct gSoundData sound; // size=20, offset=432
	short controllerMode; // size=0, offset=452
	unsigned char plan_collide_override; // size=0, offset=454
	unsigned char cheatMode; // size=0, offset=455
	char currentLvl; // size=0, offset=456
	char lastLvl; // size=0, offset=457
	struct Object* introFX; // size=76, offset=460
	struct Intro* introFXIntro; // size=76, offset=464
	unsigned long** drawOT; // size=0, offset=468
	unsigned long** dispOT; // size=0, offset=472
	P_TAG* savedOTStart; // size=8, offset=476
	P_TAG* savedOTEnd; // size=8, offset=480
	long introWaitTime; // size=0, offset=484
	long mirrorZPush; // size=0, offset=488
	long defVVRemoveDist; // size=0, offset=492
	long defRemoveDist; // size=0, offset=496
	_Position forcedStartPosition; // size=6, offset=500
	short hudCollDisplay; // size=0, offset=506
	long primMemUsed; // size=0, offset=508
	long cheatTimerCount; // size=0, offset=512
	long playerCheatFlags; // size=0, offset=516
	long savedPlayerCameraMode; // size=0, offset=520
	long debugDrawFlags; // size=0, offset=524
	void* planningPool; // size=0, offset=528
	void* enemyPlanPool; // size=0, offset=532
	unsigned char block_collide_override; // size=0, offset=536
	unsigned char raziel_collide_override; // size=0, offset=537
	short timeOfDay; // size=0, offset=538
	long decoupleGame; // size=0, offset=540
	long multGameTime; // size=0, offset=544
	short spectral_fadeValue; // size=0, offset=548
	short material_fadeValue; // size=0, offset=550
	unsigned long drawTime; // size=0, offset=552
	unsigned long currentTime; // size=0, offset=556
	unsigned long currentMaterialTime; // size=0, offset=560
	unsigned long currentSpectralTime; // size=0, offset=564
	unsigned long currentTimeOfDayTime; // size=0, offset=568
	unsigned long lastLoopTime; // size=0, offset=572
	unsigned long timeMult; // size=0, offset=576
	unsigned long globalTimeMult; // size=0, offset=580
	unsigned long spectralTimeMult; // size=0, offset=584
	unsigned long materialTimeMult; // size=0, offset=588
	unsigned long currentTicks; // size=0, offset=592
	unsigned long totalTime; // size=0, offset=596
	unsigned long idleTime; // size=0, offset=600
	long visibleInstances; // size=0, offset=604
	int gameFramePassed; // size=0, offset=608
	unsigned long timeSinceLastGameFrame; // size=0, offset=612
	long frameRateLock; // size=0, offset=616
	short frameRate24fps; // size=0, offset=620
	char monster_collide_override; // size=0, offset=622
	char pad; // size=0, offset=623
} GameTracker;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern struct Camera theCamera;
extern char* primBase;
extern unsigned long(**gOt[2]); // offset 0x800D0C0C
extern struct LightInfo* gLightInfo; // offset 0x800D0C2C
extern struct _InstanceList* instanceList; // offset 0x800D0C20
extern struct _InstancePool* instancePool; // offset 0x800D0C24
extern struct GameTracker gameTrackerX; // offset 0x800D0FAC
extern struct _PrimPool(*primPool[2]); // offset 0x800D0C14
extern struct _VertexPool* gVertexPool; // offset 0x800D0C28
extern void* planningPool;
extern void* enemyPlanPool; // offset 0x800D0C30
extern struct _ObjectTracker* GlobalObjects; // offset 0x800D0C38

extern void GAMELOOP_AllocStaticMemory(); // 0x8002D7C8
extern void GAMELOOP_InitGameTracker(); // 0x8002D8B8
extern void GAMELOOP_SystemInit(struct GameTracker *gameTracker); // 0x8002D9B8
extern void GAMELOOP_ResetGameStates(); // 0x8002D9F0
extern void GAMELOOP_ClearGameTracker(); // 0x8002DA10
extern void GAMELOOP_CalcGameTime(); // 0x8002DA58
extern void GAMELOOP_SetGameTime(long timeOfDay); // 0x8002DB04
extern int GAMELOOP_GetTimeOfDay(); // 0x8002DB98
extern int GAMELOOP_GetTimeOfDayIdx(int timeOfDay); // 0x8002DBE4
extern int GAMELOOP_WaitForLoad(); // 0x8002DC28
extern struct _StreamUnit * LoadLevels(char *baseAreaName, struct GameTracker *gameTracker); // 0x8002DC60
extern void GAMELOOP_InitStandardObjects(); // 0x8002DEBC
extern void GAMELOOP_LevelLoadAndInit(char *baseAreaName, struct GameTracker *gameTracker); // 0x8002DF10
extern void GAMELOOP_StreamLevelLoadAndInit(char *baseAreaName, struct GameTracker *gameTracker, int toSignalNum, int fromSignalNum); // 0x8002E220
extern void GAMELOOP_SetScreenWipe(int time, int maxTime, int type); // 0x8002E240
extern void GAMELOOP_HandleScreenWipes(unsigned long **drawot); // 0x8002E254
extern void UpdateFogSettings(struct _StreamUnit *currentUnit, struct Level *level); // 0x8002E6B4
extern int CheckForNoBlend(struct _ColorType *Color); // 0x8002E7DC
extern void BlendToColor(struct _ColorType *target, struct _ColorType *current, struct _ColorType *dest); // 0x8002E824
extern void MainRenderLevel(struct _StreamUnit *currentUnit, unsigned long **drawot); // 0x8002E918
extern void StreamIntroInstancesForUnit(struct _StreamUnit *currentUnit); // 0x8002EE18
extern long StreamRenderLevel(struct _StreamUnit *currentUnit, struct Level *mainLevel, unsigned long **drawot, long portalFogColor); // 0x8002EE58
extern void GAMELOOP_FlipScreenAndDraw(struct GameTracker *gameTracker, unsigned long **drawot); // 0x8002F1A0
extern void GAMELOOP_AddClearPrim(unsigned long **drawot, int override); // 0x8002F254
extern void GAMELOOP_SwitchTheDrawBuffer(unsigned long **drawot); // 0x8002F2FC
extern void GAMELOOP_SetupRenderFunction(struct GameTracker *gameTracker); // 0x8002F37C
extern struct _StreamUnit * GAMELOOP_GetMainRenderUnit(); // 0x8002F398
extern void GAMELOOP_DisplayFrame(struct GameTracker *gameTracker); // 0x8002F458
extern void GAMELOOP_DrawSavedOT(unsigned long **newOT); // 0x8002FC20
extern void ResetPrimPool(); // 0x8002FD98
extern void Switch_For_Redraw(); // 0x8002FE44
extern void GAMELOOP_Set_Pause_Redraw(); // 0x8002FEC0
extern void SaveOT(); // 0x8002FED0
extern void ResetDrawPage(); // 0x80030070
extern void GAMELOOP_Set24FPS(); // 0x800300B0
extern void GAMELOOP_Reset24FPS(); // 0x800300C0
extern void GAMELOOP_DoTimeProcess(); // 0x800300CC
extern void GAMELOOP_Process(struct GameTracker *gameTracker); // 0x800302D0
extern void GAMELOOP_ModeStartRunning(); // 0x80030920
extern void GAMELOOP_ModeStartPause(); // 0x800309F8
extern void GAMELOOP_ChangeMode(); // 0x80030AD0
extern void GAMELOOP_RequestLevelChange(char *name, short number, struct GameTracker *gameTracker); // 0x80030E7C
extern void PSX_GameLoop(struct GameTracker *gameTracker); // 0x80030EFC
extern MATRIX * GAMELOOP_GetMatrices(int numMatrices); // 0x80030F28
extern struct GameTracker * GAMELOOP_GetGT(); // 0x80030F5C

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
