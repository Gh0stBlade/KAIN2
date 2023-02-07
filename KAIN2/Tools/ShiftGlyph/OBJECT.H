#pragma once

struct VramSize 
{
	short x;
	short y;
	short w;
	short h;
};

struct TextureMT3
{
	unsigned char u0;
	unsigned char v0;
	unsigned short clut;
	unsigned char u1;
	unsigned char v1;
	unsigned short tpage;
	unsigned char u2;
	unsigned char v2;
	char pad1;
	char sortPush;
	long color;
};

struct _Vertex
{
	short x;
	short y;
	short z;
};

struct _MVertex
{
	struct _Vertex vertex;
	unsigned short normal;
};

struct _SVectorNoPad 
{
	short x;
	short y;
	short z;
};

struct _Face 
{
	unsigned short v0;
	unsigned short v1;
	unsigned short v2;
};

struct _MFace 
{
	struct _Face face;
	unsigned char normal;
	unsigned char flags;
	long color;
};

struct _Position 
{
	short x;
	short y;
	short z;
};

struct _HFace 
{
	short v0;
	short v1;
	short v2;
	unsigned char attr;
	char pad;
	unsigned short normal;
	unsigned short n0;
};

struct _HSphere 
{
	long attr;
	unsigned char id;
	unsigned char rank;
	short pad;
	struct _Position position;
	unsigned short radius;
	unsigned long radiusSquared;
};

struct _HBox 
{
	short flags;
	unsigned char id;
	unsigned char rank;
	short minX;
	short minY;
	short minZ;
	short maxX;
	short maxY;
	short maxZ;
	short refMinX;
	short refMinY;
	short refMinZ;
	short refMaxX;
	short refMaxY;
	short refMaxZ;
};

struct _HInfo64
{
	long numHFaces;
	union
	{
		struct _HFace* hfaceList;
		unsigned long long hfaceList_;
	};
	long numHSpheres;
	union
	{
		struct _HSphere* hsphereList;
		unsigned long long hsphereList_;
	};
	long numHBoxes;
	union
	{
		struct _HBox* hboxList;
		unsigned long long hboxList_;
	};
};

struct _HInfo
{
	long numHFaces;
	struct _HFace* hfaceList;
	long numHSpheres;
	struct _HSphere* hsphereList;
	long numHBoxes;
	struct _HBox* hboxList;
};

struct _Segment64
{
	long flags;
	short firstTri;
	short lastTri;
	short firstVertex;
	short lastVertex;
	short px;
	short py;
	short pz;
	short parent;
	union
	{
		struct _HInfo64* hInfo;
		unsigned long long hInfo_;
	};
};

struct _Segment
{
	long flags;
	short firstTri;
	short lastTri;
	short firstVertex;
	short lastVertex;
	short px;
	short py;
	short pz;
	short parent;
	struct _HInfo* hInfo;
};

struct AniTexInfo64
{
	union
	{
		struct TextureMT3* texture;
		unsigned long long texture_;
	};
	long numFrames;
	long speed;
};

struct AniTexInfo
{
	struct TextureMT3* texture;
	long numFrames;
	long speed;
};

struct AniTex
{
	long numAniTextues;
	struct AniTexInfo aniTexInfo;
};

struct vecS
{
	short x;
	short y;
	short z;
};

struct vecL
{
	long x;
	long y;
	long z;
};

struct SplineKey
{
	short count;
	struct vecS point;
	struct vecL dd;
	struct vecL ds;
};

struct Spline64
{
	union
	{
		struct SplineKey* key;
		unsigned long long key_;
	};
	short numkeys;
	unsigned char type;
	unsigned char flags;
};

struct Spline
{
	struct SplineKey* key;
	short numkeys;
	unsigned char type;
	unsigned char flags;
};

struct _G2Quat_Type 
{
	short x;
	short y;
	short z;
	short w;
};

struct SplineRotKey
{
	short count;
	struct _G2Quat_Type q;
};

struct RSpline
{
	union
	{
		struct SplineRotKey* key;
		unsigned long long key_;
	};
	short numkeys;
	unsigned char type;
	unsigned char flags;
};

struct SplineDef 
{
	short currkey;
	unsigned short denomFlag;
	long fracCurr;
};

struct MATRIX
{
	short m[3][3];
	long t[3];
};

struct MultiSpline64
{
	union
	{
		struct Spline* positional;
		unsigned long long positional_;
	};
	union
	{
		struct RSpline* rotational;
		unsigned long long rotational_;
	};
	union
	{
		struct Spline* scaling;
		unsigned long long scaling_;
	};
	union
	{
		struct Spline* color;
		unsigned long long color_;
	};
	struct SplineDef curPositional;
	struct SplineDef curRotational;
	struct SplineDef curScaling;
	struct SplineDef curColor;
	struct MATRIX curRotMatrix;
};

