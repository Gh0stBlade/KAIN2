#ifndef KAIN2_MONOLITHIC_HEADER
#define KAIN2_MONOLITHIC_HEADER

#include "psyq.h"
#include "gex2.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 240

#define ONE_MB 1048576
#define TWO_MB ONE_MB * 2
#define BASE_ADDRESS 0x80000000
#define PACK_MAGIC_USHORT(A, B, C, D) A << 12 | B << 8 | C << 4 | D << 0
#define DEFAULT_MEM_MAGIC PACK_MAGIC_USHORT(0xB, 0xA, 0xD, 0xE)

typedef unsigned __int64 QWORD;
typedef unsigned int DWORD;
typedef unsigned short WORD;

/* Begin enums */
struct NodeType // hashcode: 0x5BF613D7 (dec: 1542853591)
{
	struct NodeType* prev; // size=8, offset=0
	struct NodeType* next; // size=8, offset=4
};





struct SplineDef // hashcode: 0x04DD5459 (dec: 81613913)
{
	short currkey; // size=0, offset=0
	unsigned short denomFlag; // size=0, offset=2
	long fracCurr; // size=0, offset=4
};

enum option_ctrl_t // Hashcode: 0xEBCEABAC (dec: -338777172)
{
	option_ctrl_bogus = 0,
	option_ctrl_none = 1,
	option_ctrl_select = 2,
	option_ctrl_left = 3,
	option_ctrl_right = 4,
	option_ctrl_num_states = 5,
};

enum _G2AnimCallbackMsg_Enum // Hashcode: 0xA8F4A89C (dec: -1460361060)
{
	G2ANIM_MSG_DONE = 1,
	G2ANIM_MSG_LOOPPOINT = 2,
	G2ANIM_MSG_SECTION_INTERPDONE = 3,
	G2ANIM_MSG_SEGCTRLR_INTERPDONE = 4,
	G2ANIM_MSG_SWALARMSET = 5,
	G2ANIM_MSG_PLAYEFFECT = 6,
};

enum MATH3D_AXIS // Hashcode: 0x37FE6D4A (dec: 939421002)
{
	AXIS_X = 0,
	AXIS_Y = 1,
	AXIS_Z = 2,
	AXIS_NEG_X = 3,
	AXIS_NEG_Y = 4,
	AXIS_NEG_Z = 5,
};

enum sfx_t // Hashcode: 0xC1FB9866 (dec: -1040476058)
{
	sfx_sound = 0,
	sfx_music = 1,
	sfx_voice = 2,
	sfx_error = 3,
};

enum bdepth // Hashcode: 0xB297A406 (dec: -1298684922)
{
	TIM_4BIT = 0,
	TIM_8BIT = 1,
	TIM_16BIT = 2,
};

enum mcpsx_err_t // Hashcode: 0xB64074E4 (dec: -1237289756)
{
	mcpsx_err_none = 0,
	mcpsx_err_card_not_exist = 1,
	mcpsx_err_card_invalid = 2,
	mcpsx_err_new_card = 3,
	mcpsx_err_not_format = 4,
	mcpsx_err_file_not_exist = 5,
	mcpsx_err_already_exists = 6,
	mcpsx_err_block_full = 7,
	mcpsx_err_busy = 8,
	mcpsx_error = 9,
	mcpsx_err_num_items = 10,
};

enum mcpsx_func_t // Hashcode: 0xA495DFC7 (dec: -1533681721)
{
	func_none = 0,
	func_exist = 1,
	func_accept = 2,
	func_read_file = 3,
	func_write_file = 4,
	func_create_file = 5,
	func_delete_file = 6,
	func_format = 7,
	func_unformat = 8,
	func_directory = 9,
	func_num_items = 10,
};

enum mcpsx_sync_t // Hashcode: 0xC0D85FD5 (dec: -1059561515)
{
	sync_idle = -1,
	sync_busy = 0,
	sync_done = 1,
	sync_func = 2,
	sync_num_items = 3,
};

enum mcpsx_mode_t // Hashcode: 0x8A03A4E4 (dec: -1979472668)
{
	mode_bogus = 0,
	mode_initialized = 1,
	mode_running = 2,
	mode_terminated = 3,
	mode_num_items = 4,
};

enum state_t // Hashcode: 0x67CC6ED5 (dec: 1741450965)
{
	fsmcard_card_not_exist = 0,
	fsmcard_card_invalid = 1,
	fsmcard_not_format = 2,
	fsmcard_new_card = 3,
	fsmcard_directory = 4,
	fsmcard_ready = 5,
	fsmcard_format = 6,
	fsmcard_unformat = 7,
	fsmcard_delete = 8,
	fsmcard_create = 9,
	fsmcard_write = 10,
	fsmcard_read = 11,
	fsmcard_error = 12,
	fsmcard_num_items = 13,
};

enum mcard_status_t // Hashcode: 0xD1B8064B (dec: -776468917)
{
	mcard_status_card_not_exist = 0,
	mcard_status_card_invalid = 1,
	mcard_status_not_format = 2,
	mcard_status_new_card = 3,
	mcard_status_ready = 4,
	mcard_status_accessing = 5,
	mcard_status_error = 6,
	mcard_status_num_items = 7,
};

enum fsm_t // Hashcode: 0x64C4BA3C (dec: 1690614332)
{
	fsm_card_not_exist = 0,
	fsm_card_invalid = 1,
	fsm_not_format = 2,
	fsm_new_card = 3,
	fsm_ready = 4,
	fsm_format = 5,
	fsm_start = 6,
	fsm_loading = 7,
	fsm_query_overwrite = 8,
	fsm_overwrite = 9,
	fsm_saving = 10,
	fsm_create = 11,
	fsm_save_complete = 12,
	fsm_error = 13,
	fsm_num_items = 14,
};
/* End enums */

/* Begin structs */
struct LightInstance // hashcode: 0xB8D5E61F (dec: -1193941473)
{
	struct _Instance *lightInstance; // size=0, offset=0
	long r; // size=0, offset=4
	long g; // size=0, offset=8
	long b; // size=0, offset=12
	short radius; // size=0, offset=16
	unsigned char segment; // size=0, offset=18
	unsigned char flags; // size=0, offset=19
};

//struct _Instance // hashcode: 0x699A5B3D (dec: 1771723581)
//{
//	struct NodeType node; // size=8, offset=0
//	struct _Instance *next; // size=616, offset=8
//	struct _Instance *prev; // size=616, offset=12
//	long instanceID; // size=0, offset=16
//	long flags; // size=0, offset=20
//	long flags2; // size=0, offset=24
//	struct Object *object; // size=76, offset=28
//	struct Intro *intro; // size=0, offset=32
//	void *data; // size=0, offset=36
//	void *introData; // size=0, offset=40
//	struct _Instance *parent; // size=616, offset=44
//	long introNum; // size=0, offset=48
//	long birthStreamUnitID; // size=0, offset=52
//	long currentStreamUnitID; // size=0, offset=56
//	long introUniqueID; // size=0, offset=60
//	struct MATRIX *matrix; // size=32, offset=64
//	struct MATRIX *oldMatrix; // size=32, offset=68
//	struct SoundInstance soundInstanceTbl[4]; // size=12, offset=72
//	unsigned long maxCheckDistance; // size=0, offset=84
//	unsigned long checkMask; // size=0, offset=88
//	struct _Position position; // size=6, offset=92
//	unsigned char lightGroup; // size=0, offset=98
//	struct _Position oldPos; // size=6, offset=100
//	unsigned char spectralLightGroup; // size=0, offset=106
//	short currentTextureAnimFrame; // size=0, offset=108
//	struct _Position initialPos; // size=6, offset=110
//	struct _Rotation rotation; // size=8, offset=116
//	struct _Rotation oldRotation; // size=8, offset=124
//	struct _SVector scale; // size=8, offset=132
//	long attachedID; // size=0, offset=140
//	long attachedSegment; // size=0, offset=144
//	struct MATRIX relativeMatrix; // size=32, offset=148
//	struct _TFace *tface; // size=12, offset=180
//	struct _TFace *oldTFace; // size=12, offset=184
//	void *tfaceLevel; // size=0, offset=188
//	void *collideInfo; // size=0, offset=192
//	long bspTree; // size=0, offset=196
//	short cachedTFace; // size=0, offset=200
//	short cachedBSPTree; // size=0, offset=202
//	void *cachedTFaceLevel; // size=0, offset=204
//	struct _Normal oldWNormal; // size=8, offset=208
//	struct _Normal wNormal; // size=8, offset=216
//	struct _SVector offset; // size=8, offset=224
//	struct _HModel *hModelList; // size=8, offset=232
//	short fadeValue; // size=0, offset=236
//	short lightMatrix; // size=0, offset=238
//	void (*processFunc)(); // size=0, offset=240
//	void (*collideFunc)(); // size=0, offset=244
//	void (*additionalCollideFunc)(); // size=0, offset=248
//	unsigned long (*queryFunc)(); // size=0, offset=252
//	void (*messageFunc)(); // size=0, offset=256
//	struct _Position shadowPosition; // size=6, offset=260
//	short currentModel; // size=0, offset=266
//	struct _TFace *waterFace; // size=12, offset=268
//	struct _TFace *oldWaterFace; // size=12, offset=272
//	struct _Terrain *waterFaceTerrain; // size=0, offset=276
//	struct _PlaneConstants halvePlane; // size=12, offset=280
//	struct _SVector splitPoint; // size=8, offset=292
//	struct _Instance *LinkParent; // size=616, offset=300
//	struct _Instance *LinkChild; // size=616, offset=304
//	struct _Instance *LinkSibling; // size=616, offset=308
//	long ParentLinkNode; // size=0, offset=312
//	void *InstanceState; // size=0, offset=316
//	char introName[12]; // size=12, offset=320
//	void *extraData; // size=0, offset=332
//	short constrictAngle; // size=0, offset=336
//	short petrifyValue; // size=0, offset=338
//	long xVel; // size=0, offset=340
//	long yVel; // size=0, offset=344
//	long zVel; // size=0, offset=348
//	long xAccl; // size=0, offset=352
//	long yAccl; // size=0, offset=356
//	long zAccl; // size=0, offset=360
//	long maxXVel; // size=0, offset=364
//	long maxYVel; // size=0, offset=368
//	long maxZVel; // size=0, offset=372
//	long currentMainState; // size=0, offset=376
//	long work0; // size=0, offset=380
//	long work1; // size=0, offset=384
//	long work2; // size=0, offset=388
//	long work3; // size=0, offset=392
//	long work4; // size=0, offset=396
//	long work5; // size=0, offset=400
//	struct _G2Anim_Type anim; // size=180, offset=404
//	enum _G2Bool_Enum (*rebuildCallback)(); // size=4, offset=584
//	short splineFlags; // size=0, offset=588
//	short targetFrame; // size=0, offset=590
//	short clipBeg; // size=0, offset=592
//	short clipEnd; // size=0, offset=594
//	struct EventAliasCommandStruct aliasCommand; // size=12, offset=596
//	long light_color; // size=0, offset=608
//	struct CVECTOR *perVertexColor; // size=4, offset=612
//};

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

struct TextureFT3 // hashcode: 0x2D2801D6 (dec: 757596630)
{
	unsigned char u0; // size=0, offset=0
	unsigned char v0; // size=0, offset=1
	unsigned short clut; // size=0, offset=2
	unsigned char u1; // size=0, offset=4
	unsigned char v1; // size=0, offset=5
	unsigned short tpage; // size=0, offset=6
	unsigned char u2; // size=0, offset=8
	unsigned char v2; // size=0, offset=9
	unsigned short attr; // size=0, offset=10
};

struct TextureMT3 // hashcode: 0xC9D0A5B9 (dec: -909072967)
{
	unsigned char u0; // size=0, offset=0
	unsigned char v0; // size=0, offset=1
	unsigned short clut; // size=0, offset=2
	unsigned char u1; // size=0, offset=4
	unsigned char v1; // size=0, offset=5
	unsigned short tpage; // size=0, offset=6
	unsigned char u2; // size=0, offset=8
	unsigned char v2; // size=0, offset=9
	char pad1; // size=0, offset=10
	char sortPush; // size=0, offset=11
	long color; // size=0, offset=12
};

struct AniTexInfo // hashcode: 0x907C013D (dec: -1870921411)
{
	struct TextureMT3 *texture; // size=16, offset=0
	long numFrames; // size=0, offset=4
	long speed; // size=0, offset=8
};

struct AniTex // hashcode: 0xE4DA2729 (dec: -455465175)
{
	long numAniTextues; // size=0, offset=0
	struct AniTexInfo aniTexInfo; // size=12, offset=4
};

struct DrMoveAniTexSrcInfo // hashcode: 0x4AD6E543 (dec: 1255597379)
{
	short pixSrcX; // size=0, offset=0
	short pixSrcY; // size=0, offset=2
	short clutSrcX; // size=0, offset=4
	short clutSrcY; // size=0, offset=6
};

struct DrMoveAniTexDestInfo // hashcode: 0xF3A982EE (dec: -206994706)
{
	short pixDstX; // size=0, offset=0
	short pixDstY; // size=0, offset=2
	short pixW; // size=0, offset=4
	short pixH; // size=0, offset=6
	short clutDstX; // size=0, offset=8
	short clutDstY; // size=0, offset=10
	short clutW; // size=0, offset=12
	short clutH; // size=0, offset=14
	short pixCurrentX; // size=0, offset=16
	short pixCurrentY; // size=0, offset=18
	short clutCurrentX; // size=0, offset=20
	short clutCurrentY; // size=0, offset=22
	long numFrames; // size=0, offset=24
	long speed; // size=0, offset=28
	struct DrMoveAniTexSrcInfo frame; // size=8, offset=32
};

struct DrMoveAniTex // hashcode: 0x0820A5D4 (dec: 136357332)
{
	long numAniTextues; // size=0, offset=0
	struct DrMoveAniTexDestInfo *aniTexInfo; // size=40, offset=4
};

struct MemHeader // hashcode: 0xDAEE4890 (dec: -621918064)
{
	unsigned short magicNumber; // size=0, offset=0
	unsigned char memStatus; // size=0, offset=2
	unsigned char memType; // size=0, offset=3
	unsigned long memSize; // size=0, offset=4
};

struct _Model // hashcode: 0x9BCE1D24 (dec: -1680990940)
{
	long numVertices; // size=0, offset=0
	struct _MVertex *vertexList; // size=8, offset=4
	long numNormals; // size=0, offset=8
	struct _SVectorNoPad *normalList; // size=6, offset=12
	long numFaces; // size=0, offset=16
	struct _MFace *faceList; // size=12, offset=20
	long numSegments; // size=0, offset=24
	struct _Segment *segmentList; // size=24, offset=28
	struct AniTex *aniTextures; // size=16, offset=32
	short maxRad; // size=0, offset=36
	short pad; // size=0, offset=38
	long maxRadSq; // size=0, offset=40
	struct MultiSpline *multiSpline; // size=0, offset=44
	struct TextureMT3 *startTextures; // size=16, offset=48
	struct TextureMT3 *endTextures; // size=16, offset=52
};

struct _BVertex // hashcode: 0xADD3BD2F (dec: -1378632401)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct _BFace // hashcode: 0x69100F99 (dec: 1762660249)
{
	short v0; // size=0, offset=0
	short v1; // size=0, offset=2
	short v2; // size=0, offset=4
	short pad; // size=0, offset=6
	struct TextureFT3 *texture; // size=12, offset=8
};

struct _BGObject // hashcode: 0xEC69FB30 (dec: -328598736)
{
	short flags; // size=0, offset=0
	short numFaces; // size=0, offset=2
	struct _BVertex *vertexList; // size=8, offset=4
	struct _BFace *faceList; // size=12, offset=8
	short x; // size=0, offset=12
	short y; // size=0, offset=14
	short z; // size=0, offset=16
	short radius; // size=0, offset=18
	long numVertices; // size=0, offset=20
};

struct Signal // hashcode: 0xA0225F88 (dec: -1608360056)
{
	long id; // size=0, offset=0
	union {
		struct {
			union {
				long l; // size=0, offset=0
				short s[2]; // size=4, offset=0
				char c[4]; // size=4, offset=0
			} size; // size=4, offset=0, found in object files: player.o, player.o
		} misc; // size=12, offset=0, found in object files: player.o, player.o
		struct _CameraKey *cameraKey; // size=24, offset=0
		struct {
			short x; // size=0, offset=0
			short y; // size=0, offset=2
			short z; // size=0, offset=4
			short zrot; // size=0, offset=6
			struct _SVector offset; // size=8, offset=8
		} teleport; // size=16, offset=0, found in object files: player.o, player.o
		struct {
			long index; // size=0, offset=0
			long value; // size=0, offset=4
		} cameraValue; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			long index; // size=0, offset=0
			long value; // size=0, offset=4
		} miscValue; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			long index; // size=0, offset=0
			long value; // size=0, offset=4
		} logicValue; // size=8, offset=0, found in object files: player.o, player.o
		long timer; // size=0, offset=0
		long times; // size=0, offset=0
		long lightGroup; // size=0, offset=0
		long farPlane; // size=0, offset=0
		long cameraAdjust; // size=0, offset=0
		long cameraMode; // size=0, offset=0
		long cameraLock; // size=0, offset=0
		long cameraUnlock; // size=0, offset=0
		long cameraSmooth; // size=0, offset=0
		long cameraTimer; // size=0, offset=0
		struct {
			long time; // size=0, offset=0
			long scale; // size=0, offset=4
		} cameraShake; // size=8, offset=0, found in object files: player.o, player.o
		long cameraSave; // size=0, offset=0
		long cameraRestore; // size=0, offset=0
		struct _BGObject *bgObject; // size=24, offset=0
		long value[2]; // size=8, offset=0
		struct {
			long action; // size=0, offset=0
			long value[2]; // size=8, offset=4
		} sound; // size=12, offset=0, found in object files: player.o, player.o
		void *intro; // size=0, offset=0
		long deathZ; // size=0, offset=0
		struct {
			void *intro; // size=0, offset=0
			void *data; // size=0, offset=4
		} dsignal; // size=8, offset=0, found in object files: player.o, player.o
		void *mirror; // size=0, offset=0
		long fogNear; // size=0, offset=0
		long fogFar; // size=0, offset=0
		struct {
			void *intro; // size=0, offset=0
			long frame; // size=0, offset=4
		} gotoFrame; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			void *intro; // size=0, offset=0
			long model; // size=0, offset=4
		} changeModel; // size=8, offset=0, found in object files: player.o, player.o
		void *vmObject; // size=0, offset=0
		struct {
			unsigned char target; // size=0, offset=0
			unsigned char source1; // size=0, offset=1
			unsigned char source2; // size=0, offset=2
			unsigned char pad; // size=0, offset=3
			struct Signal *gotoLabel; // size=0, offset=4
		} logic; // size=8, offset=0, found in object files: player.o, player.o
		void *callSignal; // size=0, offset=0
		struct _SVector offset; // size=8, offset=0
		struct Signal *gotoLabel; // size=0, offset=0
		struct Signal *gosubLabel; // size=0, offset=0
		struct {
			long timer; // size=0, offset=0
			long action; // size=0, offset=4
		} control; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			short index; // size=0, offset=0
			short z; // size=0, offset=2
			struct MultiSignal *upSignal; // size=0, offset=4
			struct MultiSignal *downSignal; // size=0, offset=8
		} zSignal; // size=12, offset=0, found in object files: player.o, player.o
		struct {
			long currentnum; // size=0, offset=0
			long streamID; // size=0, offset=4
			char toname[16]; // size=16, offset=8
		} StreamLevel; // size=24, offset=0, found in object files: player.o, player.o
		struct {
			char num; // size=0, offset=0
			char vel; // size=0, offset=1
			short misc; // size=0, offset=2
			char flags; // size=0, offset=4
			char timeToLive; // size=0, offset=5
			short dummy2; // size=0, offset=6
			struct Object *object; // size=0, offset=8
		} shards; // size=12, offset=0, found in object files: player.o, player.o
		struct {
			long index; // size=0, offset=0
			void *intro; // size=0, offset=4
		} cameraSpline; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			short type; // size=0, offset=0
			short time; // size=0, offset=2
		} screenWipe; // size=4, offset=0, found in object files: player.o, player.o
		long voiceSitId; // size=0, offset=0
		struct {
			long time; // size=0, offset=0
			void *intro; // size=0, offset=4
		} introFX; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			void *intro; // size=0, offset=0
			struct Signal *gotoLabel; // size=0, offset=4
		} introActive; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			char world[9]; // size=9, offset=0
			char level[9]; // size=9, offset=9
		} levelChange; // size=18, offset=0, found in object files: player.o, player.o
		long blendStart; // size=0, offset=0
		struct {
			struct Signal *signal; // size=0, offset=0
			long times; // size=0, offset=4
		} setTimes; // size=8, offset=0, found in object files: player.o, player.o
		struct {
			unsigned char r; // size=0, offset=0
			unsigned char g; // size=0, offset=1
			unsigned char b; // size=0, offset=2
			unsigned char pad; // size=0, offset=3
		} color; // size=4, offset=0, found in object files: player.o, player.o
		struct {
			short x; // size=0, offset=0
			short y; // size=0, offset=2
			short time; // size=0, offset=4
			short index; // size=0, offset=6
			char *text; // size=0, offset=8
		} print; // size=12, offset=0, found in object files: player.o, player.o
		long slideAngle; // size=0, offset=0
	} data; // size=24, offset=4, found in object files: player.o, player.o
};

struct _MultiSignal // hashcode: 0x0620B768 (dec: 102807400)
{
	long numSignals; // size=0, offset=0
	short signalNum; // size=0, offset=4
	short flags; // size=0, offset=6
	struct Signal signalList[32]; // size=896, offset=8
};

struct _HFace // hashcode: 0x30CA5CA6 (dec: 818568358)
{
	short v0; // size=0, offset=0
	short v1; // size=0, offset=2
	short v2; // size=0, offset=4
	unsigned char attr; // size=0, offset=6
	char pad; // size=0, offset=7
	unsigned short normal; // size=0, offset=8
	unsigned short n0; // size=0, offset=10
};

struct _HSphere // hashcode: 0x8E7B7191 (dec: -1904512623)
{
	long attr; // size=0, offset=0
	unsigned char id; // size=0, offset=4
	unsigned char rank; // size=0, offset=5
	short pad; // size=0, offset=6
	struct _Position position; // size=6, offset=8
	unsigned short radius; // size=0, offset=14
	unsigned long radiusSquared; // size=0, offset=16
};

struct _HBox // hashcode: 0x47CD8BDA (dec: 1204653018)
{
	short flags; // size=0, offset=0
	unsigned char id; // size=0, offset=2
	unsigned char rank; // size=0, offset=3
	short minX; // size=0, offset=4
	short minY; // size=0, offset=6
	short minZ; // size=0, offset=8
	short maxX; // size=0, offset=10
	short maxY; // size=0, offset=12
	short maxZ; // size=0, offset=14
	short refMinX; // size=0, offset=16
	short refMinY; // size=0, offset=18
	short refMinZ; // size=0, offset=20
	short refMaxX; // size=0, offset=22
	short refMaxY; // size=0, offset=24
	short refMaxZ; // size=0, offset=26
};

struct _HPrim // hashcode: 0x988F5E11 (dec: -1735434735)
{
	unsigned char hpFlags; // size=0, offset=0
	unsigned char withFlags; // size=0, offset=1
	unsigned char type; // size=0, offset=2
	unsigned char segment; // size=0, offset=3
	union {
		struct _HSphere *hsphere; // size=20, offset=0
		struct _HBox *hbox; // size=28, offset=0
		struct _HFace *hface; // size=12, offset=0
	} data; // size=4, offset=4, found in object files: player.o, player.o
};

struct _HModel // hashcode: 0xBC680356 (dec: -1134034090)
{
	struct _HPrim *hPrimList; // size=8, offset=0
	long numHPrims; // size=0, offset=4
};

struct _HInfo // hashcode: 0x6AC9807F (dec: 1791590527)
{
	long numHFaces; // size=0, offset=0
	struct _HFace *hfaceList; // size=12, offset=4
	long numHSpheres; // size=0, offset=8
	struct _HSphere *hsphereList; // size=20, offset=12
	long numHBoxes; // size=0, offset=16
	struct _HBox *hboxList; // size=28, offset=20
};

struct _Segment // hashcode: 0x2F06BF28 (dec: 788971304)
{
	long flags; // size=0, offset=0
	short firstTri; // size=0, offset=4
	short lastTri; // size=0, offset=6
	short firstVertex; // size=0, offset=8
	short lastVertex; // size=0, offset=10
	short px; // size=0, offset=12
	short py; // size=0, offset=14
	short pz; // size=0, offset=16
	short parent; // size=0, offset=18
	struct _HInfo *hInfo; // size=24, offset=20
};

struct MultiSpline // hashcode: 0x2823B2CF (dec: 673428175)
{
	struct Spline *positional; // size=8, offset=0
	struct RSpline *rotational; // size=8, offset=4
	struct Spline *scaling; // size=8, offset=8
	struct Spline *color; // size=8, offset=12
	struct SplineDef curPositional; // size=8, offset=16
	struct SplineDef curRotational; // size=8, offset=24
	struct SplineDef curScaling; // size=8, offset=32
	struct SplineDef curColor; // size=8, offset=40
	struct MATRIX curRotMatrix; // size=32, offset=48
};

struct ObjectEffect // hashcode: 0x2892438D (dec: 680674189)
{
	unsigned char effectNumber; // size=0, offset=0
	unsigned char modifierList[3]; // size=3, offset=1
};

struct _TFace // hashcode: 0x85FE430F (dec: -2046934257)
{
	struct _Face face; // size=6, offset=0
	unsigned char attr; // size=0, offset=6
	char sortPush; // size=0, offset=7
	unsigned short normal; // size=0, offset=8
	unsigned short textoff; // size=0, offset=10
};

struct _TVertex // hashcode: 0x0883ABD5 (dec: 142846933)
{
	struct _Vertex vertex; // size=6, offset=0
	unsigned short rgb15; // size=0, offset=6
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
};

struct _InstancePool // hashcode: 0xC591AB92 (dec: -980309102)
{
	long nextInstanceID; // size=0, offset=0
	long numFreeInstances; // size=0, offset=4
	struct _Instance instance[62]; // size=38192, offset=8
	struct _Instance *first_free; // size=616, offset=38200
};

struct _InstanceList // hashcode: 0x7594B1C7 (dec: 1972679111)
{
	long numInstances; // size=0, offset=0
	struct _Instance *first; // size=616, offset=4
	struct _InstancePool *pool; // size=38204, offset=8
	struct NodeType group[32]; // size=256, offset=12
};

struct _ObjectTracker // hashcode: 0xFE4678BF (dec: -28936001)
{
	char name[16]; // size=16, offset=0
	struct Object *object; // size=76, offset=16
	short objectStatus; // size=0, offset=20
	short numInUse; // size=0, offset=22
	void *vramBlock; // size=0, offset=24
	char numObjectsUsing; // size=0, offset=28
	char objectsUsing[7]; // size=7, offset=29
};

struct _BoundingBox // hashcode: 0x2183782B (dec: 562264107)
{
	short minX; // size=0, offset=0
	short minY; // size=0, offset=2
	short minZ; // size=0, offset=4
	short maxX; // size=0, offset=6
	short maxY; // size=0, offset=8
	short maxZ; // size=0, offset=10
};

struct CDLight // hashcode: 0xD90757B8 (dec: -653830216)
{
	struct NodeType node; // size=8, offset=0
	unsigned char r; // size=0, offset=8
	unsigned char g; // size=0, offset=9
	unsigned char b; // size=0, offset=10
	unsigned char flags; // size=0, offset=11
	struct _Sphere sphere; // size=12, offset=12
};

struct _BSPNode // hashcode: 0x60C20F69 (dec: 1623330665)
{
	struct _Sphere_noSq sphere; // size=8, offset=0
	short a; // size=0, offset=8
	short b; // size=0, offset=10
	short c; // size=0, offset=12
	short flags; // size=0, offset=14
	long d; // size=0, offset=16
	void *front; // size=0, offset=20
	void *back; // size=0, offset=24
	struct _Sphere_noSq spectralSphere; // size=8, offset=28
	short front_spectral_error; // size=0, offset=36
	short back_spectral_error; // size=0, offset=38
	short front_material_error; // size=0, offset=40
	short back_material_error; // size=0, offset=42
};

struct _BSPLeaf // hashcode: 0xB2C1AC71 (dec: -1295930255)
{
	struct _Sphere_noSq sphere; // size=8, offset=0
	struct _TFace *faceList; // size=12, offset=8
	short numFaces; // size=0, offset=12
	short flags; // size=0, offset=14
	struct _BoundingBox box; // size=12, offset=16
	struct _BoundingBox spectralBox; // size=12, offset=28
	struct _Sphere_noSq spectralSphere; // size=8, offset=40
};

struct _MorphVertex // hashcode: 0xAAB8DABE (dec: -1430725954)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short vindex; // size=0, offset=6
	short hx; // size=0, offset=8
	short hy; // size=0, offset=10
	short hz; // size=0, offset=12
};

struct _MorphColor // hashcode: 0xA6BA0326 (dec: -1497758938)
{
	short morphColor15; // size=0, offset=0
};

struct _SavedCinematic // hashcode: 0x1E29151B (dec: 506008859)
{
	struct _Position position; // size=6, offset=0
	struct _Position focusPoint; // size=6, offset=6
	struct _Position base; // size=6, offset=12
	struct _Position targetPos; // size=6, offset=18
	struct _Position targetFocusPoint; // size=6, offset=24
	struct _Position targetBase; // size=6, offset=30
	struct _Rotation baseRot; // size=8, offset=36
	struct _Rotation focusRotation; // size=8, offset=44
	struct _Rotation targetFocusRotation; // size=8, offset=52
	short focusDistance; // size=0, offset=60
	short targetFocusDistance; // size=0, offset=62
	struct _SVector focusPointVel; // size=8, offset=64
	struct _SVector focusPointAccl; // size=8, offset=72
	struct _SVector baseVel; // size=8, offset=80
	struct _SVector baseAccl; // size=8, offset=88
	long maxVel; // size=0, offset=96
	struct MultiSpline *posSpline; // size=0, offset=100
	struct MultiSpline *targetSpline; // size=0, offset=104
	struct Level *level; // size=0, offset=108
};

