#ifndef __CORE_H
#define __CORE_H

#if defined(PC_VERSION)
#include <Windows.h>
#include "psyq.h"
#include "PC/LIBGTE.H"
#include "PC/LIBGPU.H"
#include "PC/libspu.h"
#else	// psx basically
#include <LIBAPI.H>
#include <LIBETC.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBSPU.H>
#include <LIBCD.H>
#include <LIBPAD.H>

#if defined(PSXPC_VERSION)//Temporary
#define LoadImage LoadImagePSX
#endif

#if !defined(PSXPC_VERSION)
// wrappers for winapi crap
#define OutputDebugStringA	printf
#define vprintf_s(a,b,c,d)	vprintf(a,c,d)
#endif
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH		512
#define SCREEN_HEIGHT		240

#define ONE_MB				1048576
#define TWO_MB				ONE_MB * 2
#define BASE_ADDRESS		0x80000000
#define PACK_MAGIC_USHORT(A, B, C, D)	(A << 12) | (B << 8) | (C << 4) | (D << 0)
#define DEFAULT_MEM_MAGIC				PACK_MAGIC_USHORT(0xB, 0xA, 0xD, 0xE)

#define null	0

#ifndef WINAPI
typedef unsigned char BYTE;
typedef unsigned long long QWORD;
typedef unsigned int DWORD;
typedef unsigned short WORD;
#ifndef PSX_VERSION//?
typedef unsigned int bool;
#endif
#endif

typedef struct _SVector // hashcode: 0x73B07C09 (dec: 1940945929)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
} _SVector;

typedef struct _Vector // hashcode: 0x5DEB6D24 (dec: 1575709988)
{
	long x; // size=0, offset=0
	long y; // size=0, offset=4
	long z; // size=0, offset=8
} _Vector;

typedef struct _SVectorNoPad // hashcode: 0xA6EDBBC6 (dec: -1494369338)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
} _SVectorNoPad;

typedef struct _Position // hashcode: 0x2523C22F (dec: 623100463)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
} _Position;

typedef struct _Rotation // hashcode: 0x5A40CBB0 (dec: 1514195888)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
} _Rotation;

typedef struct _Normal // hashcode: 0xC59D5A5B (dec: -979543461)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
} _Normal;

typedef struct _PlaneConstants // hashcode: 0xE1AAD777 (dec: -508897417)
{
	short a; // size=0, offset=0
	short b; // size=0, offset=2
	short c; // size=0, offset=4
	short flags; // size=0, offset=6
	long d; // size=0, offset=8
} _PlaneConstants;

typedef struct _Vertex // hashcode: 0x97101469 (dec: -1760553879)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
} _Vertex;

typedef struct _Face // hashcode: 0x8775891B (dec: -2022340325)
{
	unsigned short v0; // size=0, offset=0
	unsigned short v1; // size=0, offset=2
	unsigned short v2; // size=0, offset=4
} _Face;

typedef struct _MVertex // hashcode: 0x2B95C05B (dec: 731234395)
{
	struct _Vertex vertex; // size=6, offset=0
	unsigned short normal; // size=0, offset=6
} _MVertex;

typedef struct _MFace // hashcode: 0xB75C604D (dec: -1218682803)
{
	struct _Face face; // size=6, offset=0
	unsigned char normal; // size=0, offset=6
	unsigned char flags; // size=0, offset=7
	long color; // size=0, offset=8
} _MFace;

typedef struct _Sphere // hashcode: 0x361BDD31 (dec: 907795761)
{
	struct _Position position; // size=6, offset=0
	unsigned short radius; // size=0, offset=6
	unsigned long radiusSquared; // size=0, offset=8
} _Sphere;

typedef struct _Triangle2D // hashcode: 0xD422B451 (dec: -735923119)
{
	short x0; // size=0, offset=0
	short y0; // size=0, offset=2
	short x1; // size=0, offset=4
	short y1; // size=0, offset=6
	short x2; // size=0, offset=8
	short y2; // size=0, offset=10
} _Triangle2D;

typedef struct _HNormal // hashcode: 0xC53FB2BB (dec: -985681221)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
} _HNormal;

typedef struct _RGBAColor // hashcode: 0xF7174D90 (dec: -149467760)
{
	unsigned char r; // size=0, offset=0
	unsigned char g; // size=0, offset=1
	unsigned char b; // size=0, offset=2
	unsigned char a; // size=0, offset=3
} _RGBAColor;

