#include "CORE.H"
#include "CAMERA.H"
#include "GAMELOOP.H"
#include "GAMEPAD.H"
#include "SOUND.H"
#include "EVENT.H"
#include "OBTABLE.H"
#include "MEMPACK.H"
#include "PSX/MAIN.H"
#include "TIMER.H"
#include "PSX/DRAWS.H"
#include "DRAW.H"
#include "SAVEINFO.H"
#include "PLAN/PLANAPI.H"
#include "PLAN/ENMYPLAN.H"
#include "FX.H"
#include "STRMLOAD.H"
#include "SIGNAL.H"
#include "VOICEXA.H"
#include "G2/ANIMG2.H"
#include "FONT.H"
#include "DEBUG.H"
#include "G2/INSTNCG2.H"
#include "COLLIDE.H"
#include "PIPE3D.H"
#include "VM.H"
#include "LIGHT3D.H"
#include "UNDRWRLD.H"
#include "GLYPH.H"
#include "PSX/DRAWS.H"
#include "PSX/COLLIDES.H"
#include "BSP.H"
#include "MENU/MENU.H"
#include "MENU/MENUDEFS.H"
#include "G2/MAING2.H"
#include "MATH3D.H"

//#include <assert.h>

char* primBase;
unsigned long(**gOt[2]); // offset 0x800D0C0C
struct LightInfo* gLightInfo; // offset 0x800D0C2C
void* enemyPlanPool; // offset 0x800D0C30
struct _ObjectTracker* GlobalObjects; // offset 0x800D0C38
void* planningPool; // offset 0x800D0C34
struct _VertexPool* gVertexPool; // offset 0x800D0C28
struct _PrimPool(*primPool[2]); // offset 0x800D0C14
struct _PolytopeList* gPolytopeList; // offset 0x800d2fb4
struct GameTracker gameTrackerX;
struct Camera theCamera;
struct _PrimPool primPool0, primPool1;

struct _InstanceList* instanceList; // offset 0x800D0C20
struct _InstancePool* instancePool; // offset 0x800D0C24

struct GameTracker* gameTracker;

void GAMELOOP_AllocStaticMemory() // Matching - 100%
{
	instanceList = (struct _InstanceList*)MEMPACK_Malloc(sizeof(struct _InstanceList), 6);
	instancePool = (struct _InstancePool*)MEMPACK_Malloc(sizeof(struct _InstancePool), 6);

#if defined(PSXPC_VERSION)//Increase primitive memory pools to allow 32_BIT_ADDR mode.
#if defined(GAME_X64)
	primBase = MEMPACK_Malloc(216600 * 12, 6);
	gOt[1] = (unsigned long**)(primBase + ((3072 * 8) * 2));
	gOt[0] = (unsigned long**)(primBase);
	primPool[0] = (struct _PrimPool*)(primBase + (((3072 * 8) * 2) + ((3072 * 8) * 2)));
	primPool[1] = (struct _PrimPool*)(primBase + (((3072 * 8) * 2) + ((3072 * 8) * 2) + (96012 * 2)));
#else
	primBase = MEMPACK_Malloc(216600 * 6, 6);
	gOt[1] = (unsigned long**)(primBase + ((3072 * 4) * 2));
	gOt[0] = (unsigned long**)(primBase);
	primPool[0] = (struct _PrimPool*)(primBase + (((3072 * 4) * 2) + ((3072 * 4) * 2)));
	primPool[1] = (struct _PrimPool*)(primBase + (((3072 * 4) * 2) + ((3072 * 4) * 2) + (96012 * 2)));
#endif
	gLightInfo = (struct LightInfo*)MEMPACK_Malloc(sizeof(struct LightInfo), 6);
	memset(gLightInfo, 0, sizeof(struct LightInfo));
#else
	primBase = MEMPACK_Malloc(216600, 6);
	gOt[1] = (unsigned long**)(primBase + (3072 * 4));
	gOt[0] = (unsigned long**)(primBase);
	primPool[0] = (struct _PrimPool*)(primBase + ((3072 * 4) + (3072 * 4)));
	primPool[1] = (struct _PrimPool*)(primBase + ((3072 * 4) + (3072 * 4) + (96012)));
	gLightInfo = (struct LightInfo*)MEMPACK_Malloc(sizeof(struct LightInfo), 6);
	memset(gLightInfo, 0, sizeof(struct LightInfo));
#endif

	gVertexPool = (struct _VertexPool*)MEMPACK_Malloc(sizeof(struct _VertexPool), 6);
	gPolytopeList = (struct _PolytopeList*)gVertexPool;
	fxTracker = (struct _FXTracker*)MEMPACK_Malloc(sizeof(struct _FXTracker), 6);
	gFXT = fxTracker;
	planningPool = MEMPACK_Malloc(3000, 6);
	enemyPlanPool = MEMPACK_Malloc(1000, 6);
	GlobalObjects = (struct _ObjectTracker*)MEMPACK_Malloc(1728, 6);
	G2Anim_Install();
}


void GAMELOOP_InitGameTracker()
{ 
	int i;

#if defined(PSXPC_VERSION)
	primPool[0]->lastPrim = &primPool[0]->prim[23990*2];
	primPool[1]->lastPrim = &primPool[1]->prim[23990*2];
#else
	primPool[0]->lastPrim = &primPool[0]->prim[23990];
	primPool[1]->lastPrim = &primPool[1]->prim[23990];
#endif

	primPool[0]->nextPrim = &primPool[0]->prim[0];
	primPool[1]->nextPrim = &primPool[1]->prim[0];

	gameTrackerX.instancePool = instancePool;
	gameTrackerX.vertexPool = gVertexPool;
	gameTrackerX.multGameTime = 10;
	gameTrackerX.material_fadeValue = 4096;
	gameTrackerX.planningPool = planningPool;
	gameTrackerX.enemyPlanPool = enemyPlanPool;
	gameTrackerX.GlobalObjects = GlobalObjects;
	gameTrackerX.instanceList = instanceList;
	gameTrackerX.lightInfo = gLightInfo;
	gameTrackerX.spectral_fadeValue = 0;
	gameTrackerX.decoupleGame = 1;
	gameTrackerX.frameRateLock = 1;
	gameTrackerX.primPool = primPool[0];
	gameTrackerX.drawOT = gOt[0];
	gameTrackerX.dispOT = gOt[1];
	gameTrackerX.frameRate24fps = 1;

	for (i = 0; i < 48; i++)
	{
		gameTrackerX.GlobalObjects[i].objectStatus = 0;
	}

	gameTrackerX.gameData.asmData.MorphTime = 1000;
	
	OBTABLE_ClearObjectReferences();
	EVENT_Init();
}

void GAMELOOP_SystemInit(struct GameTracker* gameTracker) // Matching - 100%
{ 
	GAMELOOP_AllocStaticMemory();
	INSTANCE_InitInstanceList(instanceList, instancePool);
	GAMELOOP_InitGameTracker();
}

void GAMELOOP_ResetGameStates() // Matching - 100%
{
	EVENT_Init();
}

void GAMELOOP_ClearGameTracker() // Matching - 100%
{
	gameTrackerX.gameData.asmData.MorphTime = 1000;
	gameTrackerX.currentTime = 0;
	gameTrackerX.currentTicks = 0;
	gameTrackerX.gameFlags = 0;
	gameTrackerX.frameCount = 0;
	gameTrackerX.fps30Count = 0;
	gameTrackerX.currentHotSpot = 0;
	gameTrackerX.SwitchToNewStreamUnit = 0;
	gameTrackerX.gameData.asmData.drawBackFaces = 0;
	pause_redraw_flag = 0;
	pause_redraw_prim = NULL;
#if !defined(_DEBUG) || (!defined(__EMSCRIPTEN__) && !defined(_DEBUG))
	gameTrackerX.debugFlags |= 0x40000;
#endif
}

void GAMELOOP_CalcGameTime() // Matching - 100%
{
	long time;

	time = (gameTrackerX.currentTimeOfDayTime * gameTrackerX.multGameTime) / 60000;
	gameTrackerX.timeOfDay = (((((time + 720) / 60) % 24) * 100) + ((time + 720) % 60));
}

void GAMELOOP_SetGameTime(long timeOfDay) // Matching - 99.59%
{
	long tim;

	tim = ((timeOfDay / 100) * 60) + (timeOfDay % 100) - 720;

	if (tim < 0)
	{
		tim += 2160;
	}

	gameTrackerX.timeOfDay = (short)timeOfDay;
	gameTrackerX.currentTimeOfDayTime = (((((tim * 32) - tim) * 4) + tim) * 480) / gameTrackerX.multGameTime;
	gameTrackerX.currentMaterialTime = gameTrackerX.currentTimeOfDayTime;
}

int GAMELOOP_GetTimeOfDay() // Matching - 100%
{
	int timeOfDay;

	timeOfDay = gameTrackerX.timeOfDay;

	if ((timeOfDay - 601) < 99U)
	{
		return 600;
	}
	else if ((timeOfDay - 700) < 1100U)
	{
		return 700;
	}
	else if ((timeOfDay - 1800) < 100U)
	{
		return 1800;
	}

	return 1900;
}

int GAMELOOP_GetTimeOfDayIdx(int timeOfDay)//Matching - 100%
{
	if ((unsigned int)(timeOfDay - 601) < 99)
	{
		return 1;
	}

	if ((unsigned int)(timeOfDay - 700) < 1100)
	{
		return 2;
	}

	if ((unsigned int)(timeOfDay - 1800) < 100)
	{
		return 3;
	}

	return 0;
}

int GAMELOOP_WaitForLoad() // Matching - 100%
{ 
	if ((gameTrackerX.debugFlags & 0x80000))
	{
		VOICEXA_Tick();
	}

	return STREAM_PollLoadQueue();
}

struct _StreamUnit* LoadLevels(char* baseAreaName, struct GameTracker* gameTracker) // Matching - 100%
{
	struct _SVector offset;
	struct _StreamUnit* streamUnit;
	static char oldArea[16] = { "under1" };

	if (strlen(oldArea) != 0)
	{
		STREAM_AbortAreaLoad(oldArea);
	}

	strcpy(oldArea, baseAreaName);

#if !defined(_DEBUG) && !defined(__EMSCRIPTEN__) || defined(NO_FILESYSTEM)
	LOAD_ChangeDirectory(baseAreaName);
#endif

	streamUnit = STREAM_LoadLevel(baseAreaName, NULL, 0);

	if (streamUnit->used == 1)
	{
		int num;
		int waitFor;

		DRAW_LoadingMessage();

		while (streamUnit->used == 1)
		{
			GAMELOOP_WaitForLoad();
		}

		STREAM_NextLoadFromHead();

		STREAM_LoadMainVram(gameTracker, baseAreaName, streamUnit);

		STREAM_NextLoadAsNormal();

		waitFor = GAMELOOP_WaitForLoad() - 1;

		do
		{
			num = GAMELOOP_WaitForLoad();

			if (num == 0)
			{
				break;
			}

		} while (num >= waitFor);
	}