struct BSPTree // hashcode: 0x17800192 (dec: 394264978)
{
	struct _BSPNode *bspRoot; // size=44, offset=0
	struct _BSPLeaf *startLeaves; // size=48, offset=4
	struct _BSPLeaf *endLeaves; // size=48, offset=8
	struct _Position globalOffset; // size=6, offset=12
	short flags; // size=0, offset=18
	struct _Position localOffset; // size=6, offset=20
	short ID; // size=0, offset=26
	long splineID; // size=0, offset=28
	struct _Instance *instanceSpline; // size=616, offset=32
};

struct LightGroup // hashcode: 0xBB5FF847 (dec: -1151338425)
{
	struct MATRIX lightMatrix; // size=32, offset=0
	struct MATRIX colorMatrix; // size=32, offset=32
};

struct LightList // hashcode: 0xC8448BB7 (dec: -935031881)
{
	struct CVECTOR ambient; // size=4, offset=0
	long numLightGroups; // size=0, offset=4
	struct LightGroup *lightGroupList; // size=64, offset=8
};

struct PointLight // hashcode: 0xFEFDD2F2 (dec: -16919822)
{
	struct NodeType node; // size=8, offset=0
	unsigned char r; // size=0, offset=8
	unsigned char g; // size=0, offset=9
	unsigned char b; // size=0, offset=10
	unsigned char flags; // size=0, offset=11
	struct _Position position; // size=6, offset=12
	short radius; // size=0, offset=18
	long radiusSquared; // size=0, offset=20
};

struct SpotLight // hashcode: 0xEF8B4C2E (dec: -276083666)
{
	struct NodeType node; // size=8, offset=0
	unsigned char r; // size=0, offset=8
	unsigned char g; // size=0, offset=9
	unsigned char b; // size=0, offset=10
	unsigned char flags; // size=0, offset=11
	struct _Position centroid; // size=6, offset=12
	short radius; // size=0, offset=18
	long radiusSquared; // size=0, offset=20
	struct _Position position; // size=6, offset=24
	struct _Position direction; // size=6, offset=30
	short cosFalloffAngle; // size=0, offset=36
	short attenuationScale; // size=0, offset=38
};

struct LightInfo // hashcode: 0x2A95530E (dec: 714429198)
{
	long numPointLights; // size=0, offset=0
	struct PointLight pointLightPool[32]; // size=768, offset=4
	struct NodeType usedPointLightList; // size=8, offset=772
	struct NodeType freePointLightList; // size=8, offset=780
	struct NodeType activePointLightList; // size=8, offset=788
	long numSpotLights; // size=0, offset=796
	struct SpotLight spotLightPool[8]; // size=320, offset=800
	struct NodeType usedSpotLightList; // size=8, offset=1120
	struct NodeType freeSpotLightList; // size=8, offset=1128
	struct NodeType activeSpotLightList; // size=8, offset=1136
	long numSavedColors; // size=0, offset=1144
};

struct _VMOffset // hashcode: 0x680AC445 (dec: 1745536069)
{
	short dx; // size=0, offset=0
	short dy; // size=0, offset=2
	short dz; // size=0, offset=4
};

struct _VMColorOffset // hashcode: 0xFD3FB530 (dec: -46156496)
{
	char dr; // size=0, offset=0
	char dg; // size=0, offset=1
	char db; // size=0, offset=2
};

struct _VMInterpolated // hashcode: 0x3A40B5C0 (dec: 977319360)
{
	short tvIdx; // size=0, offset=0
	short startIdx; // size=0, offset=2
	short endIdx; // size=0, offset=4
	short time; // size=0, offset=6
};

struct _VMOffsetTable // hashcode: 0xE4A13881 (dec: -459196287)
{
	long numVMOffsets; // size=0, offset=0
	union {
		struct _VMOffset moveOffsets; // size=6, offset=0
		struct _VMColorOffset colorOffsets; // size=3, offset=0
	} offsets; // size=6, offset=4, found in object files: player.o, player.o
};

struct _VMObject // hashcode: 0x436E75EB (dec: 1131312619)
{
	unsigned short flags; // size=0, offset=0
	short bspIdx; // size=0, offset=2
	short materialIdx; // size=0, offset=4
	short spectralIdx; // size=0, offset=6
	short currentIdx; // size=0, offset=8
	short timeScale; // size=0, offset=10
	long timer; // size=0, offset=12
	struct _Position position; // size=6, offset=16
	short radius; // size=0, offset=22
	long radiusSquared; // size=0, offset=24
	long numVMOffsetTables; // size=0, offset=28
	struct _VMOffsetTable **vmoffsetTableList; // size=12, offset=32
	struct _VMOffsetTable *curVMOffsetTable; // size=12, offset=36
	long numVMVertices; // size=0, offset=40
	void *vmvertexList; // size=0, offset=44
	long numVMInterpolated; // size=0, offset=48
	struct _VMInterpolated *vminterpolatedList; // size=8, offset=52
	char *name; // size=0, offset=56
};

struct _VGroup // hashcode: 0x4421FB39 (dec: 1143077689)
{
	long id; // size=0, offset=0
	long numVertices; // size=0, offset=4
	struct _TVertex **vertexList; // size=12, offset=8
};

struct _HotSpot // hashcode: 0x6EC4F776 (dec: 1858402166)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short id; // size=0, offset=6
};

struct _PlanMkr // hashcode: 0xBC0DD043 (dec: -1139945405)
{
	struct _Position pos; // size=6, offset=0
	short id; // size=0, offset=6
};

struct _SFXMkr // hashcode: 0x74BB4848 (dec: 1958430792)
{
	unsigned char *soundData; // size=0, offset=0
	long uniqueID; // size=0, offset=4
	struct SoundInstance sfxTbl[4]; // size=12, offset=8
	struct _Position pos; // size=6, offset=20
	short pad; // size=0, offset=26
	long livesInOnePlace; // size=0, offset=28
	long inSpectral; // size=0, offset=32
};

struct TClassAttr // hashcode: 0x4DC02369 (dec: 1304437609)
{
	short flags; // size=0, offset=0
	unsigned short sound; // size=0, offset=2
};

struct _ColorType // hashcode: 0x440E837C (dec: 1141801852)
{
	unsigned char r; // size=0, offset=0
	unsigned char g; // size=0, offset=1
	unsigned char b; // size=0, offset=2
	unsigned char code; // size=0, offset=3
};

struct _CameraCore_Type // hashcode: 0xD341D51C (dec: -750660324)
{
	struct _Position position; // size=6, offset=0
	short pad1; // size=0, offset=6
	struct MATRIX vvNormalWorVecMat[2]; // size=64, offset=8
	long vvPlaneConsts[6]; // size=24, offset=72
	struct MATRIX *wcTransform; // size=32, offset=96
	long farPlane; // size=0, offset=100
	long projDistance; // size=0, offset=104
	long nearPlane; // size=0, offset=108
	struct MATRIX *wcTransform2; // size=32, offset=112
	struct MATRIX *cwTransform2; // size=32, offset=116
	struct _Normal viewVolumeNormal[5]; // size=40, offset=120
	int leftX; // size=0, offset=160
	int rightX; // size=0, offset=164
	int topY; // size=0, offset=168
	int bottomY; // size=0, offset=172
	struct _Rotation rotation; // size=8, offset=176
	short pad2; // size=0, offset=184
	struct _SVector screenScale; // size=8, offset=186
	short pad3; // size=0, offset=194
	struct _Position debugPos; // size=6, offset=196
	short pad4; // size=0, offset=202
	struct _Rotation debugRot; // size=8, offset=204
	short pad5; // size=0, offset=212
	struct _Position nondebugPos; // size=6, offset=214
	short pad6; // size=0, offset=220
};

struct PLAYER_114fake // hashcode: 0x5A1E67AD (dec: 1511942061)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: player.o, player.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: player.o, player.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: player.o, player.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: player.o, player.o
};

struct SpuVolume // hashcode: 0x1BD13981 (dec: 466696577)
{
	short left; // size=0, offset=0
	short right; // size=0, offset=2
};

struct SpuVoiceAttr // hashcode: 0x819E0A50 (dec: -2120349104)
{
	unsigned long voice; // size=0, offset=0
	unsigned long mask; // size=0, offset=4
	struct SpuVolume volume; // size=4, offset=8
	struct SpuVolume volmode; // size=4, offset=12
	struct SpuVolume volumex; // size=4, offset=16
	unsigned short pitch; // size=0, offset=20
	unsigned short note; // size=0, offset=22
	unsigned short sample_note; // size=0, offset=24
	short envx; // size=0, offset=26
	unsigned long addr; // size=0, offset=28
	unsigned long loop_addr; // size=0, offset=32
	long a_mode; // size=0, offset=36
	long s_mode; // size=0, offset=40
	long r_mode; // size=0, offset=44
	unsigned short ar; // size=0, offset=48
	unsigned short dr; // size=0, offset=50
	unsigned short sr; // size=0, offset=52
	unsigned short rr; // size=0, offset=54
	unsigned short sl; // size=0, offset=56
	unsigned short adsr1; // size=0, offset=58
	unsigned short adsr2; // size=0, offset=60
};

struct SpuExtAttr // hashcode: 0x58B0F474 (dec: 1487991924)
{
	struct SpuVolume volume; // size=4, offset=0
	long reverb; // size=0, offset=4
	long mix; // size=0, offset=8
};

struct SpuStVoiceAttr // hashcode: 0xAFDFBA7E (dec: -1344292226)
{
	char status; // size=0, offset=0
	char pad1; // size=0, offset=1
	char pad2; // size=0, offset=2
	char pad3; // size=0, offset=3
	long last_size; // size=0, offset=4
	unsigned long buf_addr; // size=0, offset=8
	unsigned long data_addr; // size=0, offset=12
};

struct AadSoundBankHdr // hashcode: 0x453DB83F (dec: 1161672767)
{
	unsigned long bankID; // size=0, offset=0
	unsigned long headerSize; // size=0, offset=4
	int bankVersion; // size=0, offset=8
	int numPrograms; // size=0, offset=12
	int numTones; // size=0, offset=16
	int numWaves; // size=0, offset=20
	int numSequences; // size=0, offset=24
	int numLabels; // size=0, offset=28
	int reverbMode; // size=0, offset=32
	int reverbDepth; // size=0, offset=36
};

struct AadProgramAtr // hashcode: 0x13FC0BEF (dec: 335285231)
{
	unsigned short numTones; // size=0, offset=0
	unsigned short firstTone; // size=0, offset=2
	unsigned char volume; // size=0, offset=4
	unsigned char panPosition; // size=0, offset=5
	unsigned short unused; // size=0, offset=6
};

struct AadToneAtr // hashcode: 0x990CD685 (dec: -1727211899)
{
	unsigned char priority; // size=0, offset=0
	unsigned char parentProgram; // size=0, offset=1
	unsigned char volume; // size=0, offset=2
	unsigned char panPosition; // size=0, offset=3
	unsigned char centerNote; // size=0, offset=4
	char centerFine; // size=0, offset=5
	unsigned char minNote; // size=0, offset=6
	unsigned char maxNote; // size=0, offset=7
	unsigned char mode; // size=0, offset=8
	unsigned char pitchBendMax; // size=0, offset=9
	unsigned short adsr1; // size=0, offset=10
	unsigned short adsr2; // size=0, offset=12
	short waveIndex; // size=0, offset=14
};

struct _AadDynSfxFileHdr // hashcode: 0xB9609773 (dec: -1184852109)
{
	unsigned long snfID; // size=0, offset=0
	unsigned short snfVersion; // size=0, offset=4
	unsigned short uniqueID; // size=0, offset=6
	unsigned short handle; // size=0, offset=8
	unsigned short numSfxInFile; // size=0, offset=10
	struct _AadDynSfxFileHdr *prevDynSfxFile; // size=20, offset=12
	struct _AadDynSfxFileHdr *nextDynSfxFile; // size=20, offset=16
};

struct AadDynSfxAttr // hashcode: 0x20751E4B (dec: 544546379)
{
	unsigned short sfxID; // size=0, offset=0
	unsigned short waveID; // size=0, offset=2
	struct AadToneAtr toneAttr; // size=16, offset=4
	unsigned long waveSize; // size=0, offset=20
};

struct AadTempoCounter // hashcode: 0x15707CC0 (dec: 359693504)
{
	unsigned long currentTick; // size=0, offset=0
	unsigned long currentError; // size=0, offset=4
	unsigned long tickTimeFixed; // size=0, offset=8
	unsigned long errorPerUpdate; // size=0, offset=12
	unsigned short ticksPerUpdate; // size=0, offset=16
	unsigned short ppqn; // size=0, offset=18
	unsigned long quarterNoteTime; // size=0, offset=20
};

struct AadSeqEvent // hashcode: 0x3E306185 (dec: 1043358085)
{
	unsigned long deltaTime; // size=0, offset=0
	unsigned char track; // size=0, offset=4
	unsigned char statusByte; // size=0, offset=5
	char dataByte[6]; // size=6, offset=6
};

struct _AadSequenceSlot // hashcode: 0xD7421DD6 (dec: -683532842)
{
	struct AadTempoCounter tempo; // size=24, offset=0
	struct AadSeqEvent eventQueue[4][16]; // size=768, offset=24
	unsigned char eventIn[16]; // size=16, offset=792
	unsigned char eventOut[16]; // size=16, offset=808
	unsigned char eventsInQueue[16]; // size=16, offset=824
	unsigned char (*sequencePosition[16]); // size=64, offset=840
	unsigned long lastEventExecutedTime[16]; // size=64, offset=904
	unsigned char runningStatus[16]; // size=16, offset=968
	unsigned char trackFlags[16]; // size=16, offset=984
	unsigned char (*loopSequencePosition[4][16]); // size=256, offset=1000
	unsigned char loopCurrentNestLevel[16]; // size=16, offset=1256
	unsigned char loopCounter[4][16]; // size=64, offset=1272
	struct _AadSequenceSlot *selectedSlotPtr; // size=1488, offset=1336
	unsigned char selectedSlotNum; // size=0, offset=1340
	unsigned char selectedChannel; // size=0, offset=1341
	unsigned char sequenceNumberAssigned; // size=0, offset=1342
	unsigned char thisSlotNumber; // size=0, offset=1343
	unsigned short status; // size=0, offset=1344
	unsigned short channelMute; // size=0, offset=1346
	unsigned short delayedMuteMode; // size=0, offset=1348
	unsigned short delayedMuteCmds; // size=0, offset=1350
	unsigned short delayedUnMuteCmds; // size=0, offset=1352
	unsigned short unused; // size=0, offset=1354
	unsigned short enableSustainUpdate; // size=0, offset=1356
	unsigned short ignoreTranspose; // size=0, offset=1358
	unsigned char slotFlags; // size=0, offset=1360
	unsigned char slotID; // size=0, offset=1361
	unsigned char slotVolume; // size=0, offset=1362
	unsigned char slotPan; // size=0, offset=1363
	int *masterVolPtr; // size=0, offset=1364
	int selectedDynamicBank; // size=0, offset=1368
	int sequenceAssignedDynamicBank; // size=0, offset=1372
	unsigned char currentDynamicBank[16]; // size=16, offset=1376
	unsigned short pitchWheel[16]; // size=32, offset=1392
	unsigned char currentProgram[16]; // size=16, offset=1424
	unsigned char volume[16]; // size=16, offset=1440
	unsigned char panPosition[16]; // size=16, offset=1456
	char transpose[16]; // size=16, offset=1472
};

struct AadSfxCommand // hashcode: 0xD8F832F6 (dec: -654822666)
{
	unsigned char statusByte; // size=0, offset=0
	unsigned char dataByte[3]; // size=3, offset=1
	unsigned long ulongParam; // size=0, offset=4
	short shortParam; // size=0, offset=8
	unsigned short unused; // size=0, offset=10
};

struct AadSfxSlot // hashcode: 0x6E655A78 (dec: 1852136056)
{
	struct AadSfxCommand commandQueue[32]; // size=384, offset=0
	unsigned char commandIn; // size=0, offset=384
	unsigned char commandOut; // size=0, offset=385
	unsigned short commandsInQueue; // size=0, offset=386
	unsigned short handleCounter; // size=0, offset=388
	unsigned char sfxVolume; // size=0, offset=390
	unsigned char sfxPan; // size=0, offset=391
};

struct AadSynthVoice // hashcode: 0x12E02CA2 (dec: 316681378)
{
	unsigned long voiceMask; // size=0, offset=0
	unsigned long handle; // size=0, offset=4
	unsigned char voiceID; // size=0, offset=8
	unsigned char priority; // size=0, offset=9
	unsigned char note; // size=0, offset=10
	unsigned char unused; // size=0, offset=11
	unsigned char program; // size=0, offset=12
	unsigned char baseProg; // size=0, offset=13
	unsigned char volume; // size=0, offset=14
	unsigned char updateVol; // size=0, offset=15
	unsigned char pan; // size=0, offset=16
	unsigned char voiceNum; // size=0, offset=17
	unsigned short flags; // size=0, offset=18
	struct AadToneAtr *toneAtr; // size=16, offset=20
	struct AadProgramAtr *progAtr; // size=8, offset=24
};

struct AadDynamicLoadRequest // hashcode: 0xBC9D60B7 (dec: -1130536777)
{
	int type; // size=0, offset=0
	int handle; // size=0, offset=4
	long directoryID; // size=0, offset=8
	long flags; // size=0, offset=12
	char fileName[12]; // size=12, offset=16
};

struct AadDynamicBankLoadInfo // hashcode: 0x6A3AE9FB (dec: 1782245883)
{
	char sndFileName[32]; // size=32, offset=0
	char smpFileName[32]; // size=32, offset=32
	int dynamicBankIndex; // size=0, offset=64
	int loadOption; // size=0, offset=68
	void (*userCallbackProc)(); // size=0, offset=72
	unsigned long sramDataSize; // size=0, offset=76
	unsigned long nextSramAddr; // size=0, offset=80
	unsigned long flags; // size=0, offset=84
};

struct AadDynamicSfxLoadInfo // hashcode: 0xEDD41D4C (dec: -304865972)
{
	char snfFileName[40]; // size=40, offset=0
	char smfFileName[40]; // size=40, offset=40
	int fileHandle; // size=0, offset=80
	long directoryID; // size=0, offset=84
	long loadFlags; // size=0, offset=88
	long flags; // size=0, offset=92
	int smfLoadingState; // size=0, offset=96
	unsigned long bytesToLoad; // size=0, offset=100
	int numSfxToLoad; // size=0, offset=104
	int error; // size=0, offset=108
	unsigned long waveTransferAddr; // size=0, offset=112
	unsigned long totalSramUsed; // size=0, offset=116
	struct _AadDynSfxFileHdr *snfFile; // size=20, offset=120
	struct AadDynSfxAttr attr; // size=24, offset=124
};

struct AadNewSramBlockDesc // hashcode: 0x4DF7A665 (dec: 1308075621)
{
	unsigned short waveID; // size=0, offset=0
	unsigned short address; // size=0, offset=2
	unsigned short size; // size=0, offset=4
	unsigned char prevIndex; // size=0, offset=6
	unsigned char nextIndex; // size=0, offset=7
};

struct AadLoadedSfxToneAttr // hashcode: 0xC1F66431 (dec: -1040817103)
{
	unsigned short referenceCount; // size=0, offset=0
	unsigned short waveID; // size=0, offset=2
	struct AadToneAtr toneAttr; // size=16, offset=4
};

struct AadLoadedSfxWaveAttr // hashcode: 0x3EFD9321 (dec: 1056805665)
{
	unsigned short referenceCount; // size=0, offset=0
	unsigned char unused; // size=0, offset=2
	unsigned char sramHandle; // size=0, offset=3
};

struct AadSramDefragInfo // hashcode: 0xB08B77B9 (dec: -1333037127)
{
	int status; // size=0, offset=0
	unsigned char *fragBuffer; // size=0, offset=4
	int masterListEntry; // size=0, offset=8
	int waveID; // size=0, offset=12
	unsigned long destSramAddr; // size=0, offset=16
	unsigned long srcSramAddr; // size=0, offset=20
	unsigned long moveSize; // size=0, offset=24
	unsigned long readSize; // size=0, offset=28
};

struct AadVolFader // hashcode: 0x053CCA3C (dec: 87870012)
{
	int volumeStep; // size=0, offset=0
	int targetVolume; // size=0, offset=4
	void (*fadeCompleteCallback)(); // size=0, offset=8
};

struct MusicLoadCmd // hashcode: 0x575883B8 (dec: 1465418680)
{
	int type; // size=0, offset=0
	int data; // size=0, offset=4
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

struct Spline // hashcode: 0xA7B8D53F (dec: -1481059009)
{
	struct SplineKey *key; // size=32, offset=0
	short numkeys; // size=0, offset=4
	unsigned char type; // size=0, offset=6
	unsigned char flags; // size=0, offset=7
};



struct SplineRotKey // hashcode: 0xD2E3A87E (dec: -756832130)
{
	short count; // size=0, offset=0
	struct _G2Quat_Type q; // size=8, offset=2
};

struct RSpline // hashcode: 0xD9C500BD (dec: -641400643)
{
	struct SplineRotKey *key; // size=10, offset=0
	short numkeys; // size=0, offset=4
	unsigned char type; // size=0, offset=6
	unsigned char flags; // size=0, offset=7
};

struct SplineKey // hashcode: 0x5D3D994D (dec: 1564318029)
{
	short count; // size=0, offset=0
	struct vecS point; // size=6, offset=2
	struct vecL dd; // size=12, offset=8
	struct vecL ds; // size=12, offset=20
};

struct _HVertex // hashcode: 0xC488A83D (dec: -997676995)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short normal; // size=0, offset=6
	long pad; // size=0, offset=8
};

struct POLY_F4 // hashcode: 0xE2AC295D (dec: -492033699)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	short x1; // size=0, offset=12
	short y1; // size=0, offset=14
	short x2; // size=0, offset=16
	short y2; // size=0, offset=18
	short x3; // size=0, offset=20
	short y3; // size=0, offset=22
};

struct _SmallRotation // hashcode: 0xB9CA5EE3 (dec: -1177919773)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

struct EventPointers // hashcode: 0x3787E212 (dec: 931652114)
{
	long numPuzzles; // size=0, offset=0
	struct Event (*eventInstances[1]); // size=4, offset=4
};

struct Camera // hashcode: 0x0670C20F (dec: 108053007)
{
	struct _CameraCore_Type core; // size=224, offset=0
	long shake; // size=0, offset=224
	short shakeFrame; // size=0, offset=228
	short shakeScale; // size=0, offset=230
	long flags; // size=0, offset=232
	long lock; // size=0, offset=236
	short mode; // size=0, offset=240
	short state; // size=0, offset=242
	short posState; // size=0, offset=244
	short rotState; // size=0, offset=246
	short tiltState; // size=0, offset=248
	short distanceState; // size=0, offset=250
	short lagZ; // size=0, offset=252
	short tilt; // size=0, offset=254
	struct _Position focusPoint; // size=6, offset=256
	short focusDistance; // size=0, offset=262
	struct _Instance *focusInstance; // size=616, offset=264
	struct _Position oldFocusInstancePos; // size=6, offset=268
	struct _Position newFocusInstancePos; // size=6, offset=274
	struct _Rotation oldFocusInstanceRot; // size=8, offset=280
	struct _Rotation newFocusInstanceRot; // size=8, offset=288
	struct _SVector focusInstanceVelVec; // size=8, offset=296
	struct _Position focusLeadPivot; // size=6, offset=304
	struct _Position focusOffset; // size=6, offset=310
	struct _Rotation focusRotation; // size=8, offset=316
	short tiltVel; // size=0, offset=324
	short tiltAccl; // size=0, offset=326
	struct _SVector positionVel; // size=8, offset=328
	struct _SVector positionAccl; // size=8, offset=336
	struct _SVector focusPointVel; // size=8, offset=344
	struct _SVector focusPointAccl; // size=8, offset=352
	short focusDistanceVel; // size=0, offset=360
	short focusDistanceAccl; // size=0, offset=362
	struct _SVector focusRotVel; // size=8, offset=364
	struct _SVector focusRotAccl; // size=8, offset=372
	struct _SVector maxFocusVel; // size=8, offset=380
	struct _SVector rotationVel; // size=8, offset=388
	struct _SVector rotationAccl; // size=8, offset=396
	short maxVel; // size=0, offset=404
	short maxRot; // size=0, offset=406
	struct _Position targetPos; // size=6, offset=408
	short targetTilt; // size=0, offset=414
	struct _Rotation targetRotation; // size=8, offset=416
	short targetFocusDistance; // size=0, offset=424
	struct _Position targetFocusPoint; // size=6, offset=426
	short collisionTargetFocusDistance; // size=0, offset=432
	struct _Rotation targetFocusRotation; // size=8, offset=434
	short tfaceTilt; // size=0, offset=442
	struct _Rotation collisionTargetFocusRotation; // size=8, offset=444
	short smooth; // size=0, offset=452
	struct _CameraKey *cameraKey; // size=24, offset=456
	struct _Sphere focusSphere; // size=12, offset=460
	struct _Sphere smallBaseSphere; // size=12, offset=472
	struct _Sphere posSphere; // size=12, offset=484
	short lookTimer; // size=0, offset=496
	short lookSavedMode; // size=0, offset=498
	short presetIndex; // size=0, offset=500
	short presetFocusDistance[3]; // size=6, offset=502
	long minFocusDistance; // size=0, offset=508
	long maxFocusDistance; // size=0, offset=512
	long minTilt; // size=0, offset=516
	long maxTilt; // size=0, offset=520
	long pad[3]; // size=12, offset=524
	long minXYDist; // size=0, offset=536
	long maxXYDist; // size=0, offset=540
	long oppositeError; // size=0, offset=544
	long tempsplinePointAhead; // size=0, offset=548
	long noTeleport; // size=0, offset=552
	long focusDistanceList[3][3]; // size=36, offset=556
	long tiltList[3][3]; // size=36, offset=592
	long cineControl; // size=0, offset=628
	short stack; // size=0, offset=632
	short targetStack; // size=0, offset=634
	long savedMode[3]; // size=12, offset=636
	long savedTargetFocusDistance[3]; // size=12, offset=648
	struct _Rotation savedFocusRotation; // size=8, offset=660
	short savedFocusDistance; // size=0, offset=668
	short signalFocusDistance; // size=0, offset=670
	struct _SavedCinematic savedCinematic[3]; // size=336, offset=672
	struct _Rotation savedfocusRotation; // size=8, offset=1008
	struct _Position signalPos; // size=6, offset=1016
	short pad11; // size=0, offset=1022
	struct _Rotation signalRot; // size=8, offset=1024
	short forced_movement; // size=0, offset=1032
	short last_forced_movement; // size=0, offset=1034
	short rotDirection; // size=0, offset=1036
	unsigned long lastModTime; // size=0, offset=1040
	struct _Rotation lookRot; // size=8, offset=1044
	struct MultiSpline *Spline00; // size=0, offset=1052
	struct MultiSpline *Spline01; // size=0, offset=1056
	struct {
		struct {
			struct MultiSpline *posSpline; // size=0, offset=0
			struct MultiSpline *targetSpline; // size=0, offset=4
			struct _SVector lastSplinePos; // size=8, offset=8
			short splinePointAhead; // size=0, offset=16
			short cinema_done; // size=0, offset=18
			int splinecam_currkey; // size=0, offset=20
			int splinecam_helpkey; // size=0, offset=24
		} Cinematic; // size=28, offset=0, found in object files: debug.o, debug.o
		struct {
			struct _Position offset; // size=6, offset=0
			struct _Rotation rotation; // size=8, offset=6
			long focusDistance; // size=0, offset=16
		} Idle; // size=20, offset=28, found in object files: debug.o, debug.o
		struct {
			struct _TFace *tface; // size=12, offset=0
			struct _TFace *oldTFace; // size=12, offset=4
			struct _SVector oldTargetRot; // size=8, offset=8
			long timer; // size=0, offset=16
			struct _SVector oldFocusInstancePos; // size=8, offset=20
		} Behind; // size=28, offset=48, found in object files: debug.o, debug.o
		struct {
			long stopTimer; // size=0, offset=0
			long stopHit; // size=0, offset=4
			struct _TFace *tface; // size=12, offset=8
			struct _TFace *oldTFace; // size=12, offset=12
			long hit; // size=0, offset=16
		} Follow; // size=20, offset=76, found in object files: debug.o, debug.o
	} data; // size=96, offset=1060, found in object files: debug.o, debug.o
	long cuckooTimer; // size=0, offset=1156
	short teleportZRot; // size=0, offset=1160
	short extraZRot; // size=0, offset=1162
	short extraXRot; // size=0, offset=1164
	short leftTimer; // size=0, offset=1166
	short rightTimer; // size=0, offset=1168
	short collideRotControl; // size=0, offset=1170
	short collision_lastPush; // size=0, offset=1172
	short collision_timer; // size=0, offset=1174
	unsigned long prev_instance_mode; // size=0, offset=1176
	unsigned long instance_mode; // size=0, offset=1180
	long instance_xyvel; // size=0, offset=1184
	long instance_prev_xyvel; // size=0, offset=1188
	struct _Position real_focuspoint; // size=6, offset=1192
	short always_rotate_flag; // size=0, offset=1198
	short follow_flag; // size=0, offset=1200
	short actual_x_rot; // size=0, offset=1202
	short actual_vel_x; // size=0, offset=1204
	short actual_acc_x; // size=0, offset=1206
	short x_rot_change; // size=0, offset=1208
	short focuspoint_fallz; // size=0, offset=1210
	short lead_angle; // size=0, offset=1212
	short lead_vel; // size=0, offset=1214
	short lead_accl; // size=0, offset=1216
	short lead_timer; // size=0, offset=1218
};

struct DebugMenuLine // hashcode: 0x1BD18E81 (dec: 466718337)
{
	enum DEBUG_LINE_TYPE type; // size=4, offset=0
	long lower; // size=0, offset=4
	long upper; // size=0, offset=8
	char *text; // size=0, offset=12
	long *var_address; // size=0, offset=16
	long bit_mask; // size=0, offset=20
};