typedef struct vecS // hashcode: 0xEADB8F3A (dec: -354709702)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
} vecS;

typedef struct vecL // hashcode: 0x7575CB14 (dec: 1970653972)
{
	long x; // size=0, offset=0
	long y; // size=0, offset=4
	long z; // size=0, offset=8
} vecL;

typedef struct _HVertex // hashcode: 0xC488A83D (dec: -997676995)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short normal; // size=0, offset=6
	long pad; // size=0, offset=8
} _HVertex;

typedef struct _HFaceInfo // hashcode: 0x0E79FF8C (dec: 242876300)
{
	long flags; // size=0, offset=0
	long attr; // size=0, offset=4
	struct _HFace* hface; // size=12, offset=8
	struct _SVector normal; // size=8, offset=12
	struct _HVertex* vertex0; // size=12, offset=20
	struct _HVertex* vertex1; // size=12, offset=24
	struct _HVertex* vertex2; // size=12, offset=28
} _HFaceInfo;

typedef struct NodeType // hashcode: 0x5BF613D7 (dec: 1542853591)
{
	struct NodeType* prev; // size=8, offset=0
	struct NodeType* next; // size=8, offset=4
} NodeType;

#if defined(PC_VERSION)
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
#include "MATH3D.H"
#include "PLAN/PLANAPI.H"

#elif defined(PSX_VERSION) || defined(PSXPC_VERSION)
#include "VRAM.H"
#include "INSTANCE.H"//For struct LightInstance
#endif

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

struct _ObjectTracker // hashcode: 0xFE4678BF (dec: -28936001)
{
	char name[16]; // size=16, offset=0
	struct Object* object; // size=76, offset=16
	short objectStatus; // size=0, offset=20
	short numInUse; // size=0, offset=22
	void* vramBlock; // size=0, offset=24
	char numObjectsUsing; // size=0, offset=28
	char objectsUsing[7]; // size=7, offset=29
};

struct Intro // hashcode: 0x796E766D (dec: 2037282413)
{
	char name[16]; // size=16, offset=0
	long intronum; // size=0, offset=16
	long UniqueID; // size=0, offset=20
	struct _Rotation rotation; // size=8, offset=24
	struct _Position position; // size=6, offset=32
	short maxRad; // size=0, offset=38
	long maxRadSq; // size=0, offset=40
	long flags; // size=0, offset=44
	void* data; // size=0, offset=48
	struct _Instance* instance; // size=616, offset=52
	struct MultiSpline* multiSpline; // size=0, offset=56
	void* dsignal; // size=0, offset=60
	short specturalLightGroup; // size=0, offset=64
	short meshColor; // size=0, offset=66
	struct _Position spectralPosition; // size=6, offset=68
	short spad; // size=0, offset=74
};

struct _Terrain // hashcode: 0x5D541B4E (dec: 1565793102)
{
	short UnitChangeFlags; // size=0, offset=0
	short spad; // size=0, offset=2
	long lpad2; // size=0, offset=4
	long numIntros; // size=0, offset=8
	struct Intro* introList; // size=76, offset=12
	long numVertices; // size=0, offset=16
	long numFaces; // size=0, offset=20
	long numNormals; // size=0, offset=24
	struct _TVertex* vertexList; // size=12, offset=28
	struct _TFace* faceList; // size=12, offset=32
	struct _Normal* normalList; // size=8, offset=36
	struct DrMoveAniTex* aniList; // size=8, offset=40
	long pad; // size=0, offset=44
	void* StreamUnits; // size=0, offset=48
	struct TextureFT3* StartTextureList; // size=12, offset=52
	struct TextureFT3* EndTextureList; // size=12, offset=56
	struct _MorphVertex* MorphDiffList; // size=14, offset=60
	struct _MorphColor* MorphColorList; // size=2, offset=64
	long numBSPTrees; // size=0, offset=68
	struct BSPTree* BSPTreeArray; // size=36, offset=72
	short* morphNormalIdx; // size=0, offset=76
	struct _MultiSignal* signals; // size=904, offset=80
};

#if defined(PC_VERSION)
#include "GAMELOOP.H"
#include "STREAM.H"
#include "BSP.H"
#include "MONSTER/MONSTER.H"
#endif

#endif
