#ifndef GAMELOOP_H
#define GAMELOOP_H


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

extern struct MATRIX * GAMELOOP_GetMatrices(int numMatrices); // 0x80030F28

extern struct GameTracker * GAMELOOP_GetGT(); // 0x80030F5C


#endif
