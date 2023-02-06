#pragma once

#include "META.H"

enum DTPFlags : int
{
	DTP_FLAGS_NONE               = 0,
	DTP_FLAGS_BASE_TYPE    = (1 << 0),
	DTP_FLAGS_DTP          = (1 << 1),
};

struct DTPList
{
	void* ptr;
};

BOOST_FUSION_ADAPT_STRUCT(DTPList,
	ptr
);


struct CString
{
	char* pString;
};

BOOST_FUSION_ADAPT_STRUCT(CString,
	pString
);

struct VramSize 
{
	short x;
	short y;
	short w;
	short h;
};

BOOST_FUSION_ADAPT_STRUCT(VramSize, 
	x, 
	y, 
	w, 
	h
);

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

BOOST_FUSION_ADAPT_STRUCT(TextureMT3,
	u0,
	v0,
	clut,
	u1,
	v1,
	tpage,
	u2,
	v2,
	pad1,
	sortPush,
	color
);

struct _Vertex
{
	short x;
	short y;
	short z;
};

BOOST_FUSION_ADAPT_STRUCT(_Vertex,
	x,
	y,
	z
);

struct _MVertex
{
	struct _Vertex vertex;
	unsigned short normal;
};

BOOST_FUSION_ADAPT_STRUCT(_MVertex,
	vertex,
	normal
);

struct _SVectorNoPad 
{
	short x;
	short y;
	short z;
};

BOOST_FUSION_ADAPT_STRUCT(_SVectorNoPad,
	x,
	y,
	z
);

struct _Face 
{
	unsigned short v0;
	unsigned short v1;
	unsigned short v2;
};

BOOST_FUSION_ADAPT_STRUCT(_Face,
	v0,
	v1,
	v2
);

struct _MFace 
{
	struct _Face face;
	unsigned char normal;
	unsigned char flags;
	long color;
};

BOOST_FUSION_ADAPT_STRUCT(_MFace,
	face,
	normal,
	flags,
	color
);

struct _Position 
{
	short x;
	short y;
	short z;
};

BOOST_FUSION_ADAPT_STRUCT(_Position,
	x,
	y,
	z
);

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

BOOST_FUSION_ADAPT_STRUCT(_HFace,
	v0,
	v1,
	v2,
	attr,
	pad,
	normal,
	n0
);

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

BOOST_FUSION_ADAPT_STRUCT(_HSphere,
	attr,
	id,
	rank,
	pad,
	position,
	radius,
	radiusSquared
);

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

BOOST_FUSION_ADAPT_STRUCT(_HBox,
	flags,
	id,
	rank,
	minX,
	minY,
	minZ,
	maxX,
	maxY,
	maxZ,
	refMinX,
	refMinY,
	refMinZ,
	refMaxX,
	refMaxY,
	refMaxZ
);

struct _HInfo
{
	long numHFaces;
	struct _HFace* hfaceList;
	long numHSpheres;
	struct _HSphere* hsphereList;
	long numHBoxes;
	struct _HBox* hboxList;
};

BOOST_FUSION_ADAPT_STRUCT(_HInfo,
	numHFaces,
	hfaceList,
	numHSpheres,
	hsphereList,
	numHBoxes,
	hboxList
);

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

BOOST_FUSION_ADAPT_STRUCT(_Segment,
	flags,
	firstTri,
	lastTri,
	firstVertex,
	lastVertex,
	px,
	py,
	pz,
	parent,
	hInfo
);

struct AniTexInfo
{
	struct TextureMT3* texture;
	long numFrames;
	long speed;
};

BOOST_FUSION_ADAPT_STRUCT(AniTexInfo,
	texture,
	numFrames,
	speed
);

struct AniTex
{
	long numAniTextues;
	struct AniTexInfo aniTexInfo;
};

BOOST_FUSION_ADAPT_STRUCT(AniTex,
	numAniTextues,
	aniTexInfo
);

struct vecS
{
	short x;
	short y;
	short z;
};

BOOST_FUSION_ADAPT_STRUCT(vecS,
	x,
	y,
	z
);

struct vecL
{
	long x;
	long y;
	long z;
};

BOOST_FUSION_ADAPT_STRUCT(vecL,
	x,
	y,
	z
);

struct SplineKey
{
	short count;
	struct vecS point;
	struct vecL dd;
	struct vecL ds;
};

BOOST_FUSION_ADAPT_STRUCT(SplineKey,
	count,
	point,
	dd,
	ds
);

struct Spline
{
	struct SplineKey* key;
	short numkeys;
	unsigned char type;
	unsigned char flags;
};

BOOST_FUSION_ADAPT_STRUCT(Spline,
	key,
	numkeys,
	type,
	flags
);

struct _G2Quat_Type {
	short x;
	short y;
	short z;
	short w;
};

BOOST_FUSION_ADAPT_STRUCT(_G2Quat_Type,
	x,
	y,
	z,
	w
);

struct SplineRotKey
{
	short count;
	struct _G2Quat_Type q;
};

BOOST_FUSION_ADAPT_STRUCT(SplineRotKey,
	count,
	q
);

struct RSpline
{
	struct SplineRotKey* key;
	short numkeys;
	unsigned char type;
	unsigned char flags;
};

BOOST_FUSION_ADAPT_STRUCT(RSpline,
	key,
	numkeys,
	type,
	flags
);

struct SplineDef 
{
	short currkey;
	unsigned short denomFlag;
	long fracCurr;
};

BOOST_FUSION_ADAPT_STRUCT(SplineDef,
	currkey,
	denomFlag,
	fracCurr,
);

struct MATRIX
{
	short m[3][3];
	long t[3];
};

BOOST_FUSION_ADAPT_STRUCT(MATRIX,
	m,
	t,
);

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

BOOST_FUSION_ADAPT_STRUCT(MultiSpline,
	positional,
	rotational,
	scaling,
	color,
	curPositional,
	curRotational,
	curScaling,
	curColor,
	curRotMatrix
);

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

BOOST_FUSION_ADAPT_STRUCT(_Model,
	numVertices,
	vertexList,
	numNormals,
	normalList,
	numFaces,
	faceList,
	numSegments,
	segmentList,
	aniTextures,
	maxRad,
	pad,
	maxRadSq,
	multiSpline,
	startTextures,
	endTextures
);

struct Object
{
	long oflags;
	short id;
	short sfxFileHandle;
	short numModels;
	short numAnims;
	struct _Model** modelList;
	int** animList;
	short introDist;
	short vvIntroDist;
	short removeDist;
	short vvRemoveDist;
	int* data;
	CString* script;
	CString* name;
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

BOOST_FUSION_ADAPT_STRUCT(Object, 
	oflags, 
	id, 
	sfxFileHandle, 
	numModels, 
	numAnims, 
	modelList, 
	animList, 
	introDist,
	vvIntroDist,
	removeDist,
	vvRemoveDist,
	data,
	script,
	name,
	soundData,
	oflags2,
	sectionA,
	sectionB,
	sectionC,
	numberOfEffects,
	effectList,
	relocList,
	relocModule,
	vramSize
);