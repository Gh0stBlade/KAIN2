#ifndef BSP_H
#define BSP_H

#include "CORE.H"

struct _Sphere_noSq 
{
	struct _Position position; // size=6, offset=0
	unsigned short radius; // size=0, offset=6
};

struct _BoundingBox 
{
	short minX; // size=0, offset=0
	short minY; // size=0, offset=2
	short minZ; // size=0, offset=4
	short maxX; // size=0, offset=6
	short maxY; // size=0, offset=8
	short maxZ; // size=0, offset=10
};

struct _BSPNode 
{
	struct _Sphere_noSq sphere; // size=8, offset=0
	short a; // size=0, offset=8
	short b; // size=0, offset=10
	short c; // size=0, offset=12
	short flags; // size=0, offset=14
	long d; // size=0, offset=16
	void* front; // size=0, offset=20
	void* back; // size=0, offset=24
	struct _Sphere_noSq spectralSphere; // size=8, offset=28
	short front_spectral_error; // size=0, offset=36
	short back_spectral_error; // size=0, offset=38
	short front_material_error; // size=0, offset=40
	short back_material_error; // size=0, offset=42
};

struct _BSPLeaf 
{
	struct _Sphere_noSq sphere; // size=8, offset=0
	struct _TFace* faceList; // size=12, offset=8
	short numFaces; // size=0, offset=12
	short flags; // size=0, offset=14
	struct _BoundingBox box; // size=12, offset=16
	struct _BoundingBox spectralBox; // size=12, offset=28
	struct _Sphere_noSq spectralSphere; // size=8, offset=40
};

struct BSPTree 
{
	struct _BSPNode* bspRoot; // size=44, offset=0
	struct _BSPLeaf* startLeaves; // size=48, offset=4
	struct _BSPLeaf* endLeaves; // size=48, offset=8
	struct _Position globalOffset; // size=6, offset=12
	short flags; // size=0, offset=18
	struct _Position localOffset; // size=6, offset=20
	short ID; // size=0, offset=26
	long splineID; // size=0, offset=28
	struct _Instance* instanceSpline; // size=668, offset=32
};

struct _SBSPLeaf 
{
	struct _Sphere_noSq sphere; // size=8, offset=0
	struct Intro** introList; // size=76, offset=8
	short numIntros; // size=0, offset=12
	short flags; // size=0, offset=14
	struct CDLight** lightList; // size=24, offset=16
	short numLights; // size=0, offset=20
	short pad; // size=0, offset=22
};

struct BSPScratch 
{
	//struct MATRIX vv[2]; // size=64, offset=0
	MATRIX vv[2]; // size=64, offset=0
	//struct MATRIX camMatrix; // size=32, offset=64
	MATRIX camMatrix; // size=32, offset=64
	//struct VECTOR dpv[3]; // size=48, offset=96
	VECTOR dpv[3]; // size=48, offset=96
	long d[6]; // size=24, offset=144
	//struct MATRIX boundMatrix; // size=32, offset=168
	MATRIX boundMatrix; // size=32, offset=168
	long in_spectral; // size=0, offset=200
};

typedef struct _SBSPLeaf SBSPLeaf;

extern void SBSP_IntroduceInstances(struct _Terrain* terrain, int unitID);  // 0x8003DF34
extern void SBSP_IntroduceInstancesAndLights(struct _Terrain* terrain, struct _CameraCore_Type* cameraCore, struct LightInfo* lightInfo, int unitID);  // 0x8003DF98

#endif