struct _ButtonTexture // hashcode: 0x9E750D8C (dec: -1636495988)
{
	short tpage; // size=0, offset=0
	short clut; // size=0, offset=2
	short textureW; // size=0, offset=4
	short textureH; // size=0, offset=6
	struct _BlockVramEntry *vramBlock; // size=0, offset=8
	long xshift; // size=0, offset=12
};

struct _BlockVramEntry // hashcode: 0x24B2FBC5 (dec: 615709637)
{
	struct _BlockVramEntry *next; // size=28, offset=0
	unsigned char type; // size=0, offset=4
	unsigned char flags; // size=0, offset=5
	short time; // size=0, offset=6
	short newX; // size=0, offset=8
	short newY; // size=0, offset=10
	short x; // size=0, offset=12
	short y; // size=0, offset=14
	short w; // size=0, offset=16
	short h; // size=0, offset=18
	long area; // size=0, offset=20
	union {
		struct _StreamUnit *streamUnit; // size=64, offset=0
		struct _ObjectTracker *streamObject; // size=36, offset=0
		struct _ButtonTexture *button; // size=16, offset=0
	} udata; // size=4, offset=24, found in object files: debug.o, debug.o
};

struct _BigFileEntry // hashcode: 0xA904436B (dec: -1459338389)
{
	long fileHash; // size=0, offset=0
	long fileLen; // size=0, offset=4
	long filePos; // size=0, offset=8
	long checkSumFull; // size=0, offset=12
};

struct _BigFileDirEntry // hashcode: 0x7D70C603 (dec: 2104542723)
{
	short streamUnitID; // size=0, offset=0
	short numFiles; // size=0, offset=2
	long subDirOffset; // size=0, offset=4
};

struct _BigFileDir // hashcode: 0x4E39D3E2 (dec: 1312412642)
{
	long numFiles; // size=0, offset=0
	struct _BigFileEntry fileList[0]; // size=0, offset=4
};

struct _BigFile // hashcode: 0x50C273F3 (dec: 1354920947)
{
	long bigfileBaseOffset; // size=0, offset=0
	struct _BigFileDir *rootDir; // size=4, offset=4
	struct _BigFileDir *currentDir; // size=4, offset=8
	struct _BigFileDir *cachedDir; // size=4, offset=12
	long currentDirID; // size=0, offset=16
	long cachedDirID; // size=0, offset=20
	long searchDirID; // size=0, offset=24
	long numSubDirs; // size=0, offset=28
	struct _BigFileDirEntry *subDirList; // size=8, offset=32
};

struct _NonBlockLoadEntry // hashcode: 0x722059B4 (dec: 1914722740)
{
	long fileHash; // size=0, offset=0
	long dirHash; // size=0, offset=4
	long filePos; // size=0, offset=8
	long *loadAddr; // size=0, offset=12
	long loadSize; // size=0, offset=16
	long checksum; // size=0, offset=20
	long checksumType; // size=0, offset=24
	long memType; // size=0, offset=28
	long posInFile; // size=0, offset=32
	long mallocFailOk; // size=0, offset=36
	void *retFunc; // size=0, offset=40
	void *retData; // size=0, offset=44
	void *retData2; // size=0, offset=48
	void **retPointer; // size=0, offset=52
};

struct _ReadQueueEntry // hashcode: 0x5360DCDA (dec: 1398856922)
{
	long readStartPos; // size=0, offset=0
	long readSize; // size=0, offset=4
	long readCurSize; // size=0, offset=8
	void *readStartDest; // size=0, offset=12
	void *readCurDest; // size=0, offset=16
	long readStatus; // size=0, offset=20
	long checksum; // size=0, offset=24
	long checksumType; // size=0, offset=28
	void *retFunc; // size=0, offset=32
	void *retData; // size=0, offset=36
	void *retData2; // size=0, offset=40
};

struct _FX_MATRIX // hashcode: 0x13190ADD (dec: 320408285)
{
	struct NodeType node; // size=8, offset=0
	long flags; // size=0, offset=8
	struct MATRIX lwTransform; // size=32, offset=12
};

struct _FX_PRIM // hashcode: 0x16BF818E (dec: 381649294)
{
	struct NodeType node; // size=8, offset=0
	struct TextureMT3 *texture; // size=16, offset=8
	long flags; // size=0, offset=12
	short timeToLive; // size=0, offset=16
	short curFrame; // size=0, offset=18
	void *process; // size=0, offset=20
	struct _FX_MATRIX *matrix; // size=44, offset=24
	long color; // size=0, offset=28
	long startColor; // size=0, offset=32
	long endColor; // size=0, offset=36
	short colorFadeValue; // size=0, offset=40
	short colorFadeStep; // size=0, offset=42
	struct _Position position; // size=6, offset=44
	short fadeStep; // size=0, offset=50
	short fadeValue[4]; // size=8, offset=52
	struct _Position v0; // size=6, offset=60
	short work0; // size=0, offset=66
	struct _Position v1; // size=6, offset=68
	short work1; // size=0, offset=74
	struct _Position v2; // size=6, offset=76
	short work2; // size=0, offset=82
	struct _Position v3; // size=6, offset=84
	short work3; // size=0, offset=90
	union {
		struct {
			struct _Instance *parent; // size=616, offset=0
			long segment; // size=0, offset=4
		} flame; // size=8, offset=0, found in object files: debug.o, debug.o
		struct {
			short xVel; // size=0, offset=0
			short yVel; // size=0, offset=2
			short zVel; // size=0, offset=4
			short xAccl; // size=0, offset=6
			short yAccl; // size=0, offset=8
			short zAccl; // size=0, offset=10
		} phys; // size=12, offset=0, found in object files: debug.o, debug.o
	} duo; // size=12, offset=92, found in object files: debug.o, debug.o
};

struct BASIC_LINE_G2 // hashcode: 0x684545B2 (dec: 1749370290)
{
	unsigned char r0; // size=0, offset=0
	unsigned char g0; // size=0, offset=1
	unsigned char b0; // size=0, offset=2
	unsigned char code; // size=0, offset=3
	short x0; // size=0, offset=4
	short y0; // size=0, offset=6
	unsigned char r1; // size=0, offset=8
	unsigned char g1; // size=0, offset=9
	unsigned char b1; // size=0, offset=10
	unsigned char p1; // size=0, offset=11
	short x1; // size=0, offset=12
	short y1; // size=0, offset=14
};

struct _BASIC_G4 // hashcode: 0xC15AD9EF (dec: -1051010577)
{
	unsigned char r0; // size=0, offset=0
	unsigned char g0; // size=0, offset=1
	unsigned char b0; // size=0, offset=2
	unsigned char code; // size=0, offset=3
	short x0; // size=0, offset=4
	short y0; // size=0, offset=6
	unsigned char r1; // size=0, offset=8
	unsigned char g1; // size=0, offset=9
	unsigned char b1; // size=0, offset=10
	unsigned char pad1; // size=0, offset=11
	short x1; // size=0, offset=12
	short y1; // size=0, offset=14
	unsigned char r2; // size=0, offset=16
	unsigned char g2; // size=0, offset=17
	unsigned char b2; // size=0, offset=18
	unsigned char pad2; // size=0, offset=19
	short x2; // size=0, offset=20
	short y2; // size=0, offset=22
	unsigned char r3; // size=0, offset=24
	unsigned char g3; // size=0, offset=25
	unsigned char b3; // size=0, offset=26
	unsigned char pad3; // size=0, offset=27
	short x3; // size=0, offset=28
	short y3; // size=0, offset=30
};

struct _BASIC_F4 // hashcode: 0x2871008E (dec: 678494350)
{
	unsigned char r0; // size=0, offset=0
	unsigned char g0; // size=0, offset=1
	unsigned char b0; // size=0, offset=2
	unsigned char code; // size=0, offset=3
	short x0; // size=0, offset=4
	short y0; // size=0, offset=6
	short x1; // size=0, offset=8
	short y1; // size=0, offset=10
	short x2; // size=0, offset=12
	short y2; // size=0, offset=14
	short x3; // size=0, offset=16
	short y3; // size=0, offset=18
};

struct __BubbleParams // hashcode: 0xE483C494 (dec: -461126508)
{
	short DisperseFrames; // size=0, offset=0
	short KillScale; // size=0, offset=2
	short MinSplashSize; // size=0, offset=4
	short MaxSpeed; // size=0, offset=6
	short MaxSpeedRange; // size=0, offset=8
	short ScaleRate; // size=0, offset=10
	short ScaleRateRange; // size=0, offset=12
	short StartScale; // size=0, offset=14
	short StartScaleRange; // size=0, offset=16
	short UniqueBubbles; // size=0, offset=18
};

struct _GenericParticleParams // hashcode: 0x7A6370AD (dec: 2053337261)
{
	short size; // size=0, offset=0
	unsigned char StartOnInit; // size=0, offset=2
	unsigned char type; // size=0, offset=3
	short birthRadius; // size=0, offset=4
	char startSegment; // size=0, offset=6
	char endSegment; // size=0, offset=7
	struct _Position direction; // size=6, offset=8
	unsigned char spectral_colorize; // size=0, offset=14
	unsigned char absolute_direction; // size=0, offset=15
	char accx; // size=0, offset=16
	char accy; // size=0, offset=17
	char accz; // size=0, offset=18
	char useInstanceModel; // size=0, offset=19
	unsigned char startColor_red; // size=0, offset=20
	unsigned char startColor_green; // size=0, offset=21
	unsigned char startColor_blue; // size=0, offset=22
	char model; // size=0, offset=23
	unsigned char endColor_red; // size=0, offset=24
	unsigned char endColor_green; // size=0, offset=25
	unsigned char endColor_blue; // size=0, offset=26
	char texture; // size=0, offset=27
	short lifeTime; // size=0, offset=28
	unsigned char primLifeTime; // size=0, offset=30
	char use_child; // size=0, offset=31
	short startFadeValue; // size=0, offset=32
	short fadeStep; // size=0, offset=34
	char numberBirthParticles; // size=0, offset=36
	char z_undulation_mode; // size=0, offset=37
	short scaleSpeed; // size=0, offset=38
	struct _Position offset; // size=6, offset=40
	short startScale; // size=0, offset=46
};

struct _GenericRibbonParams // hashcode: 0xD243E3C9 (dec: -767302711)
{
	char id; // size=0, offset=0
	char StartOnInit; // size=0, offset=1
	char startSegment; // size=0, offset=2
	char endSegment; // size=0, offset=3
	char type; // size=0, offset=4
	char use_child; // size=0, offset=5
	short ribbonLifeTime; // size=0, offset=6
	short faceLifeTime; // size=0, offset=8
	short startFadeValue; // size=0, offset=10
	long startColor; // size=0, offset=12
	long endColor; // size=0, offset=16
};

struct _GenericGlowParams // hashcode: 0xF67692B8 (dec: -160001352)
{
	char StartOnInit; // size=0, offset=0
	unsigned char segment; // size=0, offset=1
	unsigned char segmentEnd; // size=0, offset=2
	char numSegments; // size=0, offset=3
	unsigned char color_num; // size=0, offset=4
	char use_child; // size=0, offset=5
	unsigned char numColors; // size=0, offset=6
	char id; // size=0, offset=7
	long atuColorCycleRate; // size=0, offset=8
	short width; // size=0, offset=12
	short height; // size=0, offset=14
	long lifetime; // size=0, offset=16
	short fadein_time; // size=0, offset=20
	short fadeout_time; // size=0, offset=22
};

struct _GenericLightningParams // hashcode: 0x226C68CF (dec: 577530063)
{
	char type; // size=0, offset=0
	char use_child; // size=0, offset=1
	short lifeTime; // size=0, offset=2
	short deg; // size=0, offset=4
	short deg_inc; // size=0, offset=6
	struct _Position start_offset; // size=6, offset=8
	char startSeg; // size=0, offset=14
	char endSeg; // size=0, offset=15
	struct _Position end_offset; // size=6, offset=16
	char matrixSeg; // size=0, offset=22
	unsigned char segs; // size=0, offset=23
	short width; // size=0, offset=24
	short small_width; // size=0, offset=26
	short sine_size; // size=0, offset=28
	short variation; // size=0, offset=30
	long color; // size=0, offset=32
	long glow_color; // size=0, offset=36
};

struct _GenericBlastringParams // hashcode: 0xAB3B2462 (dec: -1422187422)
{
	char type; // size=0, offset=0
	char use_child; // size=0, offset=1
	short lifeTime; // size=0, offset=2
	struct _Position offset; // size=6, offset=4
	short matrixSeg; // size=0, offset=10
	char segment; // size=0, offset=12
	char sortInWorld; // size=0, offset=13
	short radius; // size=0, offset=14
	short size1; // size=0, offset=16
	short size2; // size=0, offset=18
	short endRadius; // size=0, offset=20
	short colorchange_radius; // size=0, offset=22
	int vel; // size=0, offset=24
	int accl; // size=0, offset=28
	short height1; // size=0, offset=32
	short height2; // size=0, offset=34
	short height3; // size=0, offset=36
	char predator_offset; // size=0, offset=38
	char stay_in_place; // size=0, offset=39
	long startColor; // size=0, offset=40
	long endColor; // size=0, offset=44
};

struct _GenericFlashParams // hashcode: 0x06D8DB60 (dec: 114875232)
{
	short type; // size=0, offset=0
	short timeToColor; // size=0, offset=2
	long color; // size=0, offset=4
	short timeAtColor; // size=0, offset=8
	short timeFromColor; // size=0, offset=10
};

struct _StreamUnit // hashcode: 0xAC366E40 (dec: -1405718976)
{
	long StreamUnitID; // size=0, offset=0
	short used; // size=0, offset=4
	short flags; // size=0, offset=6
	struct Level *level; // size=392, offset=8
	char baseAreaName[16]; // size=16, offset=12
	short toSignal; // size=0, offset=28
	short fromSignal; // size=0, offset=30
	long FrameCount; // size=0, offset=32
	short eventVariables[6]; // size=12, offset=36
	long FogColor; // size=0, offset=48
	short UnitFogFar; // size=0, offset=52
	short UnitFogNear; // size=0, offset=54
	short TargetFogFar; // size=0, offset=56
	short TargetFogNear; // size=0, offset=58
	short sfxFileHandle; // size=0, offset=60
	short pad; // size=0, offset=62
};

struct SavedBasic // hashcode: 0x6198E1EF (dec: 1637409263)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
};

struct _SavedIntro // hashcode: 0x8389D216 (dec: -2088119786)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	unsigned char lightGroup; // size=0, offset=2
	unsigned char specturalLightGroup; // size=0, offset=3
	char name[8]; // size=8, offset=4
	short introUniqueID; // size=0, offset=12
	short streamUnitID; // size=0, offset=14
	short birthUnitID; // size=0, offset=16
	unsigned short attachedUniqueID; // size=0, offset=18
	struct _SmallRotation smallRotation; // size=6, offset=20
	struct _Position position; // size=6, offset=26
	long flags; // size=0, offset=32
	long flags2; // size=0, offset=36
};

struct SavedEvent // hashcode: 0x429B6172 (dec: 1117479282)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	short eventNumber; // size=0, offset=4
	short eventVariables[5]; // size=10, offset=6
};

struct SavedBspTree // hashcode: 0x7AE7209C (dec: 2061967516)
{
	struct _Position localOffset; // size=6, offset=0
	unsigned char importantFlagsSaved; // size=0, offset=6
	unsigned char bspIndex; // size=0, offset=7
};

struct EventBasicObject // hashcode: 0x9FC1DB2A (dec: -1614685398)
{
	short id; // size=0, offset=0
};

struct EventWildCardObject // hashcode: 0x24FF3630 (dec: 620705328)
{
	short id; // size=0, offset=0
	short spad; // size=0, offset=2
	long unitID; // size=0, offset=4
	char *objectName; // size=0, offset=8
};

struct Event // hashcode: 0x68F4AD26 (dec: 1760865574)
{
	short eventNumber; // size=0, offset=0
	short numInstances; // size=0, offset=2
	unsigned char numActions; // size=0, offset=4
	unsigned char processingPuppetShow; // size=0, offset=5
	short eventVariables[5]; // size=10, offset=6
	struct EventBasicObject **instanceList; // size=2, offset=16
	struct ScriptPCode **conditionalList; // size=8, offset=20
	struct ScriptPCode **actionList; // size=8, offset=24
};

struct ScriptPCode // hashcode: 0x5E69DEB9 (dec: 1583996601)
{
	unsigned short sizeOfPcodeStream; // size=0, offset=0
	unsigned short conditionBits; // size=0, offset=2
	short *data; // size=0, offset=4
};

struct InstanceSpline // hashcode: 0x88E38857 (dec: -1998354345)
{
	struct _Instance *instance; // size=616, offset=0
	struct MultiSpline *spline; // size=80, offset=4
	long attribute; // size=0, offset=8
	long isParent; // size=0, offset=12
	long isClass; // size=0, offset=16
	long splineFlags; // size=0, offset=20
};

struct Vector3d // hashcode: 0x10BF06FC (dec: 280954620)
{
	short vx; // size=0, offset=0
	short vy; // size=0, offset=2
	short vz; // size=0, offset=4
	short pad1; // size=0, offset=6
	short errorx; // size=0, offset=8
	short errory; // size=0, offset=10
	short errorz; // size=0, offset=12
	short pad2; // size=0, offset=14
	long streamUnitID; // size=0, offset=16
};

struct Rotation3d // hashcode: 0x646160C4 (dec: 1684103364)
{
	short vx; // size=0, offset=0
	short vy; // size=0, offset=2
	short vz; // size=0, offset=4
	short pad1; // size=0, offset=6
	short errorx; // size=0, offset=8
	short errory; // size=0, offset=10
	short errorz; // size=0, offset=12
	short pad2; // size=0, offset=14
	long attribute; // size=0, offset=16
};

struct InstanceObject // hashcode: 0xA445CB1D (dec: -1538929891)
{
	struct _Instance *instance; // size=616, offset=0
	long attribute; // size=0, offset=4
};

struct EventObject // hashcode: 0x38B0B53D (dec: 951104829)
{
	struct Event *event; // size=28, offset=0
	long attribute; // size=0, offset=4
};

struct SavedEventObject // hashcode: 0xC69C23B2 (dec: -962845774)
{
	struct SavedBasic *savedEvent; // size=2, offset=0
	long attribute; // size=0, offset=4
	long areaID; // size=0, offset=8
	long eventNumber; // size=0, offset=12
};

struct GameObject // hashcode: 0xFA1A30A9 (dec: -98946903)
{
	long attribute; // size=0, offset=0
};

struct SignalObject // hashcode: 0x42CFA9E2 (dec: 1120905698)
{
	struct _MultiSignal *msignal; // size=904, offset=0
	long attribute; // size=0, offset=4
};

struct IntroObject // hashcode: 0x13CCBC8C (dec: 332184716)
{
	struct Intro *intro; // size=76, offset=0
	long attribute; // size=0, offset=4
};

struct InstanceAnimate // hashcode: 0x200AA21A (dec: 537567770)
{
	struct _Instance *instance; // size=616, offset=0
	long attribute; // size=0, offset=4
};

struct InstanceAnimateTexture // hashcode: 0x5266C0BB (dec: 1382465723)
{
	struct _Instance *instance; // size=616, offset=0
	struct AniTex *aniTextures; // size=16, offset=4
	long attribute; // size=0, offset=8
};

struct ListObject // hashcode: 0x1323C7B3 (dec: 321111987)
{
	struct EventWildCardObject *eventInstance; // size=12, offset=0
	long attribute[5]; // size=20, offset=4
	long numberOfAttributes; // size=0, offset=24
	long lineID; // size=0, offset=28
};

struct SubListObject // hashcode: 0xFDCC28E5 (dec: -36951835)
{
	struct _Instance **instanceList; // size=616, offset=0
	long numberOfInstances; // size=0, offset=4
	long numberOfAttributes; // size=0, offset=8
	long attribute[5]; // size=20, offset=12
};

struct TGroupObject // hashcode: 0x669B4B67 (dec: 1721453415)
{
	struct BSPTree *bspTree; // size=36, offset=0
	struct _StreamUnit *streamUnit; // size=64, offset=4
	long attribute; // size=0, offset=8
	short arg[2]; // size=4, offset=12
};

struct ConstrictInfo // hashcode: 0xA586BA8A (dec: -1517897078)
{
	struct _Instance *instance; // size=616, offset=0
	long attribute; // size=0, offset=4
};

struct CameraObject // hashcode: 0xD3BF3ACE (dec: -742442290)
{
	struct Camera *camera; // size=1220, offset=0
	long frames; // size=0, offset=4
	long attribute; // size=0, offset=8
};

struct EventVmObject // hashcode: 0x9DE5D8F8 (dec: -1645881096)
{
	struct _VMObject *vmObjectPtr; // size=60, offset=0
	struct Level *level; // size=392, offset=4
	long attribute; // size=0, offset=8
};

struct AreaObject // hashcode: 0x38AC5A77 (dec: 950819447)
{
	struct _StreamUnit *streamUnit; // size=64, offset=0
	long attribute; // size=0, offset=4
	long unitID; // size=0, offset=8
};

struct SoundObject // hashcode: 0x9DA5ECE7 (dec: -1650070297)
{
	short flags; // size=0, offset=0
	short attribute; // size=0, offset=2
	long soundNumber; // size=0, offset=4
	long value; // size=0, offset=8
	long duration; // size=0, offset=12
	union {
		struct _Instance *instance; // size=616, offset=0
		struct _SFXMkr *sfxMarker; // size=36, offset=0
	} data; // size=4, offset=16, found in object files: debug.o, debug.o
};

struct StackType // hashcode: 0x9CE4C465 (dec: -1662729115)
{
	long id; // size=0, offset=0
	union {
		struct {
			long instanceNumber; // size=0, offset=0
			long attribute; // size=0, offset=4
		} Object; // size=8, offset=0, found in object files: debug.o, debug.o
		struct {
			struct _SavedIntro *intro; // size=40, offset=0
			long attribute; // size=0, offset=4
		} SavedIntroObject; // size=8, offset=0, found in object files: debug.o, debug.o
		struct InstanceSpline instanceSpline; // size=24, offset=0
		struct {
			long currentNumber; // size=0, offset=0
			long error; // size=0, offset=4
			short flags; // size=0, offset=8
		} Number; // size=12, offset=0, found in object files: debug.o, debug.o
		struct {
			short vx; // size=0, offset=0
			short vy; // size=0, offset=2
		} Vector2d; // size=4, offset=0, found in object files: debug.o, debug.o
		struct {
			char *pointer; // size=0, offset=0
		} CharPointer; // size=4, offset=0, found in object files: debug.o, debug.o
		struct {
			short *pointer; // size=0, offset=0
		} ShortPointer; // size=4, offset=0, found in object files: debug.o, debug.o
		struct {
			long *pointer; // size=0, offset=0
		} LongPointer; // size=4, offset=0, found in object files: debug.o, debug.o
		struct AreaObject areaObject; // size=12, offset=0
		struct InstanceObject instanceObject; // size=8, offset=0
		struct Vector3d vector3d; // size=20, offset=0
		struct Rotation3d rotation3d; // size=20, offset=0
		struct SignalObject signalObject; // size=8, offset=0
		struct EventObject eventObject; // size=8, offset=0
		struct GameObject gameObject; // size=4, offset=0
		struct ListObject listObject; // size=32, offset=0
		struct IntroObject introObject; // size=8, offset=0
		struct InstanceAnimate instanceAnimate; // size=8, offset=0
		struct InstanceAnimateTexture instanceAniTexture; // size=12, offset=0
		struct SavedEventObject savedEventObject; // size=16, offset=0
		struct SubListObject subListObject; // size=32, offset=0
		struct TGroupObject terrainGroup; // size=16, offset=0
		struct ConstrictInfo constrictInfo; // size=8, offset=0
		struct CameraObject cameraObject; // size=12, offset=0
		struct EventVmObject vmObject; // size=12, offset=0
		struct SoundObject soundObject; // size=20, offset=0
	} data; // size=32, offset=4, found in object files: debug.o, debug.o
};

struct __ReaverSaveDat // hashcode: 0x18BBFFA1 (dec: 414973857)
{
	unsigned long pad; // size=0, offset=0
};

struct GameCheat // hashcode: 0x9BFF36C0 (dec: -1677773120)
{
	unsigned char cheatKeys[12]; // size=12, offset=0
	short cheatLen; // size=0, offset=12
	short cheatStage; // size=0, offset=14
};

struct debug_dispatch_t // hashcode: 0x928EA8E4 (dec: -1836144412)
{
	enum DEBUG_LINE_TYPE type; // size=4, offset=0
	void (*fn)(); // size=0, offset=4
};

struct debug_format_t // hashcode: 0xCC4C6103 (dec: -867409661)
{
	int xpos; // size=0, offset=0
	int ypos; // size=0, offset=4
	int is_centered; // size=0, offset=8
};

struct SIGNAL_109fake // hashcode: 0xD12486F0 (dec: -786135312)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: signal.o, signal.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: signal.o, signal.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: signal.o, signal.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: signal.o, signal.o
};

struct _FXTracker // hashcode: 0xCC65A064 (dec: -865755036)
{
	struct _FX_MATRIX matrixPool[32]; // size=1408, offset=0
	struct NodeType usedMatrixList; // size=8, offset=1408
	struct NodeType freeMatrixList; // size=8, offset=1416
	struct _FX_PRIM primPool[256]; // size=26624, offset=1424
	struct NodeType usedPrimList; // size=8, offset=28048
	struct NodeType usedPrimListSprite; // size=8, offset=28056
	struct NodeType freePrimList; // size=8, offset=28064
};

struct SandHFScratch // hashcode: 0x31C4E20D (dec: 834986509)
{
	struct _SVector normal; // size=8, offset=0
	struct MATRIX posMatrix; // size=32, offset=8
	struct _Vector dpv; // size=12, offset=40
	struct _SVector triPoint; // size=8, offset=52
	struct _SVector planePoint; // size=8, offset=60
};

struct SandTScratch // hashcode: 0x2773448F (dec: 661865615)
{
	struct MATRIX posMatrix; // size=32, offset=0
	struct _Vector dpv; // size=12, offset=32
	struct _HNormal *normalList; // size=8, offset=44
	struct _CollideInfo collideInfo; // size=48, offset=48
	struct _HFaceInfo hfaceInfo; // size=32, offset=96
	struct _SVector midPoint; // size=8, offset=128
	struct _SVector spherePos; // size=8, offset=136
	void (*collideFunc)(); // size=0, offset=144
	struct _Instance *instance; // size=616, offset=148
	long edge; // size=0, offset=152
	long in_spectral; // size=0, offset=156
	struct _SVector oldPos; // size=8, offset=160
	void *prim; // size=0, offset=168
	struct _SVector normal; // size=8, offset=172
	struct _Sphere sphere; // size=12, offset=180
	short result; // size=0, offset=192
	short i; // size=0, offset=194
	long collide_ignoreAttr; // size=0, offset=196
	long collide_acceptAttr; // size=0, offset=200
	long midRadius; // size=0, offset=204
	struct _TVertex *vertexList; // size=12, offset=208
};

struct IandTScratch // hashcode: 0xE0F1425D (dec: -521059747)
{
	struct MATRIX posMatrix; // size=32, offset=0
	struct _SVector normal; // size=8, offset=32
	struct _Vector dpv; // size=12, offset=40
	struct _SVector planePoint; // size=8, offset=52
	struct _SVector line; // size=8, offset=60
	struct _SVector oldPos; // size=8, offset=68
	struct _SVector newPos; // size=8, offset=76
	int i; // size=0, offset=84
	long in_spectral; // size=0, offset=88
	short *normalList; // size=0, offset=92
	struct _TVertex *vertexList; // size=12, offset=96
};

struct PandTFScratch // hashcode: 0x218F0B73 (dec: 563022707)
{
	struct MATRIX posMatrix; // size=32, offset=0
	struct _SVector planePoint; // size=8, offset=32
	struct _SVector newPos; // size=8, offset=40
	struct _SVector oldPos; // size=8, offset=48
	struct _SVector normal; // size=8, offset=56
	struct _Vector dpv; // size=12, offset=64
};



struct POLY_GT3 // hashcode: 0x405DA246 (dec: 1079878214)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	unsigned short clut; // size=0, offset=14
	unsigned char r1; // size=0, offset=16
	unsigned char g1; // size=0, offset=17
	unsigned char b1; // size=0, offset=18
	unsigned char p1; // size=0, offset=19
	short x1; // size=0, offset=20
	short y1; // size=0, offset=22
	unsigned char u1; // size=0, offset=24
	unsigned char v1; // size=0, offset=25
	unsigned short tpage; // size=0, offset=26
	unsigned char r2; // size=0, offset=28
	unsigned char g2; // size=0, offset=29
	unsigned char b2; // size=0, offset=30
	unsigned char p2; // size=0, offset=31
	short x2; // size=0, offset=32
	short y2; // size=0, offset=34
	unsigned char u2; // size=0, offset=36
	unsigned char v2; // size=0, offset=37
	unsigned short pad2; // size=0, offset=38
};

struct POLY_GT4 // hashcode: 0x596BAA44 (dec: 1500228164)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	unsigned short clut; // size=0, offset=14
	unsigned char r1; // size=0, offset=16
	unsigned char g1; // size=0, offset=17
	unsigned char b1; // size=0, offset=18
	unsigned char p1; // size=0, offset=19
	short x1; // size=0, offset=20
	short y1; // size=0, offset=22
	unsigned char u1; // size=0, offset=24
	unsigned char v1; // size=0, offset=25
	unsigned short tpage; // size=0, offset=26
	unsigned char r2; // size=0, offset=28
	unsigned char g2; // size=0, offset=29
	unsigned char b2; // size=0, offset=30
	unsigned char p2; // size=0, offset=31
	short x2; // size=0, offset=32
	short y2; // size=0, offset=34
	unsigned char u2; // size=0, offset=36
	unsigned char v2; // size=0, offset=37
	unsigned short pad2; // size=0, offset=38
	unsigned char r3; // size=0, offset=40
	unsigned char g3; // size=0, offset=41
	unsigned char b3; // size=0, offset=42
	unsigned char p3; // size=0, offset=43
	short x3; // size=0, offset=44
	short y3; // size=0, offset=46
	unsigned char u3; // size=0, offset=48
	unsigned char v3; // size=0, offset=49
	unsigned short pad3; // size=0, offset=50
};

