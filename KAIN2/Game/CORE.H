#ifndef CORE_H
#define CORE_H

#if defined(PC_VERSION)
#include <Windows.h>
#include "psyq.h"
#include "PC/LIBGTE.H"
#include "PC/LIBGPU.H"
#include "PC/libspu.h"
#include "PC/LIBAPI.H"
#include "PC/LIBPAD.H"
#include "PC/LIBSN.H"

#elif defined(PSX_VERSION) && !defined(PSXPC_VERSION)

#include <SYS/TYPES.H>
#include <LIBAPI.H>
#include <LIBETC.H>
#include <LIBGTE.H>
#include <LIBSPU.H>
#include <LIBCD.H>
#include <LIBPAD.H>
#include <LIBGPU.H>
#include <LIBSN.H>
#include <LIBMCRD.H>
#include <LIBPRESS.H>
#include <INLINE_C.H>
#include "INLINE_C_EXT.H"
#include <STDARG.H>


#define UNIMPLEMENTED()

#undef LoadImage
#define LoadImagePSX LoadImage
#define PSX_EnterCriticalSection	EnterCriticalSection
#define PSX_ExitCriticalSection		ExitCriticalSection
#define setAbr(p, abr) ((p)->dr_tpage) |= ((short)abr << 5)
#define setPolyFT4_ST(p) setlen(p, 6),  setcode(p, 0x2A)

typedef RECT PSX_RECT;

#define PTR_32

#elif defined(PSXPC_VERSION)

#include <EMULATOR_PUBLIC.H>

#if defined(UWP)
#include <Windows.h>
#endif

#include <LIBAPI.H>
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGTE.H>
#include <LIBSPU.H>
#include <LIBCD.H>
#include <LIBPAD.H>
#include <LIBSN.H>
#include <LIBMCRD.H>
#include <LIBPRESS.H>
#include <INLINE_C.H>
#include "INLINE_C_EXT.H"

#define LoadImage LoadImagePSX
#define PSX_EnterCriticalSection	EnterCriticalSection
#define PSX_ExitCriticalSection		ExitCriticalSection
#define setAbr(p, abr) ((p)->dr_tpage) |= ((short)abr << 5)
#define setlen_ST( p, _len) 	(((POLY_F4_SEMITRANS *)(p))->len  = (u_char)(_len))
#define setcode_ST(p, _code)	(((POLY_F4_SEMITRANS *)(p))->code = (u_char)(_code))
#define setPolyFT4_ST(p) setlen_ST(p, 6),  setcode_ST(p, 0x2A)

#if defined(_WIN64)
#define PTR_32 __ptr32
#else
#define PTR_32
#endif

//Custom GTE macros
#define gte_st_mac0( r0 ) \
	((int*)&r0)[0] = MAC0;   \

#define gte_st_mac1( r0 ) \
	((int*)&r0)[1] = MAC1;   \

#define gte_ld_vxy0( r0 )    \
	((int*)&VX0)[0] = r0;     \

#pragma warning(disable: 4101)//Unreferenced local var.
#if defined(__ANDROID__) || defined(SN_TARGET_PSP2) || defined(PLATFORM_NX_ARM) || defined(__linux__)
#define strcmpi strcmp
#define _strcmpi strcmp
#endif

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#define strcmpi strcmp
#define _strcmpi strcmp
#endif
#endif

#if !defined(PSXPC_VERSION) && !defined(PSX_VERSION)
// wrappers for winapi crap
#define OutputDebugStringA	printf
#define vprintf_s(a,b,c,d)	vprintf(a,c,d)
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH		512
#define SCREEN_HEIGHT		240

#if defined(_DEBUG) && 0
#define FRAMERATE_MULT      2
#else
#define FRAMERATE_MULT      1
#endif

#define ONE_MB				1048576
#define TWO_MB				ONE_MB * 2
#if defined(PC_VERSION) || defined(PSXPC_VERSION)
#define BASE_ADDRESS		(unsigned long)&memBuffer
#elif defined(PSX_VERSION)
#define BASE_ADDRESS		0x80000000
#endif
#define PACK_MAGIC_USHORT(A, B, C, D)	(A << 12) | (B << 8) | (C << 4) | (D << 0)
#define DEFAULT_MEM_MAGIC				PACK_MAGIC_USHORT(0xB, 0xA, 0xD, 0xE)

#define null	0

#ifndef ABS//I know one of the psyq headers has this so maybe we should just use the one from psyq at some point.
#define ABS(x) ((x)<0 ? -(x) : (x))
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
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

struct _TFace // hashcode: 0x85FE430F (dec: -2046934257)
{
	_Face face; // size=6, offset=0
	unsigned char attr; // size=0, offset=6
	char sortPush; // size=0, offset=7
	unsigned short normal; // size=0, offset=8
	unsigned short textoff; // size=0, offset=10
};

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
	_Position position; // size=6, offset=0
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

struct _TVertex // hashcode: 0x0883ABD5 (dec: 142846933)
{
	struct _Vertex vertex; // size=6, offset=0
	unsigned short rgb15; // size=0, offset=6
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
};

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
	struct NodeType* PTR_32 prev; // size=8, offset=0
	struct NodeType* PTR_32 next; // size=8, offset=4
} NodeType;

#if defined(PC_VERSION)
#include "gex2.h"
#include "FONT.H"
#include "DEBUG.H"
#include "MEMPACK.H"

#include "SPLINE.H"
#include "CAMERA.H"
#include "Game/INSTANCE.H"
#include "COLLIDE.H"
#include "PHYSICS.H"

#include "RESOLVE.H"
#include "VRAM.H"
#include "MATH3D.H"
#include "PLAN/PLANAPI.H"

#elif defined(PSX_VERSION) || defined(PSXPC_VERSION)
#include "gex2.h"
#include "VRAM.H"
#include "Game/INSTANCE.H"//For struct LightInstance
#endif

#if defined(PC_VERSION)
#include "GAMELOOP.H"
#include "STREAM.H"
#include "BSP.H"
#include "MONSTER/MONSTER.H"
#endif

#endif
