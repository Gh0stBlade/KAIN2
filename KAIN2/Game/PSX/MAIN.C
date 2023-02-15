#include "Game/CORE.H"

#include "MAIN.H"
#include "Game/G2/MAING2.H"
#include "Game/MEMPACK.H"
#include "Game/LOAD3D.H"
#include "Game/STRMLOAD.H"
#include "Game/LOCAL/LOCALSTR.H"
#include "Game/GAMELOOP.H"
#include "Game/SOUND.H"
#include "Game/VOICEXA.H"
#include "AADSFX.H"
#include "Game/STREAM.H"
#include "AADLIB.H"
#include "Game/SAVEINFO.H"
#include "Game/FONT.H"
#include "Game/GAMEPAD.H"
#include "Game/VRAM.H"
#include "Game/CINEMA/CINEPSX.H"
#include "DRAWS.H"
#include "Game/TIMER.H"
#include "Game/RAZIEL/RAZLIB.H"
#include "Game/DEBUG.H"
#include "Game/MENU/MENUFACE.H"
#include "Game/MENU/MENU.H"
#include "Game/MENU/MENUDEFS.H"
#include "Game/DRAW.H"

#include <assert.h>

long mainMenuMode; // offset 0x800CE6C4
struct DebugMenuLine mainMenu[8] = { DEBUG_LINE_TYPE_FORMAT,  0, 0, "-abs 256 40 -center", 0, 0,
									 DEBUG_LINE_TYPE_ENDLIST, 0, 0, "",                    0, 0,
									 DEBUG_LINE_TYPE_ENDLIST, 0, 0, "",                    0, 0,
									 DEBUG_LINE_TYPE_ENDLIST, 0, 0, "",                    0, 0,
									 DEBUG_LINE_TYPE_ENDLIST, 0, 0, "",                    0, 0,
									 DEBUG_LINE_TYPE_ENDLIST, 0, 0, "",                    0, 0,
									 DEBUG_LINE_TYPE_ENDLIST, 0, 0, "",                    0, 0,
									 DEBUG_LINE_TYPE_ENDLIST, 0, 0, "",                    0, 0,
									
								    }; // offset 0x800C8418
short mainMenuTimeOut; // offset 0x800D0FA8
int mainMenuSfx; // offset 0x800D0E1C
int* mainMenuScreen; // offset 0x800CE6C8
short gEndGameNow; // offset 0x800CE562
short mainMenuFading; // offset 0x800CE6D2
long DoMainMenu; // offset 0x800CE6C0
char mainOptionsInit; // offset 0x800CE560
struct MainTracker mainTrackerX; // offset 0x800D121C
long gTimerEnabled; // offset 0x800CE8D4
unsigned long __timerEvent; // offset 0x800D0F84
int nosound; // offset 0x800CE568
int nomusic; // offset 0x800CE56C
int devstation; // offset 0x800D0E68
struct BLK_FILL clearRect[2]; // offset 0x800D0F88
DRAWENV draw[2]; // offset 0x800D0E6C
DISPENV disp[2]; // offset 0x800D0E40

struct InterfaceItem InterfaceItems[] = 
{ 
	{ "\\PUBLOGO.STR;1", 0, 0, 0, 1 },
	{ "\\CRYLOGO.STR;1", 0, 0, 0, 5 },
	{ "\\KAININT.STR;1", 0, 0, 0, -1 },
	{ "\\VERSE.STR;1", 0, 0, 0, 4 },
	{ "\\CREDITS.STR;1", 0, 0, 0, -1 },
#if defined(DEMO_BUILD) || 0
	{ "\\kain2\\game\\psx\\mainmenu\\legal.tim", 330, 330, 1, 2},
	{ "\\kain2\\game\\psx\\mainmenu\\instruct.tim", 1800, 10, 1, 7},
	{ "\\kain2\\game\\psx\\mainmenu\\controls.tim", 1800, 10, 1, -1},
	{ "\\kain2\\game\\psx\\mainmenu\\screens1.tim", 900, 10, 1, 11},
	{ "\\kain2\\game\\psx\\mainmenu\\screens2.tim", 900, 10, 1, 10},
	{ "\\kain2\\game\\psx\\mainmenu\\screens3.tim", 900, 10, 1, 11},
	{ "\\kain2\\game\\psx\\mainmenu\\screens4.tim", 900, 10, 1, -1},
	{ "\\kain2\\game\\psx\\mainmenu\\story.tim", 1800, 10, 1, 6},
	{ "\\kain2\\game\\psx\\mainmenu\\usps.tim", 1800, 10, 1, 8},
#else
	{ "\\kain2\\game\\psx\\mainmenu\\legal.tim", 165, 165, 1, -1},
#endif

};

#if defined(EDITOR)
int stopGameThread = 0;
#endif