	else
	{
		STREAM_DumpLoadingObjects();

		STREAM_LoadMainVram(gameTracker, baseAreaName, streamUnit);
	}

	if (streamUnit->level->startUnitMainSignal != NULL)
	{
		if (gameTracker->playerInstance != NULL)
		{
			streamUnit->level->startUnitMainSignal->flags |= 0x1;

			SIGNAL_HandleSignal(gameTracker->playerInstance, streamUnit->level->startUnitMainSignal->signalList, 0);

			EVENT_AddSignalToReset(streamUnit->level->startUnitMainSignal);
		}
	}

	ADD_VEC(&offset, &streamUnit->level->terrain->BSPTreeArray->bspRoot->sphere.position, &streamUnit->level->terrain->BSPTreeArray->globalOffset);

	offset.x = -offset.x;
	offset.y = -offset.y;
	offset.z = -offset.z;

#if defined(EDITOR)
	extern struct _SVector g_relocationOffset;
	g_relocationOffset = offset;
#endif

	PreloadAllConnectedUnits(streamUnit, &offset);

	return streamUnit;
}

void GAMELOOP_InitStandardObjects() // Matching - 95%
{
	static char* sobjects[10] = {
		(char*)"raziel",
		(char*)"paths",
		(char*)"glphicon",
		(char*)"sreavr",
		(char*)"soul",
		(char*)"force",
		(char*)"particle",
		(char*)"healths",
		(char*)"eaggot",
		(char*)"eaggots"
	};

	int i;

	LOAD_DumpCurrentDir();

	for (i = 0; i < 10U; i++)
	{
		InsertGlobalObject(sobjects[i], &gameTrackerX);
	}
}

void GAMELOOP_LevelLoadAndInit(char* baseAreaName, struct GameTracker* gameTracker) // Matching - 100%
{
	long i;
	struct _StreamUnit* streamUnit;

	short _x1;
	short _y1;
	short _z1;
	struct _Position* _v0;
	struct _Position* _v1;

	struct BLK_FILL* temp;  // not from SYMDUMP

	G2Anim_ResetInternalState();
	gameTracker->playerInstance = NULL;

	INSTANCE_InitInstanceList(instanceList, instancePool);
	GAMELOOP_ClearGameTracker();
	CAMERA_Initialize(&theCamera);
	PLANAPI_InitPlanning(planningPool);
	ENMYPLAN_InitEnemyPlanPool(enemyPlanPool);
	FX_Init(fxTracker);
	WARPGATE_Init();
	DRAW_InitShadow();
	GAMELOOP_InitStandardObjects();

	streamUnit = LoadLevels(baseAreaName, gameTracker);

	while (STREAM_PollLoadQueue() != 0)
	{

	}

	fontsObject = (struct Object*)objectAccess[2].object;

	gameTracker->introFX = (struct Object*)objectAccess[6].object;

	RENDER_currentStreamUnitID = (unsigned short)gameTracker->StreamUnitID;

	for (i = 0; i < streamUnit->level->numIntros; i++)
	{
#if defined(UWP)
		if (_strcmpi(streamUnit->level->introList[i].name, "raziel") == 0)
#else
		if (strcmpi(streamUnit->level->introList[i].name, "raziel") == 0)
#endif
		{
			INSTANCE_IntroduceInstance(&streamUnit->level->introList[i], (short)streamUnit->StreamUnitID);
			break;
		}
	}

	gameTracker->playerInstance->data = gameTracker->playerInstance->object->data;

	CAMERA_SetInstanceFocus(&theCamera, gameTracker->playerInstance);

	_v1 = &gameTracker->playerInstance->position;
	_x1 = _v1->x;
	_y1 = _v1->y;
	_z1 = _v1->z;

	_v0 = &theCamera.core.position;
	_v0->x = _x1;
	_v0->y = _y1;
	_v0->z = _z1;

#if defined(EDITOR)
	extern struct _Position overrideEditorPosition;
	extern struct _Rotation overrideEditorRotation;

	overrideEditorPosition = theCamera.core.position;
#endif

	SetFogNearFar(streamUnit->level->fogNear, streamUnit->level->fogFar, 320);
	SetFarColor(0, 0, 0);

	temp = clearRect;

	temp->r0 = streamUnit->level->backColorR;
	temp->g0 = streamUnit->level->backColorG;
	temp->b0 = streamUnit->level->backColorB;

	temp++;

	temp->r0 = streamUnit->level->backColorR;
	temp->g0 = streamUnit->level->backColorG;
	temp->b0 = streamUnit->level->backColorB;

	gameTracker->wipeType = 10;
	gameTracker->hideBG = 0;
	gameTracker->wipeTime = 30;
	gameTracker->maxWipeTime = 30 * FRAMERATE_MULT;

	if (streamUnit->level->startSignal != NULL)
	{
		streamUnit->level->startSignal->flags |= 0x1;
		SIGNAL_HandleSignal(gameTracker->playerInstance, streamUnit->level->startSignal->signalList, 0);
		EVENT_AddSignalToReset(streamUnit->level->startSignal);
	}

	gameTracker->vblFrames = 0;

	if (streamUnit->level->startUnitMainSignal != NULL && gameTracker->playerInstance != NULL)
	{
		streamUnit->level->startUnitMainSignal->flags |= 0x1;
		SIGNAL_HandleSignal(gameTracker->playerInstance, streamUnit->level->startUnitMainSignal->signalList, 0);
		EVENT_AddSignalToReset(streamUnit->level->startUnitMainSignal);
	}
}

void GAMELOOP_StreamLevelLoadAndInit(char* baseAreaName, struct GameTracker* gameTracker, int toSignalNum, int fromSignalNum) // Matching - 100%
{
	LoadLevels(baseAreaName, gameTracker);
}

void GAMELOOP_SetScreenWipe(int time, int maxTime, int type) // Matching - 100%
{
	gameTrackerX.maxWipeTime = maxTime * FRAMERATE_MULT;
	gameTrackerX.wipeTime = time;
	gameTrackerX.wipeType = type;
}

void GAMELOOP_HandleScreenWipes(unsigned long **drawot)
{
	long temp;
	struct _PrimPool *primPool;
	
	primPool = gameTrackerX.primPool;

	if ((GlobalSave->flags & 0x1))
	{
		DRAW_FlatQuad(&gameTrackerX.wipeColor, 0, 0, SCREEN_WIDTH, 0, 0, 30, SCREEN_WIDTH, 30, primPool, drawot);
		DRAW_FlatQuad(&gameTrackerX.wipeColor, 0, 210, SCREEN_WIDTH, 210, 0, SCREEN_HEIGHT, SCREEN_WIDTH, 210, primPool, drawot);
	}

	if (gameTrackerX.wipeTime > 0)
	{
		if (gameTrackerX.wipeType == 10)
		{
			temp = ((gameTrackerX.wipeTime << 8) - (gameTrackerX.wipeTime)) / gameTrackerX.maxWipeTime;
			DRAW_TranslucentQuad(0, 0, SCREEN_WIDTH, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, temp, temp, temp, 2, primPool, drawot);
		}
		else if (gameTrackerX.wipeType == 11)
		{
			temp = ((gameTrackerX.wipeTime << 8) - (gameTrackerX.wipeTime)) / gameTrackerX.maxWipeTime;
			DRAW_TranslucentQuad(0, 0, SCREEN_WIDTH, 0, 0, 30, SCREEN_WIDTH, 30, temp, temp, temp, 2, primPool, drawot);
			DRAW_TranslucentQuad(0, 210, SCREEN_WIDTH, 210, 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, temp, temp, temp, 2, primPool, drawot);

			GlobalSave->flags &= 0xFFFE;
		}
		
		if (gameTrackerX.gameFramePassed != 0)
		{
			gameTrackerX.wipeTime--;
		}
	}
	else
	{
		if (gameTrackerX.wipeTime < -1)
		{
			if (gameTrackerX.wipeType == 10)
			{
				temp = ((((gameTrackerX.maxWipeTime + gameTrackerX.wipeTime) + 2) << 8) - ((gameTrackerX.maxWipeTime + gameTrackerX.wipeTime) + 2)) / gameTrackerX.maxWipeTime;
				DRAW_TranslucentQuad(0, 0, SCREEN_WIDTH, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, temp, temp, temp, 2, primPool, drawot);
			}
			else if (gameTrackerX.wipeType == 11)
			{
				temp = (((gameTrackerX.maxWipeTime + gameTrackerX.wipeTime) << 8) - ((gameTrackerX.maxWipeTime + gameTrackerX.wipeTime) + 2)) / gameTrackerX.maxWipeTime;
				DRAW_TranslucentQuad(0, 0, SCREEN_WIDTH, 0, 0, 30, SCREEN_WIDTH, 30, temp, temp, temp, 2, primPool, drawot);
				DRAW_TranslucentQuad(0, 210, SCREEN_WIDTH, 210, 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, temp, temp, temp, 2, primPool, drawot);

				if (gameTrackerX.wipeTime == -2)
				{
					GlobalSave->flags |= 0x1;
				}
			}

			if (gameTrackerX.gameFramePassed != 0)
			{
				gameTrackerX.wipeTime++;
			}
		}

		if (gameTrackerX.wipeTime == -1)
		{
			if (gameTrackerX.wipeType == 11)
			{
				GlobalSave->flags |= 0x1;
			}
			else
			{
				DRAW_FlatQuad(&gameTrackerX.wipeColor, 0, 0, SCREEN_WIDTH, 0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, primPool, drawot);
			}
		}
		else
		{
			theCamera.core.screenScale.z = 4096;
			theCamera.core.screenScale.y = 4096;
			theCamera.core.screenScale.x = 4096;
		}
	}
}

void UpdateFogSettings(struct _StreamUnit* currentUnit, struct Level* level) // Matching - 100%
{
	int changed;
	int FogFar;
	int FogNear;
	int setflag;

	changed = 0;

	FogNear = currentUnit->TargetFogNear;

	FogFar = currentUnit->TargetFogFar;

	setflag = 0;

	if (FogNear < level->fogNear)
	{
		level->fogNear -= 500;

		changed = 1;

		if (FogNear >= level->fogNear)
		{
			setflag = 1;

			level->fogNear = FogNear;
		}
	}
	else if (level->fogNear < FogNear)
	{
		level->fogNear += 500;

		changed = 1;

		if (level->fogNear >= FogNear)
		{
			setflag = 1;

			level->fogNear = FogNear;
		}
	}
	else
	{
		setflag = 1;
	}

	if (FogFar < level->fogFar)
	{
		level->fogFar -= 500;

		changed = 1;

		if (FogFar >= level->fogFar)
		{
			level->fogFar = FogFar;
		}
		else
		{
			setflag = 0;
		}
	}
	else if (level->fogFar < FogFar)
	{
		level->fogFar += 500;

		changed = 1;

		if (level->fogFar >= FogFar)
		{
			level->fogFar = FogFar;
		}
		else
		{
			setflag = 0;
		}
	}

	if (changed != 0)
	{
		LIGHT_CalcDQPTable(level);

		if (setflag != 0)
		{
			currentUnit->TargetFogNear = level->fogNear;
			currentUnit->TargetFogFar = level->fogFar;
		}
	}
}

