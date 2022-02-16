#ifndef MAING2_H
#define MAING2_H

struct _SVector // hashcode: 0x73B07C09 (dec: 1940945929)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
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

struct _RGBAColor // hashcode: 0xF7174D90 (dec: -149467760)
{
	unsigned char r; // size=0, offset=0
	unsigned char g; // size=0, offset=1
	unsigned char b; // size=0, offset=2
	unsigned char a; // size=0, offset=3
};

struct _PVertex // hashcode: 0x4AC0ECE8 (dec: 1254157544)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	long otz; // size=0, offset=4
};

struct _VertexPool // hashcode: 0x98F8C96E (dec: -1728525970)
{
	struct _PVertex vertex[528]; // size=4224, offset=0
	struct CVECTOR color[528]; // size=2112, offset=4224
};

struct _PrimPool // hashcode: 0x9529B283 (dec: -1792429437)
{
	long numPrims; // size=0, offset=0
	unsigned long* nextPrim; // size=0, offset=4
	unsigned long* lastPrim; // size=0, offset=8
	unsigned long prim[24000]; // size=96000, offset=12
};

struct _Sphere // hashcode: 0x361BDD31 (dec: 907795761)
{
	struct _Position position; // size=6, offset=0
	unsigned short radius; // size=0, offset=6
	unsigned long radiusSquared; // size=0, offset=8
};

struct _Sphere_noSq // hashcode: 0xA031058C (dec: -1607400052)
{
	struct _Position position; // size=6, offset=0
	unsigned short radius; // size=0, offset=6
};

extern enum _G2Bool_Enum MainG2_InitEngine(void *appData, unsigned int screenWidth, unsigned int screenHeight, char *filesystemName); // 0x80095D8C
extern void MainG2_ShutDownEngine(void *appData); // 0x80095D94

#endif
