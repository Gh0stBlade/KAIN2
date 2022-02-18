#ifndef __CORE_H
#define __CORE_H

#ifdef _WIN32
#include "psyq.h"
#include "PC/LIBGTE.H"
#include "PC/LIBGPU.H"
#include "PC/libspu.h"
#endif

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 240

#define ONE_MB 1048576
#define TWO_MB ONE_MB * 2
#define BASE_ADDRESS 0x80000000
#define PACK_MAGIC_USHORT(A, B, C, D) A << 12 | B << 8 | C << 4 | D << 0
#define DEFAULT_MEM_MAGIC PACK_MAGIC_USHORT(0xB, 0xA, 0xD, 0xE)

#define null	0

#ifndef WINAPI
typedef unsigned char BYTE;
typedef unsigned long long QWORD;
typedef unsigned int DWORD;
typedef unsigned short WORD;

typedef unsigned int bool;
#endif

struct _SVector // hashcode: 0x73B07C09 (dec: 1940945929)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct _Vector // hashcode: 0x5DEB6D24 (dec: 1575709988)
{
	long x; // size=0, offset=0
	long y; // size=0, offset=4
	long z; // size=0, offset=8
};

struct _SVectorNoPad // hashcode: 0xA6EDBBC6 (dec: -1494369338)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

struct _Position // hashcode: 0x2523C22F (dec: 623100463)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

struct _Rotation // hashcode: 0x5A40CBB0 (dec: 1514195888)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct _Normal // hashcode: 0xC59D5A5B (dec: -979543461)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct _PlaneConstants // hashcode: 0xE1AAD777 (dec: -508897417)
{
	short a; // size=0, offset=0
	short b; // size=0, offset=2
	short c; // size=0, offset=4
	short flags; // size=0, offset=6
	long d; // size=0, offset=8
};

struct _Vertex // hashcode: 0x97101469 (dec: -1760553879)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

struct _Face // hashcode: 0x8775891B (dec: -2022340325)
{
	unsigned short v0; // size=0, offset=0
	unsigned short v1; // size=0, offset=2
	unsigned short v2; // size=0, offset=4
};

struct _MVertex // hashcode: 0x2B95C05B (dec: 731234395)
{
	struct _Vertex vertex; // size=6, offset=0
	unsigned short normal; // size=0, offset=6
};

struct _MFace // hashcode: 0xB75C604D (dec: -1218682803)
{
	struct _Face face; // size=6, offset=0
	unsigned char normal; // size=0, offset=6
	unsigned char flags; // size=0, offset=7
	long color; // size=0, offset=8
};

struct _Sphere // hashcode: 0x361BDD31 (dec: 907795761)
{
	struct _Position position; // size=6, offset=0
	unsigned short radius; // size=0, offset=6
	unsigned long radiusSquared; // size=0, offset=8
};

struct _Triangle2D // hashcode: 0xD422B451 (dec: -735923119)
{
	short x0; // size=0, offset=0
	short y0; // size=0, offset=2
	short x1; // size=0, offset=4
	short y1; // size=0, offset=6
	short x2; // size=0, offset=8
	short y2; // size=0, offset=10
};

struct _HNormal // hashcode: 0xC53FB2BB (dec: -985681221)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct _RGBAColor // hashcode: 0xF7174D90 (dec: -149467760)
{
	unsigned char r; // size=0, offset=0
	unsigned char g; // size=0, offset=1
	unsigned char b; // size=0, offset=2
	unsigned char a; // size=0, offset=3
};

struct vecS // hashcode: 0xEADB8F3A (dec: -354709702)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

struct vecL // hashcode: 0x7575CB14 (dec: 1970653972)
{
	long x; // size=0, offset=0
	long y; // size=0, offset=4
	long z; // size=0, offset=8
};

struct _HVertex // hashcode: 0xC488A83D (dec: -997676995)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short normal; // size=0, offset=6
	long pad; // size=0, offset=8
};

