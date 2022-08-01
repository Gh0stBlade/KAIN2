#ifndef G2TYPES_H
#define G2TYPES_H

#include "CORE.H"

struct _ColorType // hashcode: 0x440E837C (dec: 1141801852)
{
	unsigned char r; // size=0, offset=0
	unsigned char g; // size=0, offset=1
	unsigned char b; // size=0, offset=2
	unsigned char code; // size=0, offset=3
};

struct LightInstance // hashcode: 0xB8D5E61F (dec: -1193941473)
{
	struct _Instance* lightInstance; // size=0, offset=0
	long r; // size=0, offset=4
	long g; // size=0, offset=8
	long b; // size=0, offset=12
	short radius; // size=0, offset=16
	unsigned char segment; // size=0, offset=18
	unsigned char flags; // size=0, offset=19
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

struct ObjectEffect
{
	unsigned char effectNumber; // size=0, offset=0
	unsigned char modifierList[3]; // size=3, offset=1
};

struct Object // hashcode: 0xEC12E9AC (dec: -334304852)
{
	long oflags; // size=0, offset=0
	short id; // size=0, offset=4
	short sfxFileHandle; // size=0, offset=6
	short numModels; // size=0, offset=8
	short numAnims; // size=0, offset=10
	struct _Model** PTR_32 modelList; // size=56, offset=12
	struct _G2AnimKeylist_Type** PTR_32 animList; // size=20, offset=16
	short introDist; // size=0, offset=20
	short vvIntroDist; // size=0, offset=22
	short removeDist; // size=0, offset=24
	short vvRemoveDist; // size=0, offset=26
	void* PTR_32 data; // size=0, offset=28
	char* PTR_32 script; // size=0, offset=32
	char* PTR_32 name; // size=0, offset=36
	unsigned char* PTR_32 soundData; // size=0, offset=40
	long oflags2; // size=0, offset=44
	short sectionA; // size=0, offset=48
	short sectionB; // size=0, offset=50
	short sectionC; // size=0, offset=52
	short numberOfEffects; // size=0, offset=54
	struct ObjectEffect* PTR_32 effectList; // size=4, offset=56
	unsigned long* PTR_32 relocList; // size=0, offset=60
	void* PTR_32 relocModule; // size=0, offset=64
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
	_Position position; // size=6, offset=32
	short maxRad; // size=0, offset=38
	long maxRadSq; // size=0, offset=40
	long flags; // size=0, offset=44
	void* data; // size=0, offset=48
	struct _Instance* instance; // size=616, offset=52
	struct MultiSpline* multiSpline; // size=0, offset=56
	void* dsignal; // size=0, offset=60
	short specturalLightGroup; // size=0, offset=64
	short meshColor; // size=0, offset=66
	_Position spectralPosition; // size=6, offset=68
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

#endif