struct _PFace // hashcode: 0xD6E8EAC4 (dec: -689378620)
{
	unsigned long *tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	short clut; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	unsigned char u1; // size=0, offset=20
	unsigned char v1; // size=0, offset=21
	unsigned short tpage; // size=0, offset=22
	short x2; // size=0, offset=24
	short y2; // size=0, offset=26
	unsigned char u2; // size=0, offset=28
	unsigned char v2; // size=0, offset=29
	short pad1; // size=0, offset=30
	long pad2; // size=0, offset=32
	long pad3; // size=0, offset=36
};

struct _PolytopeList // hashcode: 0xAC7B9901 (dec: -1401186047)
{
	long numPolytopes; // size=0, offset=0
	void (*polytope[1024]); // size=4096, offset=4
};

struct DRAW_109fake // hashcode: 0x13B53E5F (dec: 330645087)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: draw.o, draw.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: draw.o, draw.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: draw.o, draw.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: draw.o, draw.o
};

struct UVTYPE // hashcode: 0x7D142F71 (dec: 2098474865)
{
	unsigned char u; // size=0, offset=0
	unsigned char v; // size=0, offset=1
	short pad; // size=0, offset=2
};

struct POLY_G3_SEMITRANS // hashcode: 0x323808D7 (dec: 842533079)
{
	unsigned long tag; // size=0, offset=0
	unsigned long dr_tpage; // size=0, offset=4
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
	short x0; // size=0, offset=12
	short y0; // size=0, offset=14
	unsigned char r1; // size=0, offset=16
	unsigned char g1; // size=0, offset=17
	unsigned char b1; // size=0, offset=18
	unsigned char pad1; // size=0, offset=19
	short x1; // size=0, offset=20
	short y1; // size=0, offset=22
	unsigned char r2; // size=0, offset=24
	unsigned char g2; // size=0, offset=25
	unsigned char b2; // size=0, offset=26
	unsigned char pad2; // size=0, offset=27
	short x2; // size=0, offset=28
	short y2; // size=0, offset=30
};

struct _POLY_NG4 // hashcode: 0x65B704BE (dec: 1706493118)
{
	unsigned long tag; // size=0, offset=0
	unsigned long drawTPage1; // size=0, offset=4
	struct _BASIC_G4 p1; // size=32, offset=8
};

struct _GT3ZVertexD3D_Type // hashcode: 0xBD55FA10 (dec: -1118438896)
{
	float x; // size=0, offset=0
	float y; // size=0, offset=4
	float z; // size=0, offset=8
	float rhw; // size=0, offset=12
	unsigned int rgba; // size=0, offset=16
	float u; // size=0, offset=20
	float v; // size=0, offset=24
};

struct SP_SUBDIV_STRUCT // hashcode: 0x4C4B55E8 (dec: 1280005608)
{
	struct _SVector vertex0; // size=8, offset=0
	struct _SVector vertex1; // size=8, offset=8
	struct _SVector vertex2; // size=8, offset=16
	SVECTORface_v0; // size=8, offset=24
	SVECTORface_v1; // size=8, offset=32
	SVECTORface_v2; // size=8, offset=40
	SVECTORface_v01; // size=8, offset=48
	SVECTORface_v12; // size=8, offset=56
	SVECTORface_v20; // size=8, offset=64
	struct UVTYPE face_uv0; // size=4, offset=72
	struct UVTYPE face_uv1; // size=4, offset=76
	struct UVTYPE face_uv2; // size=4, offset=80
	struct UVTYPE face_uv01; // size=4, offset=84
	struct UVTYPE face_uv12; // size=4, offset=88
	struct UVTYPE face_uv20; // size=4, offset=92
	struct _ColorType color0; // size=4, offset=96
	struct _ColorType color1; // size=4, offset=100
	struct _ColorType color2; // size=4, offset=104
	struct _ColorType color01; // size=4, offset=108
	struct _ColorType color12; // size=4, offset=112
	struct _ColorType color20; // size=4, offset=116
	struct _Vector out[4]; // size=48, offset=120
	long color[4]; // size=16, offset=168
	long texinfo[3]; // size=12, offset=184
	struct DVECTOR sxy[4]; // size=16, offset=196
};

struct LIGHT3D_109fake // hashcode: 0xCB7DE4CA (dec: -880941878)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: light3d.o, light3d.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: light3d.o, light3d.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: light3d.o, light3d.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: light3d.o, light3d.o
};

struct CdlFILE // hashcode: 0x2EE126E0 (dec: 786507488)
{
	struct CdlLOC pos; // size=4, offset=0
	unsigned long size; // size=0, offset=4
	char name[16]; // size=16, offset=8
};

struct LOAD3D_114fake // hashcode: 0x1C723D51 (dec: 477248849)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: load3d.o, load3d.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: load3d.o, load3d.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: load3d.o, load3d.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: load3d.o, load3d.o
};

struct DRAWENV // hashcode: 0x1D93A150 (dec: 496214352)
{
	struct PSX_RECT clip; // size=8, offset=0
	short ofs[2]; // size=4, offset=8
	struct PSX_RECT tw; // size=8, offset=12
	unsigned short tpage; // size=0, offset=20
	unsigned char dtd; // size=0, offset=22
	unsigned char dfe; // size=0, offset=23
	unsigned char isbg; // size=0, offset=24
	unsigned char r0; // size=0, offset=25
	unsigned char g0; // size=0, offset=26
	unsigned char b0; // size=0, offset=27
	struct DR_ENV dr_env; // size=64, offset=28
};

struct DISPENV // hashcode: 0x286FCE3D (dec: 678415933)
{
	struct PSX_RECT disp; // size=8, offset=0
	struct PSX_RECT screen; // size=8, offset=8
	unsigned char isinter; // size=0, offset=16
	unsigned char isrgb24; // size=0, offset=17
	unsigned char pad0; // size=0, offset=18
	unsigned char pad1; // size=0, offset=19
};

struct MAIN_114fake // hashcode: 0x6BBD5265 (dec: 1807569509)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: main.o, main.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: main.o, main.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: main.o, main.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: main.o, main.o
};

struct AnalogueData // hashcode: 0xD8EB0F12 (dec: -655683822)
{
	unsigned short pad; // size=0, offset=0
	unsigned char xR; // size=0, offset=2
	unsigned char yR; // size=0, offset=3
	unsigned char xL; // size=0, offset=4
	unsigned char yL; // size=0, offset=5
};

struct POLY_F4_SEMITRANS // hashcode: 0xE3D17AA2 (dec: -472810846)
{
	unsigned long tag; // size=0, offset=0
	unsigned long dr_tpage; // size=0, offset=4
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
	short x0; // size=0, offset=12
	short y0; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	short x2; // size=0, offset=20
	short y2; // size=0, offset=22
	short x3; // size=0, offset=24
	short y3; // size=0, offset=26
};

struct PhysObProperties // hashcode: 0xD5487B33 (dec: -716670157)
{
	unsigned short version; // size=0, offset=0
	unsigned short family; // size=0, offset=2
	unsigned short ID; // size=0, offset=4
	unsigned short Type; // size=0, offset=6
};

struct PhysObSplinter // hashcode: 0x933D3752 (dec: -1824704686)
{
	long numSplintData; // size=0, offset=0
	void *splinterData; // size=0, offset=4
};

struct PhysObWeaponAttributes // hashcode: 0x6EB0170A (dec: 1857033994)
{
	long Damage; // size=0, offset=0
	long AltDamage; // size=0, offset=4
	short knockBackDistance; // size=0, offset=8
	char knockBackFrames; // size=0, offset=10
	char dropSound; // size=0, offset=11
	char Class; // size=0, offset=12
	char ThrowSphere; // size=0, offset=13
	char LeftHandSphere; // size=0, offset=14
	char RightHandSphere; // size=0, offset=15
	struct _PhysObLight *Light; // size=0, offset=16
	struct PhysObSplinter splinter; // size=8, offset=20
};

struct _PhysObLight // hashcode: 0x873DE727 (dec: -2025986265)
{
	unsigned char length; // size=0, offset=0
	unsigned char segment; // size=0, offset=1
	short speed; // size=0, offset=2
	struct LightTableEntry *lightTable; // size=8, offset=4
};

struct __PhysObProjectileData // hashcode: 0x12820E13 (dec: 310513171)
{
	char model; // size=0, offset=0
	char startAnim; // size=0, offset=1
	char loopAnim; // size=0, offset=2
	char endAnim; // size=0, offset=3
	long flags; // size=0, offset=4
	struct PhysObWeaponAttributes *weapon; // size=28, offset=8
};

struct SwitchData // hashcode: 0x34EE19D8 (dec: 888019416)
{
	short state; // size=0, offset=0
	short accumulator; // size=0, offset=2
};

struct LightTableEntry // hashcode: 0x952494E4 (dec: -1792764700)
{
	short r; // size=0, offset=0
	short g; // size=0, offset=2
	short b; // size=0, offset=4
	short radius; // size=0, offset=6
};

struct POLY_FT4 // hashcode: 0xAD2AA0E4 (dec: -1389715228)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	unsigned short clut; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	unsigned char u1; // size=0, offset=20
	unsigned char v1; // size=0, offset=21
	unsigned short tpage; // size=0, offset=22
	short x2; // size=0, offset=24
	short y2; // size=0, offset=26
	unsigned char u2; // size=0, offset=28
	unsigned char v2; // size=0, offset=29
	unsigned short pad1; // size=0, offset=30
	short x3; // size=0, offset=32
	short y3; // size=0, offset=34
	unsigned char u3; // size=0, offset=36
	unsigned char v3; // size=0, offset=37
	unsigned short pad2; // size=0, offset=38
};

struct FontTracker // hashcode: 0xE32D3F0A (dec: -483574006)
{
	struct FontChar font_buffer[256]; // size=1536, offset=0
	long font_xpos; // size=0, offset=1536
	long font_ypos; // size=0, offset=1540
	long font_buffIndex; // size=0, offset=1544
	short font_tpage; // size=0, offset=1548
	short font_clut; // size=0, offset=1550
	short font_vramU; // size=0, offset=1552
	short font_vramV; // size=0, offset=1554
	short font_width; // size=0, offset=1556
	short sprite_sort_push; // size=0, offset=1558
	short font_vramX; // size=0, offset=1560
	short font_vramY; // size=0, offset=1562
	char color_global; // size=0, offset=1564
	char color_local; // size=0, offset=1565
};

struct FONT_118fake // hashcode: 0x5664C5CE (dec: 1449444814)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: font.o, font.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: font.o, font.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: font.o, font.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: font.o, font.o
};

struct font_color_t // hashcode: 0x5F515B79 (dec: 1599167353)
{
	int r; // size=0, offset=0
	int g; // size=0, offset=4
	int b; // size=0, offset=8
};

struct FontPos // hashcode: 0x0084D7DD (dec: 8706013)
{
	char x; // size=0, offset=0
	char y; // size=0, offset=1
	char w; // size=0, offset=2
	char h; // size=0, offset=3
};

struct StreamUnitPortal // hashcode: 0x7C52D8D9 (dec: 2085804249)
{
	char tolevelname[16]; // size=16, offset=0
	long MSignalID; // size=0, offset=16
	long streamID; // size=0, offset=20
	short minx; // size=0, offset=24
	short miny; // size=0, offset=26
	short minz; // size=0, offset=28
	short flags; // size=0, offset=30
	short maxx; // size=0, offset=32
	short maxy; // size=0, offset=34
	short maxz; // size=0, offset=36
	short pad2; // size=0, offset=38
	struct _StreamUnit *toStreamUnit; // size=64, offset=40
	struct _SVector t1[3]; // size=24, offset=44
	struct _SVector t2[3]; // size=24, offset=68
};

struct GAMEPAD_114fake // hashcode: 0xA495E8C3 (dec: -1533679421)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: gamepad.o, gamepad.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: gamepad.o, gamepad.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: gamepad.o, gamepad.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: gamepad.o, gamepad.o
};

struct JoypadMappingStruct // hashcode: 0x312F2BE8 (dec: 825175016)
{
	unsigned short psxData; // size=0, offset=0
	unsigned short translation; // size=0, offset=2
};

struct _AnimSoundData_Type // hashcode: 0x5367FF66 (dec: 1399324518)
{
	short sfxToneID; // size=0, offset=0
	short volume; // size=0, offset=2
	short pitch; // size=0, offset=4
	short minVolDistance; // size=0, offset=6
};

struct _InstanceHandle // hashcode: 0xDC7D3685 (dec: -595773819)
{
	struct _Instance *instance; // size=616, offset=0
	long instanceID; // size=0, offset=4
};

struct evCollideInstanceStatsData // hashcode: 0xD8198216 (dec: -669416938)
{
	struct _Instance *instance; // size=616, offset=0
	unsigned long distance; // size=0, offset=4
	struct _SVector relativePosition; // size=8, offset=8
	long xyDistance; // size=0, offset=16
	long zDelta; // size=0, offset=20
};

struct INICommand // hashcode: 0x04B02DE1 (dec: 78654945)
{
	short command; // size=0, offset=0
	short numParameters; // size=0, offset=2
	long parameter[0]; // size=0, offset=4
};

struct SavedIntroSmall // hashcode: 0x42C7682C (dec: 1120364588)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short introUniqueID; // size=0, offset=2
};

struct SavedIntroSpline // hashcode: 0x018B74DF (dec: 25916639)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short introUniqueID; // size=0, offset=2
	short splineFlags; // size=0, offset=4
	short splineKeyFrame; // size=0, offset=6
	short splineClipBeg; // size=0, offset=8
	short splineClipEnd; // size=0, offset=10
};

struct _SavedIntroWithIntro // hashcode: 0xF234DCD6 (dec: -231416618)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	unsigned short attachedUniqueID; // size=0, offset=2
	short introOffset; // size=0, offset=4
	short birthUnitID; // size=0, offset=6
	short introUniqueID; // size=0, offset=8
	unsigned char lightGroup; // size=0, offset=10
	unsigned char specturalLightGroup; // size=0, offset=11
	long flags; // size=0, offset=12
	long flags2; // size=0, offset=16
	struct _SmallRotation smallRotation; // size=6, offset=20
	struct _Position position; // size=6, offset=26
};

struct _MonsterAnim // hashcode: 0xF5F8CA4B (dec: -168244661)
{
	unsigned short velocity; // size=0, offset=0
	unsigned short distance; // size=0, offset=2
	unsigned short playSpeed; // size=0, offset=4
	unsigned char interpFrames; // size=0, offset=6
	unsigned char startFrame; // size=0, offset=7
	unsigned char interpOut; // size=0, offset=8
	unsigned char alphaTable; // size=0, offset=9
	unsigned char alphaTableOut; // size=0, offset=10
	unsigned char controllingSection; // size=0, offset=11
	char index[3]; // size=3, offset=12
	char pad; // size=0, offset=15
};

struct _MonsterIdle // hashcode: 0x181712FA (dec: 404165370)
{
	char anim; // size=0, offset=0
	char alertMod; // size=0, offset=1
	char probability; // size=0, offset=2
	char pad; // size=0, offset=3
};

struct _MonsterBehavior // hashcode: 0x6D7D1660 (dec: 1836914272)
{
	char alertness; // size=0, offset=0
	char idleFreq; // size=0, offset=1
	char numIdles; // size=0, offset=2
	char idleList[5]; // size=5, offset=3
};

struct _MonsterSenses // hashcode: 0x1AA0E325 (dec: 446751525)
{
	short sightArc; // size=0, offset=0
	short sightRadius; // size=0, offset=2
	short sightElevation; // size=0, offset=4
	short soundArc; // size=0, offset=6
	short soundRadius; // size=0, offset=8
	short soundElevation; // size=0, offset=10
	unsigned short scentRadius; // size=0, offset=12
	short alarmRadius; // size=0, offset=14
	unsigned short allyRadius; // size=0, offset=16
};

struct _MonsterAllegiances // hashcode: 0x1F15B182 (dec: 521515394)
{
	unsigned long enemies; // size=0, offset=0
	unsigned long allies; // size=0, offset=4
	unsigned long food; // size=0, offset=8
	unsigned long gods; // size=0, offset=12
};

struct _MonsterAttackAttributes // hashcode: 0x4F8BA493 (dec: 1334551699)
{
	short attackRange; // size=0, offset=0
	short attackHeight; // size=0, offset=2
	short knockBackDistance; // size=0, offset=4
	char knockBackDuration; // size=0, offset=6
	char damage; // size=0, offset=7
	char sphereSegment; // size=0, offset=8
	char sphereOnFrame; // size=0, offset=9
	char sphereOnAnim; // size=0, offset=10
	char sphereOffFrame; // size=0, offset=11
	char sphereOffAnim; // size=0, offset=12
	char turnFrames; // size=0, offset=13
	char numAnims; // size=0, offset=14
	char attackProbability[12]; // size=12, offset=15
	char animList[5]; // size=5, offset=27
};

struct _MonsterMissile // hashcode: 0xC37A9403 (dec: -1015376893)
{
	unsigned short speed; // size=0, offset=0
	unsigned short range; // size=0, offset=2
	unsigned char frame; // size=0, offset=4
	unsigned char anim; // size=0, offset=5
	unsigned char segment; // size=0, offset=6
	unsigned char damage; // size=0, offset=7
	unsigned char type; // size=0, offset=8
	unsigned char graphic; // size=0, offset=9
	char gravity; // size=0, offset=10
	char reload; // size=0, offset=11
	char fireChance; // size=0, offset=12
	unsigned char numAnims; // size=0, offset=13
	char animList[2]; // size=2, offset=14
};

struct _MonsterCombatAttributes // hashcode: 0x1CD3787B (dec: 483620987)
{
	short stunTime; // size=0, offset=0
	short damageTime; // size=0, offset=2
	short recovery; // size=0, offset=4
	short surpriseTime; // size=0, offset=6
	short grabTime; // size=0, offset=8
	short combatRange; // size=0, offset=10
	short surpriseRange; // size=0, offset=12
	short allyRange; // size=0, offset=14
	short enemyAttackRange; // size=0, offset=16
	short enemyRunAttackRange; // size=0, offset=18
	short preferredCombatRange; // size=0, offset=20
	short suckPower; // size=0, offset=22
	short suckRange; // size=0, offset=24
	short suckTime; // size=0, offset=26
	char hitPoints; // size=0, offset=28
	char numAttacks; // size=0, offset=29
	char missileAttack; // size=0, offset=30
	char ambushAttack; // size=0, offset=31
	char evadeProbability[12]; // size=12, offset=32
	char attackList[1]; // size=1, offset=44
};

struct _MonsterSubAttributes // hashcode: 0xD249D756 (dec: -766912682)
{
	char *animList; // size=0, offset=0
	struct _MonsterSenses *senses; // size=18, offset=4
	struct _MonsterCombatAttributes *combatAttributes; // size=46, offset=8
	struct _MonsterAllegiances *allegiances; // size=16, offset=12
	char *behaviorList; // size=0, offset=16
	short scale; // size=0, offset=20
	short fallDistance; // size=0, offset=22
	short defAmbushRange; // size=0, offset=24
	short fleeRange; // size=0, offset=26
	short speedPivotTurn; // size=0, offset=28
	short speedWalkTurn; // size=0, offset=30
	short speedRunTurn; // size=0, offset=32
	short speedFleeTurn; // size=0, offset=34
	short forgetTime; // size=0, offset=36
	unsigned char modelNum; // size=0, offset=38
	unsigned char physAbility; // size=0, offset=39
	unsigned char stunnable; // size=0, offset=40
	unsigned char grabable; // size=0, offset=41
	unsigned char numSections; // size=0, offset=42
	unsigned char sectionEnd[3]; // size=3, offset=43
	short defPlayerAttitude; // size=0, offset=46
	short defWanderRange; // size=0, offset=48
	short defGuardRange; // size=0, offset=50
	unsigned char defInitialBehavior; // size=0, offset=52
	unsigned char defTriggeredBehavior; // size=0, offset=53
	char defActiveBehavior; // size=0, offset=54
	unsigned char defSpectral; // size=0, offset=55
	unsigned char sunVuln; // size=0, offset=56
	unsigned char fireVuln; // size=0, offset=57
	unsigned char waterVuln; // size=0, offset=58
	unsigned char impaleVuln; // size=0, offset=59
	unsigned char soundVuln; // size=0, offset=60
	unsigned char weaponVuln; // size=0, offset=61
	unsigned char missileVuln; // size=0, offset=62
	unsigned char bruiseRed; // size=0, offset=63
	unsigned char bruiseGreen; // size=0, offset=64
	unsigned char bruiseBlue; // size=0, offset=65
	short minSpikeRange; // size=0, offset=66
	short maxSpikeRange; // size=0, offset=68
	short maxSpikeAngle; // size=0, offset=70
	short minSpikeHorzSpeed; // size=0, offset=72
	short maxSpikeHorzSpeed; // size=0, offset=74
	short maxSpikeVertSpeed; // size=0, offset=76
	short upOnGroundOffset; // size=0, offset=78
	short downOnGroundOffset; // size=0, offset=80
};

struct SOUND_114fake // hashcode: 0xEE28F284 (dec: -299306364)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: sound.o, sound.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: sound.o, sound.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: sound.o, sound.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: sound.o, sound.o
};

struct AadInitAttr // hashcode: 0x0549E29A (dec: 88728218)
{
	int updateMode; // size=0, offset=0
	int numSlots; // size=0, offset=4
	void (*nonBlockLoadProc)(); // size=0, offset=8
	void (*nonBlockBufferedLoadProc)(); // size=0, offset=12
	void (**memoryMallocProc)(); // size=0, offset=16
	void (*memoryFreeProc)(); // size=0, offset=20
};

struct ObjectPeriodicSound // hashcode: 0x931DBADB (dec: -1826768165)
{
	unsigned char type; // size=0, offset=0
	unsigned char numSfxIDs; // size=0, offset=1
	unsigned char soundInst; // size=0, offset=2
	unsigned char flags; // size=0, offset=3
	unsigned short minVolDistance; // size=0, offset=4
	short pitch; // size=0, offset=6
	unsigned short pitchVariation; // size=0, offset=8
	unsigned char maxVolume; // size=0, offset=10
	unsigned char maxVolVariation; // size=0, offset=11
	unsigned char initialDelay; // size=0, offset=12
	unsigned char initialDelayVariation; // size=0, offset=13
	unsigned char onTime; // size=0, offset=14
	unsigned char onTimeVariation; // size=0, offset=15
	unsigned char offTime; // size=0, offset=16
	unsigned char offTimeVariation; // size=0, offset=17
};

struct ObjectEventSound // hashcode: 0x02927CD2 (dec: 43154642)
{
	unsigned char type; // size=0, offset=0
	unsigned char numSfxIDs; // size=0, offset=1
	unsigned char soundInst; // size=0, offset=2
	unsigned char flags; // size=0, offset=3
	unsigned short minVolDistance; // size=0, offset=4
	short pitch; // size=0, offset=6
	unsigned short pitchVariation; // size=0, offset=8
	unsigned char maxVolume; // size=0, offset=10
	unsigned char maxVolVariation; // size=0, offset=11
	unsigned char initialDelay; // size=0, offset=12
	unsigned char initialDelayVariation; // size=0, offset=13
};

struct ObjectOneShotSound // hashcode: 0xF16FD213 (dec: -244329965)
{
	unsigned char type; // size=0, offset=0
	unsigned char numSfxIDs; // size=0, offset=1
	unsigned char soundInst; // size=0, offset=2
	unsigned char flags; // size=0, offset=3
	unsigned short minVolDistance; // size=0, offset=4
	short pitch; // size=0, offset=6
	unsigned short pitchVariation; // size=0, offset=8
	unsigned char maxVolume; // size=0, offset=10
	unsigned char maxVolVariation; // size=0, offset=11
	unsigned char initialDelay; // size=0, offset=12
	unsigned char initialDelayVariation; // size=0, offset=13
};

struct SoundEffectChannel // hashcode: 0xA19502FD (dec: -1584069891)
{
	unsigned char inUse; // size=0, offset=0
	unsigned char unused; // size=0, offset=1
	unsigned short unused1; // size=0, offset=2
	unsigned long handle; // size=0, offset=4
	short volume; // size=0, offset=8
	short volumeChangeTime; // size=0, offset=10
	short volumeChangeTimeSave; // size=0, offset=12
	short volumeChangePerUpdate; // size=0, offset=14
	short volumeChangeSign; // size=0, offset=16
	short volumeChangeErrPerUpdate; // size=0, offset=18
	short volumeChangeError; // size=0, offset=20
	short pitch; // size=0, offset=22
	short pitchChangeTime; // size=0, offset=24
	short pitchChangeTimeSave; // size=0, offset=26
	short pitchChangePerUpdate; // size=0, offset=28
	short pitchChangeSign; // size=0, offset=30
	short pitchChangeErrPerUpdate; // size=0, offset=32
	short pitchChangeError; // size=0, offset=34
};

struct _SBSPLeaf // hashcode: 0xAD21B7BA (dec: -1390299206)
{
	struct _Sphere_noSq sphere; // size=8, offset=0
	struct Intro **introList; // size=76, offset=8
	short numIntros; // size=0, offset=12
	short flags; // size=0, offset=14
	struct CDLight **lightList; // size=24, offset=16
	short numLights; // size=0, offset=20
	short pad; // size=0, offset=22
};

struct BSP_109fake // hashcode: 0xF4A997D6 (dec: -190212138)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: bsp.o, bsp.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: bsp.o, bsp.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: bsp.o, bsp.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: bsp.o, bsp.o
};

struct BSPScratch // hashcode: 0x17CFF45B (dec: 399504475)
{
	struct MATRIX vv[2]; // size=64, offset=0
	struct MATRIX camMatrix; // size=32, offset=64
	struct VECTOR dpv[3]; // size=48, offset=96
	long d[6]; // size=24, offset=144
	struct MATRIX boundMatrix; // size=32, offset=168
	long in_spectral; // size=0, offset=200
};

struct _ObjectOwnerInfo // hashcode: 0x04BCED5C (dec: 79490396)
{
	long magicnum; // size=0, offset=0
	short animID; // size=0, offset=4
	char objectName[0]; // size=0, offset=6
};

struct OBTABLE_114fake // hashcode: 0xA297FC58 (dec: -1567097768)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: obtable.o, obtable.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: obtable.o, obtable.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: obtable.o, obtable.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: obtable.o, obtable.o
};

struct ObjectAccess // hashcode: 0x1268C513 (dec: 308856083)
{
	char *objectName; // size=0, offset=0
	void *object; // size=0, offset=4
};

struct OBTABLE2_114fake // hashcode: 0xAA6A3EB1 (dec: -1435877711)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: obtable2.o, obtable2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: obtable2.o, obtable2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: obtable2.o, obtable2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: obtable2.o, obtable2.o
};

struct ObjectFunc // hashcode: 0xFDE6E2DA (dec: -35200294)
{
	char *scriptName; // size=0, offset=0
	void (*initFunc)(); // size=0, offset=4
	void (*processFunc)(); // size=0, offset=8
	void (*collideFunc)(); // size=0, offset=12
	unsigned long (*queryFunc)(); // size=0, offset=16
	void (*messageFunc)(); // size=0, offset=20
	void (*additionalCollideFunc)(); // size=0, offset=24
	void (*relocateTuneFunc)(); // size=0, offset=28
	void (*relocateInstObFunc)(); // size=0, offset=32
};

struct GENERIC_114fake // hashcode: 0xDFE61644 (dec: -538569148)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: generic.o, generic.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: generic.o, generic.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: generic.o, generic.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: generic.o, generic.o
};

struct evAnimationInstanceSwitchData // hashcode: 0xEE7F2A6C (dec: -293655956)
{
	struct _Instance *instance; // size=616, offset=0
	int anim; // size=0, offset=4
	int frame; // size=0, offset=8
	int frames; // size=0, offset=12
	int mode; // size=0, offset=16
};

struct evControlSaveDataData // hashcode: 0xA76A68A9 (dec: -1486198615)
{
	long length; // size=0, offset=0
	void *data; // size=0, offset=4
};

struct GenericTune // hashcode: 0x1B714C34 (dec: 460409908)
{
	long flags; // size=0, offset=0
	void *shatterData; // size=0, offset=4
};

struct POLY_FT3 // hashcode: 0x1FB59984 (dec: 531995012)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	unsigned short clut; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	unsigned char u1; // size=0, offset=20
	unsigned char v1; // size=0, offset=21
	unsigned short tpage; // size=0, offset=22
	short x2; // size=0, offset=24
	short y2; // size=0, offset=26
	unsigned char u2; // size=0, offset=28
	unsigned char v2; // size=0, offset=29
	unsigned short pad1; // size=0, offset=30
};

struct _Mirror // hashcode: 0x6161DBD4 (dec: 1633803220)
{
	short a; // size=0, offset=0
	short b; // size=0, offset=2
	short c; // size=0, offset=4
	short d; // size=0, offset=6
};

struct PIPE3D_109fake // hashcode: 0xEADE4BF5 (dec: -354530315)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: pipe3d.o, pipe3d.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: pipe3d.o, pipe3d.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: pipe3d.o, pipe3d.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: pipe3d.o, pipe3d.o
};

struct _FXHalvePlane // hashcode: 0x10EBCD5A (dec: 283888986)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	long diffTime; // size=0, offset=16
	struct _PlaneConstants ringPlane; // size=12, offset=20
	long currentColor; // size=0, offset=32
	long *colorArray; // size=0, offset=36
	short numColors; // size=0, offset=40
	short colorBlendLife; // size=0, offset=42
};

struct _FXGlowEffect // hashcode: 0xCE07D6ED (dec: -838347027)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	unsigned long diffTime; // size=0, offset=16
	long *colorArray; // size=0, offset=20
	long currentColor; // size=0, offset=24
	short numColors; // size=0, offset=28
	short colorBlendCycle; // size=0, offset=30
	short width; // size=0, offset=32
	short height; // size=0, offset=34
	unsigned char numSegments; // size=0, offset=36
	unsigned char SegmentInc; // size=0, offset=37
	short segment; // size=0, offset=38
	short fadein_time; // size=0, offset=40
	short fadeout_time; // size=0, offset=42
};