int CheckForNoBlend(struct _ColorType* Color) // Matching - 100%
{
	if ((Color->r < 5) && (Color->g < 5) && (Color->b < 5))
	{
		return 1;
	}

	return 0;
}

void BlendToColor(struct _ColorType* target, struct _ColorType* current, struct _ColorType* dest) // Matching - 100%
{
	LoadAverageCol((unsigned char*)target, (unsigned char*)current, 512, 3584, (unsigned char*)dest);

	if ((target->r - dest->r) >= 0)
	{
		if ((target->r - dest->r) >= 5)
		{
			dest->code = 0;

			return;
		}
	}
	else if ((dest->r - target->r) >= 5)
	{
		dest->code = 0;

		return;
	}

	if ((target->g - dest->g) >= 0)
	{
		if ((target->g - dest->g) >= 5)
		{
			dest->code = 0;

			return;
		}
	}
	else if ((dest->g - target->g) >= 5)
	{
		dest->code = 0;

		return;
	}

	if ((target->b - dest->b) >= 0)
	{
		if ((target->b - dest->b) >= 5)
		{
			dest->code = 0;

			return;
		}
	}
	else if ((dest->b - target->b) >= 5)
	{
		dest->code = 0;

		return;
	}

	*(int*)dest = *(int*)target;

	dest->code = 0;
}

void MainRenderLevel(struct _StreamUnit* currentUnit, unsigned long** drawot)
{
	struct Level* level;
	struct GameTracker* gameTracker;
	struct _Terrain* terrain;
	int curTree;
	long BackColor;
	struct _Position cam_pos_save;
	MATRIX cam_mat_save;
	struct _Instance* saveLightInstance;
	int time;
	int tod;
	struct _SVector tmp;
	struct BSPTree* bsp;

	level = currentUnit->level;
	terrain = level->terrain;

	UpdateFogSettings(currentUnit, level);

	gameTracker = &gameTrackerX;
	currentUnit->FrameCount = gameTracker->displayFrameCount;

	SetFogNearFar(level->fogNear, level->fogFar, 320);
	SetFarColor(0, 0, 0);

	clearRect[0].r0 = level->backColorR;
	clearRect[0].g0 = level->backColorG;
	clearRect[0].b0 = level->backColorB;

	clearRect[1].r0 = level->backColorR;
	clearRect[1].g0 = level->backColorG;
	clearRect[1].b0 = level->backColorB;

	if (gameTrackerX.gameData.asmData.MorphTime != 1000)
	{
		time = (gameTrackerX.gameData.asmData.MorphTime << 12) / 1000;

		if (gameTrackerX.gameData.asmData.MorphType == 1)
		{
			time = 4096 - time;
		}

		LoadAverageCol(&level->specturalColorR, &level->backColorR, 4096, 4096, (unsigned char*)&BackColor);
	}
	else
	{
		if (gameTrackerX.gameData.asmData.MorphType == 1)
		{
			BackColor = ((long*)&level->specturalColorR)[0];
		}
		else
		{
			BackColor = ((long*)&level->backColorR)[0];
		}
	}

	BlendToColor((struct _ColorType*)&BackColor, (struct _ColorType*)&currentUnit->FogColor, (struct _ColorType*)&currentUnit->FogColor);

	depthQBackColor = currentUnit->FogColor;

	tod = GAMELOOP_GetTimeOfDay();

	if (tod == 600 && tod != 1800 || !(level->unitFlags & 0x2))
	{
		if (gameTrackerX.gameData.asmData.MorphTime == 1000)
		{
			depthQBackColor = (depthQBackColor & 0xFFF8F8F8) | 0x40404;
		}
	}

	depthQFogStart = level->fogNear;
	depthQFogFar = level->fogFar;

	if (CheckForNoBlend((struct _ColorType*)&depthQBackColor) == 0)
	{
		depthQBlendStart = depthQFogStart;
	}
	else
	{
		depthQBlendStart = 0xFFFF;
	}

	clearRect[0].r0 = ((struct _ColorType*)&depthQBackColor)->r;
	clearRect[0].g0 = ((struct _ColorType*)&depthQBackColor)->g;
	clearRect[0].b0 = ((struct _ColorType*)&depthQBackColor)->b;

	clearRect[1].r0 = ((struct _ColorType*)&depthQBackColor)->r;
	clearRect[1].g0 = ((struct _ColorType*)&depthQBackColor)->g;
	clearRect[1].b0 = ((struct _ColorType*)&depthQBackColor)->b;

	PIPE3D_AnimateTerrainTextures(terrain->aniList, gameTracker->frameCount, gameTracker->primPool, (unsigned int**)drawot);
	PIPE3D_AnimateTerrainTextures(level->bgAniList, gameTracker->frameCount, gameTracker->primPool, (unsigned int**)drawot);

	PIPE3D_InstanceListTransformAndDraw(currentUnit, gameTracker, (unsigned int**)drawot, &theCamera.core);

	cam_pos_save.x = theCamera.core.position.x;
	cam_pos_save.y = theCamera.core.position.y;
	cam_pos_save.z = theCamera.core.position.z;

	cam_mat_save.m[0][0] = theCamera.core.wcTransform->m[0][0];
	cam_mat_save.m[0][1] = theCamera.core.wcTransform->m[0][1];
	cam_mat_save.m[0][2] = theCamera.core.wcTransform->m[0][2];
	cam_mat_save.m[1][0] = theCamera.core.wcTransform->m[1][0];
	cam_mat_save.m[1][1] = theCamera.core.wcTransform->m[1][1];
	cam_mat_save.m[1][2] = theCamera.core.wcTransform->m[1][2];
	cam_mat_save.m[2][0] = theCamera.core.wcTransform->m[2][0];
	cam_mat_save.m[2][1] = theCamera.core.wcTransform->m[2][1];
	cam_mat_save.m[2][2] = theCamera.core.wcTransform->m[2][2];

	cam_mat_save.t[0] = theCamera.core.wcTransform->t[0];
	cam_mat_save.t[1] = theCamera.core.wcTransform->t[1];
	cam_mat_save.t[2] = theCamera.core.wcTransform->t[2];

	if (terrain->numBSPTrees > 0)
	{
		for (curTree = 0; curTree < terrain->numBSPTrees; curTree++)
		{
			bsp = &terrain->BSPTreeArray[curTree];

			if (bsp->ID >= 0 && !(bsp->flags & 0x1))
			{
				saveLightInstance = NULL;

				if ((bsp->flags & 0x40))
				{
					saveLightInstance = gameTrackerX.gameData.asmData.lightInstances[0].lightInstance;
					gameTrackerX.gameData.asmData.lightInstances[0].lightInstance = NULL;
				}

				theCamera.core.position.x = cam_pos_save.x - bsp->globalOffset.x;
				theCamera.core.position.y = cam_pos_save.y - bsp->globalOffset.y;
				theCamera.core.position.z = cam_pos_save.z - bsp->globalOffset.z;

				tmp.x = -(cam_pos_save.x - bsp->globalOffset.x);
				tmp.y = -(cam_pos_save.y - bsp->globalOffset.y);
				tmp.z = -(cam_pos_save.z - bsp->globalOffset.z);

				ApplyMatrix(&cam_mat_save, (SVECTOR*)&tmp, (VECTOR*)&theCamera.core.wcTransform->t[0]);

				BSP_MarkVisibleLeaves_S(bsp, &theCamera, gPolytopeList, (unsigned int**)drawot, curTree, saveLightInstance, terrain, gameTracker, currentUnit);

				gameTracker->primPool->nextPrim = gameTracker->drawDisplayPolytopeListFunc(gPolytopeList, terrain, &theCamera, gameTracker->primPool, (unsigned int**)drawot, &bsp->globalOffset);

				if ((bsp->flags & 0x40))
				{
					gameTracker->gameData.asmData.lightInstances[0].lightInstance = saveLightInstance;
				}
			}
		}
	}

	theCamera.core.position.x = cam_pos_save.x;
	theCamera.core.position.y = cam_pos_save.y;
	theCamera.core.position.z = cam_pos_save.z;

	theCamera.core.wcTransform->m[0][0] = cam_mat_save.m[0][0];
	theCamera.core.wcTransform->m[0][1] = cam_mat_save.m[0][1];
	theCamera.core.wcTransform->m[0][2] = cam_mat_save.m[0][2];
	theCamera.core.wcTransform->m[1][0] = cam_mat_save.m[1][0];
	theCamera.core.wcTransform->m[1][1] = cam_mat_save.m[1][1];
	theCamera.core.wcTransform->m[1][2] = cam_mat_save.m[1][2];
	theCamera.core.wcTransform->m[2][0] = cam_mat_save.m[2][0];
	theCamera.core.wcTransform->m[2][1] = cam_mat_save.m[2][1];
	theCamera.core.wcTransform->m[2][2] = cam_mat_save.m[2][2];

	theCamera.core.wcTransform->t[0] = cam_mat_save.t[0];
	theCamera.core.wcTransform->t[1] = cam_mat_save.t[1];
	theCamera.core.wcTransform->t[2] = cam_mat_save.t[2];

	SBSP_IntroduceInstancesAndLights(terrain, &theCamera.core, gLightInfo, RENDER_currentStreamUnitID);

	///@TODO PSX_VERSION sp speedup.
	//t2 = &StackSave
	//StackSave = $sp
	//$sp = 0x1F8003F0

	///@TODO no FX tracker in place yet!
	///FX_DrawList(fxTracker, &gameTrackerX, gameTrackerX.drawOT, theCamera.core.wcTransform);

	if (gameTrackerX.playerInstance->currentStreamUnitID == currentUnit->StreamUnitID)
	{
		FX_DrawReaver(gameTrackerX.primPool, (unsigned int**)gameTrackerX.drawOT, theCamera.core.wcTransform);
	}

	//$sp = StackSave
}

void StreamIntroInstancesForUnit(struct _StreamUnit* currentUnit)
{
	if (currentUnit->used == 2)
	{
		SBSP_IntroduceInstances(currentUnit->level->terrain, currentUnit->StreamUnitID);
	}
}

