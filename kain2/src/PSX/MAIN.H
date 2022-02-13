#ifndef MAIN_H
#define MAIN_H

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 240

extern void ClearDisplay(); // 0x80037EF8

extern void screen_to_vram(long *screen, int buffer); // 0x80037FB4

extern void show_screen(char *name); // 0x80037FE4

extern void play_movie(char *name); // 0x80038028

extern void InitMainTracker(struct MainTracker *mainTracker); // 0x80038070

extern char * FindTextInLine(char *search_match, char *search_str); // 0x80038080

extern void ExtractWorldName(char *worldName, char *levelName); // 0x80038108

extern void ExtractLevelNum(char *levelNum, char *levelName); // 0x80038154

extern void ProcessArgs(char *baseAreaName, struct GameTracker *gameTracker); // 0x800381C0

extern void InitDisplay(); // 0x800383D8

extern void StartTimer(); // 0x80038554

extern void VblTick(); // 0x800385C0

extern void DrawCallback(); // 0x8003862C

extern void FadeOutSayingLoading(struct GameTracker *gameTracker); // 0x80038684

extern void CheckForDevStation(); // 0x800387D4

extern void MAIN_ShowLoadingScreen(); // 0x80038834

extern long * MAIN_LoadTim(char *name); // 0x800388E4

extern void init_menus(struct GameTracker *gt); // 0x80038904

extern void MAIN_DoMainInit(); // 0x80038950

extern void MAIN_InitVolume(); // 0x80038A44

extern void MAIN_ResetGame(); // 0x80038A84

extern void MAIN_MainMenuInit(); // 0x80038AD4

extern void MAIN_FreeMainMenuStuff(); // 0x80038C6C

extern void MAIN_StartGame(); // 0x80038CDC

extern long MAIN_DoMainMenu(struct GameTracker *gameTracker, struct MainTracker *mainTracker, long menuPos); // 0x80038D48

extern int MainG2(void *appData); // 0x80038E1C


#endif