struct PIPE3D_239fake // hashcode: 0x11DBF281 (dec: 299627137)
{
	long m[3]; // size=12, offset=0
};

struct PIPE3D_240fake // hashcode: 0xB73CD4A8 (dec: -1220750168)
{
	long m[5]; // size=20, offset=0
};

struct TransformAnimatedInstanceVerticesWork_t // hashcode: 0x7584B74F (dec: 1971631951)
{
	struct MATRIX scTransform; // size=32, offset=0
	struct MATRIX mirrorMatrix; // size=32, offset=32
	struct MATRIX llm; // size=32, offset=64
	struct _Vector vector; // size=12, offset=96
};

struct SCRIPT_114fake // hashcode: 0x918B5BD9 (dec: -1853137959)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: script.o, script.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: script.o, script.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: script.o, script.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: script.o, script.o
};

struct FXSplinter // hashcode: 0x3E763456 (dec: 1047934038)
{
	short flags; // size=0, offset=0
	short soundFx; // size=0, offset=2
	short chunkVelXY; // size=0, offset=4
	short chunkVelZ; // size=0, offset=6
	short chunkVelRng; // size=0, offset=8
	short triVelRng; // size=0, offset=10
	short lifetime; // size=0, offset=12
	short faceLimit; // size=0, offset=14
	short rotRateRng; // size=0, offset=16
	short gravityZ; // size=0, offset=18
};

struct TIMER_109fake // hashcode: 0x38B8D1E3 (dec: 951636451)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: timer.o, timer.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: timer.o, timer.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: timer.o, timer.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: timer.o, timer.o
};

struct DR_TPAGE // hashcode: 0xA93EABEF (dec: -1455510545)
{
	unsigned long tag; // size=0, offset=0
	unsigned long code[1]; // size=4, offset=4
};

struct FX_109fake // hashcode: 0x76D46C98 (dec: 1993632920)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: fx.o, fx.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: fx.o, fx.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: fx.o, fx.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: fx.o, fx.o
};

struct _POLY_SG4 // hashcode: 0xA49EA007 (dec: -1533108217)
{
	unsigned long tag; // size=0, offset=0
	unsigned long drawTPage1; // size=0, offset=4
	struct _BASIC_G4 p1; // size=32, offset=8
	unsigned long drawTPage2; // size=0, offset=40
	struct _BASIC_F4 p2; // size=20, offset=44
};

struct _FXGeneralEffect // hashcode: 0xC4783451 (dec: -998755247)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
};

struct _FXRibbon // hashcode: 0xB11070B8 (dec: -1324322632)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	short numberVerts; // size=0, offset=16
	short endIndex; // size=0, offset=18
	SVECTOR*vertexPool; // size=8, offset=20
	short startSegment; // size=0, offset=24
	short endSegment; // size=0, offset=26
	long startColor; // size=0, offset=28
	long endColor; // size=0, offset=32
	short colorStepValue; // size=0, offset=36
	short faceLifeTime; // size=0, offset=38
	short startFadeValue; // size=0, offset=40
	short fadeStep; // size=0, offset=42
};

struct _FXParticle // hashcode: 0xDD6F143E (dec: -579922882)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	struct TextureMT3 *texture; // size=16, offset=16
	void *fxprim_modify_process; // size=0, offset=20
	void *fxprim_process; // size=0, offset=24
	short size; // size=0, offset=28
	short primLifeTime; // size=0, offset=30
	short birthRadius; // size=0, offset=32
	short scaleSpeed; // size=0, offset=34
	struct _Position direction; // size=6, offset=36
	unsigned short flag_bits; // size=0, offset=42
	struct _Position acceleration; // size=6, offset=44
	short startScale; // size=0, offset=50
	struct _Position offset; // size=6, offset=52
	char startSegment; // size=0, offset=58
	char z_undulate; // size=0, offset=59
	long startColor; // size=0, offset=60
	long endColor; // size=0, offset=64
	short colorStepValue; // size=0, offset=68
	char numberBirthParticles; // size=0, offset=70
	char endSegment; // size=0, offset=71
	short startFadeValue; // size=0, offset=72
	short fadeStep; // size=0, offset=74
};

struct _FXLightBeam // hashcode: 0xDC9CA53F (dec: -593713857)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	short startSeg; // size=0, offset=16
	short endSeg; // size=0, offset=18
	long color; // size=0, offset=20
};

struct _FXBlastringEffect // hashcode: 0x2F8F0E55 (dec: 797904469)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	struct _SVector position; // size=8, offset=16
	struct MATRIX matrix; // size=32, offset=24
	short segment; // size=0, offset=56
	short pad; // size=0, offset=58
	int radius; // size=0, offset=60
	int size1; // size=0, offset=64
	int size2; // size=0, offset=68
	int startRadius; // size=0, offset=72
	int endRadius; // size=0, offset=76
	int colorchange_radius; // size=0, offset=80
	int vel; // size=0, offset=84
	int accl; // size=0, offset=88
	int height1; // size=0, offset=92
	int height2; // size=0, offset=96
	int height3; // size=0, offset=100
	char predator_offset; // size=0, offset=104
	char predator_offset_orig; // size=0, offset=105
	char stay_in_place; // size=0, offset=106
	char sortInWorld; // size=0, offset=107
	long color; // size=0, offset=108
	long startColor; // size=0, offset=112
	long endColor; // size=0, offset=116
};

struct _FXForceFieldEffect // hashcode: 0x59105BC4 (dec: 1494244292)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	struct _SVector offset; // size=8, offset=16
	short size; // size=0, offset=24
	short size_diff; // size=0, offset=26
	short size_change; // size=0, offset=28
	short deg; // size=0, offset=30
	short deg_change; // size=0, offset=32
	short start_fade; // size=0, offset=34
	short end_fade; // size=0, offset=36
	long color; // size=0, offset=40
};

struct _FXLightning // hashcode: 0x552145A3 (dec: 1428243875)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	short deg; // size=0, offset=16
	short deg_inc; // size=0, offset=18
	struct _Position start_offset; // size=6, offset=20
	short startSeg; // size=0, offset=26
	struct _Instance *end_instance; // size=616, offset=28
	struct _Position end_offset; // size=6, offset=32
	short endSeg; // size=0, offset=38
	short matrixSeg; // size=0, offset=40
	short width; // size=0, offset=42
	short small_width; // size=0, offset=44
	short segs; // size=0, offset=46
	short sine_size; // size=0, offset=48
	short variation; // size=0, offset=50
	long color; // size=0, offset=52
	long glow_color; // size=0, offset=56
};

struct _FXFlash // hashcode: 0xD3DE17A3 (dec: -740419677)
{
	void *next; // size=0, offset=0
	void *continue_process; // size=0, offset=4
	struct _Instance *instance; // size=616, offset=8
	unsigned char effectType; // size=0, offset=12
	unsigned char type; // size=0, offset=13
	short lifeTime; // size=0, offset=14
	long timeToColor; // size=0, offset=16
	long timeAtColor; // size=0, offset=20
	long timeFromColor; // size=0, offset=24
	long currentTime; // size=0, offset=28
	long color; // size=0, offset=32
};

struct GenericFXObject // hashcode: 0xD22FD65C (dec: -768616868)
{
	struct _GenericParticleParams *ParticleList; // size=48, offset=0
	struct _GenericRibbonParams *RibbonList; // size=20, offset=4
	struct _GenericGlowParams *GlowList; // size=24, offset=8
	struct _GenericLightningParams *LightningList; // size=40, offset=12
	struct _GenericBlastringParams *BlastList; // size=48, offset=16
	struct _GenericFlashParams *FlashList; // size=12, offset=20
	long *ColorList; // size=0, offset=24
};

struct evObjectDraftData // hashcode: 0x73C6643E (dec: 1942381630)
{
	short force; // size=0, offset=0
	unsigned short radius; // size=0, offset=2
	unsigned short radiusCoef; // size=0, offset=4
	unsigned short height; // size=0, offset=6
	unsigned short maxVelocity; // size=0, offset=8
};

struct _GlyphTuneData // hashcode: 0x14FFD660 (dec: 352310880)
{
	short glyph_size; // size=0, offset=0
	short glyph_darkness; // size=0, offset=2
	char glyph_costs[8]; // size=8, offset=4
	short glyph_range[8]; // size=16, offset=12
	long color_array[28]; // size=112, offset=28
};

struct __ReaverData // hashcode: 0xD1EA4FE0 (dec: -773173280)
{
	char ReaverPickedUp; // size=0, offset=0
	char ReaverOn; // size=0, offset=1
	short ReaverSize; // size=0, offset=2
	short CurrentReaver; // size=0, offset=4
	short ReaverDeg; // size=0, offset=6
	long ReaverChargeTime; // size=0, offset=8
	long ReaverShockAmount; // size=0, offset=12
	long ReaverGlowColor; // size=0, offset=16
	long ReaverBladeColor; // size=0, offset=20
	long ReaverBladeGlowColor; // size=0, offset=24
	short ReaverScale; // size=0, offset=28
	short ReaverTargetScale; // size=0, offset=30
	struct __ReaverSaveDat saveData; // size=4, offset=32
};

struct __ReaverTuneData // hashcode: 0xE4E74D43 (dec: -454603453)
{
	unsigned long spectralGlowColor; // size=0, offset=0
	unsigned long materialGlowColor; // size=0, offset=4
	unsigned long sunlightGlowColor; // size=0, offset=8
	unsigned long waterGlowColor; // size=0, offset=12
	unsigned long stoneGlowColor; // size=0, offset=16
	unsigned long fireGlowColor; // size=0, offset=20
	unsigned long spiritGlowColor; // size=0, offset=24
	unsigned long soundGlowColor; // size=0, offset=28
	unsigned long spectralInnerColor; // size=0, offset=32
	unsigned long materialInnerColor; // size=0, offset=36
	unsigned long sunlightInnerColor; // size=0, offset=40
	unsigned long waterInnerColor; // size=0, offset=44
	unsigned long stoneInnerColor; // size=0, offset=48
	unsigned long fireInnerColor; // size=0, offset=52
	unsigned long spiritInnerColor; // size=0, offset=56
	unsigned long soundInnerColor; // size=0, offset=60
	unsigned long spectralInnerGlowColor; // size=0, offset=64
	unsigned long materialInnerGlowColor; // size=0, offset=68
	unsigned long sunlightInnerGlowColor; // size=0, offset=72
	unsigned long waterInnerGlowColor; // size=0, offset=76
	unsigned long stoneInnerGlowColor; // size=0, offset=80
	unsigned long fireInnerGlowColor; // size=0, offset=84
	unsigned long spiritInnerGlowColor; // size=0, offset=88
	unsigned long soundInnerGlowColor; // size=0, offset=92
	unsigned long icon_colors[24]; // size=96, offset=96
};

struct _POLY_TF4 // hashcode: 0x871FC08E (dec: -2027962226)
{
	unsigned long tag; // size=0, offset=0
	unsigned long drawTPage; // size=0, offset=4
	struct _BASIC_F4 p1; // size=20, offset=8
};

struct _POLY_2G4 // hashcode: 0x3FC1C531 (dec: 1069663537)
{
	unsigned long tag; // size=0, offset=0
	struct _BASIC_G4 p1; // size=32, offset=4
	struct _BASIC_G4 p2; // size=32, offset=36
};

struct _POLY_2G4T // hashcode: 0xA357E01D (dec: -1554522083)
{
	unsigned long tag; // size=0, offset=0
	unsigned long drawTPage1; // size=0, offset=4
	struct _BASIC_G4 p1; // size=32, offset=8
	struct _BASIC_G4 p2; // size=32, offset=40
};

struct VM_109fake // hashcode: 0x94EF070A (dec: -1796274422)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: vm.o, vm.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: vm.o, vm.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: vm.o, vm.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: vm.o, vm.o
};

struct AadVolume // hashcode: 0xDE43B1D6 (dec: -565988906)
{
	short left; // size=0, offset=0
	short right; // size=0, offset=2
};

struct AadSequenceHdr // hashcode: 0x09B09BCF (dec: 162569167)
{
	unsigned long sequenceID; // size=0, offset=0
	unsigned long quarterNoteTime; // size=0, offset=4
	unsigned short ppqn; // size=0, offset=8
	unsigned short version; // size=0, offset=10
	unsigned short numTracks; // size=0, offset=12
	unsigned short unused; // size=0, offset=14
};

struct AadTempo // hashcode: 0x0AB3CE4B (dec: 179555915)
{
	unsigned long quarterNoteTime; // size=0, offset=0
	int ppqn; // size=0, offset=4
}

struct AadSlotAttributes // hashcode: 0x7DE97E8C (dec: 2112454284)
{
	int sequenceNumberAssigned; // size=0, offset=0
	int sequenceAssignedDynamicBank; // size=0, offset=4
	int playingStatus; // size=0, offset=8
	int channelMute; // size=0, offset=12
	int slotVolume; // size=0, offset=16
	int slotPan; // size=0, offset=20
	int eventsInQueue[16]; // size=64, offset=24
};

struct AADLIB_153fake // hashcode: 0x95DB25F7 (dec: -1780800009)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: aadlib.o, aadlib.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: aadlib.o, aadlib.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: aadlib.o, aadlib.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: aadlib.o, aadlib.o
};

struct POLY_G4 // hashcode: 0x82B947DC (dec: -2101786660)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char r1; // size=0, offset=12
	unsigned char g1; // size=0, offset=13
	unsigned char b1; // size=0, offset=14
	unsigned char pad1; // size=0, offset=15
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	unsigned char r2; // size=0, offset=20
	unsigned char g2; // size=0, offset=21
	unsigned char b2; // size=0, offset=22
	unsigned char pad2; // size=0, offset=23
	short x2; // size=0, offset=24
	short y2; // size=0, offset=26
	unsigned char r3; // size=0, offset=28
	unsigned char g3; // size=0, offset=29
	unsigned char b3; // size=0, offset=30
	unsigned char pad3; // size=0, offset=31
	short x3; // size=0, offset=32
	short y3; // size=0, offset=34
};

struct DR_AREA // hashcode: 0xE0CED39B (dec: -523316325)
{
	unsigned long tag; // size=0, offset=0
	unsigned long code[2]; // size=8, offset=4
};

struct evPositionData // hashcode: 0x853C21E0 (dec: -2059656736)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct FaceExtents // hashcode: 0x22E8BE79 (dec: 585678457)
{
	long smallx; // size=0, offset=0
	long bigx; // size=0, offset=4
	long smally; // size=0, offset=8
	long bigy; // size=0, offset=12
	long smallz; // size=0, offset=16
	long bigz; // size=0, offset=20
};

struct WarpRoom // hashcode: 0x34E00E66 (dec: 887098982)
{
	char name[16]; // size=16, offset=0
	struct _StreamUnit *streamUnit; // size=64, offset=16
};

struct PlanningNode // hashcode: 0xDB9F3065 (dec: -610324379)
{
	struct _Position pos; // size=6, offset=0
	unsigned short flags; // size=0, offset=6
	unsigned long connectionStatus; // size=0, offset=8
	unsigned long connections; // size=0, offset=12
	unsigned short cost; // size=0, offset=16
	unsigned short parent; // size=0, offset=18
	unsigned short nodeType; // size=0, offset=20
	unsigned short id; // size=0, offset=22
	long streamUnitID; // size=0, offset=24
};

struct VramBuffer // hashcode: 0x13A80E34 (dec: 329780788)
{
	long flags; // size=0, offset=0
	short x; // size=0, offset=4
	short y; // size=0, offset=6
	short w; // size=0, offset=8
	short h; // size=0, offset=10
	short yOffset; // size=0, offset=12
	short lengthOfLeftOverData; // size=0, offset=14
	short *lineOverFlow; // size=0, offset=16
};

struct STREAM_294fake // hashcode: 0x4C00FE3C (dec: 1275133500)
{
	int warpgate_in_use; // size=0, offset=0
	char loading; // size=0, offset=4
	char blocked; // size=0, offset=5
	short fadeValue; // size=0, offset=6
	long curTime; // size=0, offset=8
	long maxTime; // size=0, offset=12
	struct _Instance *warpFaceInstance; // size=616, offset=16
};

struct PHYSOBS_109fake // hashcode: 0x64BC6ACF (dec: 1690069711)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: PhysObs.o, PhysObs.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: PhysObs.o, PhysObs.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: PhysObs.o, PhysObs.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: PhysObs.o, PhysObs.o
};

struct evObjectData // hashcode: 0xE261F276 (dec: -496897418)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short PathNumber; // size=0, offset=4
	short LinkNode; // size=0, offset=6
	struct _Instance *Force; // size=616, offset=8
	int rc; // size=0, offset=12
};

struct evObjectThrowData // hashcode: 0x293A2507 (dec: 691676423)
{
	short type; // size=0, offset=0
	short spinType; // size=0, offset=2
	union {
		struct _Instance *target; // size=616, offset=0
		struct _Position position; // size=6, offset=0
		struct _Rotation direction; // size=8, offset=0
		struct _Position throwVector; // size=6, offset=0
	} data; // size=8, offset=4, found in object files: PhysObs.o, PhysObs.o
	struct _SVector angularVel; // size=8, offset=12
	unsigned short speed; // size=0, offset=20
	short gravity; // size=0, offset=22
	short initialXRot; // size=0, offset=24
	short zVel; // size=0, offset=26
};

struct evObjectBirthProjectileData // hashcode: 0xAA355F00 (dec: -1439342848)
{
	struct _Instance *instance; // size=616, offset=0
	int joint; // size=0, offset=4
	int type; // size=0, offset=8
	struct _Instance *birthInstance; // size=616, offset=12
};

struct PhysObWeaponProperties // hashcode: 0xC3612955 (dec: -1017042603)
{
	struct PhysObProperties Properties; // size=8, offset=0
	struct PhysObWeaponAttributes WeaponAttributes; // size=28, offset=8
};

struct PhysObInteractProperties // hashcode: 0x668EE420 (dec: 1720640544)
{
	struct PhysObProperties Properties; // size=8, offset=0
	unsigned short conditions; // size=0, offset=8
	unsigned short auxConditions; // size=0, offset=10
	unsigned short action; // size=0, offset=12
	unsigned short auxAction; // size=0, offset=14
	unsigned char startAnim; // size=0, offset=16
	unsigned char endAnim; // size=0, offset=17
	unsigned char razielAnim; // size=0, offset=18
	unsigned char razielAuxAnim; // size=0, offset=19
	unsigned char frame; // size=0, offset=20
	unsigned char startAnimMode; // size=0, offset=21
	unsigned short distance; // size=0, offset=22
	unsigned short newType; // size=0, offset=24
	unsigned short newClass; // size=0, offset=26
	unsigned short mode; // size=0, offset=28
	unsigned short engageXYDistance; // size=0, offset=30
	short engageZMinDelta; // size=0, offset=32
	short engageZMaxDelta; // size=0, offset=34
	unsigned char engageYCone; // size=0, offset=36
	unsigned char engageZCone; // size=0, offset=37
	unsigned short pad; // size=0, offset=38
	struct PhysObWeaponAttributes *weapon; // size=28, offset=40
};

struct __PhysObProjectileProperties // hashcode: 0x0F1971B2 (dec: 253325746)
{
	struct PhysObProperties Properties; // size=8, offset=0
	long totalProjectiles; // size=0, offset=8
	struct __PhysObProjectileData *data; // size=12, offset=12
};

struct PhysObDraftProperties // hashcode: 0x734F01FC (dec: 1934557692)
{
	struct PhysObProperties Properties; // size=8, offset=0
	short force; // size=0, offset=8
	unsigned short radius; // size=0, offset=10
	unsigned short radiusCoef; // size=0, offset=12
	short maxVelocity; // size=0, offset=14
	unsigned long height; // size=0, offset=16
};

struct PhysObSwitchProperties // hashcode: 0xF10C9EBB (dec: -250831173)
{
	struct PhysObProperties Properties; // size=8, offset=0
	short Distance; // size=0, offset=8
	short Class; // size=0, offset=10
	unsigned char onAnim; // size=0, offset=12
	unsigned char offAnim; // size=0, offset=13
	unsigned char failedOnAnim; // size=0, offset=14
	unsigned char enableAnim; // size=0, offset=15
	unsigned char shutAnim; // size=0, offset=16
	unsigned char onWalkTimer; // size=0, offset=17
	unsigned char offWalkTimer; // size=0, offset=18
	unsigned char walkMode; // size=0, offset=19
	unsigned char razielOnAnim; // size=0, offset=20
	unsigned char razielOffAnim; // size=0, offset=21
	unsigned char razielFailedOnAnim; // size=0, offset=22
	unsigned char razielEnableAnim; // size=0, offset=23
	unsigned char startMode; // size=0, offset=24
	unsigned char startAnim; // size=0, offset=25
	unsigned short engageXYDistance; // size=0, offset=26
	char engageZMinDelta; // size=0, offset=28
	char engageZMaxDelta; // size=0, offset=29
	unsigned char engageYCone; // size=0, offset=30
	unsigned char engageZCone; // size=0, offset=31
};

struct PhysObCollectibleProperties // hashcode: 0x80B1E807 (dec: -2135824377)
{
	struct PhysObProperties Properties; // size=8, offset=0
	short collectClass; // size=0, offset=8
	short collectValue; // size=0, offset=10
	unsigned long distance; // size=0, offset=12
	unsigned char idleAnim; // size=0, offset=16
	unsigned char collectAnim; // size=0, offset=17
	unsigned short spatialNode; // size=0, offset=18
};

struct PhysObAnimatedProperties // hashcode: 0xC40C87E4 (dec: -1005811740)
{
	struct PhysObProperties Properties; // size=8, offset=0
	unsigned short flags; // size=0, offset=8
	unsigned short pad; // size=0, offset=10
};

struct PhysObData // hashcode: 0x95351304 (dec: -1791683836)
{
	long Mode; // size=0, offset=0
	short xForce; // size=0, offset=4
	short yForce; // size=0, offset=6
	struct _Instance *Force; // size=616, offset=8
	short LinkNode; // size=0, offset=12
	short px; // size=0, offset=14
	short py; // size=0, offset=16
	short pz; // size=0, offset=18
	short rx1; // size=0, offset=20
	short ry1; // size=0, offset=22
	short rz1; // size=0, offset=24
	short Segment1; // size=0, offset=26
	short rx2; // size=0, offset=28
	short ry2; // size=0, offset=30
	short rz2; // size=0, offset=32
	short Segment2; // size=0, offset=34
	short Steps; // size=0, offset=36
	short Step; // size=0, offset=38
	short PathNumber; // size=0, offset=40
	short Pad1; // size=0, offset=42
	struct _HInfo *RightCollision; // size=24, offset=44
	struct _HInfo *LeftCollision; // size=24, offset=48
	short burnAmpl; // size=0, offset=52
	short throwFlags; // size=0, offset=54
	short initialXRot; // size=0, offset=56
	short xRotVel; // size=0, offset=58
	short yRotVel; // size=0, offset=60
	short zRotVel; // size=0, offset=62
	struct _Instance *throwingInstance; // size=616, offset=64
	int physObTimer; // size=0, offset=68
};

struct BreakOffData // hashcode: 0x47B171A7 (dec: 1202811303)
{
	short NewType; // size=0, offset=0
	short NewClass; // size=0, offset=2
};

struct ProjectileData // hashcode: 0x05689657 (dec: 90740311)
{
	int Type; // size=0, offset=0
};

struct _PhysObSaveData // hashcode: 0xA353E0E7 (dec: -1554784025)
{
	struct SwitchData switchData; // size=4, offset=0
	long Mode; // size=0, offset=4
};

struct RAZCNTRL_109fake // hashcode: 0x79D2E6E3 (dec: 2043864803)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: razcntrl.o, razcntrl.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: razcntrl.o, razcntrl.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: razcntrl.o, razcntrl.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: razcntrl.o, razcntrl.o
};

struct STATE_109fake // hashcode: 0x5CC5AB24 (dec: 1556458276)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: state.o, state.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: state.o, state.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: state.o, state.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: state.o, state.o
};

struct __CharacterState // hashcode: 0x6C0823BB (dec: 1812472763)
{
	struct _Instance *CharacterInstance; // size=616, offset=0
	int TotalSections; // size=0, offset=4
	struct __State SectionList[3]; // size=852, offset=8
};

struct evMonsterHitData // hashcode: 0x5BC7AF7A (dec: 1539813242)
{
	struct _Instance *sender; // size=616, offset=0
	struct _Instance *lastHit; // size=616, offset=4
	short knockBackDistance; // size=0, offset=8
	short knockBackDuration; // size=0, offset=10
	long power; // size=0, offset=12
};

struct evMonsterThrownData // hashcode: 0x1A3E9058 (dec: 440307800)
{
	struct _Instance *sender; // size=616, offset=0
	struct _Rotation direction; // size=8, offset=4
	short power; // size=0, offset=12
};

struct evMonsterAlarmData // hashcode: 0x5CC4F325 (dec: 1556411173)
{
	struct _Instance *sender; // size=616, offset=0
	struct _Position position; // size=6, offset=4
	short type; // size=0, offset=10
};

struct evMonsterSoulSuckData // hashcode: 0x24FA0E9A (dec: 620367514)
{
	struct _Instance *sender; // size=616, offset=0
	struct _Position Destination; // size=6, offset=4
};

struct evPhysicsDropOffData // hashcode: 0x240C7D71 (dec: 604798321)
{
	short xOffset; // size=0, offset=0
	short yOffset; // size=0, offset=2
	short DropOffset; // size=0, offset=4
	short slipSlope; // size=0, offset=6
	int UpperOffset; // size=0, offset=8
};

struct evPhysicsEdgeData // hashcode: 0x9E9E1CB3 (dec: -1633805133)
{
	short UpperOffset; // size=0, offset=0
	short ForwardOffset; // size=0, offset=2
	short AboveOffset; // size=0, offset=4
	short XDistance; // size=0, offset=6
	short YDistance; // size=0, offset=8
	short ZDistance; // size=0, offset=10
	struct _SVector *Normal1; // size=8, offset=12
	struct _SVector *Normal2; // size=8, offset=16
	struct _SVector *Delta; // size=8, offset=20
	struct _Instance *instance; // size=616, offset=24
	int zRot; // size=0, offset=28
	int rc; // size=0, offset=32
};

struct evPhysicsWallCrawlData // hashcode: 0x27DFBF06 (dec: 668974854)
{
	short Segment; // size=0, offset=0
	short Length; // size=0, offset=2
	short ForwardOffset; // size=0, offset=4
	short NormalDistance; // size=0, offset=6
	struct _SVector DropNormal; // size=8, offset=8
	struct _SVector ForwardNormal; // size=8, offset=16
	struct _Position NewPosition; // size=6, offset=24
	struct _Rotation DropRotation; // size=8, offset=30
	short ForwardXRotation; // size=0, offset=38
	int rc; // size=0, offset=40
};

struct evPhysicsSwimData // hashcode: 0x947F2BE5 (dec: -1803605019)
{
	short CheckDepth; // size=0, offset=0
	short SwimDepth; // size=0, offset=2
	short WadeDepth; // size=0, offset=4
	short TreadDepth; // size=0, offset=6
	struct _SVector *iVelocity; // size=8, offset=8
	short Depth; // size=0, offset=12
	short WaterDepth; // size=0, offset=14
	short rc; // size=0, offset=16
	short pad; // size=0, offset=18
	int WaterLevel; // size=0, offset=20
};

struct evPhysicsDropHeightData // hashcode: 0x5DCE3926 (dec: 1573796134)
{
	struct _Position origin; // size=6, offset=0
	short mode; // size=0, offset=6
	short DropOffset; // size=0, offset=8
	short result; // size=0, offset=10
	struct BSPTree *bsp; // size=36, offset=12
	struct _TFace *tface; // size=12, offset=16
};

struct evPhysicsLinkedMoveData // hashcode: 0xB4C6EBA9 (dec: -1262031959)
{
	struct _Instance *instance; // size=616, offset=0
	int segment; // size=0, offset=4
	struct _SVector posDelta; // size=8, offset=8
	struct _SVector rotDelta; // size=8, offset=16
};

struct evFXHitData // hashcode: 0x3D609E61 (dec: 1029742177)
{
	struct _SVector location; // size=8, offset=0
	struct _SVector velocity; // size=8, offset=8
	short amount; // size=0, offset=16
	short type; // size=0, offset=18
};

struct evAnimationControllerDoneData // hashcode: 0x06BA7BE4 (dec: 112884708)
{
	struct _Instance *instance; // size=616, offset=0
	long segment; // size=0, offset=4
	long type; // size=0, offset=8
	int data; // size=0, offset=12
};

struct evMonsterImpaleData // hashcode: 0x6537B50B (dec: 1698149643)
{
	struct _Instance *weapon; // size=616, offset=0
	struct _Rotation direction; // size=8, offset=4
	struct _Position position; // size=6, offset=12
	short distance; // size=0, offset=18
};

struct evObjectBreakOffData // hashcode: 0xE7DCD1A1 (dec: -404958815)
{
	struct _Instance *force; // size=616, offset=0
	unsigned short node; // size=0, offset=4
	short distance; // size=0, offset=6
	unsigned short animation; // size=0, offset=8
	unsigned short frame; // size=0, offset=10
	unsigned short type; // size=0, offset=12
	unsigned short action; // size=0, offset=14
};

struct evObjectAbsorbData // hashcode: 0x1379C145 (dec: 326746437)
{
	struct _Instance *force; // size=616, offset=0
	unsigned short node; // size=0, offset=4
	unsigned short steps; // size=0, offset=6
};

struct evObjectIdleData // hashcode: 0x7C5C48CC (dec: 2086422732)
{
	long mode; // size=0, offset=0
	struct _Instance *instance; // size=616, offset=4
};

struct evControlInitHangData // hashcode: 0xA248117B (dec: -1572335237)
{
	struct _Instance *instance; // size=616, offset=0
	int frame; // size=0, offset=4
	int frames; // size=0, offset=8
};

struct evControlInitIdleData // hashcode: 0xB37CA3C6 (dec: -1283677242)
{
	int mode; // size=0, offset=0
	int frame; // size=0, offset=4
	int frames; // size=0, offset=8
};