long StreamRenderLevel(struct _StreamUnit* currentUnit, struct Level* mainLevel, unsigned int** drawot, long portalFogColor)
{
	struct GameTracker* gameTracker;
	struct Level* level;
	struct _Terrain* terrain;
	int curTree;
	int farplanesave;
	struct _Position cam_pos_save;
	MATRIX cam_mat_save;
	struct _SVector tmp;
	struct BSPTree* bsp;

	level = currentUnit->level;

	farplanesave = theCamera.core.farPlane;

	terrain = level->terrain;

	SetFarColor(0, 0, 0);

	UpdateFogSettings(currentUnit, level);

	gameTracker = &gameTrackerX;

	depthQBackColor = portalFogColor;

	currentUnit->FogColor = portalFogColor;

	depthQFogFar = level->fogFar;

	depthQFogStart = level->fogNear;

	theCamera.core.farPlane = level->fogFar;

	if (CheckForNoBlend((struct _ColorType*)&depthQBackColor) == 0)
	{
		depthQBlendStart = depthQFogStart;
	}
	else
	{
		depthQBlendStart = 65535;
	}

	SetFogNearFar(depthQFogStart, depthQFogFar, 320);

	PIPE3D_AnimateTerrainTextures(terrain->aniList, gameTracker->frameCount, gameTracker->primPool, drawot);

	PIPE3D_AnimateTerrainTextures(level->bgAniList, gameTracker->frameCount, gameTracker->primPool, drawot);

	PIPE3D_InstanceListTransformAndDraw(currentUnit, gameTracker, drawot, &theCamera.core);

	cam_pos_save.x = theCamera.core.position.x;
	cam_pos_save.y = theCamera.core.position.y;
	cam_pos_save.z = theCamera.core.position.z;

	cam_mat_save.m[0][0] = theCamera.core.wcTransform->m[0][0];
	cam_mat_save.m[0][1] = theCamera.core.wcTransform->m[0][1];
	cam_mat_save.m[0][2] = theCamera.core.wcTransform->m[0][2];
	cam_mat_save.m[1][0] = theCamera.core.wcTransform->m[1][0];
	cam_mat_save.m[1][1] = theCamera.core.wcTransform->m[1][1];
	cam_mat_save.m[1][2] = theCamera.core.wcTransform->m[1][2];
	cam_mat_save.m[2][0] = theCamera.core.wcTransform->m[2][0];
	cam_mat_save.m[2][1] = theCamera.core.wcTransform->m[2][1];
	cam_mat_save.m[2][2] = theCamera.core.wcTransform->m[2][2];

	cam_mat_save.t[0] = theCamera.core.wcTransform->t[0];
	cam_mat_save.t[1] = theCamera.core.wcTransform->t[1];
	cam_mat_save.t[2] = theCamera.core.wcTransform->t[2];

	for (curTree = 0; curTree < terrain->numBSPTrees; curTree++)
	{
		bsp = &terrain->BSPTreeArray[curTree];

		if (bsp->ID >= 0 && !(bsp->flags & 0x1))
		{
			theCamera.core.position.x = cam_pos_save.x - bsp->globalOffset.x;
			theCamera.core.position.y = cam_pos_save.y - bsp->globalOffset.y;
			theCamera.core.position.z = cam_pos_save.z - bsp->globalOffset.z;
		
			tmp.x = -(cam_pos_save.x - bsp->globalOffset.x);
			tmp.y = -(cam_pos_save.y - bsp->globalOffset.y);
			tmp.z = -(cam_pos_save.z - bsp->globalOffset.z);

			ApplyMatrix(&cam_mat_save, (SVECTOR*)&tmp, (VECTOR*)&theCamera.core.wcTransform->t[0]);
		
			BSP_MarkVisibleLeaves_S(bsp, &theCamera, gPolytopeList, drawot, curTree, NULL, terrain, gameTracker, currentUnit);

			gameTracker->primPool->nextPrim = gameTracker->drawDisplayPolytopeListFunc(gPolytopeList, terrain, &theCamera, gameTracker->primPool, drawot, &bsp->globalOffset);
		}
	}
	
	theCamera.core.position.x = cam_pos_save.x;
	theCamera.core.position.y = cam_pos_save.y;
	theCamera.core.position.z = cam_pos_save.z;

	InStreamUnit = 1;

	theCamera.core.wcTransform->m[0][0] = cam_mat_save.m[0][0];
	theCamera.core.wcTransform->m[0][1] = cam_mat_save.m[0][1];
	theCamera.core.wcTransform->m[0][2] = cam_mat_save.m[0][2];
	theCamera.core.wcTransform->m[1][0] = cam_mat_save.m[1][0];
	theCamera.core.wcTransform->m[1][1] = cam_mat_save.m[1][1];
	theCamera.core.wcTransform->m[1][2] = cam_mat_save.m[1][2];
	theCamera.core.wcTransform->m[2][0] = cam_mat_save.m[2][0];
	theCamera.core.wcTransform->m[2][1] = cam_mat_save.m[2][1];
	theCamera.core.wcTransform->m[2][2] = cam_mat_save.m[2][2];

	theCamera.core.wcTransform->t[0] = cam_mat_save.t[0];
	theCamera.core.wcTransform->t[1] = cam_mat_save.t[1];
	theCamera.core.wcTransform->t[2] = cam_mat_save.t[2];

	SBSP_IntroduceInstancesAndLights(terrain, &theCamera.core, gLightInfo, RENDER_currentStreamUnitID);

	theCamera.core.farPlane = farplanesave;
	
	InStreamUnit = 0;

	if (gameTrackerX.playerInstance->currentStreamUnitID == currentUnit->StreamUnitID)
	{
		hackOT = drawot;

		//StackSave[0] = sp
		//sp = 0x1F8003F0

		FX_DrawReaver(gameTrackerX.primPool, hackOT, theCamera.core.wcTransform);

		//sp = StackSave[0];
	}

	return 0;
}

void GAMELOOP_FlipScreenAndDraw(struct GameTracker* gameTracker, unsigned long** drawot)
{
#if defined(USE_32_BIT_ADDR)
	DrawOTag((unsigned int*)drawot + 3071 * 2);
#else
	DrawOTag((unsigned int*)drawot + 3071);
#endif

#if !defined(PSXPC_VERSION)
	while (CheckVolatile(gameTracker->drawTimerReturn) != 0)
	{
	}
#endif

	ResetPrimPool();
	PutDrawEnv(&draw[gameTracker->drawPage]);

#if !defined(PSXPC_VERSION)
	while (CheckVolatile(gameTracker->reqDisp) != 0)
	{

	}
#endif
	
#if defined(PSXPC_VERSION)
	gameTracker->drawTimerReturn = (unsigned long long*)&gameTracker->drawTime;
	gameTracker->usecsStartDraw = Emulator_GetPerformanceCounter() / (Emulator_GetPerformanceFrequency() / 1000000);
#else
	gameTracker->drawTimerReturn = (long*)&gameTracker->drawTime;
	gameTracker->usecsStartDraw = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);
#endif
	gameTracker->gameData.asmData.dispPage = 1 - gameTracker->gameData.asmData.dispPage;
}

void GAMELOOP_AddClearPrim(unsigned long** drawot, int override)
{
	struct BLK_FILL* blkfill;

	if (!(gameTrackerX.gameFlags & 0x8000000) || override)
	{
		blkfill = (struct BLK_FILL*)gameTrackerX.primPool->nextPrim;

#if defined(USE_32_BIT_ADDR)
		blkfill->len = clearRect[gameTrackerX.drawPage].len;
#else
		blkfill->tag = clearRect[gameTrackerX.drawPage].tag;
#endif
		blkfill->r0 = clearRect[gameTrackerX.drawPage].r0;
		blkfill->g0 = clearRect[gameTrackerX.drawPage].g0;
		blkfill->b0 = clearRect[gameTrackerX.drawPage].b0;
		blkfill->code = clearRect[gameTrackerX.drawPage].code;
		blkfill->x0 = clearRect[gameTrackerX.drawPage].x0;
		blkfill->y0 = clearRect[gameTrackerX.drawPage].y0;
		blkfill->w = clearRect[gameTrackerX.drawPage].w;
		blkfill->h = clearRect[gameTrackerX.drawPage].h;

		gameTrackerX.primPool->nextPrim = (unsigned int*)(blkfill + 1);
		
#if defined(USE_32_BIT_ADDR)
		setlen(blkfill, 3);
	
		addPrim(drawot[3071 * 2], blkfill);
#else
		setlen(blkfill, 3);
		addPrim(drawot[3071], blkfill);
#endif
	}
	else
	{
		blkfill = (struct BLK_FILL*)gameTrackerX.savedOTStart;
		blkfill->y0 = clearRect[gameTrackerX.drawPage].y0;
	}
}

void GAMELOOP_SwitchTheDrawBuffer(unsigned long **drawot)
{
	GAMELOOP_AddClearPrim(drawot, 0);

	DrawSync(0);

	if (gameTrackerX.drawTimerReturn != NULL)
	{
		gameTrackerX.drawTimerReturn = NULL;

		gameTrackerX.reqDisp = (void*)(((DISPENV*)gameTrackerX.disp) + gameTrackerX.gameData.asmData.dispPage);
	}

	PutDrawEnv(&draw[gameTrackerX.drawPage]);
}

void GAMELOOP_SetupRenderFunction(struct GameTracker *gameTracker)
{
	gameTracker->drawAnimatedModelFunc = &DRAW_AnimatedModel_S;
	gameTracker->drawDisplayPolytopeListFunc = &DRAW_DisplayPolytopeList_S;
}

struct _StreamUnit * GAMELOOP_GetMainRenderUnit() // Matching - 100%
{
	struct _StreamUnit* streamUnit;
	struct _Instance* focusInstance;
	struct _StreamUnit* cameraUnit;

	if (theCamera.mode == 5)
	{
		streamUnit = STREAM_WhichUnitPointerIsIn(theCamera.data.Cinematic.posSpline);
	}
	else
	{
		focusInstance = theCamera.focusInstance;

		if (focusInstance == gameTrackerX.playerInstance && gameTrackerX.SwitchToNewStreamUnit != 0)
		{
			streamUnit = STREAM_GetStreamUnitWithID(gameTrackerX.moveRazielToStreamID);

			if (streamUnit == NULL)
			{
				return STREAM_GetStreamUnitWithID(focusInstance->currentStreamUnitID);
			}
		}
		else
		{
			streamUnit = STREAM_GetStreamUnitWithID(focusInstance->currentStreamUnitID);
		}

		cameraUnit = COLLIDE_CameraWithStreamSignals(&theCamera);

		if (cameraUnit != NULL)
		{
			streamUnit = cameraUnit;
		}
	}

	return streamUnit;
}

