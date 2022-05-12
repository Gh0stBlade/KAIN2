#ifndef QUATG2_H
#define QUATG2_H

//struct _G2AnimSection_Type {
//	unsigned char flags; // size=0, offset=0
//	unsigned char sectionID; // size=0, offset=1
//	unsigned char firstSeg; // size=0, offset=2
//	unsigned char segCount; // size=0, offset=3
//	short elapsedTime; // size=0, offset=4
//	short storedTime; // size=0, offset=6
//	short loopStartTime; // size=0, offset=8
//	short loopEndTime; // size=0, offset=10
//	short* swAlarmTable; // size=0, offset=12
//	long speedAdjustment; // size=0, offset=16
//	unsigned short keylistID; // size=0, offset=20
//	unsigned long alarmFlags; // size=0, offset=24
//	long* callback(); // size=0, offset=28
//	void* callbackData; // size=0, offset=32
//	struct _G2AnimKeylist_Type* keylist; // size=20, offset=36
//	struct _G2AnimChanStatusBlock_Type* chanStatusBlockList; // size=36, offset=40
//	struct _G2AnimInterpInfo_Type* interpInfo; // size=12, offset=44
//};

//struct _G2SVector3_Type 
//{
//	short x; // size=0, offset=0
//	short y; // size=0, offset=2
//	short z; // size=0, offset=4
//};

//struct _G2Anim_Type 
//{
//	unsigned char sectionCount; // size=0, offset=0
//	unsigned char masterSection; // size=0, offset=1
//	unsigned short controllerList; // size=0, offset=2
//	unsigned short disabledControllerList; // size=0, offset=4
//	unsigned short pad; // size=0, offset=6
//	struct _G2SVector3_Type rootTrans; // size=6, offset=8
//	unsigned short flags; // size=0, offset=14
//	struct _Model* modelData; // size=0, offset=16
//	struct _G2Matrix_Type* segMatrices; // size=32, offset=20
//	unsigned long disabledBits[3]; // size=12, offset=24
//	struct _G2AnimSection_Type section[3]; // size=144, offset=36
//};
//
//struct _G2Quat_Type 
//{
//	short x; // size=0, offset=0
//	short y; // size=0, offset=2
//	short z; // size=0, offset=4
//	short w; // size=0, offset=6
//};

extern void G2Quat_ToEuler(struct _G2Quat_Type *quat, struct _G2EulerAngles_Type *euler, int order); // 0x80095ED8
extern void G2EulerAngles_FromMatrix(struct _G2EulerAngles_Type *euler, struct _G2Matrix_Type *matrix, long order); // 0x80095F1C

#endif