struct evActionLookAroundData // hashcode: 0x27BD6BB6 (dec: 666725302)
{
	short *rotx; // size=0, offset=0
	short *rotz; // size=0, offset=4
	short minx; // size=0, offset=8
	short maxx; // size=0, offset=10
	short minz; // size=0, offset=12
	short maxz; // size=0, offset=14
};

struct evActionPlayHostAnimationData // hashcode: 0x89FEF84B (dec: -1979778997)
{
	struct _Instance *instance; // size=616, offset=0
	struct _Instance *host; // size=616, offset=4
	int newAnim; // size=0, offset=8
	int newFrame; // size=0, offset=12
	int frames; // size=0, offset=16
	int mode; // size=0, offset=20
};

struct evShadowSegmentData // hashcode: 0xE814E960 (dec: -401282720)
{
	unsigned long totalShadowSegments; // size=0, offset=0
	unsigned short shadowSegments[4]; // size=8, offset=4
};

struct STRMLOAD_116fake // hashcode: 0x9FFC6FB9 (dec: -1610846279)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: strmload.o, strmload.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: strmload.o, strmload.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: strmload.o, strmload.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: strmload.o, strmload.o
};

struct SCRIPT2_114fake // hashcode: 0x48B4C9D2 (dec: 1219807698)
{
	struct {
		struct MultiSpline *posSpline; // size=80, offset=0
		struct MultiSpline *targetSpline; // size=80, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: script2.o, script2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: script2.o, script2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: script2.o, script2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: script2.o, script2.o
};

struct LitShaftProperties // hashcode: 0x6DFDA06C (dec: 1845338220)
{
	short fadeValue; // size=0, offset=0
	short pad; // size=0, offset=2
};

struct SavedEventSmallVars // hashcode: 0xE3137261 (dec: -485264799)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	char eventNumber; // size=0, offset=4
	char eventVariables[5]; // size=5, offset=5
};


struct _PCodeStack // hashcode: 0x88A5C96B (dec: -2002400917)
{
	long topOfStack; // size=0, offset=0
	struct StackType stack[32]; // size=1152, offset=4
};



struct SignalResetStruct // hashcode: 0x8D08210C (dec: -1928847092)
{
	struct _MultiSignal *mSignal; // size=904, offset=0
	long timeLeft; // size=0, offset=4
};

struct burntTuneType // hashcode: 0x127E6332 (dec: 310272818)
{
	int burntDist; // size=0, offset=0
	int fadeDelta; // size=0, offset=4
};

struct VRAM_114fake // hashcode: 0xBB89CDD4 (dec: -1148596780)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: vram.o, vram.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: vram.o, vram.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: vram.o, vram.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: vram.o, vram.o
};

struct PHYSICS_109fake // hashcode: 0xC9F7DBFE (dec: -906503170)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: physics.o, physics.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: physics.o, physics.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: physics.o, physics.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: physics.o, physics.o
};

struct REAVER_114fake // hashcode: 0x25B74FFD (dec: 632770557)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: reaver.o, reaver.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: reaver.o, reaver.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: reaver.o, reaver.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: reaver.o, reaver.o
};







struct evPhysicsLOSData // hashcode: 0x3AEAF56B (dec: 988476779)
{
	struct _Position destination; // size=6, offset=0
	short arc; // size=0, offset=6
	struct _Position origin; // size=6, offset=8
	short flags; // size=0, offset=14
	short result; // size=0, offset=16
};

struct MONTABLE_109fake // hashcode: 0xD09BCEF5 (dec: -795095307)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: montable.o, montable.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: montable.o, montable.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: montable.o, montable.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: montable.o, montable.o
};

struct RadarInfo // hashcode: 0xB8957CDE (dec: -1198162722)
{
	short angle; // size=0, offset=0
	short x; // size=0, offset=2
	short y; // size=0, offset=4
};

struct burntMessType // hashcode: 0x8C763516 (dec: -1938410218)
{
	int closestvert; // size=0, offset=0
	int closestdist; // size=0, offset=4
	int closestseg; // size=0, offset=8
	struct burntTuneType *burntTune; // size=8, offset=12
};

struct bloodyMessType // hashcode: 0x5F4737FF (dec: 1598502911)
{
	int closestvert; // size=0, offset=0
	int closestdist; // size=0, offset=4
	int closestseg; // size=0, offset=8
	int bloodiedAVert; // size=0, offset=12
	int bloodIntensity; // size=0, offset=16
};

struct PLANAPI_151fake // hashcode: 0xCFA0CAB4 (dec: -811545932)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: planapi.o, planapi.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: planapi.o, planapi.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: planapi.o, planapi.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: planapi.o, planapi.o
};

struct PoolManagementData // hashcode: 0xA005D5E0 (dec: -1610230304)
{
	char state; // size=0, offset=0
	unsigned char numNodesInPool; // size=0, offset=1
	struct _Position playerPosAtLastPlanMkrUpdate; // size=6, offset=2
	struct PlanningNode *expansionNode1; // size=28, offset=8
	struct PlanningNode *expansionNode2; // size=28, offset=12
	unsigned short (*distanceMatrix)[32]; // size=4, offset=16
	unsigned short pairType; // size=0, offset=20
	struct _Position peakPos; // size=6, offset=22
	unsigned char currentNode; // size=0, offset=28
	unsigned char currentBit; // size=0, offset=29
};

struct EnemyPlanData // hashcode: 0xE792BD02 (dec: -409813758)
{
	unsigned char nodeSkipArray[8]; // size=8, offset=0
	unsigned char nodeTypeArray[8]; // size=8, offset=8
	struct _Position wayPointArray[8]; // size=48, offset=16
	short goalUnitID; // size=0, offset=64
	unsigned char numWayPoints; // size=0, offset=66
};

struct _PlanCollideInfo // hashcode: 0x3B1266CC (dec: 991061708)
{
	long StreamUnitID; // size=0, offset=0
	struct _TFace *tFace; // size=12, offset=4
	struct _Position collidePos; // size=6, offset=8
};

struct PLANCOLL_114fake // hashcode: 0xB6BC32DF (dec: -1229180193)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: plancoll.o, plancoll.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: plancoll.o, plancoll.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: plancoll.o, plancoll.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: plancoll.o, plancoll.o
};

struct _fth // hashcode: 0xDEECEA3E (dec: -554898882)
{
	char x; // size=0, offset=0
	char y; // size=0, offset=1
	char z; // size=0, offset=2
	char nodeType; // size=0, offset=3
};

struct PLANDRAW_114fake // hashcode: 0x6D041DD0 (dec: 1828986320)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: plandraw.o, plandraw.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: plandraw.o, plandraw.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: plandraw.o, plandraw.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: plandraw.o, plandraw.o
};

struct PLANOPT_151fake // hashcode: 0xD60D918D (dec: -703753843)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: planopt.o, planopt.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: planopt.o, planopt.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: planopt.o, planopt.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: planopt.o, planopt.o
};

struct ENMYPLAN_114fake // hashcode: 0xA6FD1322 (dec: -1493363934)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: enmyplan.o, enmyplan.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: enmyplan.o, enmyplan.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: enmyplan.o, enmyplan.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: enmyplan.o, enmyplan.o
};

struct EnemyPlanSlotData // hashcode: 0xA348D4B1 (dec: -1555508047)
{
	char slotInUse; // size=0, offset=0
	char state; // size=0, offset=1
	char wayPointBeingServoedTo; // size=0, offset=2
	struct _Position startPos; // size=6, offset=4
	struct _Position goalPos; // size=6, offset=10
	struct _Position oldCurrentPos; // size=6, offset=16
	struct EnemyPlanData planData; // size=68, offset=22
	char targetType; // size=0, offset=90
	short timer; // size=0, offset=92
};

struct PLAN_114fake // hashcode: 0xA74547B8 (dec: -1488631880)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: plan.o, plan.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: plan.o, plan.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: plan.o, plan.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: plan.o, plan.o
};

struct INSTNCG2_109fake // hashcode: 0x507B5C94 (dec: 1350261908)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: InstncG2.o, InstncG2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: InstncG2.o, InstncG2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: InstncG2.o, InstncG2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: InstncG2.o, InstncG2.o
};

struct MATRIXG2_109fake // hashcode: 0x245522F0 (dec: 609559280)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: MatrixG2.o, MatrixG2.o
};

struct _G2PoolMemPool_Type // hashcode: 0x987744BB (dec: -1737014085)
{
	unsigned short blockSize; // size=0, offset=0
	unsigned short stackTop; // size=0, offset=2
	unsigned short stackSize; // size=0, offset=4
	unsigned short *stack; // size=0, offset=8
	struct _G2PoolMem_Type *blockPool; // size=0, offset=12
};

struct TIMERG2_109fake // hashcode: 0xDA8B298B (dec: -628414069)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: TimerG2.o, TimerG2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: TimerG2.o, TimerG2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: TimerG2.o, TimerG2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: TimerG2.o, TimerG2.o
};

struct PLANPOOL_151fake // hashcode: 0x1F99F3B2 (dec: 530183090)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: planpool.o, planpool.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: planpool.o, planpool.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: planpool.o, planpool.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: planpool.o, planpool.o
};

struct PLANSRCH_151fake // hashcode: 0x490AB669 (dec: 1225438825)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: plansrch.o, plansrch.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: plansrch.o, plansrch.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: plansrch.o, plansrch.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: plansrch.o, plansrch.o
};

struct CONTROL_109fake // hashcode: 0x745D7876 (dec: 1952282742)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: control.o, control.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: control.o, control.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: control.o, control.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: control.o, control.o
};

struct __Force // hashcode: 0x2CC8F311 (dec: 751366929)
{
	int Type; // size=0, offset=0
	struct _Vector LinearForce; // size=12, offset=4
	short Friction; // size=0, offset=16
};

struct __Idle // hashcode: 0xA4E3256E (dec: -1528617618)
{
	unsigned char anim; // size=0, offset=0
	unsigned char frame; // size=0, offset=1
	unsigned char frames; // size=0, offset=2
	unsigned char type; // size=0, offset=3
};

struct __VAnim // hashcode: 0xDC662191 (dec: -597286511)
{
	unsigned char anim0; // size=0, offset=0
	unsigned char anim1; // size=0, offset=1
	unsigned char anim2; // size=0, offset=2
	unsigned char frames; // size=0, offset=3
	unsigned char mode; // size=0, offset=4
	unsigned char alpha; // size=0, offset=5
	unsigned short frame; // size=0, offset=6
};

struct __SAnim // hashcode: 0xD19E5798 (dec: -778152040)
{
	struct __VAnim *anim; // size=8, offset=0
	struct __SAnim *nextAnim; // size=16, offset=4
	short mode; // size=0, offset=8
	short data; // size=0, offset=10
	short speedAdjust; // size=0, offset=12
	short pad; // size=0, offset=14
};

struct __AttackItem // hashcode: 0x6E34BA91 (dec: 1848949393)
{
	unsigned char anim; // size=0, offset=0
	unsigned char framesIn; // size=0, offset=1
	unsigned char alphaIn; // size=0, offset=2
	unsigned char framesOut; // size=0, offset=3
	unsigned char switchDelay; // size=0, offset=4
	unsigned char ignoreDelay; // size=0, offset=5
	unsigned char segmentToGlow; // size=0, offset=6
	unsigned char startCollisionFrame; // size=0, offset=7
	unsigned char handsToCollide; // size=0, offset=8
	unsigned char endCollisionFrame; // size=0, offset=9
	unsigned char ribbonStartFrame; // size=0, offset=10
	unsigned char ribbonStartSegment; // size=0, offset=11
	unsigned char ribbonEndSegment; // size=0, offset=12
	unsigned char ribbonLifeTime; // size=0, offset=13
	unsigned char ribbonFaceLifeTime; // size=0, offset=14
	unsigned char knockBackFrames; // size=0, offset=15
	unsigned char glowFadeInFrames; // size=0, offset=16
	unsigned char glowFadeOutFrames; // size=0, offset=17
	unsigned short ribbonStartOpacity; // size=0, offset=18
	unsigned long ribbonStartColor; // size=0, offset=20
	unsigned long ribbonEndColor; // size=0, offset=24
	unsigned long glowColor; // size=0, offset=28
	unsigned short knockBackDistance; // size=0, offset=32
	unsigned short hitPowerScale; // size=0, offset=34
};

struct __ThrowItem // hashcode: 0xABF5F7A3 (dec: -1409943645)
{
	unsigned char prepAnim; // size=0, offset=0
	unsigned char prepAnimFramesIn; // size=0, offset=1
	unsigned char prepAnimAlphaIn; // size=0, offset=2
	unsigned char coilAnim; // size=0, offset=3
	unsigned char coilAnimFramesIn; // size=0, offset=4
	unsigned char coilAnimAlphaIn; // size=0, offset=5
	unsigned char throwAnim; // size=0, offset=6
	unsigned char throwAnimFramesInFactor; // size=0, offset=7
	unsigned char throwFrameToLaunch; // size=0, offset=8
	unsigned char framesOut; // size=0, offset=9
	short launchPointX; // size=0, offset=10
	short launchPointY; // size=0, offset=12
	short launchPointZ; // size=0, offset=14
	short maxXRotation; // size=0, offset=16
	short minXRotation; // size=0, offset=18
	short maxZRotation; // size=0, offset=20
	short minZRotation; // size=0, offset=22
	short velocity; // size=0, offset=24
	short gravity; // size=0, offset=26
	short coilRot; // size=0, offset=28
	short selfAdjustingFlag; // size=0, offset=30
};

struct __EngagedInstance // hashcode: 0x504DFFAD (dec: 1347289005)
{
	struct _Instance *instance; // size=616, offset=0
	unsigned long distance; // size=0, offset=4
};

struct __FitPoint // hashcode: 0xFA3F31C2 (dec: -96521790)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
};

struct __SensoryData // hashcode: 0x3A2E27FD (dec: 976103421)
{
	int Flags; // size=0, offset=0
	struct _SVector ForwardNormal; // size=8, offset=4
	struct _SVector AboveNormal; // size=8, offset=12
	struct _SVector BelowNormal; // size=8, offset=20
	struct _SVector BelowPoint; // size=8, offset=28
	struct _SVector Delta; // size=8, offset=36
	struct _Instance *HitMonster; // size=616, offset=44
	struct _Instance *CurrentAutoFace; // size=616, offset=48
	struct _Instance *LastAutoFace; // size=616, offset=52
	struct _Instance *Portal; // size=616, offset=56
	struct __EngagedInstance *EngagedList; // size=8, offset=60
	unsigned long EngagedMask; // size=0, offset=64
	struct _Vector lookAtPoint; // size=12, offset=68
	unsigned long heldClass; // size=0, offset=80
};

struct __Player // hashcode: 0x0712E7A5 (dec: 118679461)
{
	int footPrint; // size=0, offset=0
	struct __CharacterState State; // size=860, offset=4
	int Mode; // size=0, offset=864
	int Anim; // size=0, offset=868
	int RotationSegment; // size=0, offset=872
	struct _SVector iVelocity; // size=8, offset=876
	int Magnitude; // size=0, offset=884
	int input; // size=0, offset=888
	int lastInput; // size=0, offset=892
	int ZDirection; // size=0, offset=896
	short Bearing; // size=0, offset=900
	short LastBearing; // size=0, offset=902
	int steeringMode; // size=0, offset=904
	short steeringVelocity; // size=0, offset=908
	short steeringAcceleration; // size=0, offset=910
	int steeringLockRotation; // size=0, offset=912
	short movementMinRate; // size=0, offset=916
	short movementMaxRate; // size=0, offset=918
	short movementMinAnalog; // size=0, offset=920
	short movementMaxAnalog; // size=0, offset=922
	struct __SensoryData Senses; // size=84, offset=924
	struct _Instance *attachedPlatform; // size=616, offset=1008
	short dropOffHeight; // size=0, offset=1012
	short fallZVelocity; // size=0, offset=1014
	short HealthScale; // size=0, offset=1016
	short HealthBalls; // size=0, offset=1018
	int HitPoints; // size=0, offset=1020
	int DamageFrequency; // size=0, offset=1024
	int invincibleTimer; // size=0, offset=1028
	int Abilities; // size=0, offset=1032
	struct _Instance *GlyphSystem; // size=616, offset=1036
	unsigned short GlyphManaBalls; // size=0, offset=1040
	unsigned short GlyphManaMax; // size=0, offset=1042
	struct __FitPoint *constrictData; // size=4, offset=1044
	short constrictIndex; // size=0, offset=1048
	short constrictWaitIndex; // size=0, offset=1050
	struct _SVector constrictCenter; // size=8, offset=1052
	short constrictFlag; // size=0, offset=1060
	short constrictGoodCircle; // size=0, offset=1062
	unsigned long constrictXTotal; // size=0, offset=1064
	unsigned long constrictYTotal; // size=0, offset=1068
	struct _Instance *soulReaver; // size=616, offset=1072
	unsigned long currentSoulReaver; // size=0, offset=1076
	int CurrentPlane; // size=0, offset=1080
	int slipSlope; // size=0, offset=1084
	unsigned long idleCount; // size=0, offset=1088
	struct _Instance *idleInstance; // size=616, offset=1092
	struct __AttackItem *attack; // size=36, offset=1096
	int attackFlags; // size=0, offset=1100
	int currentAttack; // size=0, offset=1104
	struct _Instance *attackCurrentHit; // size=616, offset=1108
	struct _Instance *attackLastHit; // size=616, offset=1112
	struct _FXGlowEffect *glowEffect; // size=44, offset=1116
	struct _Instance *attackedBy; // size=616, offset=1120
	struct __ThrowItem *throwData; // size=32, offset=1124
	int throwMode; // size=0, offset=1128
	struct _Position throwTarget; // size=6, offset=1132
	struct _Instance *throwInstance; // size=616, offset=1140
	short throwXRot; // size=0, offset=1144
	short throwZRot; // size=0, offset=1146
	int throwReturnRot; // size=0, offset=1148
	struct __MessageQueue padCommands; // size=136, offset=1152
	void (*returnState)(); // size=0, offset=1288
	unsigned long passedMask; // size=0, offset=1292
	struct _G2SVector3_Type extraRot; // size=6, offset=1296
	long blankPad; // size=0, offset=1304
	int effectsFlags; // size=0, offset=1308
	short effectsFadeSource; // size=0, offset=1312
	short effectsFadeDest; // size=0, offset=1314
	int effectsFadeStep; // size=0, offset=1316
	int effectsFadeSteps; // size=0, offset=1320
	short autoFaceZone; // size=0, offset=1324
	short autoFaceAngle; // size=0, offset=1326
	int autoFaceLastRootAngle; // size=0, offset=1328
	int autoFaceRootAngle; // size=0, offset=1332
	int autoFaceTrueAngle; // size=0, offset=1336
	int autoFaceAnim; // size=0, offset=1340
	int autoFaceLastAnim; // size=0, offset=1344
	int nothingCounter; // size=0, offset=1348
	int timeAccumulator; // size=0, offset=1352
	struct _Position puppetMoveToPoint; // size=6, offset=1356
	struct _Position puppetRotToPoint; // size=6, offset=1362
	int puppetHint; // size=0, offset=1368
	int soundModifier; // size=0, offset=1372
	short alarmTable; // size=0, offset=1376
	short terminator; // size=0, offset=1378
	short swimSpeedAdjustment; // size=0, offset=1380
	short swimTargetSpeed; // size=0, offset=1382
	unsigned long soundHandle; // size=0, offset=1384
	long soundTimer; // size=0, offset=1388
	long soundTotalTime; // size=0, offset=1392
	unsigned long soundDistance; // size=0, offset=1396
	short soundStartPitch; // size=0, offset=1400
	short soundEndPitch; // size=0, offset=1402
	short soundStartVolume; // size=0, offset=1404
	short soundEndVolume; // size=0, offset=1406
	unsigned long soundHandle2; // size=0, offset=1408
	long soundTimer2; // size=0, offset=1412
	long soundTotalTime2; // size=0, offset=1416
	unsigned long soundDistance2; // size=0, offset=1420
	short soundStartPitch2; // size=0, offset=1424
	short soundEndPitch2; // size=0, offset=1426
	short soundStartVolume2; // size=0, offset=1428
	short soundEndVolume2; // size=0, offset=1430
	long soundTimerNext; // size=0, offset=1432
	unsigned long soundTimerData; // size=0, offset=1436
	unsigned long forcedGlideSpeed; // size=0, offset=1440
	struct _SVector collisionEdgeOffset; // size=8, offset=1444
	struct __SAnim *currentSAnim; // size=16, offset=1452
	unsigned long playerEvent; // size=0, offset=1456
	unsigned long playerEventHistory; // size=0, offset=1460
	int currentHint; // size=0, offset=1464
};

struct __CannedSound // hashcode: 0x8A7588C2 (dec: -1972008766)
{
	short bank; // size=0, offset=0
	short sound; // size=0, offset=2
	short startPitch; // size=0, offset=4
	short endPitch; // size=0, offset=6
	short startVolume; // size=0, offset=8
	short endVolume; // size=0, offset=10
	long time; // size=0, offset=12
	long distance; // size=0, offset=16
};

struct __AlarmData // hashcode: 0x49AF2B39 (dec: 1236216633)
{
	short command; // size=0, offset=0
	short data; // size=0, offset=2
};

struct _SoundRamp // hashcode: 0xB5503009 (dec: -1253036023)
{
	unsigned long soundHandle; // size=0, offset=0
	long soundTimer; // size=0, offset=4
	long soundTotalTime; // size=0, offset=8
	unsigned long soundDistance; // size=0, offset=12
	short soundStartPitch; // size=0, offset=16
	short soundEndPitch; // size=0, offset=18
	short soundStartVolume; // size=0, offset=20
	short soundEndVolume; // size=0, offset=22
};

struct PlayerSaveData // hashcode: 0x12C2C060 (dec: 314753120)
{
	unsigned long abilities; // size=0, offset=0
	unsigned long currentPlane; // size=0, offset=4
	unsigned long healthScale; // size=0, offset=8
	unsigned long healthBalls; // size=0, offset=12
	unsigned short manaBalls; // size=0, offset=16
	unsigned short manaMax; // size=0, offset=18
	unsigned long playerEventHistory; // size=0, offset=20
};

struct RazielData // hashcode: 0x40AA7C96 (dec: 1084914838)
{
	unsigned long version; // size=0, offset=0
	unsigned long nonBurningRibbonStartColor; // size=0, offset=4
	unsigned long nonBurningRibbonEndColor; // size=0, offset=8
	struct __Idle ***idleList; // size=4, offset=12
	struct __AttackItem ***attackList; // size=36, offset=16
	struct __ThrowItem **throwList; // size=32, offset=20
	struct __VAnim *virtualAnimations; // size=8, offset=24
	struct __VAnim *virtualAnimSingle; // size=8, offset=28
	struct __SAnim **stringAnimations; // size=16, offset=32
	short throwFadeValue; // size=0, offset=36
	short throwFadeInRate; // size=0, offset=38
	int throwFadeOutRate; // size=0, offset=40
	int throwManualDistance; // size=0, offset=44
	short healthMaterialRate; // size=0, offset=48
	short healthSpectralRate; // size=0, offset=50
	short healthInvinciblePostHit; // size=0, offset=52
	short healthInvinciblePostShunt; // size=0, offset=54
	short forceMinPitch; // size=0, offset=56
	short forceMaxPitch; // size=0, offset=58
	short forceMinVolume; // size=0, offset=60
	short forceMaxVolume; // size=0, offset=62
	unsigned long forceRampTime; // size=0, offset=64
	int SwimPhysicsFallDamping; // size=0, offset=68
	int SwimPhysicsWaterFrequency; // size=0, offset=72
	int SwimPhysicsWaterAmplitude; // size=0, offset=76
	int SwimPhysicsUnderDeceleration; // size=0, offset=80
	int SwimPhysicsUnderKickVelocity; // size=0, offset=84
	int SwimPhysicsUnderKickAccel; // size=0, offset=88
	int SwimPhysicsUnderVelocity; // size=0, offset=92
	int SwimPhysicsUnderKickDecel; // size=0, offset=96
	int SwimPhysicsUnderStealthAdjust; // size=0, offset=100
	int SwimPhysicsCoilVelocity; // size=0, offset=104
	int SwimPhysicsCoilDecelerationIn; // size=0, offset=108
	int SwimPhysicsCoilDecelerationOut; // size=0, offset=112
	int SwimPhysicsShotVelocity; // size=0, offset=116
	int SwimPhysicsShotAccelerationIn; // size=0, offset=120
	int SwimPhysicsShotAccelerationOut; // size=0, offset=124
	int SwimPhysicsSurfVelocity; // size=0, offset=128
	int SwimPhysicsSurfAccelerationIn; // size=0, offset=132
	int SwimPhysicsSurfAccelerationOut; // size=0, offset=136
	int SwimPhysicsSurfKickVelocity; // size=0, offset=140
	int SwimPhysicsSurfKickAccel; // size=0, offset=144
	int SwimPhysicsSurfMinRotation; // size=0, offset=148
	int SwimPhysicsSurfMaxRotation; // size=0, offset=152
	int SwimPhysicsSurfKickDecel; // size=0, offset=156
};

struct __razController // hashcode: 0x694854B2 (dec: 1766347954)
{
	unsigned short segment; // size=0, offset=0
	unsigned short type; // size=0, offset=2
};

struct SavedInfoTracker // hashcode: 0x03231C9A (dec: 52632730)
{
	char *MemoryCardBuffer; // size=0, offset=0
	char *InfoStart; // size=0, offset=4
	char *InfoEnd; // size=0, offset=8
	char *EndOfMemory; // size=0, offset=12
};

struct SavedLevel // hashcode: 0xABBBA745 (dec: -1413765307)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	short waterZ; // size=0, offset=4
	short numberBSPTreesSaved; // size=0, offset=6
};

struct ActualSavedLevel // hashcode: 0xE75849FD (dec: -413644291)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	short waterZ; // size=0, offset=4
	short numberBSPTreesSaved; // size=0, offset=6
	struct SavedBspTree bspTreeArray[1]; // size=8, offset=8
};

struct SavedDeadDeadBits // hashcode: 0x00B3140D (dec: 11736077)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	char deadDeadBits[832]; // size=832, offset=2
};

struct _GlobalSaveTracker // hashcode: 0x1972AA1A (dec: 426945050)
{
	short savedID; // size=0, offset=0
	short saveSize; // size=0, offset=2
	short flags; // size=0, offset=4
	short saveVersion; // size=0, offset=6
	unsigned long currentTime; // size=0, offset=8
	short humanOpinionOfRaziel; // size=0, offset=12
	char numRegens; // size=0, offset=14
	struct _MONAPI_Regenerator regenEntries[10]; // size=80, offset=16
	struct gSoundData sound; // size=20, offset=96
	short sizeUsedInBlock; // size=0, offset=116
	short CurrentBirthID; // size=0, offset=118
};

struct CdlFILTER // hashcode: 0x62FCDD39 (dec: 1660738873)
{
	unsigned char file; // size=0, offset=0
	unsigned char chan; // size=0, offset=1
	unsigned short pad; // size=0, offset=2
};

struct SpuCommonAttr // hashcode: 0xCB591BFF (dec: -883352577)
{
	unsigned long mask; // size=0, offset=0
	struct SpuVolume mvol; // size=4, offset=4
	struct SpuVolume mvolmode; // size=4, offset=8
	struct SpuVolume mvolx; // size=4, offset=12
	struct SpuExtAttr cd; // size=12, offset=16
	struct SpuExtAttr ext; // size=12, offset=28
};

struct VOICEXA_175fake // hashcode: 0xD4103ED4 (dec: -737132844)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: voicexa.o, voicexa.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: voicexa.o, voicexa.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: voicexa.o, voicexa.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: voicexa.o, voicexa.o
};

struct CINEPSX_109fake // hashcode: 0xCA47EDB9 (dec: -901255751)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: cinepsx.o, cinepsx.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: cinepsx.o, cinepsx.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: cinepsx.o, cinepsx.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: cinepsx.o, cinepsx.o
};

struct menu_t // hashcode: 0x8AB0D2F2 (dec: -1968123150)
{
	int nmenus; // size=0, offset=0
	int nitems; // size=0, offset=4
	int nbytes; // size=0, offset=8
	struct menu_stack_t stack[5]; // size=180, offset=12
	struct menu_item_t items[12]; // size=192, offset=192
	char bytes[512]; // size=512, offset=384
	void (*drawfn)(); // size=0, offset=896
	void *opaque; // size=0, offset=900
};

struct MENU_113fake // hashcode: 0x792A6BAF (dec: 2032823215)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: menu.o, menu.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: menu.o, menu.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: menu.o, menu.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: menu.o, menu.o
};

struct menu_format_t // hashcode: 0xD91CEBF0 (dec: -652416016)
{
	int xpos; // size=0, offset=0
	int ypos; // size=0, offset=4
	int lineskip; // size=0, offset=8
	int itemskip; // size=0, offset=12
	int width; // size=0, offset=16
	int center; // size=0, offset=20
	int border; // size=0, offset=24
};

struct menu_stack_t // hashcode: 0x04101191 (dec: 68161937)
{
	int (*fn)(); // size=0, offset=0
	int index; // size=0, offset=4
	struct menu_format_t format; // size=28, offset=8
};

struct menu_item_t // hashcode: 0xF260D8A8 (dec: -228534104)
{
	char *text; // size=0, offset=0
	int (*fn)(); // size=0, offset=4
	long parameter; // size=0, offset=8
	long flags; // size=0, offset=12
};

struct Extents2d // hashcode: 0x49E3CF7D (dec: 1239666557)
{
	int xmin; // size=0, offset=0
	int xmax; // size=0, offset=4
	int ymin; // size=0, offset=8
	int ymax; // size=0, offset=12
};

struct MENUUTIL_109fake // hashcode: 0x36B25773 (dec: 917657459)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: menuutil.o, menuutil.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: menuutil.o, menuutil.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: menuutil.o, menuutil.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: menuutil.o, menuutil.o
};

struct menu_sound_entry_t // hashcode: 0xFFB1DFB2 (dec: -5120078)
{
	enum menu_sound_t sound; // size=4, offset=0
	int sfx; // size=0, offset=4
};

struct MENUDEFS_109fake // hashcode: 0x99AA84CD (dec: -1716878131)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: menudefs.o, menudefs.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: menudefs.o, menudefs.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: menudefs.o, menudefs.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: menudefs.o, menudefs.o
};