void GAMELOOP_DisplayFrame(struct GameTracker* gameTracker)
{
	unsigned long** drawot;
	struct Level* mainLevel;
	struct StreamUnitPortal* streamPortal;
	int numportals;
	int d;
	struct _StreamUnit* mainStreamUnit;
#if defined(UWP)
	void* savedNextPrim = NULL;
#else
	void* savedNextPrim;
#endif
	struct _StreamUnit* toStreamUnit;
	long toStreamUnitID;
	struct StreamUnitPortal* streamPortal2;
	int i;
	int draw;
	PSX_RECT cliprect;
	int streamID;
	struct _Instance* instance;
	int v0;///@fixme macro
	int v1;///@fixme macro
	drawot = gameTracker->drawOT;

	if (!(gameTrackerX.gameFlags & 0x8000000) || pause_redraw_flag != 0)
	{
		if (pause_redraw_flag != 0)
		{
			savedNextPrim = gameTrackerX.primPool->nextPrim;

			DrawSync(0);
			Switch_For_Redraw();

			drawot = gameTracker->drawOT;

			ClearOTagR((unsigned int*)gameTrackerX.drawOT, 3072);

			if (pause_redraw_prim != NULL)
			{
				gameTrackerX.primPool->nextPrim = (unsigned int*)pause_redraw_prim;
			}
			else
			{
				gameTrackerX.primPool->nextPrim = &gameTrackerX.primPool->prim[0];
			}
		}
		else
		{
			pause_redraw_prim = gameTrackerX.primPool->nextPrim;
		}

		gameTrackerX.displayFrameCount++;
		gameTrackerX.visibleInstances = 0;

		GAMELOOP_SetupRenderFunction(&gameTrackerX);

		if (!(GlobalSave->flags & 0x1) && (gameTracker->wipeType != 11 || gameTracker->wipeTime == 0) && (gameTracker->debugFlags2 & 0x800))
		{
			FX_Spiral(gameTrackerX.primPool, drawot);
		}

		if (pause_redraw_flag == 0)
		{
			HUD_Draw();
		}

		mainStreamUnit = GAMELOOP_GetMainRenderUnit();
		mainLevel = mainStreamUnit->level;

		if ((gameTracker->debugFlags & 0x4))
		{
			FONT_Print("Cameraunit: %s\n", mainLevel->worldName);
		}

		theCamera.core.rightX = 320;
		theCamera.core.leftX = 0;
		theCamera.core.topY = 0;
		theCamera.core.bottomY = 240;

		RENDER_currentStreamUnitID = (short)mainStreamUnit->StreamUnitID;

		CAMERA_SetViewVolume(&theCamera);

		if (MEMPACK_MemoryValidFunc((char*)mainLevel) != 0)
		{
			if (mainLevel->fogFar != theCamera.core.farPlane)
			{
				theCamera.core.farPlane = mainLevel->fogFar;
			}

			if (!(gameTrackerX.debugFlags & 0x8000))
			{
				MainRenderLevel(mainStreamUnit, drawot);
#if defined(EDITOR)
				extern void Editor_DoDebug();
				Editor_DoDebug();
#endif
			}
		}

		numportals = ((int*)mainLevel->terrain->StreamUnits)[0];
		streamPortal = (struct StreamUnitPortal*)((long*)(mainLevel->terrain->StreamUnits) + 1);

		for (d = 0; d < numportals; d++, streamPortal++)
		{
			toStreamUnit = streamPortal->toStreamUnit;
			toStreamUnitID = streamPortal->streamID;

			if (toStreamUnit == NULL || toStreamUnit->FrameCount != gameTrackerX.displayFrameCount)
			{
				cliprect.x = SCREEN_WIDTH;
				cliprect.y = SCREEN_HEIGHT;
				cliprect.w = -SCREEN_WIDTH;
				cliprect.h = -SCREEN_HEIGHT;

				theCamera.core.rightX = 320;
				theCamera.core.leftX = 0;
				theCamera.core.topY = 0;
				theCamera.core.bottomY = 240;

				CAMERA_SetViewVolume(&theCamera);

				draw = 0;

				for (i = 0; i < numportals; i++)
				{
					streamPortal2 = (struct StreamUnitPortal*)(&((int*)mainLevel->terrain->StreamUnits)[1]) + i;

					if (streamPortal2->streamID == toStreamUnitID)
					{
						if (STREAM_GetClipRect(streamPortal2, &cliprect) != 0)
						{
							draw = 1;
						}
						else if ((theCamera.instance_mode & 0x2000000))
						{
							streamID = streamPortal2->toStreamUnit->StreamUnitID;
							INSTANCE_Query(gameTrackerX.playerInstance, 0x22);

							if (streamID == gameTrackerX.playerInstance->currentStreamUnitID || streamPortal2->toStreamUnit != NULL && *(int*)&streamPortal2->toStreamUnit->TargetFogFar == streamID)
							{
								draw = 1;

								cliprect.w = SCREEN_WIDTH;
								cliprect.x = 0;
								cliprect.y = 0;
								cliprect.h = SCREEN_HEIGHT;
							}
						}
					}
				}

				if (draw != 0)
				{
					theCamera.core.leftX = 320 * cliprect.x / 512;
					theCamera.core.topY = cliprect.y;
					theCamera.core.rightX = 320 * (cliprect.x + cliprect.w) / 512;
					theCamera.core.bottomY = cliprect.y + cliprect.h;

					CAMERA_SetViewVolume(&theCamera);

					SetRotMatrix(theCamera.core.wcTransform);

					SetTransMatrix(theCamera.core.wcTransform);

					if ((((short*)mainLevel->terrain->StreamUnits + 17)[46 * d] & 0x1))
					{
						if ((mainStreamUnit->flags & 0x8))
						{
							if (toStreamUnit != NULL)
							{
								if (toStreamUnit->FrameCount != gameTrackerX.displayFrameCount)
								{
									toStreamUnit->FrameCount = gameTrackerX.displayFrameCount;
								}
							}
							else
							{
								STREAM_RenderWarpGate(drawot, streamPortal, mainStreamUnit, &cliprect);
							}
						}
						else
						{
							WARPGATE_IsItActive(mainStreamUnit);
						}
					}
					else if (toStreamUnit != NULL)
					{
						if (toStreamUnit->FrameCount != gameTrackerX.displayFrameCount)
						{
							toStreamUnit->FrameCount = gameTrackerX.displayFrameCount;

							STREAM_RenderAdjacantUnit(drawot, streamPortal, toStreamUnit, mainStreamUnit, &cliprect);
						}
					}
				}
				else if (toStreamUnit != NULL)
				{
					if (toStreamUnit->FrameCount != gameTrackerX.displayFrameCount)
					{
						toStreamUnit->FrameCount = gameTrackerX.displayFrameCount;

					}

					StreamIntroInstancesForUnit(toStreamUnit);
				}
			}
		}

		for (d = 0; d < 16; d++)
		{
			if (StreamTracker.StreamList[d].used == 2)
			{
				if (StreamTracker.StreamList[d].FrameCount != gameTrackerX.displayFrameCount)
				{
					StreamTracker.StreamList[d].FrameCount = gameTrackerX.displayFrameCount;

					StreamIntroInstancesForUnit(&StreamTracker.StreamList[d]);
				}
			}
		}

		theCamera.core.rightX = 320;
		theCamera.core.leftX = 0;
		theCamera.core.topY = 0;
		theCamera.core.bottomY = 240;

		CAMERA_SetViewVolume(&theCamera);

		if (pause_redraw_flag != 0)
		{
			GAMELOOP_AddClearPrim(drawot, 1);

			SaveOT();

			ClearOTagR((unsigned int*)gameTrackerX.drawOT, 3072);

			Switch_For_Redraw();

			drawot = gameTracker->drawOT;

			pause_redraw_flag = 0;

			gameTrackerX.primPool->nextPrim = (unsigned int*)savedNextPrim;
		}
	}

	if ((gameTrackerX.gameFlags & 0x8000000))
	{
		HUD_Draw();
	}

	DEBUG_Draw(gameTracker, drawot);

	FONT_Flush();

	GAMELOOP_SwitchTheDrawBuffer(drawot);

#if defined(PSXPC_VERSION)
	gameTracker->idleTime = Emulator_GetPerformanceCounter() / (Emulator_GetPerformanceFrequency() / 1000000);
#else
	gameTracker->idleTime = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);
#endif

	if (gameTracker->frameRateLock >= (long)gameTracker->vblFrames)
	{
		if (CheckVolatile(gameTracker->reqDisp) != 0)
		{
			return;
		}
	}
	else
	{
		if (gameTracker->reqDisp != NULL)
		{
			PutDispEnv((DISPENV*)gameTracker->reqDisp);

			gameTracker->reqDisp = NULL;
			gameTracker->vblFrames = 0;
		}
	}

	gameTracker->idleTime = TIMER_TimeDiff(gameTracker->idleTime);

	gameTrackerX.gameData.asmData.dispPage = 1 - gameTrackerX.gameData.asmData.dispPage;

	DEBUG_DrawShrinkCels(drawot + 3071);

	GAMELOOP_HandleScreenWipes(drawot);

#if defined(PSXPC_VERSION)
	gameTracker->drawTimerReturn = (unsigned long long*) & gameTracker->drawTime;
#else
	gameTracker->drawTimerReturn = (long*)&gameTracker->drawTime;
#endif

#if defined(PSXPC_VERSION)
	gameTracker->usecsStartDraw = Emulator_GetPerformanceCounter() / (Emulator_GetPerformanceFrequency() / 1000000);
#else
	gameTracker->usecsStartDraw = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);
#endif

	if ((gameTrackerX.gameFlags & 0x8000000))
	{
		GAMELOOP_DrawSavedOT(drawot);
	}
	else
	{
#if defined(USE_32_BIT_ADDR)
		DrawOTag((unsigned int*)drawot + 3071 * 2);
#else
		DrawOTag((unsigned int*)drawot + 3071);
#endif
	}
}

void GAMELOOP_DrawSavedOT(unsigned long** newOT)
{
	P_TAG* tag;
	int y;
	int tpage;

	tag = gameTrackerX.savedOTStart;
	y = draw[gameTrackerX.drawPage].ofs[1];

	if (tag != (P_TAG*)gameTrackerX.savedOTEnd)
	{
		do
		{
			if (getcode(tag) == 0x34)
			{
				tpage = ((POLY_GT3*)tag)->tpage;

				if ((tpage & 0xF) < 8)
				{
					if (y != 0)
					{
						((POLY_GT3*)tag)->tpage |= 0x10;
					}
					else
					{
						((POLY_GT3*)tag)->tpage &= ~0x10;
					}
				}
			}
			else if (getcode(tag) == 0x24)
			{
				tpage = ((POLY_FT3*)tag)->tpage;

				if ((tpage & 0xF) < 8)
				{
					if (y != 0)
					{
						((POLY_FT3*)tag)->tpage |= 0x10;
					}
					else
					{
						((POLY_FT3*)tag)->tpage &= ~0x10;
					}
				}
			}
			else if (getcode(tag) == 0xE3)
			{
				if (y != 0)
				{
					((DR_TPAGE*)tag)->code[0] |= 0x40000;
					((DR_TPAGE*)tag)->code[1] |= 0x40000;
				}
				else
				{
					((DR_TPAGE*)tag)->code[0] &= 0xFFFBFFFF;
					((DR_TPAGE*)tag)->code[1] &= 0xFFFBFFFF;
				}
			}

			tag = (P_TAG*)nextPrim(tag);

		} while (tag != gameTrackerX.savedOTEnd);
	}

#if defined(PSXPC_VERSION)
	setaddr(gameTrackerX.savedOTEnd, newOT + 3071 * 2);
#else
	setaddr(gameTrackerX.savedOTEnd, newOT + 3071);
#endif

	DrawOTag((unsigned int*)gameTrackerX.savedOTStart);
}

void ResetPrimPool()
{
	ResetDrawPage();

	if (!(gameTrackerX.gameFlags & 0x8000000))
	{
		if (gameTrackerX.primPool == primPool[0])
		{
			gameTrackerX.primPool = primPool[1];
		}
		else
		{
			gameTrackerX.primPool = primPool[0];
		}

		gameTrackerX.primPool->nextPrim = &gameTrackerX.primPool->prim[0];
	}
	else
	{
		if (gameTrackerX.drawPage != 0)
		{
			gameTrackerX.primPool->nextPrim = &gameTrackerX.primPool->prim[16492];
		}
		else
		{
			gameTrackerX.primPool->nextPrim = &gameTrackerX.primPool->prim[9000];
		}
	}

	gameTrackerX.primPool->numPrims = 0;
}

