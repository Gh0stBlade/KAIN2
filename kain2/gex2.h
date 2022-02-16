#pragma once

enum _G2Bool_Enum // Hashcode: 0xFDD7E4E7 (dec: -36182809)
{
	G2FALSE = 0,
};

struct _G2SVector3_Type // hashcode: 0x60FF7CCD (dec: 1627356365)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

struct _G2LVector3_Type // hashcode: 0xD8D79EE7 (dec: -656957721)
{
	long x; // size=0, offset=0
	long y; // size=0, offset=4
	long z; // size=0, offset=8
};

struct _G2Matrix_Type // hashcode: 0x647D6F57 (dec: 1685942103)
{
	short rotScale[3][3]; // size=18, offset=0
	short scaleFlag; // size=0, offset=18
	struct _G2LVector3_Type trans; // size=12, offset=20
};

struct _G2Quat_Type // hashcode: 0xC71F9EB1 (dec: -954229071)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short w; // size=0, offset=6
};

struct _G2AnimQuatInfo_Type // hashcode: 0x5A520473 (dec: 1515324531)
{
	struct _G2Quat_Type srcQuat; // size=8, offset=0
	struct _G2Quat_Type destQuat; // size=8, offset=8
	struct _G2SVector3_Type srcScale; // size=6, offset=16
	struct _G2SVector3_Type destScale; // size=6, offset=22
	struct _G2SVector3_Type srcTrans; // size=6, offset=28
	struct _G2SVector3_Type destTrans; // size=6, offset=34
};

struct _G2AnimAlphaTable_Type // hashcode: 0x2288BF48 (dec: 579387208)
{
	unsigned short size; // size=0, offset=0
	long data[1]; // size=0, offset=4
};

struct _G2AnimInterpStateBlock_Type // hashcode: 0xFB03DD89 (dec: -83632759)
{
	struct _G2AnimInterpStateBlock_Type *next; // size=164, offset=0
	struct _G2AnimQuatInfo_Type quatInfo[4]; // size=160, offset=4
};

struct _G2AnimInterpInfo_Type // hashcode: 0xAE13EB6F (dec: -1374426257)
{
	short duration; // size=0, offset=0
	short targetTime; // size=0, offset=2
	struct _G2AnimAlphaTable_Type *alphaTable; // size=4, offset=4
	struct _G2AnimInterpStateBlock_Type *stateBlockList; // size=164, offset=8
};

struct _G2AnimChanStatus_Type // hashcode: 0x575B81AB (dec: 1465614763)
{
	short keyData; // size=0, offset=0
	short index; // size=0, offset=2
};

struct _G2AnimChanStatusBlock_Type // hashcode: 0xC6C09C4F (dec: -960455601)
{
	struct _G2AnimChanStatusBlock_Type *next; // size=36, offset=0
	struct _G2AnimChanStatus_Type chunks[8]; // size=32, offset=4
};

struct _G2AnimFxHeader_Type // hashcode: 0xA004B4ED (dec: -1610304275)
{
	unsigned char sizeAndSection; // size=0, offset=0
	char type; // size=0, offset=1
	unsigned short keyframeID; // size=0, offset=2
};

struct _G2AnimKeylist_Type // hashcode: 0x87A15165 (dec: -2019471003)
{
	unsigned char sectionCount; // size=0, offset=0
	unsigned char s0TailTime; // size=0, offset=1
	unsigned char s1TailTime; // size=0, offset=2
	unsigned char s2TailTime; // size=0, offset=3
	unsigned short keyCount; // size=0, offset=4
	short timePerKey; // size=0, offset=6
	unsigned short pad00; // size=0, offset=8
	short pad01; // size=0, offset=10
	unsigned short pad10; // size=0, offset=12
	short pad11; // size=0, offset=14
	struct _G2AnimFxHeader_Type *fxList; // size=4, offset=16
	unsigned short (*sectionData[1]); // size=0, offset=20
};

struct _G2AnimSection_Type // hashcode: 0xFC80328C (dec: -58707316)
{
	unsigned char flags; // size=0, offset=0
	unsigned char sectionID; // size=0, offset=1
	unsigned char firstSeg; // size=0, offset=2
	unsigned char segCount; // size=0, offset=3
	short elapsedTime; // size=0, offset=4
	short storedTime; // size=0, offset=6
	short loopStartTime; // size=0, offset=8
	short loopEndTime; // size=0, offset=10
	short *swAlarmTable; // size=0, offset=12
	long speedAdjustment; // size=0, offset=16
	unsigned short keylistID; // size=0, offset=20
	unsigned long alarmFlags; // size=0, offset=24
	long (*callback)(); // size=0, offset=28
	void *callbackData; // size=0, offset=32
	struct _G2AnimKeylist_Type *keylist; // size=20, offset=36
	struct _G2AnimChanStatusBlock_Type *chanStatusBlockList; // size=36, offset=40
	struct _G2AnimInterpInfo_Type *interpInfo; // size=12, offset=44
};

struct _G2Anim_Type // hashcode: 0xF35FCCD2 (dec: -211825454)
{
	unsigned char sectionCount; // size=0, offset=0
	unsigned char masterSection; // size=0, offset=1
	unsigned short controllerList; // size=0, offset=2
	unsigned short disabledControllerList; // size=0, offset=4
	unsigned short pad; // size=0, offset=6
	struct _G2SVector3_Type rootTrans; // size=6, offset=8
	unsigned short flags; // size=0, offset=14
	struct _Model *modelData; // size=0, offset=16
	struct _G2Matrix_Type *segMatrices; // size=32, offset=20
	unsigned long disabledBits[3]; // size=12, offset=24
	struct _G2AnimSection_Type section[3]; // size=144, offset=36
};
