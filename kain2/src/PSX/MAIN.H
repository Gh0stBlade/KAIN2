#ifndef MAIN_H
#define MAIN_H

#include "../CORE.H"

struct InterfaceItem // hashcode: 0xB8494031 (dec: -1203158991)
{
	char name[48]; // size=48, offset=0
	unsigned short timeout; // size=0, offset=48
	unsigned short buttonTimeout; // size=0, offset=50
	short itemType; // size=0, offset=52
	short nextItem; // size=0, offset=54
};

struct MainTracker // hashcode: 0x7F315B98 (dec: 2133941144)
{
	long mainState; // size=0, offset=0
	long previousState; // size=0, offset=4
	long movieNum; // size=0, offset=8
	long done; // size=0, offset=12
};

struct BLK_FILL // hashcode: 0x2490546A (dec: 613438570)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	unsigned short x0; // size=0, offset=8
	unsigned short y0; // size=0, offset=10
	unsigned short w; // size=0, offset=12
	unsigned short h; // size=0, offset=14
};


extern int mainMenuSfx; // offset 0x800D0E1C

extern long* mainMenuScreen; // offset 0x800CE6C8

extern short gEndGameNow; // offset 0x800CE562

extern short mainMenuFading; // offset 0x800CE6D2

extern long DoMainMenu; // offset 0x800CE6C0

extern struct InterfaceItem InterfaceItems[6]; // offset 0x800CE570

extern char mainOptionsInit; // offset 0x800CE560

extern struct MainTracker mainTrackerX; // offset 0x800D121C

extern long gTimerEnabled; // offset 0x800CE8D4

extern unsigned long __timerEvent; // offset 0x800D0F84

extern int nosound; // offset 0x800CE568

extern int nomusic; // offset 0x800CE56C

extern int devstation; // offset 0x800D0E68

extern DRAWENV draw[2]; // offset 0x800D0E6C

extern DISPENV disp[2]; // offset 0x800D0E40

extern struct BLK_FILL clearRect[2]; // offset 0x800D0F88

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
