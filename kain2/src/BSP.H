#ifndef BSP_H
#define BSP_H

struct _BSPNode // hashcode: 0x60C20F69 (dec: 1623330665)
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

struct _BSPLeaf // hashcode: 0xB2C1AC71 (dec: -1295930255)
{
	struct _Sphere_noSq sphere; // size=8, offset=0
	struct _TFace* faceList; // size=12, offset=8
	short numFaces; // size=0, offset=12
	short flags; // size=0, offset=14
	struct _BoundingBox box; // size=12, offset=16
	struct _BoundingBox spectralBox; // size=12, offset=28
	struct _Sphere_noSq spectralSphere; // size=8, offset=40
};

struct BSPTree // hashcode: 0x17800192 (dec: 394264978)
{
	struct _BSPNode* bspRoot; // size=44, offset=0
	struct _BSPLeaf* startLeaves; // size=48, offset=4
	struct _BSPLeaf* endLeaves; // size=48, offset=8
	struct _Position globalOffset; // size=6, offset=12
	short flags; // size=0, offset=18
	struct _Position localOffset; // size=6, offset=20
	short ID; // size=0, offset=26
	long splineID; // size=0, offset=28
	struct _Instance* instanceSpline; // size=616, offset=32
};


extern void SBSP_IntroduceInstances(struct _Terrain *terrain, int unitID); // 0x8003D928

extern void SBSP_IntroduceInstancesAndLights(struct _Terrain *terrain, struct _CameraCore_Type *cameraCore, struct LightInfo *lightInfo, int unitID); // 0x8003D98C


#endif