struct MultiSpline
{
	struct Spline* positional;
	struct RSpline* rotational;
	struct Spline* scaling;
	struct Spline* color;
	struct SplineDef curPositional;
	struct SplineDef curRotational;
	struct SplineDef curScaling;
	struct SplineDef curColor;
	struct MATRIX curRotMatrix;
};


struct _Model64
{
	long numVertices;
	union
	{
		struct _MVertex* vertexList;
		unsigned long long vertexList_;
	};
	long numNormals;
	union
	{
		struct _SVectorNoPad* normalList;
		unsigned long long normalList_;
	};
	long numFaces;
	union
	{
		struct _MFace* faceList;
		unsigned long long faceList_;
	};
	long numSegments;
	union
	{
		struct _Segment* segmentList;
		unsigned long long segmentList_;
	};
	union
	{
		struct AniTex* aniTextures;
		unsigned long long aniTextures_;
	};
	short maxRad;
	short pad;
	long maxRadSq;
	union
	{
		struct MultiSpline* multiSpline;
		unsigned long long multiSpline_;
	};
	union
	{
		struct TextureMT3* startTextures;
		unsigned long long startTextures_;
	};
	union
	{
		struct TextureMT3* endTextures;
		unsigned long long endTextures_;
	};
};

struct _Model
{
	long numVertices;
	struct _MVertex* vertexList;
	long numNormals;
	struct _SVectorNoPad* normalList;
	long numFaces;
	struct _MFace* faceList;
	long numSegments;
	struct _Segment* segmentList;
	struct AniTex* aniTextures;
	short maxRad;
	short pad;
	long maxRadSq;
	struct MultiSpline* multiSpline;
	struct TextureMT3* startTextures;
	struct TextureMT3* endTextures;
};

struct _G2AnimFxHeader_Type 
{
	unsigned char sizeAndSection;
	char type;
	unsigned short keyframeID;
};

struct _G2AnimKeylist_Type64
{
	unsigned char sectionCount;
	unsigned char s0TailTime;
	unsigned char s1TailTime;
	unsigned char s2TailTime;
	unsigned short keyCount;
	short timePerKey;
	unsigned short pad00;
	short pad01;
	unsigned short pad10;
	short pad11;
	union
	{
		struct _G2AnimFxHeader_Type* fxList;
		unsigned long long fxList_;
	};
	union
	{
		unsigned short* sectionData[3];
		unsigned long long sectionData_[3];
	};
};

struct _G2AnimKeylist_Type 
{
	unsigned char sectionCount;
	unsigned char s0TailTime;
	unsigned char s1TailTime;
	unsigned char s2TailTime;
	unsigned short keyCount;
	short timePerKey;
	unsigned short pad00;
	short pad01;
	unsigned short pad10;
	short pad11;
	struct _G2AnimFxHeader_Type* fxList;
	unsigned short* sectionData[3];
};

struct Object64
{
	long oflags;
	short id;
	short sfxFileHandle;
	short numModels;
	short numAnims;
	union
	{
		struct _Model** modelList;
		unsigned long long modelList_;
	};
	union
	{
		struct _G2AnimKeylist_Type** animList;
		unsigned long long animList_;
	};
	short introDist;
	short vvIntroDist;
	short removeDist;
	short vvRemoveDist;
	union
	{
		int* data;
		unsigned long long data_;
	};
	union
	{
		char* script;
		unsigned long long script_;
	};
	union
	{
		char* name;
		unsigned long long name_;
	};
	union
	{
		int* soundData;
		unsigned long long soundData_;
	};
	long oflags2;
	short sectionA;
	short sectionB;
	short sectionC;
	short numberOfEffects;
	union
	{
		int* effectList;
		unsigned long long effectList_;
	};
	union
	{
		long* relocList;
		unsigned long long relocList_;
	};
	union
	{
		int* relocModule;
		unsigned long long relocModule_;
	};
	struct VramSize vramSize;
};

struct Object
{
	long oflags;
	short id;
	short sfxFileHandle;
	short numModels;
	short numAnims;
	struct _Model** modelList;
	struct _G2AnimKeylist_Type** animList;
	short introDist;
	short vvIntroDist;
	short removeDist;
	short vvRemoveDist;
	int* data;
	char* script;
	char* name;
	int* soundData;
	long oflags2;
	short sectionA;
	short sectionB;
	short sectionC;
	short numberOfEffects;
	int* effectList;
	long* relocList;
	int* relocModule;
	struct VramSize vramSize;
};