struct MENUFACE_109fake // hashcode: 0xD61A4E22 (dec: -702919134)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: menuface.o, menuface.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: menuface.o, menuface.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: menuface.o, menuface.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: menuface.o, menuface.o
};

struct menuface_t // hashcode: 0xC2732FD8 (dec: -1032638504)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	char w; // size=0, offset=4
	char h; // size=0, offset=5
	char frames; // size=0, offset=6
	short curFrame; // size=0, offset=8
	short transitionDir; // size=0, offset=10
	unsigned short delay; // size=0, offset=12
	unsigned short loaded; // size=0, offset=14
};

struct mcpsx_t // hashcode: 0x79B44761 (dec: 2041857889)
{
	struct {
		enum mcpsx_mode_t mode; // size=4, offset=0
		enum mcpsx_func_t func; // size=4, offset=4
		enum mcpsx_sync_t sync; // size=4, offset=8
		enum mcpsx_err_t err; // size=4, offset=12
		int observed; // size=0, offset=16
	} state; // size=20, offset=0, found in object files: memcard.o, memcard.o
	struct {
		long channel; // size=0, offset=0
		char *filename; // size=0, offset=4
		void *buffer; // size=0, offset=8
		long offset; // size=0, offset=12
		long nbytes; // size=0, offset=16
		long nblocks; // size=0, offset=20
		struct mcpsx_directory_t *directory; // size=24, offset=24
		long *nfilesptr; // size=0, offset=28
	} params; // size=32, offset=20, found in object files: memcard.o, memcard.o
	void *opaque; // size=0, offset=52
};

struct mcpsx_directory_t // hashcode: 0xED9CBF92 (dec: -308494446)
{
	char name[20]; // size=20, offset=0
	int nbytes; // size=0, offset=20
};

struct mcard_t // hashcode: 0x55AF6CB2 (dec: 1437559986)
{
	struct {
		enum mcpsx_err_t err; // size=4, offset=0
		enum state_t fsm; // size=4, offset=4
		enum mcard_status_t status; // size=4, offset=8
		int not_exists; // size=0, offset=12
	} state; // size=16, offset=0, found in object files: memcard.o, memcard.o
	struct {
		long parameter; // size=0, offset=0
		long *nfilesptr; // size=0, offset=4
		struct mcpsx_directory_t *directory; // size=24, offset=8
		char *filename; // size=0, offset=12
		void *buffer; // size=0, offset=16
		int nbytes; // size=0, offset=20
		int nblocks; // size=0, offset=24
	} params; // size=28, offset=16, found in object files: memcard.o, memcard.o
	struct mcpsx_t *mcpsx; // size=56, offset=44
	void *opaque; // size=0, offset=48
};

struct mcmenu_t // hashcode: 0xCDD1805D (dec: -841908131)
{
	struct {
		enum fsm_t fsm; // size=4, offset=0
		enum fsm_t fsm_prev; // size=4, offset=4
		enum mcard_status_t status; // size=4, offset=8
		int running; // size=0, offset=12
		int slot; // size=0, offset=16
	} state; // size=20, offset=0, found in object files: memcard.o, memcard.o
	struct {
		int nblocks; // size=0, offset=0
		int nbytes; // size=0, offset=4
		void *buffer; // size=0, offset=8
		struct mcpsx_directory_t directory[15]; // size=360, offset=12
		long nfiles; // size=0, offset=372
	} params; // size=376, offset=20, found in object files: memcard.o, memcard.o
	struct mcard_t *mcard; // size=52, offset=396
	void *opaque; // size=0, offset=400
};

struct mcmenu_table_t // hashcode: 0xB78C7C17 (dec: -1215529961)
{
	int (*data_size)(); // size=0, offset=0
	void (*initialize)(); // size=0, offset=4
	void (*terminate)(); // size=0, offset=8
	void (*begin)(); // size=0, offset=12
	void (*end)(); // size=0, offset=16
	int (*set_buffer)(); // size=0, offset=20
	int (*main)(); // size=0, offset=24
	int (*pause)(); // size=0, offset=28
	char *versionID; // size=0, offset=32
};

struct MEMCARD_117fake // hashcode: 0x86F561E0 (dec: -2030738976)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: memcard.o, memcard.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: memcard.o, memcard.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: memcard.o, memcard.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: memcard.o, memcard.o
};

struct memcard_t // hashcode: 0xAEC9A344 (dec: -1362517180)
{
	struct Object *object; // size=76, offset=0
	struct mcmenu_table_t *table; // size=36, offset=4
	struct mcmenu_t *mcmenu; // size=404, offset=8
	short running; // size=0, offset=12
	short wrongVerison; // size=0, offset=14
};

struct MEMCARD_216fake // hashcode: 0x1C10213E (dec: 470819134)
{
	struct mcmenu_t dummy1; // size=404, offset=0
	struct mcard_t dummy2; // size=52, offset=404
	struct mcpsx_t dummy3; // size=56, offset=456
};

struct DR_STP // hashcode: 0x8C972396 (dec: -1936252010)
{
	unsigned long tag; // size=0, offset=0
	unsigned long code[2]; // size=8, offset=4
};

struct UNDRWRLD_114fake // hashcode: 0x3CF3A383 (dec: 1022600067)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: undrwrld.o, undrwrld.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: undrwrld.o, undrwrld.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: undrwrld.o, undrwrld.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: undrwrld.o, undrwrld.o
};

struct UW_ScreenXY // hashcode: 0x88757B16 (dec: -2005566698)
{
	short sx; // size=0, offset=0
	short sy; // size=0, offset=2
	short dx; // size=0, offset=4
	short dy; // size=0, offset=6
};
/* End structs */

/* Begin unions */

union PLAYER_76fake // Hashcode: 0x35EA4737 (dec: 904546103)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: player.o, player.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: player.o, player.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: player.o, player.o
};

union PLAYER_77fake // Hashcode: 0xBB3AB3AC (dec: -1153780820)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PLAYER_208fake // Hashcode: 0xD87F8A52 (dec: -662730158)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union DEBUG_118fake // Hashcode: 0x841D74D1 (dec: -2078444335)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: debug.o, debug.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: debug.o, debug.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: debug.o, debug.o
};

union DEBUG_119fake // Hashcode: 0xED3B4C32 (dec: -314880974)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union DEBUG_224fake // Hashcode: 0x0131C19B (dec: 20038043)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union DEBUG_260fake // Hashcode: 0x8C9106F7 (dec: -1936652553)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

union SIGNAL_71fake // Hashcode: 0x60202F31 (dec: 1612721969)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: signal.o, signal.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: signal.o, signal.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: signal.o, signal.o
};

union SIGNAL_72fake // Hashcode: 0x4AF92C48 (dec: 1257843784)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SIGNAL_206fake // Hashcode: 0x2E97F42B (dec: 781710379)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union COLLIDE_76fake // Hashcode: 0x3AA65B79 (dec: 983980921)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: collide.o, collide.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: collide.o, collide.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: collide.o, collide.o
};

union COLLIDE_77fake // Hashcode: 0xCA00C1AF (dec: -905920081)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union COLLIDE_218fake // Hashcode: 0x489C3F73 (dec: 1218199411)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union COLLIDE_246fake // Hashcode: 0x095377A5 (dec: 156465061)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

union CAMERA_71fake // Hashcode: 0x0F6F6CC4 (dec: 258960580)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: camera.o, camera.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: camera.o, camera.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: camera.o, camera.o
};

union CAMERA_72fake // Hashcode: 0x598765BC (dec: 1502045628)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union CAMERA_206fake // Hashcode: 0x6CD0C7C3 (dec: 1825621955)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union DRAW_71fake // Hashcode: 0x10652FFA (dec: 275066874)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: draw.o, draw.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: draw.o, draw.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: draw.o, draw.o
};

union DRAW_72fake // Hashcode: 0x5A7FA683 (dec: 1518315139)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union DRAW_212fake // Hashcode: 0xA8C66167 (dec: -1463393945)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union LIGHT3D_71fake // Hashcode: 0xEEF7B666 (dec: -285755802)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: light3d.o, light3d.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: light3d.o, light3d.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: light3d.o, light3d.o
};

union LIGHT3D_72fake // Hashcode: 0xA2C28EAD (dec: -1564307795)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union LIGHT3D_209fake // Hashcode: 0xF250A22A (dec: -229596630)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union LOAD3D_76fake // Hashcode: 0x8D4B684D (dec: -1924437939)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: load3d.o, load3d.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: load3d.o, load3d.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: load3d.o, load3d.o
};

union LOAD3D_77fake // Hashcode: 0xD696E2BF (dec: -694754625)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MAIN_76fake // Hashcode: 0x1DB2E0D5 (dec: 498262229)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: main.o, main.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: main.o, main.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: main.o, main.o
};

union MAIN_77fake // Hashcode: 0x7C857C0F (dec: 2089122831)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MAIN_213fake // Hashcode: 0xFBB8D531 (dec: -71772879)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MAIN_243fake // Hashcode: 0xD076C382 (dec: -797523070)
{
	unsigned short pad; // size=0, offset=0
	unsigned short multitap[16]; // size=32, offset=0
	unsigned short mouse[2]; // size=4, offset=0
	unsigned short negcon[3]; // size=6, offset=0
	struct AnalogueData analogue; // size=6, offset=0
};

union FONT_76fake // Hashcode: 0x7BE39A46 (dec: 2078513734)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: font.o, font.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: font.o, font.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: font.o, font.o
};

union FONT_77fake // Hashcode: 0xADC7F886 (dec: -1379403642)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union FONT_213fake // Hashcode: 0x740A6D3C (dec: 1946840380)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union GAMELOOP_76fake // Hashcode: 0x8976D081 (dec: -1988702079)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: gameloop.o, gameloop.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: gameloop.o, gameloop.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: gameloop.o, gameloop.o
};

union GAMELOOP_77fake // Hashcode: 0x5136BCA3 (dec: 1362541731)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union GAMELOOP_192fake // Hashcode: 0x2687240D (dec: 646390797)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

union GAMELOOP_236fake // Hashcode: 0x41C73D01 (dec: 1103576321)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union GAMEPAD_76fake // Hashcode: 0xCC53CC9A (dec: -866923366)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: gamepad.o, gamepad.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: gamepad.o, gamepad.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: gamepad.o, gamepad.o
};

union GAMEPAD_77fake // Hashcode: 0xD392046D (dec: -745405331)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union GAMEPAD_200fake // Hashcode: 0xE2465038 (dec: -498708424)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union INSTANCE_76fake // Hashcode: 0x868EC9CC (dec: -2037462580)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: instance.o, instance.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: instance.o, instance.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: instance.o, instance.o
};

union INSTANCE_77fake // Hashcode: 0x2031C6D4 (dec: 540133076)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union INSTANCE_204fake // Hashcode: 0x3FD2E4BE (dec: 1070785726)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union INSTANCE_270fake // Hashcode: 0xB44B3EBC (dec: -1270137156)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

union SOUND_76fake // Hashcode: 0xA905F6AB (dec: -1459226965)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: sound.o, sound.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: sound.o, sound.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: sound.o, sound.o
};

union SOUND_77fake // Hashcode: 0xFD6718B0 (dec: -43575120)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SOUND_205fake // Hashcode: 0x2C15C540 (dec: 739624256)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union BSP_71fake // Hashcode: 0x81368942 (dec: -2127132350)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: bsp.o, bsp.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: bsp.o, bsp.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: bsp.o, bsp.o
};

union BSP_72fake // Hashcode: 0x1CB63B8E (dec: 481704846)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union BSP_208fake // Hashcode: 0x2E8AEFCF (dec: 780857295)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union OBTABLE_76fake // Hashcode: 0x95622E13 (dec: -1788727789)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: obtable.o, obtable.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: obtable.o, obtable.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: obtable.o, obtable.o
};

union OBTABLE_77fake // Hashcode: 0x8EF4F21E (dec: -1896549858)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union OBTABLE_196fake // Hashcode: 0x921E9F5C (dec: -1843486884)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union OBTABLE2_76fake // Hashcode: 0x1E9F108F (dec: 513740943)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: obtable2.o, obtable2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: obtable2.o, obtable2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: obtable2.o, obtable2.o
};

union OBTABLE2_77fake // Hashcode: 0xA0B14AC6 (dec: -1598993722)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union OBTABLE2_209fake // Hashcode: 0xB29B0D0C (dec: -1298461428)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union GENERIC_76fake // Hashcode: 0x55C9A583 (dec: 1439278467)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: generic.o, generic.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: generic.o, generic.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: generic.o, generic.o
};

union GENERIC_77fake // Hashcode: 0x95D27D31 (dec: -1781367503)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union GENERIC_201fake // Hashcode: 0x7031E694 (dec: 1882318484)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MATH3D_76fake // Hashcode: 0x663290A4 (dec: 1714589860)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: math3d.o, math3d.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: math3d.o, math3d.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: math3d.o, math3d.o
};

union MATH3D_77fake // Hashcode: 0xA8CF04EA (dec: -1462827798)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PIPE3D_71fake // Hashcode: 0x1EBB5F62 (dec: 515596130)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: pipe3d.o, pipe3d.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: pipe3d.o, pipe3d.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: pipe3d.o, pipe3d.o
};

union PIPE3D_72fake // Hashcode: 0x17C0E47B (dec: 398517371)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PIPE3D_199fake // Hashcode: 0x416C539D (dec: 1097618333)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union SCRIPT_76fake // Hashcode: 0x07CCD3E7 (dec: 130864103)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: script.o, script.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: script.o, script.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: script.o, script.o
};

union SCRIPT_77fake // Hashcode: 0xA89A9D98 (dec: -1466262120)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SCRIPT_202fake // Hashcode: 0x28E126E1 (dec: 685844193)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union TIMER_71fake // Hashcode: 0x3CBEC8D9 (dec: 1019136217)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: timer.o, timer.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: timer.o, timer.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: timer.o, timer.o
};

union TIMER_72fake // Hashcode: 0x2BECCC5B (dec: 736939099)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SPLINE_71fake // Hashcode: 0x4CDE9A19 (dec: 1289656857)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: spline.o, spline.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: spline.o, spline.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: spline.o, spline.o
};

union SPLINE_72fake // Hashcode: 0x9E0EEC67 (dec: -1643189145)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union FX_71fake // Hashcode: 0xB3324C63 (dec: -1288549277)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: fx.o, fx.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: fx.o, fx.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: fx.o, fx.o
};

union FX_72fake // Hashcode: 0x20C14C3E (dec: 549538878)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union FX_204fake // Hashcode: 0x83EDAC95 (dec: -2081575787)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union TABLES_71fake // Hashcode: 0xBB5F5001 (dec: -1151381503)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: tables.o, tables.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: tables.o, tables.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: tables.o, tables.o
};

union TABLES_72fake // Hashcode: 0x42364A63 (dec: 1110854243)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union VM_71fake // Hashcode: 0x577C0C3A (dec: 1467747386)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: vm.o, vm.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: vm.o, vm.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: vm.o, vm.o
};

union VM_72fake // Hashcode: 0x27CA656B (dec: 667575659)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union VM_199fake // Hashcode: 0x1A8628B8 (dec: 444999864)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MEMPACK_71fake // Hashcode: 0x13120B58 (dec: 319949656)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: mempack.o, mempack.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: mempack.o, mempack.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: mempack.o, mempack.o
};

union MEMPACK_72fake // Hashcode: 0x11956E16 (dec: 295005718)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MEMPACK_188fake // Hashcode: 0x583B4A58 (dec: 1480280664)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MEMPACK_243fake // Hashcode: 0xFF7DC21B (dec: -8535525)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

union GXASSERT_76fake // Hashcode: 0x6E0FED62 (dec: 1846537570)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: gxassert.o, gxassert.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: gxassert.o, gxassert.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: gxassert.o, gxassert.o
};

union GXASSERT_77fake // Hashcode: 0xB6DAB56F (dec: -1227180689)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union AADASSRT_118fake // Hashcode: 0x94794D33 (dec: -1803989709)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: aadassrt.o, aadassrt.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: aadassrt.o, aadassrt.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: aadassrt.o, aadassrt.o
};

union AADASSRT_119fake // Hashcode: 0x8D49167A (dec: -1924589958)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union AADLIB_115fake // Hashcode: 0xBBFB2637 (dec: -1141168585)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: aadlib.o, aadlib.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: aadlib.o, aadlib.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: aadlib.o, aadlib.o
};

union AADLIB_116fake // Hashcode: 0x1F25FD4D (dec: 522583373)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union AADLIB_190fake // Hashcode: 0x9F6EB891 (dec: -1620133743)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union STREAM_76fake // Hashcode: 0x4C037A0C (dec: 1275296268)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: stream.o, stream.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: stream.o, stream.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: stream.o, stream.o
};

union STREAM_77fake // Hashcode: 0x616584A0 (dec: 1634043040)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union STREAM_210fake // Hashcode: 0x3DCA0E3B (dec: 1036652091)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union STREAM_261fake // Hashcode: 0x825DBD24 (dec: -2107785948)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

union PHYSOBS_71fake // Hashcode: 0xF2195964 (dec: -233219740)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: PhysObs.o, PhysObs.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: PhysObs.o, PhysObs.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: PhysObs.o, PhysObs.o
};

union PHYSOBS_72fake // Hashcode: 0x97A84143 (dec: -1750580925)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union RAZCNTRL_71fake // Hashcode: 0x71649135 (dec: 1902416181)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: razcntrl.o, razcntrl.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: razcntrl.o, razcntrl.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: razcntrl.o, razcntrl.o
};

union RAZCNTRL_72fake // Hashcode: 0xF95E971E (dec: -111241442)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union STATE_71fake // Hashcode: 0xAA58AF71 (dec: -1437028495)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: state.o, state.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: state.o, state.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: state.o, state.o
};

union STATE_72fake // Hashcode: 0x5FE524E4 (dec: 1608852708)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union STRMLOAD_76fake // Hashcode: 0x11D0D844 (dec: 298899524)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: strmload.o, strmload.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: strmload.o, strmload.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: strmload.o, strmload.o
};

union STRMLOAD_77fake // Hashcode: 0xBBC25C92 (dec: -1144890222)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SCRIPT2_71fake // Hashcode: 0x96CD1A76 (dec: -1764943242)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: script2.o, script2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: script2.o, script2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: script2.o, script2.o
};

union SCRIPT2_72fake // Hashcode: 0x9E503CEA (dec: -1638908694)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SCRIPT2_203fake // Hashcode: 0xA02D75C0 (dec: -1607633472)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union EVENT_76fake // Hashcode: 0x5ECA26A1 (dec: 1590306465)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: event.o, event.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: event.o, event.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: event.o, event.o
};

union EVENT_77fake // Hashcode: 0xCEC39DA9 (dec: -826040919)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union EVENT_219fake // Hashcode: 0xED7CCDD7 (dec: -310587945)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MONMSG_76fake // Hashcode: 0xE9C1A94D (dec: -373184179)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monmsg.o, monmsg.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monmsg.o, monmsg.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monmsg.o, monmsg.o
};

union MONMSG_77fake // Hashcode: 0x1D317893 (dec: 489781395)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MONMSG_196fake // Hashcode: 0x160BF936 (dec: 369883446)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union VRAM_76fake // Hashcode: 0xFB5FC5A7 (dec: -77609561)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: vram.o, vram.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: vram.o, vram.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: vram.o, vram.o
};

union VRAM_77fake // Hashcode: 0xE4B92D7A (dec: -457626246)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union VRAM_210fake // Hashcode: 0x69CCA192 (dec: 1775018386)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PHYSICS_71fake // Hashcode: 0x71AB8CEE (dec: 1907068142)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: physics.o, physics.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: physics.o, physics.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: physics.o, physics.o
};

union PHYSICS_72fake // Hashcode: 0x4F67EB4B (dec: 1332210507)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PHYSICS_203fake // Hashcode: 0x0B2D1FCE (dec: 187506638)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union REAVER_76fake // Hashcode: 0x4667F1DF (dec: 1181217247)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: reaver.o, reaver.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: reaver.o, reaver.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: reaver.o, reaver.o
};

union REAVER_77fake // Hashcode: 0x1E75973B (dec: 511022907)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union REAVER_213fake // Hashcode: 0x859607D9 (dec: -2053765159)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union GLYPH_76fake // Hashcode: 0xA7755406 (dec: -1485483002)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: glyph.o, glyph.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: glyph.o, glyph.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: glyph.o, glyph.o
};

union GLYPH_77fake // Hashcode: 0x56F23318 (dec: 1458713368)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union GLYPH_213fake // Hashcode: 0x655BB5FA (dec: 1700509178)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union HUMAN_76fake // Hashcode: 0xF9B1F2DA (dec: -105778470)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: human.o, human.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: human.o, human.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: human.o, human.o
};

union HUMAN_77fake // Hashcode: 0x55AD237E (dec: 1437410174)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union HUMAN_193fake // Hashcode: 0x24019F16 (dec: 604086038)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MISSILE_76fake // Hashcode: 0x770AB642 (dec: 1997190722)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: missile.o, missile.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: missile.o, missile.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: missile.o, missile.o
};

union MISSILE_77fake // Hashcode: 0x00D3FAB3 (dec: 13892275)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MISSILE_196fake // Hashcode: 0xC6BB167F (dec: -960817537)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MONAPI_76fake // Hashcode: 0x56316C41 (dec: 1446079553)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monapi.o, monapi.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monapi.o, monapi.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monapi.o, monapi.o
};

union MONAPI_77fake // Hashcode: 0x8FFC132D (dec: -1879305427)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MONAPI_201fake // Hashcode: 0x68ED3057 (dec: 1760374871)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MONLIB_76fake // Hashcode: 0x270AB0E0 (dec: 655012064)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monlib.o, monlib.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monlib.o, monlib.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monlib.o, monlib.o
};

union MONLIB_77fake // Hashcode: 0x3370E575 (dec: 863036789)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MONLIB_208fake // Hashcode: 0x7EC2D3DE (dec: 2126697438)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MONTABLE_71fake // Hashcode: 0xD5F65CA7 (dec: -705274713)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: montable.o, montable.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: montable.o, montable.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: montable.o, montable.o
};

union MONTABLE_72fake // Hashcode: 0x14670472 (dec: 342295666)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MONTABLE_188fake // Hashcode: 0x19B6D4AD (dec: 431412397)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MORLOCK_76fake // Hashcode: 0x5ED080FA (dec: 1590722810)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: morlock.o, morlock.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: morlock.o, morlock.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: morlock.o, morlock.o
};

union MORLOCK_77fake // Hashcode: 0xAAD9422C (dec: -1428602324)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MORLOCK_193fake // Hashcode: 0xF8639DBE (dec: -127689282)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union SLUAGH_76fake // Hashcode: 0x38C12C3D (dec: 952183869)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: sluagh.o, sluagh.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: sluagh.o, sluagh.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: sluagh.o, sluagh.o
};

union SLUAGH_77fake // Hashcode: 0x781A6F6B (dec: 2014998379)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SLUAGH_193fake // Hashcode: 0x537C56CA (dec: 1400657610)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union SOUL_76fake // Hashcode: 0xCC0D2587 (dec: -871553657)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: soul.o, soul.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: soul.o, soul.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: soul.o, soul.o
};

union SOUL_77fake // Hashcode: 0x7DA1E6FA (dec: 2107762426)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SOUL_193fake // Hashcode: 0xC2BEE064 (dec: -1027678108)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MONSENSE_76fake // Hashcode: 0x2B30D328 (dec: 724620072)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monsense.o, monsense.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monsense.o, monsense.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monsense.o, monsense.o
};

union MONSENSE_77fake // Hashcode: 0x25544503 (dec: 626279683)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MONSENSE_196fake // Hashcode: 0xA7338706 (dec: -1489795322)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MONSTER_76fake // Hashcode: 0x93E73915 (dec: -1813563115)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monster.o, monster.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monster.o, monster.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monster.o, monster.o
};

union MONSTER_77fake // Hashcode: 0x2E7BD072 (dec: 779866226)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MONSTER_212fake // Hashcode: 0xDED63187 (dec: -556387961)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union ANIMG2_72fake // Hashcode: 0x67F203D2 (dec: 1743913938)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union ANIMG2_199fake // Hashcode: 0x703D7969 (dec: 1883076969)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union VWRAITH_76fake // Hashcode: 0x3BD14D9A (dec: 1003572634)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: vwraith.o, vwraith.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: vwraith.o, vwraith.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: vwraith.o, vwraith.o
};

union VWRAITH_77fake // Hashcode: 0x1DF0AFEB (dec: 502312939)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union VWRAITH_193fake // Hashcode: 0x3F7542F5 (dec: 1064649461)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union WORSHIP_76fake // Hashcode: 0x67B6B44E (dec: 1740026958)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: worship.o, worship.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: worship.o, worship.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: worship.o, worship.o
};

union WORSHIP_77fake // Hashcode: 0x11B9D34C (dec: 297390924)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union WORSHIP_193fake // Hashcode: 0xB9C6ED01 (dec: -1178145535)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MBMISS_76fake // Hashcode: 0x99AC1F62 (dec: -1716773022)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: mbmiss.o, mbmiss.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: mbmiss.o, mbmiss.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: mbmiss.o, mbmiss.o
};

union MBMISS_77fake // Hashcode: 0x2A18F047 (dec: 706277447)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MBMISS_212fake // Hashcode: 0xA059CF55 (dec: -1604726955)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PLANAPI_113fake // Hashcode: 0xC8F8AFA3 (dec: -923226205)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: planapi.o, planapi.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: planapi.o, planapi.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: planapi.o, planapi.o
};

union PLANAPI_114fake // Hashcode: 0xCACF262B (dec: -892393941)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PLANAPI_211fake // Hashcode: 0xE4701505 (dec: -462416635)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PLANCOLL_76fake // Hashcode: 0x735BBCE0 (dec: 1935391968)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: plancoll.o, plancoll.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: plancoll.o, plancoll.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: plancoll.o, plancoll.o
};

union PLANCOLL_77fake // Hashcode: 0x61131CC4 (dec: 1628642500)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PLANCOLL_216fake // Hashcode: 0x15330FC6 (dec: 355667910)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PLANDRAW_76fake // Hashcode: 0x4C57B53A (dec: 1280816442)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: plandraw.o, plandraw.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: plandraw.o, plandraw.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: plandraw.o, plandraw.o
};

union PLANDRAW_77fake // Hashcode: 0x62B7D6B4 (dec: 1656215220)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PLANDRAW_211fake // Hashcode: 0xCBD20DED (dec: -875426323)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PLANOPT_113fake // Hashcode: 0xCBAEC67C (dec: -877738372)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: planopt.o, planopt.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: planopt.o, planopt.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: planopt.o, planopt.o
};

union PLANOPT_114fake // Hashcode: 0x15E0A7CA (dec: 367044554)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PLANOPT_211fake // Hashcode: 0xC37ABAC4 (dec: -1015366972)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union QUATVM_71fake // Hashcode: 0x3F855776 (dec: 1065703286)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: QuatVM.o, QuatVM.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: QuatVM.o, QuatVM.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: QuatVM.o, QuatVM.o
};

union QUATVM_72fake // Hashcode: 0x0E200AE9 (dec: 236980969)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union ENMYPLAN_76fake // Hashcode: 0xB9153BA5 (dec: -1189790811)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: enmyplan.o, enmyplan.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: enmyplan.o, enmyplan.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: enmyplan.o, enmyplan.o
};

union ENMYPLAN_77fake // Hashcode: 0x3C88A9DD (dec: 1015589341)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union ENMYPLAN_216fake // Hashcode: 0xDAEB41F2 (dec: -622116366)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PLAN_76fake // Hashcode: 0x1EE5CF14 (dec: 518377236)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: plan.o, plan.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: plan.o, plan.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: plan.o, plan.o
};

union PLAN_77fake // Hashcode: 0xF5480003 (dec: -179830781)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PLAN_216fake // Hashcode: 0xA458DD1A (dec: -1537680102)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union INSTNCG2_23fake // Hashcode: 0x7E6DF599 (dec: 2121135513)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: InstncG2.o, InstncG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: InstncG2.o, InstncG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: InstncG2.o, InstncG2.o
};

union INSTNCG2_72fake // Hashcode: 0x7883E999 (dec: 2021910937)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MAING2_71fake // Hashcode: 0xC6D143DA (dec: -959364134)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: MainG2.o, MainG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: MainG2.o, MainG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: MainG2.o, MainG2.o
};

union MAING2_72fake // Hashcode: 0x5E3F6632 (dec: 1581213234)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MATRIXG2_71fake // Hashcode: 0xA0B77D4D (dec: -1598587571)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: MatrixG2.o, MatrixG2.o
};

union MATRIXG2_72fake // Hashcode: 0x83CA3EFF (dec: -2083897601)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MATRIXG2_199fake // Hashcode: 0x4ED53C11 (dec: 1322597393)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union QUATG2_71fake // Hashcode: 0x6D09A304 (dec: 1829348100)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: QuatG2.o, QuatG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: QuatG2.o, QuatG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: QuatG2.o, QuatG2.o
};

union QUATG2_72fake // Hashcode: 0xEADF8295 (dec: -354450795)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union TIMERG2_71fake // Hashcode: 0x8A15530F (dec: -1978313969)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: TimerG2.o, TimerG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: TimerG2.o, TimerG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: TimerG2.o, TimerG2.o
};

union TIMERG2_72fake // Hashcode: 0xD9819D78 (dec: -645816968)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PLANPOOL_113fake // Hashcode: 0x56DC200F (dec: 1457266703)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: planpool.o, planpool.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: planpool.o, planpool.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: planpool.o, planpool.o
};

union PLANPOOL_114fake // Hashcode: 0xC5FBA233 (dec: -973364685)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PLANPOOL_211fake // Hashcode: 0x03C6821C (dec: 63341084)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PLANSRCH_113fake // Hashcode: 0x580A71C1 (dec: 1477079489)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: plansrch.o, plansrch.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: plansrch.o, plansrch.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: plansrch.o, plansrch.o
};

union PLANSRCH_114fake // Hashcode: 0x3FB13975 (dec: 1068579189)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union PLANSRCH_211fake // Hashcode: 0xDCCB3C3B (dec: -590660549)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union CONTROL_71fake // Hashcode: 0x043A48B4 (dec: 70928564)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: control.o, control.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: control.o, control.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: control.o, control.o
};

