#ifndef SAVEINFO_H
#define SAVEINFO_H

#include "CORE.H"
#include "MONSTER/MONAPI.H"

struct PlayerSaveData 
{
	unsigned long abilities; // size=0, offset=0
	unsigned long currentPlane; // size=0, offset=4
	unsigned long healthScale; // size=0, offset=8
	unsigned long healthBalls; // size=0, offset=12
	unsigned short manaBalls; // size=0, offset=16
	unsigned short manaMax; // size=0, offset=18
	unsigned long playerEventHistory; // size=0, offset=20
};

struct _SmallRotation 
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

struct _SavedIntroWithIntro 
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
	_Position position; // size=6, offset=26
};

struct SavedEvent 
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	short eventNumber; // size=0, offset=4
	short eventVariables[5]; // size=10, offset=6
};

struct _SavedIntro 
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
	_Position position; // size=6, offset=26
	long flags; // size=0, offset=32
	long flags2; // size=0, offset=36
};

struct SavedIntroSmall 
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short introUniqueID; // size=0, offset=2
};

struct SavedIntroSpline 
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short introUniqueID; // size=0, offset=2
	short splineFlags; // size=0, offset=4
	short splineKeyFrame; // size=0, offset=6
	short splineClipBeg; // size=0, offset=8
	short splineClipEnd; // size=0, offset=10
};

struct SavedBasic // hashcode: 0x6198E1EF (dec: 1637409263)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
};

struct _GlobalSaveTracker {
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

struct SavedBspTree {
	struct _Position localOffset; // size=6, offset=0
	unsigned char importantFlagsSaved; // size=0, offset=6
	unsigned char bspIndex; // size=0, offset=7
};
struct ActualSavedLevel {
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	short waterZ; // size=0, offset=4
	short numberBSPTreesSaved; // size=0, offset=6
	struct SavedBspTree bspTreeArray[1]; // size=8, offset=8
};

struct SavedLevel {
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	short waterZ; // size=0, offset=4
	short numberBSPTreesSaved; // size=0, offset=6
};

extern struct _GlobalSaveTracker* GlobalSave;

extern void SAVE_GetInstanceRotation(struct _Instance *instance, struct _SmallRotation *vector); // 0x800B5560

extern void SAVE_ClearMemory(struct GameTracker *gameTracker); // 0x800B55E8

extern void SAVE_Init(struct GameTracker *gt); // 0x800B5668

extern void * SAVE_GetSavedBlock(long saveType, long extraSize); // 0x800B56EC

extern long SAVE_PurgeAMemoryBlock(); // 0x800B5808

extern long SAVE_SaveableInstance(struct _Instance *instance); // 0x800B58A8

extern struct _SavedIntro * SAVE_UpdateSavedIntro(struct _Instance *instance, struct Level *level, struct _SavedIntro *savedIntro, struct evControlSaveDataData *extraData); // 0x800B59A4

extern struct _SavedIntroWithIntro * SAVE_UpdateSavedIntroWithIntro(struct _Instance *instance, struct Level *level, struct _SavedIntroWithIntro *savedIntro, struct evControlSaveDataData *extraData); // 0x800B5AB0

extern struct SavedBasic * SAVE_GetSavedEvent(long areaID, long eventNumber); // 0x800B5BE8

extern void SAVE_DeleteSavedEvent(long areaID, long eventNumber); // 0x800B5C90

extern struct SavedBasic * SAVE_GetSavedNextEvent(long areaID, struct SavedBasic *curSave); // 0x800B5CC0

extern void SAVE_BufferIntro(struct SavedBasic *savedIntro); // 0x800B5D64

extern void SAVE_IntroduceBufferIntros(); // 0x800B5DE4

extern void SAVE_IntroForStreamID(struct _StreamUnit *streamUnit); // 0x800B5EC0

extern long SAVE_HasSavedIntro(struct Intro *intro, long currentStreamID); // 0x800B5F9C

extern struct SavedLevel * SAVE_HasSavedLevel(long areaID); // 0x800B602C

extern void SAVE_UpdateLevelWithSave(struct _StreamUnit *streamUnit); // 0x800B6090

extern struct SavedLevel * SAVE_CreatedSavedLevel(long areaID, struct Level *level); // 0x800B61DC

extern void SAVE_DeleteBlock(struct SavedBasic *savedBlock); // 0x800B6398

extern void SAVE_Instance(struct _Instance *instance, struct Level *level); // 0x800B6424

extern void SAVE_DeleteInstance(struct _Instance *instance); // 0x800B6658

extern void SAVE_SetDeadDeadBit(int uniqueID, long set); // 0x800B6728

extern void SAVE_RestoreGlobalSavePointer(); // 0x800B67EC

extern long SAVE_IsUniqueIDDeadDead(long uniqueID); // 0x800B6844

extern long SAVE_IsIntroDeadDead(struct Intro *intro); // 0x800B68F0

extern void SAVE_DoInstanceDeadDead(struct _Instance *instance); // 0x800B6914

extern void SAVE_MarkDeadDead(struct _Instance *instance); // 0x800B6944

extern void SAVE_UndestroyInstance(struct _Instance *instance); // 0x800B6958

extern struct SavedIntroSmall * SAVE_GetSavedSmallIntro(struct _Instance *instance); // 0x800B697C

extern struct SavedIntroSpline * SAVE_GetIntroSpline(struct _Instance *instance); // 0x800B69E8

extern void SAVE_UpdateGlobalSaveTracker(); // 0x800B6A54

extern void SAVE_RestoreGlobalSaveTracker(); // 0x800B6AE8

extern void SAVE_SaveEverythingInMemory(); // 0x800B6BB8

extern void SAVE_SaveGame(); // 0x800B6C6C

extern void SAVE_RestoreGame(); // 0x800B6CB8

extern long SAVE_SizeOfFreeSpace(); // 0x800B6D1C


#endif