void ClearDisplay()
{
#ifdef PSX_VERSION
	PutDrawEnv(&draw[gameTrackerX.gameData.asmData.dispPage]);
	clearRect[gameTrackerX.gameData.asmData.dispPage].r0 = 0;
	clearRect[gameTrackerX.gameData.asmData.dispPage].g0 = 0;
	clearRect[gameTrackerX.gameData.asmData.dispPage].b0 = 0;
	DrawPrim(&clearRect);
	DrawSync(0);
	PutDispEnv(&disp[gameTrackerX.gameData.asmData.dispPage]);
	SetDispMask(1);
#else
	PutDrawEnv(&draw[gameTrackerX.gameData.asmData.dispPage]);
	DrawPrim(&clearRect[gameTrackerX.gameData.asmData.dispPage]);
	DrawSync(0);
	PutDispEnv(&disp[gameTrackerX.gameData.asmData.dispPage]);
	SetDispMask(1);
#endif
}

void screen_to_vram(int *screen, int buffer)
{
	LOAD_LoadTIM2(screen, 0, buffer << 8, SCREEN_WIDTH, SCREEN_HEIGHT + 16);
}

void show_screen(char *name)
{
	int *screen;

	screen = (int*)LOAD_ReadFile(name, 11);
	
	if (screen != NULL)
	{
		screen_to_vram(screen, gameTrackerX.gameData.asmData.dispPage);
		MEMPACK_Free((char*)screen);
	}
}

void play_movie(char *name)
{
	if (CINE_Load() != 0)
	{
		CINE_Play(name, 0xFFFFu, 2);
		ClearDisplay();
		CINE_Unload();
	}
}

void InitMainTracker(struct MainTracker *mainTracker)
{
	mainTracker->mainState = 0;
	mainTracker->previousState = 0;
	mainTracker->done = 0;
}

char* FindTextInLine(char* search_match, char* search_str)
{
	char* match_pos;
	match_pos = search_match;

	while (*search_str != 0 && *search_str != '\n')
	{
		if (*search_str != '\r')
		{
			if (((*search_str++) | 0x20) == ((*match_pos) | 0x20))
			{
				match_pos++;
			}
			else
			{
				match_pos = search_match;
			}

			if (*match_pos == 0)
			{
				return search_str;
			}
		}
		else
		{
			break;
		}
	}

	return NULL;
}

void ExtractWorldName(char *worldName, char *levelName)
{ 
	while (*levelName != '-')
	{
		if ((*levelName - 0x41) < 0x1A || (*levelName - 0x61) >= 0x1A)
		{
			break;
		}

		*worldName++ = *levelName++;
	}

	*worldName = 0;
}

void ExtractLevelNum(char *levelNum, char *levelName)
{ 
	while (*levelName != '-')
	{
		if ((*levelName++ - 0x30) < 0xA)
		{
			break;
		}
	}

	do
	{
		*levelNum++ = *levelName++;
	} while ((unsigned int)(*levelName - 0x30) < 0xA);
}

void ProcessArgs(char *baseAreaName, struct GameTracker *gameTracker)
{ 
	char levelNum[32];
	char worldName[32];
	long *argData;

#if defined(PSXPC_VERSION)
	memset(levelNum, 0, sizeof(levelNum));
	memset(worldName, 0, sizeof(worldName));
#endif

	argData = LOAD_ReadFile((char*)"\\kain2\\game\\psx\\kain2.arg", 10);

	if (argData != NULL)
	{
		ExtractWorldName(worldName, (char*)argData);
		ExtractLevelNum(levelNum, (char*)argData);
		
#if defined(OVERRIDE_LEVEL) || 0
#define LEVEL_NAME "fire"
#define LEVEL_NUM "6"
		sprintf(baseAreaName, "%s%s", LEVEL_NAME, LEVEL_NUM);
#else
		sprintf(baseAreaName, "%s%s", worldName, levelNum);
#endif
		if (FindTextInLine((char*)"-NOSOUND", (char*)argData) != 0)
		{
			nosound = 1;
			nomusic = 1;
		}

		if (FindTextInLine((char*)"-NOMUSIC", (char*)argData) != 0)
		{
			nomusic = 1;
		}

		if (FindTextInLine((char*)"-TIMEOUT", (char*)argData) != 0)
		{
			gameTracker->debugFlags |= 0x20000;
		}

		if (FindTextInLine((char*)"-MAINMENU", (char*)argData) != 0)
		{
			DoMainMenu = 1;
		}

		if (FindTextInLine((char*)"-INSPECTRAL", (char*)argData) != 0)
		{
			gameTrackerX.gameData.asmData.MorphType = 1;
		}

		if (FindTextInLine((char*)"-VOICE", (char*)argData) != 0)
		{
			gameTracker->debugFlags |= 0x80000;
		}
		
		if (FindTextInLine((char*)"-DEBUG_CD", (char*)argData) != 0)
		{
			gameTracker->debugFlags |= 0x80000000;
		}

		if (FindTextInLine((char*)"-LOADGAME", (char*)argData) != 0)
		{
			gameTrackerX.streamFlags |= 0x200000;
		}

		if (FindTextInLine((char*)"-ALLWARP", (char*)argData) != 0)
		{
			gameTrackerX.streamFlags |= 0x400000;
		}

		gameTracker->debugFlags |= 0x80000;
		MEMPACK_Free((char*)argData);
	}
	else
	{
		strcpy(baseAreaName, "under1");
	}
}