void Switch_For_Redraw() // Matching - 100%
{
	unsigned long **temp;

	temp = gameTrackerX.drawOT;

	gameTrackerX.drawOT = gameTrackerX.dispOT;
	gameTrackerX.dispOT = temp;

	if (gameTrackerX.drawPage != 0)
	{
		gameTrackerX.drawPage = 0;
		gameTrackerX.gameData.asmData.dispPage = 1;
	}
	else
	{
		gameTrackerX.drawPage = 1;
		gameTrackerX.gameData.asmData.dispPage = 0;
	}

	if (gameTrackerX.primPool == primPool[0])
	{
		gameTrackerX.primPool = primPool[1];
	}
	else
	{
		gameTrackerX.primPool = primPool[0];
	}

	gameTrackerX.primPool->nextPrim = &gameTrackerX.primPool->prim[0];
	gameTrackerX.primPool->numPrims = 0;
}

void GAMELOOP_Set_Pause_Redraw() // Matching - 100%
{
	pause_redraw_flag = 1;
}

void SaveOT()
{
#if 1
	P_TAG* var_a0;
	P_TAG* var_s0;
	P_TAG* var_s1;
	long temp_a2;
	long temp_v0;
	long temp_v0_2;
	long temp_v0_3;

	DrawSync(0);

	var_s0 = NULL;
	var_a0 = (P_TAG*)(gameTrackerX.drawOT + 3071 * 2);
	var_s1 = NULL;

	if (var_a0->len != 0) {
		goto block_4;
	}
loop_2:
	temp_v0 = var_a0->addr;
	var_a0 = (P_TAG*)(temp_v0);
	if (temp_v0 == -1) {
		goto block_5;
	}
	if (var_a0->len == 0) {
		goto loop_2;
	}
block_4:
	if (var_a0->addr != -1) {
		goto block_6;
	}
block_5:
	gameTrackerX.savedOTStart = NULL;
	return;
block_6:
	gameTrackerX.savedOTStart = var_a0;
	if (var_a0->addr == -1) {
		goto block_17;
	}
	goto loop_10;
block_8:
	temp_v0_2 = var_a0->addr;
	if (temp_v0_2 == 0xFFFFFF) {
		goto block_17;
	}
	var_s1 = var_s0;
	var_s0 = var_a0;
	var_a0 = (P_TAG*)(temp_v0_2);
loop_10:
	if (var_a0->len != 0) {
		goto block_8;
	}
	if (var_a0->addr == -1) {
		goto block_17;
	}
	if (var_a0->len != 0) {
		goto block_15;
	}
loop_13:
	temp_v0_3 = var_a0->addr;
	if (temp_v0_3 == -1) {
		goto block_15;
	}
	var_a0 = (P_TAG*)(temp_v0_3);
	if (var_a0->len == 0) {
		goto loop_13;
	}
block_15:
	var_s0->addr = ((long)var_a0);

	temp_a2 = var_a0->addr;
	if (temp_a2 != -1) {
		goto loop_10;
	}
	if (temp_a2 != -1) {
		goto block_19;
	}
block_17:
	if (var_s1 == NULL) {
		goto block_19;
	}
	gameTrackerX.savedOTEnd = var_s1;
	var_s1->addr |= -1;
	return;
block_19:
	gameTrackerX.savedOTEnd = var_a0;
	return;
#else
	P_TAG* tag; // $a0
	P_TAG* last; // $s0
	P_TAG* lastlast; // $s1
	P_TAG* tag2; // $v0

	DrawSync(0);

	last = NULL;
	
#if defined(PSXPC_VERSION)
	tag = (P_TAG*)(gameTrackerX.drawOT + 3071 * 2);
#else
	tag = (P_TAG*)(gameTrackerX.drawOT + 3071);
#endif

	lastlast = NULL;

	if (tag->len != 0) 
	{
		goto block_4;
	}

loop_2:
	tag2 = tag;
	tag = (P_TAG*)nextPrim(tag);

	if (tag2->addr == -1) 
	{
		goto block_5;
	}

	if (tag->len == 0) 
	{
		goto loop_2;
	}
block_4:
	if (tag->addr != -1)
	{
		goto block_6;
	}
block_5:
	gameTrackerX.savedOTStart = NULL;
	return;
block_6:
	gameTrackerX.savedOTStart = tag;
	
	if (tag->addr == -1) 
	{
		goto block_17;
	}
	
	goto loop_10;

block_8:
	if (tag->addr == -1) 
	{
		goto block_17;
	}
	lastlast = last;
	last = tag;
	tag = (P_TAG*)nextPrim(tag);
loop_10:
	if (tag->len != 0) 
	{
		goto block_8;
	}
	if (tag->addr == -1) 
	{
		goto block_17;
	}
	if (tag->len != 0) 
	{
		goto block_15;
	}
loop_13:
	if (tag->addr == -1)
	{
		goto block_15;
	}
	tag = (P_TAG*)nextPrim(tag);
	if (tag->len == 0) 
	{
		goto loop_13;
	}
block_15:
	last->addr = (unsigned long)tag;
	if (tag->addr != -1) 
	{
		goto loop_10;
	}
	if (tag->addr != -1) 
	{
		goto block_19;
	}
block_17:
	if (lastlast == NULL) 
	{
		goto block_19;
	}
	gameTrackerX.savedOTEnd = lastlast;
	lastlast->addr = -1;
	return;
block_19:
	gameTrackerX.savedOTEnd = tag;
	return;
#endif
}


#if 0//Old

void SaveOT()
{
	P_TAG* tag; // $a0
	P_TAG* last; // $s0
	P_TAG* lastlast; // $s1

	DrawSync(0);
	
	last = NULL;
#if defined(PSXPC_VERSION)
	tag = (P_TAG*)(gameTrackerX.drawOT + 3071 * 2);
#elif defined(PSX_VERSION)
	tag = (P_TAG*)(gameTrackerX.drawOT + 3071);
#endif
	lastlast = NULL;

	if (tag->len == 0) {
	loop_2:
#if defined(PSXPC_VERSION)
		tag = (P_TAG*)tag->addr;
#elif defined(PSX_VERSION)
		tag = (P_TAG*)(unsigned long)tag->addr | 0x80000000;
#endif
		if (tag->addr != -1) {
			if (tag->len != 0) {
				goto block_4;
			}
			goto loop_2;
		}
		goto block_5;
	}
block_4:
	if (tag->addr == -1)
	{
	block_5:
		gameTrackerX.savedOTStart = NULL;
		return;
	}
	gameTrackerX.savedOTStart = tag;
	if (tag->addr != -1) {
	loop_10:
		if (tag->len == 0) {
			if (tag->addr != -1) {
				if (tag->len == 0) {
				loop_13:
					if (tag->addr != -1) {
#if defined(PSXPC_VERSION)
						tag = (P_TAG*)tag->addr;
#elif defined(PSX_VERSION)
						tag = (P_TAG*)(unsigned long)tag->addr | 0x80000000;
#endif
						if (tag->len == 0) {
							goto loop_13;
						}
					}
				}
#if defined(PSXPC_VERSION)
				last->addr = (unsigned long)tag;
#elif defined(PSX_VERSION)
				last->addr = (unsigned long)(tag) & 0xFFFFFF;
#endif
				if (tag->addr == -1) {
					if (tag->addr == -1) {
						goto block_17;
					}
					goto block_19;
				}
				goto loop_10;
			}
			goto block_17;
		}
		if (tag->addr != -1) {
			lastlast = last;
			last = tag;
#if defined(PSXPC_VERSION)
			tag = (P_TAG*)tag->addr;
#elif defined(PSX_VERSION)
			tag = (P_TAG*)(unsigned long)tag->addr | 0x80000000;
#endif
			goto loop_10;
		}
		goto block_17;
	}
block_17:
	if (lastlast != NULL) {
		gameTrackerX.savedOTEnd = lastlast;
		lastlast->addr = -1;
		return;
	}
block_19:
	gameTrackerX.savedOTEnd = tag;
}

#endif

#if 0//Older
void SaveOT()
{
	P_TAG* tag; // $a0
	P_TAG* last; // $s0
	P_TAG* lastlast; // $s1

	DrawSync(0);

#if defined(PSXPC_VERSION)
	tag = (P_TAG*)(gameTrackerX.drawOT + 3071 * 2);
#elif defined(PSX_VERSION)
	tag = (P_TAG*)(gameTrackerX.drawOT + 3071);
#endif

	last = NULL;
	lastlast = NULL;

	if (tag->len == 0)
	{

	loc_80030208:
		if (tag->addr != -1)
		{

#if defined(PSXPC_VERSION)
			tag = (P_TAG*)tag->addr;
#elif defined(PSX_VERSION)
			tag = (P_TAG*)(unsigned long)tag->addr | 0x80000000;
#endif
			if (tag->len == 0)
			{
				goto loc_80030208;
			}
		}
		else
		{
			gameTrackerX.savedOTStart = NULL;
			return;
		}
	}
	else
	{
		//loc_8003022C
		if (tag->addr == -1)
		{
			gameTrackerX.savedOTStart = NULL;
			return;
		}
	}

loc_80030250:

	gameTrackerX.savedOTStart = tag;

	if (tag->addr == -1)
	{
		if (lastlast != NULL)
		{
			gameTrackerX.savedOTEnd = lastlast;
			lastlast->addr = -1;
			return;
		}
		else
		{
			gameTrackerX.savedOTEnd = tag;
			return;
		}
	}
	else
	{
	loc_80030278:
		if (tag->addr == -1)
		{
			if (lastlast != NULL)
			{
				gameTrackerX.savedOTEnd = lastlast;
				lastlast->addr = -1;
				return;
			}
			else
			{
				gameTrackerX.savedOTEnd = tag;
				return;
			}
		}
		else
		{
			lastlast = last;
			last = tag;
#if defined(PSXPC_VERSION)
			tag = (P_TAG*)tag->addr;
#elif defined(PSX_VERSION)
			tag = (P_TAG*)(unsigned long)tag->addr | 0x80000000;
#endif
		loc_80030298:

			if (tag->len != 0)
			{
				goto loc_80030278;
			}

			if (tag->addr == -1)
			{
				if (lastlast != NULL)
				{
					gameTrackerX.savedOTEnd = lastlast;
					lastlast->addr = -1;
					return;
				}
				else
				{
					gameTrackerX.savedOTEnd = tag;
					return;
				}
			}
			else
			{
				if (tag->len == 0)
				{
				loc_800302CC:
					if (tag->addr != -1)
					{
#if defined(PSXPC_VERSION)
						tag = (P_TAG*)tag->addr;
#elif defined(PSX_VERSION)
						tag = (P_TAG*)(unsigned long)tag->addr | 0x80000000;
#endif
						if (tag->len == 0)
						{
							goto loc_800302CC;
						}
					}
				}
				//loc_800302F4
#if defined(PSXPC_VERSION)
				last->addr = (unsigned long)tag;
#elif defined(PSX_VERSION)
				last->addr = (unsigned long)(tag) & 0xFFFFFF;
#endif
				if (tag->addr != -1)
				{
					goto loc_80030298;
				}

				if (tag->addr == -1)
				{
					if (lastlast != NULL)
					{
						gameTrackerX.savedOTEnd = lastlast;
						lastlast->addr = -1;
						return;
					}
					else
					{
						gameTrackerX.savedOTEnd = tag;
						return;
					}
				}
				else
				{
					gameTrackerX.savedOTEnd = tag;
					return;
				}
			}
		}
	}
}
#endif