struct _HFaceInfo // hashcode: 0x0E79FF8C (dec: 242876300)
{
	long flags; // size=0, offset=0
	long attr; // size=0, offset=4
	struct _HFace* hface; // size=12, offset=8
	struct _SVector normal; // size=8, offset=12
	struct _HVertex* vertex0; // size=12, offset=20
	struct _HVertex* vertex1; // size=12, offset=24
	struct _HVertex* vertex2; // size=12, offset=28
};

struct NodeType // hashcode: 0x5BF613D7 (dec: 1542853591)
{
	struct NodeType* prev; // size=8, offset=0
	struct NodeType* next; // size=8, offset=4
};

#include "gex2.h"
#include "FONT.H"
#include "DEBUG.H"
#include "MEMPACK.H"

#include "SPLINE.H"
#include "CAMERA.H"
#include "INSTANCE.H"
#include "COLLIDE.H"
#include "PHYSICS.H"

#include "RESOLVE.H"
#include "VRAM.H"

struct _ColorType // hashcode: 0x440E837C (dec: 1141801852)
{
	unsigned char r; // size=0, offset=0
	unsigned char g; // size=0, offset=1
	unsigned char b; // size=0, offset=2
	unsigned char code; // size=0, offset=3
};

struct _GameTrackerASMData_Type // hashcode: 0x28F0BFB9 (dec: 686866361)
{
	long drawBackFaces; // size=0, offset=0
	long dispPage; // size=0, offset=4
	short MorphTime; // size=0, offset=8
	short MorphType; // size=0, offset=10
	struct LightInstance lightInstances[1]; // size=20, offset=12
};

struct _GameData_Type // hashcode: 0xE05EBAD0 (dec: -530662704)
{
	struct _GameTrackerASMData_Type asmData; // size=32, offset=0
};

struct gSoundData // hashcode: 0x100090FC (dec: 268472572)
{
	unsigned long gMasterVol; // size=0, offset=0
	unsigned long gMusicVol; // size=0, offset=4
	unsigned long gSfxVol; // size=0, offset=8
	unsigned long gVoiceVol; // size=0, offset=12
	char gSfxOn; // size=0, offset=16
	char gMusicOn; // size=0, offset=17
	char gVoiceOn; // size=0, offset=18
	char soundsLoaded; // size=0, offset=19
};

struct Object // hashcode: 0xEC12E9AC (dec: -334304852)
{
	long oflags; // size=0, offset=0
	short id; // size=0, offset=4
	short sfxFileHandle; // size=0, offset=6
	short numModels; // size=0, offset=8
	short numAnims; // size=0, offset=10
	struct _Model** modelList; // size=56, offset=12
	struct _G2AnimKeylist_Type** animList; // size=20, offset=16
	short introDist; // size=0, offset=20
	short vvIntroDist; // size=0, offset=22
	short removeDist; // size=0, offset=24
	short vvRemoveDist; // size=0, offset=26
	void* data; // size=0, offset=28
	char* script; // size=0, offset=32
	char* name; // size=0, offset=36
	unsigned char* soundData; // size=0, offset=40
	long oflags2; // size=0, offset=44
	short sectionA; // size=0, offset=48
	short sectionB; // size=0, offset=50
	short sectionC; // size=0, offset=52
	short numberOfEffects; // size=0, offset=54
	struct ObjectEffect* effectList; // size=4, offset=56
	unsigned long* relocList; // size=0, offset=60
	void* relocModule; // size=0, offset=64
	struct VramSize vramSize; // size=8, offset=68
};

struct GameTracker // hashcode: 0x4EB3EFC5 (dec: 1320415173)
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
	struct P_TAG* savedOTStart; // size=8, offset=476
	struct P_TAG* savedOTEnd; // size=8, offset=480
	long introWaitTime; // size=0, offset=484
	long mirrorZPush; // size=0, offset=488
	long defVVRemoveDist; // size=0, offset=492
	long defRemoveDist; // size=0, offset=496
	struct _Position forcedStartPosition; // size=6, offset=500
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
};

extern struct GameTracker gameTrackerX; // offset 0x800D0FAC

#endif