PSX_RECT rect = { SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_WIDTH };//0x800D0BF6

void InitDisplay()
{
#if defined(PSX_VERSION)
	int i;
	PSX_RECT r;

	r = rect;

	ResetGraph(3);
	SetGraphDebug(0);

	SetDefDrawEnv(&draw[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetDefDispEnv(&disp[0], 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetDefDrawEnv(&draw[1], 0, SCREEN_HEIGHT + 16, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetDefDispEnv(&disp[1], 0, SCREEN_HEIGHT + 16, SCREEN_WIDTH, SCREEN_HEIGHT);

	draw[1].dtd = 1;
	draw[0].dtd = 1;
	draw[1].dfe = 1;
	draw[0].dfe = 1;
	draw[1].isbg = 0;
	draw[0].isbg = 0;
	draw[0].r0 = 0;
	draw[0].g0 = 0;
	draw[0].b0 = 0;
	draw[1].r0 = 0;
	draw[1].g0 = 0;
	draw[1].b0 = 0;

	for (i = 0; i < 2; i++)
	{
		setlen(&clearRect[i], 3);
		setcode(&clearRect[i], 2);
		setXY0(&clearRect[i], 0, i * (SCREEN_HEIGHT + 16));
		setWH(&clearRect[i], SCREEN_WIDTH, SCREEN_HEIGHT);
		setRGB0(&clearRect[i], 0, 0, 0);
	}

	ClearDisplay();
	
	ClearOTagR((uintptr_t*)gameTrackerX.drawOT, 3072);
	ClearOTagR((uintptr_t*)gameTrackerX.dispOT, 3072);
	
	ClearImage(&r, 0, 255, 0);
#else
	TILE* t; // eax
	u_long tag; // edx
	int dispPage; // eax
	PSX_RECT rect; // [esp+Ch] [ebp-8h] BYREF

	rect.x = 512;
	rect.y = 0;
	rect.w = 512;
	rect.h = 512;
	ResetGraph(3);
	SetGraphDebug(0);
	SetDefDrawEnv(draw, 0, 0, 512, 240);
	SetDefDispEnv(disp, 0, 0, 512, 240);
	SetDefDrawEnv(&draw[1], 0, 0, 512, 240);
	SetDefDispEnv(&disp[1], 0, 0, 512, 240);
	draw[1].dtd = 1;
	draw[0].dtd = 1;
	draw[1].dfe = 1;
	draw[0].dfe = 1;
	draw[1].isbg = 0;
	draw[0].isbg = 0;
	draw[0].r0 = 0;
	draw[0].g0 = 0;
	draw[0].b0 = 0;
	draw[1].r0 = 0;
	draw[1].g0 = 0;
	draw[1].b0 = 0;
	t = (TILE*)clearRect;
	int i = 0;
	do
	{
		tag = t->tag;
		t->tag = tag & 0xFFFFFF | 0x3000000;
		t->code = 2;
		t->x0 = 0;
		t->y0 = 0;
		t->w = 512;
		t->h = 240;
		t->r0 = 0;
		t->g0 = 0;
		t->b0 = 0;
		++t;
	} while (i < 2);
	dispPage = gameTrackerX.gameData.asmData.dispPage;
	clearRect[dispPage].r0 = 0;
	clearRect[dispPage].g0 = 0;
	clearRect[dispPage].b0 = 0;
	ClearDisplay();
	GXFilePrint("ClearDisplay()\n");
	ClearOTagR((u_long*)gameTrackerX.drawOT, 3072);
	ClearOTagR((u_long*)gameTrackerX.dispOT, 3072);
	ClearImage(&rect, 0, 0xFFu, 0);
#endif
}

void StartTimer()
{
#ifdef PSX_VERSION
	EnterCriticalSection();
	__timerEvent = OpenEvent(0xF2000000, 2, 0x1000, TimerTick);
	EnableEvent(__timerEvent);
	SetRCnt(0xF2000000, 0xFFFF, 0x1001);
	StartRCnt(0xF2000000);
	ExitCriticalSection();
	gTimerEnabled = 1;
#endif
}

void VblTick()
{ 
	if (devstation != 0)
	{
#if defined(PSXPC_VERSION)
		assert(0);
#elif defined(PSX_VERSION)
		//break 1;
#endif
	}

	gameTrackerX.vblFrames++;
	gameTrackerX.vblCount++;

#if defined(PSXPC_VERSION)
	if (gameTrackerX.reqDisp != NULL && gameTrackerX.frameRateLock <= gameTrackerX.vblFrames)
#else
	if (gameTrackerX.reqDisp != NULL && gameTrackerX.frameRateLock < gameTrackerX.vblFrames)
#endif
	{
		PutDispEnv((DISPENV*)gameTrackerX.reqDisp);
		gameTrackerX.reqDisp = NULL;
		gameTrackerX.vblFrames = 0;
	}
}

void DrawCallback()
{ 
	if (gameTrackerX.drawTimerReturn != NULL)
	{
		gameTrackerX.drawTimerReturn[0] = TIMER_TimeDiff(gameTrackerX.usecsStartDraw);
		gameTrackerX.drawTimerReturn = NULL;
		gameTrackerX.reqDisp = ((DISPENV*)gameTrackerX.disp) + gameTrackerX.gameData.asmData.dispPage;
	}
}

void FadeOutSayingLoading(struct GameTracker* gameTracker)
{ 
	struct POLY_F4_SEMITRANS* transPrim;
	unsigned long** drawot;
	long fadeTime;

	fadeTime = 0;
	drawot = gameTracker->drawOT;
	transPrim = (struct POLY_F4_SEMITRANS*)gameTracker->primPool->nextPrim;

	DRAW_TranslucentQuad(0, 0, SCREEN_WIDTH, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 2, gameTracker->primPool, drawot);
	FONT_Flush();
	
	do
	{
		fadeTime += 16;

		if (fadeTime >= 256)
		{
			fadeTime = 255;
		}

		gameTracker->drawPage = 1 - gameTracker->drawPage;
		setRGB0(transPrim, fadeTime, fadeTime, fadeTime);

#if !defined(PSXPC_VERSION)
		while (CheckVolatile(gameTracker->drawTimerReturn) != 0)
		{

		}
#endif

		PutDrawEnv(&draw[gameTracker->drawPage]);

#if !defined(PSXPC_VERSION)
		while (CheckVolatile(gameTracker->reqDisp) != 0)
		{

		}
#endif
		
#if defined(PSXPC_VERSION)
		gameTracker->drawTimerReturn = (unsigned long long*)&gameTracker->drawTime;
#else
		gameTracker->drawTimerReturn = (long*)&gameTracker->drawTime;
#endif
		gameTracker->gameData.asmData.dispPage = 1 - gameTracker->gameData.asmData.dispPage;
		
		VSync(0);

#if defined(PSX_VERSION)
#if defined(USE_32_BIT_ADDR)
		DrawOTag((uintptr_t*)drawot + 3071 * 2);
#else
		DrawOTag((uintptr_t*)drawot + 3071);
#endif
#endif
	} while (fadeTime < 255);
}

void CheckForDevStation()
{
#if !defined(PSXPC_VERSION)
	long* a1 = (long*)0x80180000;
	long* a2 = (long*)0x80380000;

	a1[0] = 0x12340000;
	a2[0] = 0x12345678;

	if (a1[0] == a2[0])
	{
		devstation = 0;
	}
	else
	{
		devstation = 1;
	}
#endif
}

void MAIN_ShowLoadingScreen()
{ 
	int *loadingScreen;
	char langChar[5];
	int lang;
	char filename[64];

	langChar[0] = 'F';
	langChar[1] = 'G';
	langChar[2] = 'I';
	langChar[3] = 'S';

	VSync(0);
	lang = localstr_get_language();

	if (lang != language_english)
	{
		sprintf(filename, "\\kain2\\game\\psx\\loading%c.tim", langChar[lang - 1]);
		loadingScreen = (int*)LOAD_ReadFile(filename, 11);
	}
	else
	{
		loadingScreen = (int*)LOAD_ReadFile("\\kain2\\game\\psx\\loading.tim", 11);
	}

	if (loadingScreen != NULL)
	{
		screen_to_vram(loadingScreen, gameTrackerX.gameData.asmData.dispPage);
		MEMPACK_Free((char*)loadingScreen);
	}

#if defined(PSXPC_VERSION)
	VSync(0);
	GAMEPAD_Process(&gameTrackerX);
#endif
}

long* MAIN_LoadTim(char* name)
{
	return LOAD_ReadFile(name, 11);
}

void init_menus(struct GameTracker* gt)
{ 
	struct menu_t* menu;
	
	menu = (struct menu_t*)MEMPACK_Malloc(menu_data_size(), 0x2D);
	menu_initialize(menu, gt);
	gt->menu = menu;
}

void MAIN_DoMainInit()
{
	InitDisplay();
	InitGeom();
	SetGeomOffset(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	SetGeomScreen(320);
	VRAM_InitVramBlockCache();
	FONT_Init();
	gameTrackerX.reqDisp = NULL;

	VSyncCallback(VblTick);
	DrawSyncCallback(DrawCallback);

	GAMEPAD_Init();
	SOUND_Init();
	VOICEXA_Init();

	if (nosound != 0)
	{
		SOUND_SfxOff();

		gameTrackerX.sound.gSfxOn = 0;
		gameTrackerX.sound.gVoiceOn = 0;
	}

	if (nomusic != 0)
	{
		SOUND_MusicOff();

		gameTrackerX.sound.gMusicOn = 0;
	}

	if (!(gameTrackerX.debugFlags & 0x80000))
	{
		gameTrackerX.sound.gVoiceOn = 0;
	}

#if defined(PSX_VERSION)
	init_menus(&gameTrackerX);
	SAVE_Init(&gameTrackerX);
#else
	int v0 = menu_data_size();
	struct menu_t *v1 = MEMPACK_Malloc(v0, 0x2Du);
	menu_initialize(v1, &gameTrackerX);
	gameTrackerX.menu = v1;
#endif

	srand(0);
}

void MAIN_InitVolume()
{
	aadInitVolume();
	aadStartMasterVolumeFade(gameTrackerX.sound.gMasterVol, 256, NULL);
	gameTrackerX.sound.soundsLoaded = 1;
	aadSetNoUpdateMode(0);
}

void MAIN_ResetGame()
{
	GAMELOOP_SetScreenWipe(0, 0, -1);
	gameTrackerX.gameFlags &= 0xFFFFFF7F;
	gameTrackerX.gameFlags &= 0xFFFFFFEF;
	gameTrackerX.gameFlags &= 0xFFFFFFFE;
	RAZIEL_StartNewGame();
}

void MAIN_MainMenuInit()
{
	char sfxFileName[64];

	mainMenuMode = 0;
	mainMenuTimeOut = 0;

	strcpy(sfxFileName, "\\kain2\\sfx\\object\\mainmenu\\mainmenu.snf");
	mainMenuSfx = 0;
	if (LOAD_DoesFileExist(sfxFileName) != 0)
	{
		mainMenuSfx = aadLoadDynamicSfx("mainmenu", 0, 0);
	}

	do
	{
		if (aadGetNumLoadsQueued() == 0)
		{
			break;
		}

		aadProcessLoadQueue();
	} while (1);

	mainMenuScreen = (int*)MAIN_LoadTim("\\kain2\\game\\psx\\frontend\\title1.tim");
	VRAM_EnableTerrainArea();

#if !defined(DEMO_BUILD)//TODO face.tim load should return null if not exists.
	menuface_initialize();
#endif

	currentMenu = mainMenu;
	gameTrackerX.gameMode = 4;
	menu_set(gameTrackerX.menu, menudefs_main_menu);
}

void MAIN_FreeMainMenuStuff()
{
	menuface_terminate();
	VRAM_DisableTerrainArea();

	if (mainMenuScreen != NULL)
	{
		MEMPACK_Free((char*)mainMenuScreen);
		mainMenuScreen = NULL;
	}

	aadFreeDynamicSfx(mainMenuSfx);

	do
	{
		if (aadGetNumLoadsQueued() == 0)
		{
			break;
		}

		aadProcessLoadQueue();

	} while (1);
}

void MAIN_StartGame()
{ 
	if (mainMenuFading != 0)
	{
		mainTrackerX.mainState = 2;
		MAIN_FreeMainMenuStuff();
		gEndGameNow = 0;
		mainMenuFading = 0;
		currentMenu = &standardMenu[0];
	}
	else
	{
		gameTrackerX.gameMode = 0;
		currentMenu = NULL;
		mainMenuFading = 1;
		GAMELOOP_SetScreenWipe(-30, 30, 10);
	}
}

long MAIN_DoMainMenu(struct GameTracker *gameTracker, struct MainTracker *mainTracker, long menuPos)
{
	unsigned long **drawot;

	gameTrackerX.timeMult = 1;

	drawot = gameTracker->drawOT;
	DrawPrim(&clearRect[gameTracker->drawPage]);

	GAMEPAD_Process(gameTracker);
	DEBUG_Process(gameTracker);

	if(mainMenuScreen != NULL)
	{
		screen_to_vram(mainMenuScreen, gameTracker->drawPage);
	}

	GAMELOOP_HandleScreenWipes(drawot);
	MENUFACE_RefreshFaces();
	FONT_Flush();
	mainMenuTimeOut++;
	GAMELOOP_FlipScreenAndDraw(gameTracker, drawot);

#if !defined(EDITOR)
	if (mainMenuFading != 0 && gameTracker->wipeTime == -1)
#endif
	{
		MAIN_StartGame();
	}

	return 0;
}

#if defined(PSXPC_VERSION) && defined(NO_CD)
	#if defined(SN_TARGET_PSP2)
		#define BIGFILE_DAT "ux0:app\\TEST12345\\bigfile.dat"
	#else
		#define BIGFILE_DAT "BIGFILE.DAT"
	#endif
#else
	#define BIGFILE_DAT "\\BIGFILE.DAT;1"
#endif

#if defined(__EMSCRIPTEN__)
void CloseGame()
{
	SOUND_StopAllSound();
	SOUND_Free();
	SetDispMask(0);
	DrawSync(0);
	VSync(0);
	DrawSyncCallback(NULL);
	VSyncCallback(NULL);
	EnterCriticalSection();
	StopRCnt(0xF2000000);
	DisableEvent(__timerEvent);
	CloseEvent(__timerEvent);
	ExitCriticalSection();
	VSync(5);
	StopCallback();
	ResetGraph(0);
}

int g_initiedGame = 0;

void InitialiseGame(void* appData)
{
	struct MainTracker* mainTracker;
	struct GameTracker* gameTracker;

	if (g_initiedGame == 0)
	{
		g_initiedGame = 1;

		Emulator_Initialise(GAME_NAME, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

		CheckForDevStation();

		mainTracker = &mainTrackerX;
		gameTracker = &gameTrackerX;
		mainOptionsInit = 0;

		if (MainG2_InitEngine(appData, SCREEN_WIDTH, SCREEN_HEIGHT, NULL) == 1)
		{
			MEMPACK_Init();
			LOAD_InitCd();
			StartTimer();

			STREAM_InitLoader(BIGFILE_DAT, "");

			localstr_set_language(language_default);
			GAMELOOP_SystemInit(gameTracker);

			gameTracker->lastLvl = 255;
			gameTracker->currentLvl = 255;
			gameTracker->disp = &disp;

			ProcessArgs(&gameTracker->baseAreaName[0], gameTracker);
			InitMainTracker(mainTracker);
			MAIN_DoMainInit();

			mainTracker->mainState = 6;
			mainTracker->movieNum = 0;
		}
	}
}

void GameLoop()
{
	struct GameTracker* gameTracker = &gameTrackerX;
	struct MainTracker* mainTracker = &mainTrackerX;
	struct InterfaceItem* item;
	long menuPos = 0;
	int timer;

	InitialiseGame(NULL);

	switch (mainTrackerX.mainState)
	{
	case 1:
		SOUND_UpdateSound();

		if ((gameTracker->debugFlags & 0x80000))
		{
			VOICEXA_Tick();
		}

		PSX_GameLoop(gameTracker);

		if (gameTracker->levelDone != 0)
		{
			FadeOutSayingLoading(gameTracker);
			aadStopAllSfx();
			STREAM_DumpAllLevels(0, 0);
			RemoveAllObjects(gameTracker);

			while (aadGetNumLoadsQueued() != 0 || aadMem->sramDefragInfo.status != 0)
			{
				SOUND_UpdateSound();
				STREAM_PollLoadQueue();
			}

			SOUND_ShutdownMusic();
			MEMPACK_FreeByType(14);
			MEMPACK_DoGarbageCollection();

			if (gameTracker->levelDone == 2)
			{
				mainTracker->mainState = 8;
			}
			else if (gameTracker->levelDone == 3)
			{
				mainTracker->mainState = 6;
				mainTracker->movieNum = 4;

			}
			else if (gameTracker->levelDone == 4)
			{
				mainTracker->mainState = 2;

				if (!(gameTracker->streamFlags & 0x200000))
				{
					SAVE_ClearMemory(&gameTrackerX);
				}
			}
			else
			{
				mainTracker->mainState = 2;
			}
		}
		break;
	case 2:
		if ((gameTrackerX.streamFlags & 0x1000000))
		{
			play_movie(&InterfaceItems[2].name[0]);
			gameTrackerX.streamFlags &= 0xFEFFFFFF;
		}

		if ((gameTrackerX.streamFlags & 0x200000))
		{
			gameTrackerX.streamFlags &= 0xFFDFFFFF;
		}

		if (nosound == 0)
		{
			MAIN_InitVolume();
		}

		MAIN_ShowLoadingScreen();
		FONT_ReloadFont();
		DrawSync(0);

		STREAM_Init();
		gameTracker->frameCount = 0;
		GAMELOOP_LevelLoadAndInit(&gameTracker->baseAreaName[0], gameTracker);
		gameTracker->levelDone = 0;
		mainTracker->mainState = 1;

		while (STREAM_PollLoadQueue() != 0)
		{

		}

		gameTrackerX.vblFrames = 0;

		break;
	case 4:

#if !defined(_DEBUG) && !defined(__EMSCRIPTEN__) || defined(NO_FILESYSTEM)
		LOAD_ChangeDirectory("Menustuff");
#endif

	checkMovie:
		while ((unsigned)mainTracker->movieNum < 6)
		{
			item = &InterfaceItems[mainTracker->movieNum];
			gameTrackerX.gameFlags &= -2;
			show_screen(&item->name[0]);
#if defined(PSXPC_VERSION)
			DrawOTag(NULL);
#endif

			timer = 1;
			if (item->timeout != 0)
			{
				do
				{
					GAMEPAD_Process(gameTracker);

					if (item->buttonTimeout < timer && (gameTracker->controlCommand[0][1] & 0x80))
					{
						break;
					}

					VSync(0);
					timer++;
				} while (timer < item->timeout);
			}
			mainTracker->movieNum = item->nextItem;
			if (item->nextItem < 0)
			{
				goto checkMovie;
			}
		}

		FONT_ReloadFont();

		if (mainTracker->mainState != 6)
		{
			if (DoMainMenu == 0)
			{
				MAIN_ResetGame();
				gameTrackerX.gameMode = 0;
				mainMenuFading = 1;
				MAIN_StartGame();
			}
			else
			{
				mainTracker->mainState = 8;
			}
		}
		break;
	case 6:

		CINE_Load();
		if (mainTracker->movieNum >= 0)
		{
			do
			{
				if (CINE_Loaded() != 0)
				{
					CINE_Play(InterfaceItems[mainTracker->movieNum].name, 0xFFFFu, 2);
					ClearDisplay();
				}

				mainTracker->movieNum = InterfaceItems[mainTracker->movieNum].nextItem;

				if (InterfaceItems[mainTracker->movieNum].itemType != 0)
				{
					mainTracker->mainState = 4;
					break;
				}
			} while (mainTracker->movieNum >= 0);
		}

		CINE_Unload();

		if (mainTracker->movieNum < 0)
		{
			mainTracker->mainState = 8;
		}

		if (nosound == 0)
		{
			SOUND_StopAllSound();
		}

		if (mainTracker->movieNum < 0)
		{
			mainTracker->mainState = 8;
		}

		if (nosound == 0)
		{
			SOUND_StopAllSound();
		}

		break;
	case 7:
		mainTracker->movieNum = 1;
		break;
	case 8:

		ProcessArgs(gameTracker->baseAreaName, gameTracker);
		MAIN_ResetGame();
#if !defined(_DEBUG) && !defined(__EMSCRIPTEN__) || defined(NO_FILESYSTEM)
		LOAD_ChangeDirectory("Menustuff");
#endif
		MAIN_MainMenuInit();
		MAIN_InitVolume();
		SAVE_ClearMemory(&gameTrackerX);
		FONT_ReloadFont();
		mainTracker->mainState = 9;
		break;
	case 9:
		menuPos = MAIN_DoMainMenu(gameTracker, mainTracker, menuPos);
		break;
	case 3:
	case 5:
	default:
		break;		
	}

	STREAM_PollLoadQueue();
}

int MainG2(void* appData)
{

	emscripten_set_main_loop(GameLoop, 0, 1);

	CloseGame();

	MainG2_ShutDownEngine(appData);

	Emulator_ShutDown();

	return 0;
}

#else
int MainG2(void *appData)
{ 
	struct MainTracker* mainTracker;
	struct GameTracker* gameTracker;
	long menuPos;
#if defined(PSXPC_VERSION)
	struct InterfaceItem* item = NULL;
#else
	struct InterfaceItem* item;
#endif
	int timer;

#if defined(PSXPC_VERSION)

#if defined(EDITOR)
	Emulator_Initialise(GAME_NAME, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, ((int*)appData)[0]);
#else
	Emulator_Initialise(GAME_NAME, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
#endif
	Emulator_HintOTSize(3071);
#endif

	menuPos = 0;
	CheckForDevStation();
	
	mainTracker = &mainTrackerX;
	gameTracker = &gameTrackerX;
	mainOptionsInit = 0;

	if (MainG2_InitEngine(appData, SCREEN_WIDTH, SCREEN_HEIGHT, NULL) == 1)
	{
		MEMPACK_Init();
		LOAD_InitCd();
		StartTimer();

		STREAM_InitLoader(BIGFILE_DAT, "");

		localstr_set_language(language_default);
		GAMELOOP_SystemInit(gameTracker);

		gameTracker->lastLvl = -1;
		gameTracker->currentLvl = -1;
		gameTracker->disp = &disp;

		ProcessArgs(&gameTracker->baseAreaName[0], gameTracker);
		InitMainTracker(mainTracker);
		MAIN_DoMainInit();

		mainTracker->mainState = 6;
		mainTracker->movieNum = 0;

#if !defined(__EMSCRIPTEN__)
		while (mainTracker->done == 0)
		{
#if defined(EDITOR)
			if (stopGameThread != 0)
			{
				break;
			}
#endif
			switch (mainTrackerX.mainState)
			{
			case 1:
				SOUND_UpdateSound();

				if ((gameTracker->debugFlags & 0x80000))
				{
					VOICEXA_Tick();
				}

				PSX_GameLoop(gameTracker);

				if (gameTracker->levelDone != 0)
				{
					FadeOutSayingLoading(gameTracker);
					aadStopAllSfx();
					STREAM_DumpAllLevels(0, 0);
					RemoveAllObjects(gameTracker);

					while (aadGetNumLoadsQueued() != 0 || aadMem->sramDefragInfo.status != 0)
					{
						SOUND_UpdateSound();
						STREAM_PollLoadQueue();
					}

					SOUND_ShutdownMusic();
					MEMPACK_FreeByType(14);
					MEMPACK_DoGarbageCollection();

					if (gameTracker->levelDone == 2)
					{
						mainTracker->mainState = 8;
					}
					else if (gameTracker->levelDone == 3)
					{
						mainTracker->mainState = 6;
						mainTracker->movieNum = 4;

					}
					else if (gameTracker->levelDone == 4)
					{
						mainTracker->mainState = 2;

						if (!(gameTracker->streamFlags & 0x200000))
						{
							SAVE_ClearMemory(&gameTrackerX);
						}
					}
					else
					{
						mainTracker->mainState = 2;
					}
				}
				break;
			case 2:
				if ((gameTrackerX.streamFlags & 0x1000000))
				{
					play_movie(&InterfaceItems[2].name[0]);
					gameTrackerX.streamFlags &= 0xFEFFFFFF;
				}

				if ((gameTrackerX.streamFlags & 0x200000))
				{
					gameTrackerX.streamFlags &= 0xFFDFFFFF;
				}

				if (nosound == 0)
				{
					MAIN_InitVolume();
				}

				MAIN_ShowLoadingScreen();
				FONT_ReloadFont();
				DrawSync(0);

				STREAM_Init();
				gameTracker->frameCount = 0;
				GAMELOOP_LevelLoadAndInit(&gameTracker->baseAreaName[0], gameTracker);
				gameTracker->levelDone = 0;
				mainTracker->mainState = 1;

				while (STREAM_PollLoadQueue() != 0)
				{

				}

				gameTrackerX.vblFrames = 0;

				break;
			case 4:

#if !defined(_DEBUG) && !defined(__EMSCRIPTEN__) || defined(NO_FILESYSTEM)
				LOAD_ChangeDirectory("Menustuff");
#endif

			checkMovie:
#if !defined(DEMO_BUILD)
				while ((unsigned)mainTracker->movieNum < 6)
#else
				while ((unsigned)mainTracker->movieNum < 14)
#endif
				{
					item = &InterfaceItems[mainTracker->movieNum];
					gameTrackerX.gameFlags &= -2;
					show_screen(&item->name[0]);
#if defined(PSXPC_VERSION)
					DrawOTag(NULL);
#endif

					timer = 1;
					if (item->timeout != 0)
					{
						do
						{
							GAMEPAD_Process(gameTracker);

							if (item->buttonTimeout < timer && (gameTracker->controlCommand[0][1] & 0x80))
							{
								break;
							}

							VSync(0);
							timer++;
						} while (timer < item->timeout);
					}
					mainTracker->movieNum = item->nextItem;
					if (item->nextItem < 0)
					{
						goto checkMovie;
					}
				}
				
				FONT_ReloadFont();

				if (mainTracker->mainState != 6)
				{
					if (DoMainMenu == 0)
					{
						MAIN_ResetGame();
						gameTrackerX.gameMode = 0;
						mainMenuFading = 1;
						MAIN_StartGame();
					}
					else
					{
						mainTracker->mainState = 8;
					}
				}
				break;
			case 6:
				CINE_Load();
				if (mainTracker->movieNum >= 0)
				{
					do
					{
						if (CINE_Loaded() != 0)
						{
							CINE_Play(InterfaceItems[mainTracker->movieNum].name, 0xFFFFu, 2);
							ClearDisplay();
						}

						mainTracker->movieNum = InterfaceItems[mainTracker->movieNum].nextItem;

						if (InterfaceItems[mainTracker->movieNum].itemType != 0)
						{
							mainTracker->mainState = 4;
							break;
						}
					} while (mainTracker->movieNum >= 0);
				}

				CINE_Unload();

				if (mainTracker->movieNum < 0)
				{
					mainTracker->mainState = 8;
				}

				if (nosound == 0)
				{
					SOUND_StopAllSound();
				}

				break;
			case 7:
				mainTracker->movieNum = 1;
				break;
			case 8:
				ProcessArgs(gameTracker->baseAreaName, gameTracker);
				MAIN_ResetGame();
#if !defined(_DEBUG) && !defined(__EMSCRIPTEN__) || defined(NO_FILESYSTEM)
				LOAD_ChangeDirectory("Menustuff");
#endif
				MAIN_MainMenuInit();
				MAIN_InitVolume();
				SAVE_ClearMemory(&gameTrackerX);
				FONT_ReloadFont();
				mainTracker->mainState = 9;
				break;
			case 9:
				menuPos = MAIN_DoMainMenu(gameTracker, mainTracker, menuPos);
				break;
			case 3:
			case 5:
			default:
				break;
			}

			STREAM_PollLoadQueue();
		}
#endif

		SOUND_StopAllSound();
		SOUND_Free();
		SetDispMask(0);
		DrawSync(0);
		VSync(0);
		DrawSyncCallback(NULL);
		VSyncCallback(NULL);
		EnterCriticalSection();
		StopRCnt(0xF2000000);
		DisableEvent(__timerEvent);
		CloseEvent(__timerEvent);
		ExitCriticalSection();
		VSync(5);
		StopCallback();
		ResetGraph(0);
	}

	MainG2_ShutDownEngine(appData);

#if defined(PSXPC_VERSION)
	Emulator_ShutDown();

#if defined(EDITOR)
	stopGameThread = 0;
#endif
#endif

	return 0;
}
#endif