void ResetDrawPage() // Matching - 100%
{ 
	unsigned long **temp;

	temp = gameTrackerX.drawOT;
	gameTrackerX.drawOT = gameTrackerX.dispOT;
	gameTrackerX.dispOT = temp;
	gameTrackerX.drawPage = 1 - gameTrackerX.drawPage;

	ClearOTagR((unsigned int*)gameTrackerX.drawOT, 3072);
}

void GAMELOOP_Set24FPS() // Matching - 100%
{
	gameTrackerX.frameRate24fps = 1;
}

void GAMELOOP_Reset24FPS()
{
	gameTrackerX.frameRate24fps = 0;
}

void GAMELOOP_DoTimeProcess()
{
#if defined(PSXPC_VERSION)
	unsigned long long holdTime;
	int lockRate = 0;
	unsigned long last;

	holdTime = TIMER_GetTimeMS();

	if (!(gameTrackerX.gameFlags & 0x10000000))
	{
		gameTrackerX.totalTime = TIMER_TimeDiff(gameTrackerX.currentTicks);

		gameTrackerX.currentTicks = Emulator_GetPerformanceCounter() / (Emulator_GetPerformanceFrequency() / 1000000);

		if (gameTrackerX.frameRateLock <= 0)
		{
			gameTrackerX.frameRateLock = 1;
		}

		if (gameTrackerX.frameRateLock >= 3)
		{
			gameTrackerX.frameRateLock = 2;
		}

		if (gameTrackerX.decoupleGame == 0 && (gameTrackerX.gameFlags & 0x10000000))
		{
			if (gameTrackerX.frameRateLock == 1)
			{
				gameTrackerX.lastLoopTime = 33;
			}
			else if (gameTrackerX.frameRateLock == 2)
			{
				gameTrackerX.lastLoopTime = 50;
			}
		}
		else
		{
			if (gameTrackerX.frameRateLock == 1)
			{
				lockRate = 33;
			}
			else if (gameTrackerX.frameRateLock == 2)
			{
				lockRate = 50;
			}

			last = lockRate;

			if (gameTrackerX.lastLoopTime != -1)
			{
				last = (unsigned long)(holdTime - gameTrackerX.currentTime);
			}

			if (gameTrackerX.frameRateLock == 1 && gameTrackerX.frameRate24fps != 0)
			{
				last -= 9;
			}

			if (last >= holdTime && gameTrackerX.gameData.asmData.MorphTime == 1000)
			{
				if (last >= 67)
				{
					last = 66;
				}
			}
			else
			{
				last = lockRate;
			}

			gameTrackerX.lastLoopTime = last;
		}

		gameTrackerX.timeMult = ((last << 12) / 33);
		gameTrackerX.timeSinceLastGameFrame += gameTrackerX.timeMult;
		gameTrackerX.gameFramePassed = 0;
		gameTrackerX.globalTimeMult = gameTrackerX.timeMult;

		while (gameTrackerX.timeSinceLastGameFrame >= 4097)
		{
			gameTrackerX.gameFramePassed = 1;
			gameTrackerX.timeSinceLastGameFrame -= 4096;
			gameTrackerX.fps30Count++;
		}
	}
	else
	{
		gameTrackerX.lastLoopTime = -1;
	}

	gameTrackerX.currentTime = holdTime;

#elif defined(PSX_VERSION)

	int holdTime;
	int lockRate;
	unsigned long last;

	holdTime = TIMER_GetTimeMS();

	if (!(gameTrackerX.gameFlags & 0x10000000))
	{
		gameTrackerX.totalTime = TIMER_TimeDiff(gameTrackerX.currentTicks);

		gameTrackerX.currentTicks = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);

		if (gameTrackerX.frameRateLock <= 0)
		{
			gameTrackerX.frameRateLock = 1;
		}

		if (gameTrackerX.frameRateLock >= 3)
		{
			gameTrackerX.frameRateLock = 2;
		}

		if (gameTrackerX.decoupleGame == 0 && (gameTrackerX.gameFlags & 0x10000000))
		{
			if (gameTrackerX.frameRateLock == 1)
			{
				gameTrackerX.lastLoopTime = 33;
			}
			else if (gameTrackerX.frameRateLock == 2)
			{
				gameTrackerX.lastLoopTime = 50;
			}
		}
		else
		{
			if (gameTrackerX.frameRateLock == 1)
			{
				lockRate = 33;
			}
			else if (gameTrackerX.frameRateLock == 2)
			{
				lockRate = 50;
			}

			last = lockRate;

			if (gameTrackerX.lastLoopTime != -1)
			{
				last = holdTime - gameTrackerX.currentTime;
			}

			if (gameTrackerX.frameRateLock == 1 && gameTrackerX.frameRate24fps != 0)
			{
				last -= 9;
			}

			if (last >= holdTime && gameTrackerX.gameData.asmData.MorphTime == 1000)
			{
				if (last >= 67)
				{
					last = 66;
				}
			}
			else
			{
				last = lockRate;
			}

			gameTrackerX.lastLoopTime = last;
		}

		gameTrackerX.timeMult = ((last << 12) / 33);
		gameTrackerX.timeSinceLastGameFrame += gameTrackerX.timeMult;
		gameTrackerX.gameFramePassed = 0;
		gameTrackerX.globalTimeMult = gameTrackerX.timeMult;

		while (gameTrackerX.timeSinceLastGameFrame >= 4097)
		{
			gameTrackerX.gameFramePassed = 1;
			gameTrackerX.timeSinceLastGameFrame -= 4096;
			gameTrackerX.fps30Count++;
		}
	}
	else
	{
		gameTrackerX.lastLoopTime = -1;
	}

	gameTrackerX.currentTime = holdTime;
#endif
}

void GAMELOOP_Process(struct GameTracker* gameTracker)
{
	int d;
	int useTime;
	struct Level* level;
	int i;
	struct _SFXMkr* sfxMkr;

	if (gEndGameNow != 0)
	{
		DEBUG_ExitGame();
		gameTrackerX.levelDone = 3;
	}
	else
	{
		GAMELOOP_DoTimeProcess();

		if (gameTrackerX.gameMode != 6 && !(gameTrackerX.streamFlags & 0x100000))
		{
			MORPH_UpdateTimeMult();
			GAMELOOP_CalcGameTime();

			if (gameTracker->gameData.asmData.MorphType != 0)
			{
				gameTracker->currentSpectralTime += gameTracker->lastLoopTime;
			}
			else
			{
				useTime = 1;
				if (gameTrackerX.playerInstance != NULL)
				{
					level = STREAM_GetLevelWithID(gameTrackerX.playerInstance->currentStreamUnitID);
					if (level != NULL)
					{
						useTime = (level->unitFlags & 0x2000) < 1;
					}
				}
				if (useTime != 0)
				{
					gameTracker->currentTimeOfDayTime += gameTracker->lastLoopTime;

				}
				gameTracker->currentMaterialTime += gameTracker->lastLoopTime;
			}
		}
		gameTracker->numGSignals = 0;

		GAMELOOP_ChangeMode();

		ResetPrimPool();

		memset(gameTracker->overrideData, 0, sizeof(gameTracker->overrideData));

		if (gameTrackerX.gameMode != 6)
		{
			if (!(gameTrackerX.streamFlags & 0x100000))
			{
				if (gameTrackerX.SwitchToNewStreamUnit == 1)
				{
					INSTANCE_Post(gameTrackerX.playerInstance, 0x4000006, 0);
					STREAM_MoveIntoNewStreamUnit();
				}
			}
			if (VRAM_NeedToUpdateMorph != 0)
			{
				if (STREAM_IsCdBusy(NULL) == 0)
				{
					VRAM_UpdateMorphPalettes();
					VRAM_NeedToUpdateMorph = 0;
				}
			}

			if (gameTracker->gameData.asmData.MorphTime < 1000)
			{
				MORPH_Continue();
			}

			if ((gameTracker->streamFlags & 0x80000))
			{
				gameTracker->streamFlags &= 0xFFF7FFFF;
				UNDERWORLD_StartProcess();
			}

			EVENT_DoProcess();

			for (i = 0; i < 16; i++)
			{
				if (StreamTracker.StreamList[i].used == 2)
				{
					if (StreamTracker.StreamList[i].level->PuzzleInstances != NULL)
					{
						if ((gameTrackerX.debugFlags2 & 0x100))
						{
							FONT_Print("Processing unit %s\n", StreamTracker.StreamList[i].baseAreaName);
						}

						EVENT_ProcessEvents(StreamTracker.StreamList[i].level->PuzzleInstances, StreamTracker.StreamList[i].level);
						EVENT_BSPProcess(&StreamTracker.StreamList[i]);
					}
				}
			}
			EVENT_ResetAllOneTimeVariables();

			EVENT_ProcessHints();

			for (d = 0; d < 16; d++)
			{
				if (StreamTracker.StreamList[d].used == 2 && StreamTracker.StreamList[d].level->NumberOfSFXMarkers > 0)
				{
					for (i = 0; i < StreamTracker.StreamList[d].level->NumberOfSFXMarkers; i++)
					{
						sfxMkr = &StreamTracker.StreamList[d].level->SFXMarkerList[i];
						if (sfxMkr != NULL && sfxMkr->soundData != NULL)
						{
							SOUND_ProcessInstanceSounds(sfxMkr->soundData, sfxMkr->sfxTbl, &sfxMkr->pos, sfxMkr->livesInOnePlace, sfxMkr->inSpectral, 0, 0, NULL);
						}
					}
				}
			}

			if (!(gameTrackerX.streamFlags & 0x100000))
			{
				INSTANCE_SpatialRelationships(instanceList);
			}

			INSTANCE_ProcessFunctions(gameTracker->instanceList);
			INSTANCE_CleanUpInstanceList(gameTracker->instanceList, 0);
			INSTANCE_DeactivateFarInstances(gameTracker);
			MONAPI_ProcessGenerator();

			getScratchAddr(0)[0] = ((unsigned long*)&theCamera.core.position.x)[0];
			getScratchAddr(1)[0] = ((unsigned long*)&theCamera.core.position.z)[0];

#if 0//Right this is either done to do this fast on the scratch pad!
			//t0 = &StackSave
			sw      $sp, 0($t0)
				li      $t4, 0x1F8003F0
				move    $sp, $t4
#endif

				G2Instance_BuildTransformsForList(gameTracker->instanceList->first);


#if 0//Right this is either done to do this fast on the scratch pad!
			//t0 = &StackSave
			lw      $sp, 0($t0)
#endif
				if (!(gameTrackerX.streamFlags & 0x100000))
				{
#if 0///@TODO macro for PSX! "PUSH_STACK"
					sw      $sp, 0($t0)
						li      $t4, 0x1F8003F0
						move    $sp, $t4
#endif
						//FX_ProcessList(fxTracker);


#if 0///@TODO macro for PSX! "POP_STACK"
				//t0 = &StackSave
						lw      $sp, 0($t0)
#endif
						if (!(gameTrackerX.streamFlags & 0x100000))
						{
							VM_Tick(256);

							if ((gameTracker->debugFlags2 & 0x10000))
							{
								FONT_Print("Military Time %04d\n", gameTrackerX.timeOfDay);
							}

							for (d = 0; d < 16; d++)
							{
								if (StreamTracker.StreamList[d].used == 2)
								{
									VM_ProcessVMObjectList_S(StreamTracker.StreamList[d].level, &theCamera);
								}
							}

							if (!(gameTrackerX.streamFlags & 0x100000))
							{
								PLANAPI_UpdatePlanningDatabase(gameTracker, gameTrackerX.playerInstance);
							}
						}
				}

			DEBUG_Process(gameTracker);
			COLLIDE_InstanceList(gameTracker->instanceList);
			COLLIDE_InstanceListTerrain(gameTracker->instanceList);
			INSTANCE_AdditionalCollideFunctions(instanceList);
			COLLIDE_InstanceListWithSignals(instanceList);

			if (!(gameTrackerX.streamFlags & 0x100000))
			{
				LIGHT_CalcShadowPositions(gameTracker);
				INSTANCE_CleanUpInstanceList(gameTracker->instanceList, 16);
			}

			CAMERA_Process(&theCamera);
			PIPE3D_CalculateWCTransform(&theCamera.core);
			//theCamera.core.wcTransform2->t[0] = 0;//? padding?
			PIPE3D_InvertTransform(theCamera.core.cwTransform2, theCamera.core.wcTransform2);
			CAMERA_CalcVVClipInfo(&theCamera);

			if (gameTracker->levelChange != 0)
			{
				gameTracker->levelChange = 0;
			}

			SAVE_IntroduceBufferIntros();
		}
		else
		{
			getScratchAddr(0)[0] = ((unsigned long*)&theCamera.core.position.x)[0];
			getScratchAddr(1)[0] = ((unsigned long*)&theCamera.core.position.z)[0];
#if 0
			sw      $sp, 0($t0)
				li      $t4, 0x1F8003F0
				move    $sp, $t4
#endif

				G2Instance_BuildTransformsForList(gameTracker->instanceList->first);

#if 0
			//t0 = &StackSave
			addiu   $t0, $gp, -0x45AC
				lw      $sp, 0($t0)
#endif
				DEBUG_Process(gameTracker);
		}

		if (gameTracker->levelDone == 0)
		{
			GAMELOOP_DisplayFrame(gameTracker);
		}
		else
		{
			ResetDrawPage();
		}

		gameTracker->frameCount++;
		gameTracker->debugFlags &= 0xF7FFFFFF;
	}
}