union CONTROL_72fake // Hashcode: 0xC9FAD521 (dec: -906308319)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union CONTROL_203fake // Hashcode: 0xA503E0F9 (dec: -1526472455)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union RAZIEL_71fake // Hashcode: 0xF5AF9DDD (dec: -173040163)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: raziel.o, raziel.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: raziel.o, raziel.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: raziel.o, raziel.o
};

union RAZIEL_72fake // Hashcode: 0x9204611F (dec: -1845206753)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union RAZIEL_203fake // Hashcode: 0x2DB038C4 (dec: 766523588)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union RAZIEL_257fake // Hashcode: 0x017E7D66 (dec: 25066854)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

union SAVEINFO_76fake // Hashcode: 0xD1DD7996 (dec: -774014570)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: saveinfo.o, saveinfo.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: saveinfo.o, saveinfo.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: saveinfo.o, saveinfo.o
};

union SAVEINFO_77fake // Hashcode: 0xFB46A940 (dec: -79255232)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SAVEINFO_196fake // Hashcode: 0x37750CF1 (dec: 930417905)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union SAVEINFO_223fake // Hashcode: 0x8FDE617F (dec: -1881251457)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

union VOICEXA_86fake // Hashcode: 0x5344B4D5 (dec: 1397011669)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: voicexa.o, voicexa.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: voicexa.o, voicexa.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: voicexa.o, voicexa.o
};

union VOICEXA_87fake // Hashcode: 0xEAE9F72B (dec: -353765589)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union CINEPSX_71fake // Hashcode: 0xC9DD8A44 (dec: -908228028)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: cinepsx.o, cinepsx.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: cinepsx.o, cinepsx.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: cinepsx.o, cinepsx.o
};

union CINEPSX_72fake // Hashcode: 0x52A4E0FA (dec: 1386537210)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union CINEPSX_188fake // Hashcode: 0x6F4C9E8E (dec: 1867292302)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union CINEPSX_211fake // Hashcode: 0x8DD526F5 (dec: -1915410699)
{
	unsigned short pad; // size=0, offset=0
	unsigned short multitap[16]; // size=32, offset=0
	unsigned short mouse[2]; // size=4, offset=0
	unsigned short negcon[3]; // size=6, offset=0
	struct AnalogueData analogue; // size=6, offset=0
};

union MENU_71fake // Hashcode: 0x9417F09E (dec: -1810370402)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: menu.o, menu.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: menu.o, menu.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: menu.o, menu.o
};

union MENU_72fake // Hashcode: 0xE575C374 (dec: -445267084)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MENUUTIL_71fake // Hashcode: 0x6C5644DB (dec: 1817593051)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: menuutil.o, menuutil.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: menuutil.o, menuutil.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: menuutil.o, menuutil.o
};

union MENUUTIL_72fake // Hashcode: 0x9906017D (dec: -1727659651)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MENUDEFS_71fake // Hashcode: 0x2BC919B9 (dec: 734599609)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: menudefs.o, menudefs.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: menudefs.o, menudefs.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: menudefs.o, menudefs.o
};

union MENUDEFS_72fake // Hashcode: 0x8E6F3A2D (dec: -1905313235)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MENUFACE_71fake // Hashcode: 0x1C6ACF56 (dec: 476761942)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: menuface.o, menuface.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: menuface.o, menuface.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: menuface.o, menuface.o
};

union MENUFACE_72fake // Hashcode: 0xFB4CCE52 (dec: -78852526)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MEMCARD_79fake // Hashcode: 0xCBE8E87E (dec: -873928578)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: memcard.o, memcard.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: memcard.o, memcard.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: memcard.o, memcard.o
};

union MEMCARD_80fake // Hashcode: 0xCA90C9EC (dec: -896480788)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MEMCARD_196fake // Hashcode: 0xF7F7C377 (dec: -134757513)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union UNDRWRLD_76fake // Hashcode: 0x6E59B9C2 (dec: 1851374018)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: undrwrld.o, undrwrld.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: undrwrld.o, undrwrld.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: undrwrld.o, undrwrld.o
};

union UNDRWRLD_77fake // Hashcode: 0x395B97AA (dec: 962303914)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union UNDRWRLD_195fake // Hashcode: 0xA8618D78 (dec: -1470001800)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};
/* End unions */

/* Begin external declarations */

extern struct DebugMenuLine mainMenu[8]; // offset 0x800C8418


static struct debug_dispatch_t debug_dispatch_table[5]; // offset 0x800C84D8


extern struct DebugMenuLine *currentMenu; // offset 0x800CDB3C


static char pauseFormatString[20]; // offset 0x800CDAB8


static char mainFormatString[20]; // offset 0x800CDACC


static char *the_format_string; // offset 0x800CDAE0


extern long menuPauseSel; // offset 0x800CDAE4


extern long debug_cameraMode; // offset 0x800CDAE8


extern long debugVoiceIndex; // offset 0x800CDAEC


extern unsigned long debugTimeMult; // offset 0x800CDAF0


extern long debugFogFar; // offset 0x800CDAF4


extern long debugFogNear; // offset 0x800CDAF8


extern long debugGameTime; // offset 0x800CDAFC


extern long debugFogRed; // offset 0x800CDB00


extern long debugFogGrn; // offset 0x800CDB04


extern long debugFogBlu; // offset 0x800CDB08


extern long debugHealthLevel; // offset 0x800CDB0C


extern long debugManaLevel; // offset 0x800CDB10


static long debugMenuChoice; // offset 0x800CDB40


static long cheatCodeLastCommand; // offset 0x800CDB44


static long CheatKeyMap[10]; // offset 0x800CDB48


static struct GameCheat GameCheats[23]; // offset 0x800CDB70


extern struct DebugMenuLine *the_previous_menu; // offset 0x800CDCE0


static int cem_x_base; // offset 0x800CDCE4


static int cem_y_base; // offset 0x800CDCE8


static int cem_cursor_width; // offset 0x800CDCEC


static int cem_line_width; // offset 0x800CDCF0


static int cem_line_leading; // offset 0x800CDCF4


static int cem_item_leading; // offset 0x800CDCF8


extern short AXIS_LENGTH; // offset 0x800CDE5C


extern struct DebugMenuLine standardMenu[1]; // offset 0x800CFCB0


extern struct DebugMenuLine pauseMenu[1]; // offset 0x800CFCC8


extern unsigned long debugRazielFlags1; // offset 0x800CFD40


extern unsigned long debugRazielFlags2; // offset 0x800CFD44


extern unsigned long debugRazielFlags3; // offset 0x800CFD48


extern short debugOrgFogFar; // offset 0x800CFC94


extern short debugOrgFogNear; // offset 0x800CFC54


extern unsigned char debugOrgFogRed; // offset 0x800CFC88


extern unsigned char debugOrgFogGrn; // offset 0x800CFC8C


extern unsigned char debugOrgFogBlu; // offset 0x800CFC90


extern struct DebugMenuLine goodiesMenu[1]; // offset 0x800CFBDC


extern struct DebugMenuLine debugSoundMenu[1]; // offset 0x800CFBAC


extern struct DebugMenuLine debugImbueSoulReaverMenu[1]; // offset 0x800CFD28


extern struct DebugMenuLine debugGlyphAbilitiesMenu[1]; // offset 0x800CFBC4


extern struct DebugMenuLine debugForgedAbilitiesMenu[1]; // offset 0x800CFBF4


extern struct DebugMenuLine debugSpecialAbilitiesMenu[1]; // offset 0x800CFC0C


extern struct DebugMenuLine debugHealthSystemMenu[1]; // offset 0x800CFC3C


extern struct DebugMenuLine debugRazielMenu[1]; // offset 0x800CFD4C


extern struct DebugMenuLine level2SelectMenu[1]; // offset 0x800CFD10


extern struct DebugMenuLine levelSelectMenu[1]; // offset 0x800CFCF8


extern struct DebugMenuLine statsMenu[1]; // offset 0x800CFC98


extern struct DebugMenuLine eventMenu[1]; // offset 0x800CFB64


extern struct DebugMenuLine BossAreasMenu[1]; // offset 0x800CFB7C


extern struct DebugMenuLine saveMenu[1]; // offset 0x800CFC70


extern struct DebugMenuLine mcardMenu[1]; // offset 0x800CFC24


extern struct DebugMenuLine fogMenu[1]; // offset 0x800CFC58


extern struct DebugMenuLine AIMenu[1]; // offset 0x800CFB94


extern struct DebugMenuLine cameraMenu[1]; // offset 0x800CFCE0


extern long SignalEndGame; // offset 0x800CDFBC


extern struct SignalInfo signalInfoList[27]; // offset 0x800CDFC0


extern long dyna_clddyna[8]; // offset 0x800CE104


extern long stat_clddyna[8]; // offset 0x800CE124


extern long dyna_cldstat[8]; // offset 0x800CE144


extern long collide_ignoreAttr; // offset 0x800CE164


extern long collide_acceptAttr; // offset 0x800CE168


extern struct _SVector collide_localPoint; // offset 0x800D0370


extern struct MATRIX *collide_lwTransform; // offset 0x800D0378


extern struct _SVector *collide_point0; // offset 0x800D0360


extern struct _SVector *collide_point1; // offset 0x800D0364


extern struct _SVector *collide_normal0; // offset 0x800D037C


extern struct _SVector *collide_normal1; // offset 0x800D0380


extern long collide_t0; // offset 0x800D0368


extern long collide_t1; // offset 0x800D036C


extern long camera_modeToIndex[15]; // offset 0x800CDF38


static short shorten_count; // offset 0x800CDF74


static short shorten_flag; // offset 0x800CDF76


static short camera_still; // offset 0x800CDF78


static long roll_target; // offset 0x800CDF7C


static long current_roll_amount; // offset 0x800CDF80


static int roll_inc; // offset 0x800CDF84


static short CameraLookStickyFlag; // offset 0x800CDF96


static short panic_count; // offset 0x800CDF9A


extern struct Camera theCamera; // offset 0x800CFE90


static struct MATRIX wcTransformX; // offset 0x800CFD74


static struct MATRIX wcTransform2X; // offset 0x800CFD94


static struct MATRIX cwTransform2X; // offset 0x800CFDB4


static short combat_cam_distance; // offset 0x800CFDD4


static short combat_cam_angle; // offset 0x800CFDD6


static short combat_cam_weight; // offset 0x800CFDD8


static short combat_cam_debounce; // offset 0x800CFDDA


extern int CameraCenterDelay; // offset 0x800D035C


static short CenterFlag; // offset 0x800CFDDC


static struct _SVector camera_shakeOffset[16]; // offset 0x800CFDE4


static struct _SVector camera_plane; // offset 0x800CFE64


static struct _SVector left_point; // offset 0x800CFE6C


static struct _SVector right_point; // offset 0x800CFE74


static short hitline_rot; // offset 0x800CFE7C


static long ACE_amount; // offset 0x800CFE80


static struct _Rotation splinecam_helprot; // offset 0x800CFE84


static short Camera_lookHeight; // offset 0x800CFE8C


static short Camera_lookDist; // offset 0x800CFE8E


extern long campoint_hw; // offset 0x800D0354


extern long campoint_hh; // offset 0x800D0358


extern long draw_belowSplit; // offset 0x800CE19C


extern short forceDrawInFront; // offset 0x800CE198


extern SVECTOR*face_v0; // offset 0x800CE1A0


extern SVECTOR*face_v1; // offset 0x800CE1A4


extern SVECTOR*face_v2; // offset 0x800CE1A8


extern SVECTOR*face_v01; // offset 0x800CE1AC


extern SVECTOR*face_v12; // offset 0x800CE1B0


extern SVECTOR*face_v20; // offset 0x800CE1B4


extern struct UVTYPE *face_uv0; // offset 0x800CE1B8


extern struct UVTYPE *face_uv1; // offset 0x800CE1BC


extern struct UVTYPE *face_uv2; // offset 0x800CE1C0


extern struct UVTYPE *face_uv01; // offset 0x800CE1C4


extern struct UVTYPE *face_uv12; // offset 0x800CE1C8


extern struct UVTYPE *face_uv20; // offset 0x800CE1CC


extern long modelDQP; // offset 0x800D0388


extern unsigned char *draw_stack; // offset 0x800D0384


extern SVECTORshadow_vertices[11]; // offset 0x800D038C


static struct LightGroup default_lightgroup; // offset 0x800CE460


extern void *light_light; // offset 0x800D0D78


extern struct LightInfo *light_lightInfo; // offset 0x800D0D80


extern struct _Terrain *light_terrain; // offset 0x800D0D7C


extern char HashExtensions[7][4]; // offset 0x800CE4D0


extern int _ramsize; // offset 0x800CE558


extern int _stacksize; // offset 0x800CE55C


extern short gEndGameNow; // offset 0x800CE562


extern int pcverify; // offset 0x800CE564


extern long DoMainMenu; // offset 0x800CE6C0


extern long mainMenuMode; // offset 0x800CE6C4


extern long *mainMenuScreen; // offset 0x800CE6C8


extern long *controlsScreen; // offset 0x800CE6CC


extern short mainMenuFadeInTime; // offset 0x800CE6D0


extern short mainMenuFading; // offset 0x800CE6D2


extern struct GameTracker gameTrackerX; // offset 0x800D0FAC


extern short mainMenuTimeOut; // offset 0x800D0FA8


static int mainMenuSfx; // offset 0x800D0E1C


extern struct _ButtonTexture PauseButtons[4][2]; // offset 0x800D122C


extern struct _ButtonTexture BmenuLines[3][2]; // offset 0x800D0F24


extern struct _ButtonTexture BexitLine[2]; // offset 0x800D0E20


extern struct FontPos fontPos[72]; // offset 0x800C8500


extern char charMap[92][3]; // offset 0x800C8620


static struct font_color_t the_font_color_table[5]; // offset 0x800CE1D0


extern unsigned char fontTransTable[128]; // offset 0x800CE224


extern struct FontTracker fontTracker; // offset 0x800D05EC


extern unsigned short SpecialFogClut; // offset 0x800D05E8


static char fp_str[512]; // offset 0x800D03E4


extern long cameraMode; // offset 0x800CE2A4


extern long playerCameraMode; // offset 0x800CE2A8


extern struct Object *fontsObject; // offset 0x800CE2AC


extern struct _FXTracker *gFXT; // offset 0x800D0C44


extern unsigned long StackSave; // offset 0x800D0C54


extern char *primBase; // offset 0x800D0C5C


static struct _PolytopeList *gPolytopeList; // offset 0x800D0C1C


static struct _InstanceList *instanceList; // offset 0x800D0C20


static struct _InstancePool *instancePool; // offset 0x800D0C24


extern struct _FXTracker *fxTracker; // offset 0x800D0C58


extern short RENDER_currentStreamUnitID; // offset 0x800D0C4C


static short pause_redraw_flag; // offset 0x800D0C3C


static void *pause_redraw_prim; // offset 0x800D0C40


extern int InStreamUnit; // offset 0x800D0C48


extern unsigned long **hackOT; // offset 0x800D0C50


extern unsigned char dualshock_motors[2]; // offset 0x800C875C


extern unsigned char dualshock_align[6]; // offset 0x800C8760


static short align_flag; // offset 0x800CE358


static short dualshock_onflag; // offset 0x800CE35A


static int dualShock; // offset 0x800CE35C


extern unsigned short lastData[2]; // offset 0x800CE3A0


static int gpSaved; // offset 0x800CE3A4


extern int gamePadControllerOut; // offset 0x800D0D70


extern unsigned char controllerType[2]; // offset 0x800D0CC8


static int ignoreFind; // offset 0x800D0C64


static long dualshock0_time; // offset 0x800D0C68


static long dualshock1_time; // offset 0x800D0C6C


extern unsigned short psxData[2]; // offset 0x800D0CCC


static long overrideData[2][5]; // offset 0x800D0C70


static long overrideCommand[2][5]; // offset 0x800D0CA0


static long objectOneShotTriggerTbl[3]; // offset 0x800CEA90


extern struct SoundEffectChannel soundEffectChannelTbl[16]; // offset 0x800D12C0


extern char soundBuffer[13256]; // offset 0x800D5C08


extern struct LightInfo *sbsp_lightInfo; // offset 0x800D12AC


extern struct ObjectAccess objectAccess[28]; // offset 0x800C877C


extern struct ObjectFunc objectFunc[7]; // offset 0x800C885C


extern short identityMatrix[3][3]; // offset 0x800CE870


extern char (*selectMenuText[5]); // offset 0x800C8768


extern long FRAC_BITS; // offset 0x800CEB3C


static struct _FXGeneralEffect *FX_GeneralEffectTracker; // offset 0x800CEB64


static struct _FXRibbon *FX_ConstrictRibbon; // offset 0x800CEB68


static short FX_ConstrictStage; // offset 0x800CEB6C


static struct _Instance *FX_ConstrictInstance; // offset 0x800CEB70


static short snow_amount; // offset 0x800CEB74


static short rain_amount; // offset 0x800CEB76


static short current_rain_fade; // offset 0x800CEB78


static struct _Instance *FX_reaver_instance; // offset 0x800CEB7C


static short windx; // offset 0x800CEBA4


static short windy; // offset 0x800CEBA6


static short wind_deg; // offset 0x800CEBA8


static short wind_speed; // offset 0x800CEBAA


extern long FX_ColorArray[6]; // offset 0x800CEBAC


static short Spiral_Number; // offset 0x800CEBC4


static short Spiral_Degrees; // offset 0x800CEBC6


static long Spiral_Current; // offset 0x800CEBC8


static long Spiral_Max; // offset 0x800CEBCC


static struct _Position FX_ConstrictPosition; // offset 0x800D1594


static struct _Position *FX_ConstrictPositionPtr; // offset 0x800D159C


static struct _FX_PRIM *FX_LastUsedPrim; // offset 0x800D15A0


static short FX_Frames; // offset 0x800D15A4


static short FX_TimeCount; // offset 0x800D15A6


static struct DVECTOR Spiral_Array[65]; // offset 0x800D15B4


static struct DVECTOR Spiral_OffsetP[64]; // offset 0x800D16C4


static struct DVECTOR Spiral_OffsetM[64]; // offset 0x800D17C4


static int Spiral_Glow_X; // offset 0x800D18C4


static int Spiral_Glow_Y; // offset 0x800D18C8


static int Spiral_Glow_Size; // offset 0x800D18CC


static int Spiral_Mod; // offset 0x800D18D0


extern struct _Normal gNormalList[244]; // offset 0x800C8970


extern short ecostable[3072]; // offset 0x800C9110


static long vmRealClock; // offset 0x800CEBD4


static long vmClock; // offset 0x800CEBD8


static struct NewMemTracker newMemTracker; // offset 0x800D18D4


static char midiDataByteCount[8]; // offset 0x800CED84


extern unsigned long aadGp; // offset 0x800CA910


static unsigned short aadHblanksPerUpdate[4]; // offset 0x800CECC0


static unsigned long aadUpdateRate[4]; // offset 0x800CECC8


static int gDefragRequest; // offset 0x800CECDC


static unsigned long aadReverbModeSize[10]; // offset 0x800CED5C


static unsigned long __hblankEvent; // offset 0x800D18E8


static unsigned char *smfDataPtr; // offset 0x800D18EC


static unsigned long smfBytesLeft; // offset 0x800D18F0


static struct AadDynamicSfxLoadInfo *smfInfo; // offset 0x800D18F4


static struct AadProgramAtr aadDummySfxProgram; // offset 0x800CEF48


extern unsigned short aadPitchTable[85]; // offset 0x800CEF50


extern unsigned long aadStepsPerCent[85]; // offset 0x800CEFFC


extern short aadStepsPerSemitone[85]; // offset 0x800CF150


extern long CurrentWarpNumber; // offset 0x800CF1FC


extern struct WarpRoom WarpRoomArray[14]; // offset 0x800CF200

extern struct STREAM_294fake WarpGateLoadInfo; // offset 0x800D18FC


extern struct _TFace *MORPH_SavedFace; // offset 0x800D1D20


extern struct Level *MORPH_SavedLevel; // offset 0x800D1910


extern short MORPH_Component[3]; // offset 0x800D1914


extern short MORPH_Track[2]; // offset 0x800D191C


static int s_zval; // offset 0x800D18F8


extern long RazielCommands[10]; // offset 0x800CF560


static int Pending; // offset 0x800CF588


static int Up; // offset 0x800CF58C


static int Down; // offset 0x800CF590


static struct _G2AnimAlphaTable_Type (*G2AlphaTables[7]); // offset 0x800CF594


static void *circWhere; // offset 0x800CF5B0


static char circBuf[4096]; // offset 0x800D2E2C


static int loadFromHead; // offset 0x800CF4E0


static int loadCanFail; // offset 0x800CF4E4


static short EventAbortLine; // offset 0x800CF534


static short EventJustRecievedTimer; // offset 0x800CF536


static long MoveCodeStreamExtra; // offset 0x800CF538


static long CurrentEventLine; // offset 0x800CF53C


static long EventCurrentEventIndex; // offset 0x800CF540


extern long NumSignalsToReset; // offset 0x800CF548


static struct Event *currentEventInstance; // offset 0x800D2734


static struct ScriptPCode *currentActionScript; // offset 0x800D2738


static short *EventAbortedPosition; // offset 0x800D273C


extern long eventListNumInstances[20]; // offset 0x800D2A1C


extern struct _Instance (*eventListArray[20][10]); // offset 0x800D2B0C


static struct WaterLevelProcess WaterLevelArray[5]; // offset 0x800D2744


static struct Level *CurrentPuzzleLevel; // offset 0x800D27D0


extern struct SignalResetStruct ResetSignalArray[16]; // offset 0x800D2A74



extern long VRAM_NeedToUpdateMorph; // offset 0x800CF5B4


extern short M_TrackClutUpdate; // offset 0x800D4804


extern struct _BlockVramEntry TerrainVramBlock; // offset 0x800D480C


static short FireReaverFlag; // offset 0x800D48B8


static short fx_going; // offset 0x800CF5CC


extern struct _FXBlastringEffect *fx_blastring; // offset 0x800CF5D0


static short HUD_Captured; // offset 0x800CF5D4


static short MANNA_Position; // offset 0x800D48BC


static short MANNA_Pos_vel; // offset 0x800D48BE


static short glyph_time; // offset 0x800D48C0


static short glyph_trigger; // offset 0x800D48C2


static short glyph_cost; // offset 0x800D48C4


static short HUD_Count; // offset 0x800D48C6


static short HUD_Count_Overall; // offset 0x800D48C8


static int blast_range; // offset 0x800D48CC


static int fx_radius_old; // offset 0x800D48D0


static int MANNA_Pickup_Time; // offset 0x800D48D4


static short HUD_Rotation; // offset 0x800D48D8


static short HUD_Rot_vel; // offset 0x800D48DA


static short HUD_Position; // offset 0x800D48DC


static short HUD_Pos_vel; // offset 0x800D48DE


static short HUD_State; // offset 0x800D48E0


static short HUD_Wait; // offset 0x800D48E2


static struct _SVector HUD_Cap_Pos; // offset 0x800D48E4


static struct _SVector HUD_Cap_Vel; // offset 0x800D48EC


static int warpDraw; // offset 0x800D48F4


static int glowdeg; // offset 0x800D48F8


extern int hud_warp_arrow_flash; // offset 0x800D48FC


extern struct _MonsterStateChoice HUMAN_StateChoiceTable[6]; // offset 0x800CA978


extern struct _MonsterFunctionTable HUMAN_FunctionTable; // offset 0x800CF5E0


static struct MissileGraphics MISSILE_objectTable[4]; // offset 0x800CA9C0


static struct MONTABLE_207fake functionChoiceTable[6]; // offset 0x800CA9F0


static struct _MonsterState DefaultStateTable[31]; // offset 0x800CAA20


static struct _MonsterFunctionTable DefaultFunctionTable; // offset 0x800CF68C


extern struct _MonsterStateChoice SLUAGH_StateChoiceTable[4]; // offset 0x800CAB18


extern struct _MonsterFunctionTable SLUAGH_FunctionTable; // offset 0x800CF6AC


extern struct _MonsterStateChoice SOUL_StateChoiceTable[7]; // offset 0x800CAB48


extern struct _MonsterFunctionTable SOUL_FunctionTable; // offset 0x800CF6D4


static int gNumMonsters; // offset 0x800CF62C


static int gNumSpectralMonsters; // offset 0x800CF630


static int gNumMaterialMonsters; // offset 0x800CF634


static struct RadarInfo radarDir[8]; // offset 0x800CF638


static unsigned char radarDirIndex[8]; // offset 0x800CF668


static struct _MonsterVars (*monsterSenseArray[40]); // offset 0x800D4900


static char monsterSensed[40]; // offset 0x800D49A0


static int lastSenseFrame; // offset 0x800D49C8


static struct burntTuneType burntTest; // offset 0x800CF674


static int _indexTable[16]; // offset 0x800CAC2C


static int _stepSizeTable[64]; // offset 0x800CAC6C


static struct _G2AnimChanStatusBlockPool_Type _chanStatusBlockPool; // offset 0x800D49CC


static struct _G2AnimSegValue_Type _segValues[80]; // offset 0x800D49DC


static struct _G2AnimControllerPool_Type _controllerPool; // offset 0x800D515C


static struct _G2AnimInterpStateBlockPool_Type _interpStateBlockPool; // offset 0x800D516C


extern struct _MonsterStateChoice VWRAITH_StateChoiceTable[7]; // offset 0x800CAB9C


extern struct _MonsterFunctionTable VWRAITH_FunctionTable; // offset 0x800CF6F4


extern struct _MonsterStateChoice WORSHIP_StateChoiceTable[5]; // offset 0x800CABF0


extern struct _MonsterFunctionTable WORSHIP_FunctionTable; // offset 0x800CF714


extern struct PoolManagementData *poolManagementData; // offset 0x800D51D0


static struct PoolManagementData poolManagementDataStorage; // offset 0x800D5184


static long timerArray[10]; // offset 0x800D51A4


static long timerIndex; // offset 0x800D51CC


static struct _fth directionList[6]; // offset 0x800CF75C


static struct _G2AppDataVM_Type _appDataVM; // offset 0x800D517C


static struct __Force *ExternalForces; // offset 0x800D51D4


static struct MATRIX TempMat; // offset 0x800D51E4


extern int LoopCounter; // offset 0x800CF774


static int WaterStatus; // offset 0x800CF778


static unsigned char PickupList[4]; // offset 0x800CF77C


static unsigned short cowlList[19]; // offset 0x800CF780


extern struct __razController controllerList[24]; // offset 0x800CF7A8


extern struct __CannedSound cannedSound[4]; // offset 0x800CF808


static int AlgoControlFlag; // offset 0x800CF858


static int ZoneDelta; // offset 0x800CF85C


extern struct __Idle **currentIdleList; // offset 0x800CF874


extern int currentIdleListCount; // offset 0x800CF878


extern int currentIdleCount; // offset 0x800CF87C


static int BlockCount; // offset 0x800CF880


static int LastBlock; // offset 0x800CF884


static struct __Player Raziel; // offset 0x800D5214


static struct RazielData *PlayerData; // offset 0x800D57D0


static struct _G2AnimInterpInfo_Type razInterpInfo[3]; // offset 0x800D57D4


static struct __FitPoint constrictData[32]; // offset 0x800D5804


static struct MATRIX TempMat; // offset 0x800D5884


static struct GameTracker *gameTracker; // offset 0x800D58A4


static long *PadData; // offset 0x800D58A8


static struct _G2SVector3_Type ExtraRotData; // offset 0x800D58AC


static struct _G2SVector3_Type *ExtraRot; // offset 0x800D58B4


extern int ControlFlag; // offset 0x800D59AC


static struct __Force ExternalForces[4]; // offset 0x800D58C4


static int PhysicsMode; // offset 0x800D5914


static struct __EngagedInstance EngagedList[15]; // offset 0x800D5924


static struct _Normal Norm; // offset 0x800D599C


extern int AutoFaceAngle; // offset 0x800D59A8


static int LastRC; // offset 0x800D59A4


extern struct _GlobalSaveTracker *GlobalSave; // offset 0x800CF8A8


static long numbufferedIntros; // offset 0x800CF8AC


static int the_header_size; // offset 0x800CF8B0


extern long SaveArraySize[10]; // offset 0x800CF8B4


static struct SavedInfoTracker savedInfoTracker; // offset 0x800D59C0


static struct SavedBasic (*bufferSavedIntroArray[64]); // offset 0x800D59D0


extern char monVersion[12]; // offset 0x800CF95C


extern int StCdIntrFlag; // offset 0x800D8FD0

static struct menu_sound_entry_t the_menu_sounds[6]; // offset 0x800CAD98


static char (*the_attract_movies[4]); // offset 0x800CADC8


static int MAIN_YPOS; // offset 0x800CFA48


static int PAUSE_YPOS; // offset 0x800CFA4C


static int MAIN_XPOS; // offset 0x800CFA50


static int MAIN_WIDTH; // offset 0x800CFA54


static int PAUSE_XPOS; // offset 0x800CFA58


static int PAUSE_WIDTH; // offset 0x800CFA5C


static int LINESKIP; // offset 0x800CFA60


static int ITEMSKIP; // offset 0x800CFA64


static int hack_reset_attract; // offset 0x800CFA68


static int StartGameFading; // offset 0x800CFA6C


static int hack_attract; // offset 0x800CFA70


static int hack_attract_movie; // offset 0x800CFA74


extern long *mainMenuScreen; // offset 0x4


static struct menuface_t MenuFaces[8]; // offset 0x800CFABC


static int hack_initialized; // offset 0x800CFB3C


static struct _ButtonTexture *FaceButtons; // offset 0x800D5C04


extern struct mcmenu_table_t the_mcmenu_table; // offset 0x24


static struct MEMCARD_216fake gMcmenu; // offset 0x800D8FD4


extern struct memcard_t gMemcard; // offset 0x800D91D4


static struct UW_ScreenXY *ScreenMorphArray; // offset 0x800CF894


static long UW_angle; // offset 0x800D59B0


static long UW_scalex; // offset 0x800D59B4


static long UW_scalexInc; // offset 0x800D59B8


static long UW_angleInc; // offset 0x800D59BC

/* End external declarations */

#endif