void GAMELOOP_ModeStartRunning() // Matching - 98.15%
{
	if (gameTrackerX.gameMode == 4 || gameTrackerX.gameMode == 6)
	{
		DEBUG_ExitMenus();

		if (gameTrackerX.gameMode == 6)
		{
			currentMenu = standardMenu;

			SOUND_ResumeAllSound();

			VOICEXA_Resume();
		}
	}

	if ((gameTrackerX.gameFlags & 0x8000000))
	{
		gameTrackerX.gameFlags &= ~0x8000000;

		gameTrackerX.savedOTStart = NULL;

		DrawSync(0);
	}

	gameTrackerX.gameMode = 0;
	gameTrackerX.gameFlags &= ~0x10000000;
	gameTrackerX.playerInstance->flags &= ~0x100;
	gameTrackerX.gameMode = 0;

	GAMEPAD_RestoreControllers();

	INSTANCE_Post(gameTrackerX.playerInstance, 0x10000A, 0);
}

void GAMELOOP_ModeStartPause() // Matching - 98.89%
{
	gameTrackerX.gameMode = 6;

	INSTANCE_Post(gameTrackerX.playerInstance, 0x10000A, 1);

	currentMenu = pauseMenu;

	menu_set(gameTrackerX.menu, menudefs_pause_menu);

	SOUND_PauseAllSound();

	VOICEXA_Pause();

	SndPlay(5);

	gameTrackerX.gameFlags |= 0x10000000;

	GAMEPAD_SaveControllers();

	gameTrackerX.gameFlags |= 0x8000000;

	if (gameTrackerX.primPool == primPool[0])
	{
		gameTrackerX.primPool = primPool[1];
	}
	else
	{
		gameTrackerX.primPool = primPool[0];
	}

	gameTrackerX.primPool->nextPrim = &gameTrackerX.primPool->prim[0];
	gameTrackerX.primPool->numPrims = 0;

	SaveOT();

	pause_redraw_flag = 1;
}

void GAMELOOP_ChangeMode() // Matching - 99.74%
{
	long* controlCommand;
	int temp;  // not from SYMDUMP

	controlCommand = (long*)gameTrackerX.controlCommand;

	if (!(gameTrackerX.debugFlags & 0x40000))
	{
		if (!(gameTrackerX.debugFlags & 0x200000))
		{
			if ((gameTrackerX.controlCommand[0][0] & 0xA01) == 0xA01)
			{
				theCamera.forced_movement = 1;

				gameTrackerX.playerInstance->position.z += 100;
				gameTrackerX.playerInstance->zVel = 0;
				gameTrackerX.cheatMode = 1;

				INSTANCE_Post(gameTrackerX.playerInstance, 0x100010, 1);

				gameTrackerX.playerInstance->flags &= ~0x800;
			}
			else if ((gameTrackerX.controlCommand[0][0] & 0xA02) == 0xA02)
			{
				theCamera.forced_movement = 1;

				gameTrackerX.playerInstance->position.z -= 100;
				gameTrackerX.playerInstance->zVel = 0;
				gameTrackerX.cheatMode = 0;

				INSTANCE_Post(gameTrackerX.playerInstance, 0x100010, 0);

				gameTrackerX.gameMode = 0;
			}
		}
	}

	if ((!(gameTrackerX.debugFlags & 0x40000)) || ((gameTrackerX.playerCheatFlags & 0x2)))
	{
		if (((controlCommand[1] & 0x60) == 0x60) && (!(controlCommand[0] & 0xF)))
		{
			if (gameTrackerX.gameMode == 0)
			{
				gameTrackerX.gameMode = 4;

				currentMenu = (struct DebugMenuLine*)&standardMenu;

				if ((unsigned char)gameTrackerX.sound.gVoiceOn != 0)
				{
					gameTrackerX.debugFlags |= 0x80000;
				}
				else
				{
					gameTrackerX.debugFlags &= ~0x80000;
				}

				if ((unsigned char)gameTrackerX.sound.gMusicOn != 0)
				{
					gameTrackerX.debugFlags2 |= 0x1000;
				}
				else
				{
					gameTrackerX.debugFlags2 &= ~0x1000;
				}

				if ((unsigned char)gameTrackerX.sound.gSfxOn != 0)
				{
					gameTrackerX.debugFlags2 |= 0x2000;
				}
				else
				{
					gameTrackerX.debugFlags2 &= ~0x2000;
				}
			}
			else if (gameTrackerX.gameMode == 7)
			{
				DEBUG_EndViewVram(&gameTrackerX);

				gameTrackerX.gameMode = 0;
			}
			else
			{
				GAMELOOP_ModeStartRunning();
			}
		}
	}

	if ((((controlCommand[1] & 0x4000)) || (gamePadControllerOut >= 6)) && (gameTrackerX.gameMode == 0)
		&& (!(gameTrackerX.gameFlags & 0x80)) && ((gameTrackerX.wipeTime == 0) || ((gameTrackerX.wipeType != 11)
			&& (gameTrackerX.wipeTime == -1))))
	{
		GAMELOOP_ModeStartPause();
	}
	else
	{
		temp = controlCommand[1] & 0x4000;

		if (((temp != 0) || ((gameTrackerX.gameFlags & 0x40000000))) && (gameTrackerX.gameMode != 0)
			&& (!(gameTrackerX.gameFlags & 0x20000000)) && ((gameTrackerX.wipeTime == 0) || ((gameTrackerX.wipeType != 11)
				&& (gameTrackerX.wipeTime == -1))))
		{
			if (temp != 0)
			{
				if (!(gameTrackerX.gameFlags & 0x40000000))
				{
					SndPlay(5);
				}
			}

			gameTrackerX.gameFlags &= ~0x40000000;

			GAMELOOP_ModeStartRunning();
		}
	}

	if ((gameTrackerX.controlCommand[0][0] & 0x40000000))
	{
		gameTrackerX.playerInstance->flags |= 0x100;
		return;
	}

	if ((gameTrackerX.controlCommand[0][2] & 0x40000000))
	{
		gameTrackerX.playerInstance->flags &= ~0x100;
	}
}

#if defined(__EMSCRIPTEN__)
extern "C"
{
void EMSCRIPTEN_KEEPALIVE GAMELOOP_RequestLevelChangeHTML(char* name, short number)
{
	struct GameTracker* gameTracker;

	gameTracker = &gameTrackerX;

	if (gameTracker->levelChange == 0)
	{
		gameTracker->gameFlags |= 0x1;

		SOUND_ResetAllSound();

		sprintf(gameTracker->baseAreaName, "%s%d", name, number);

		gameTracker->levelChange = 1;

		gameTracker->levelDone = 1;
	}
}
}
#endif
void GAMELOOP_RequestLevelChange(char* name, short number, struct GameTracker* gameTracker) // Matching - 100%
{
	if (gameTracker->levelChange == 0)
	{
		gameTracker->gameFlags |= 0x1;

		SOUND_ResetAllSound();

		sprintf(gameTracker->baseAreaName, "%s%d", name, number);

		gameTracker->levelChange = 1;

		gameTracker->levelDone = 1;
	}
}

void PSX_GameLoop(struct GameTracker *gameTracker) // Matching - 100%
{
	GAMEPAD_Process(gameTracker);
	GAMELOOP_Process(gameTracker);
}

MATRIX* GAMELOOP_GetMatrices(int numMatrices) // Matching - 100%
{
	MATRIX* matrix;
	struct _PrimPool* pool;

	pool = gameTrackerX.primPool;

	matrix = (MATRIX*)pool->nextPrim;

	if (matrix + numMatrices < (MATRIX*)pool->lastPrim)
	{
		pool->nextPrim = (unsigned int*)(matrix + numMatrices);

		return matrix;
	}

	return NULL;
}

struct GameTracker* GAMELOOP_GetGT() // Matching - 100%
{
	return &gameTrackerX;
}

#if defined(EDITOR)
char* GAMELOOP_GetBaseAreaName()
{
	return GAMELOOP_GetGT()->baseAreaName;
}
#endif
