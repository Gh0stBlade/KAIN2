#ifndef ANIMG2_H
#define ANIMG2_H

#include "Game/gex2.h"

struct _G2AnimController_Type // hashcode: 0x74838F3C (dec: 1954778940)
{
	unsigned short next; // size=0, offset=0
	unsigned char type; // size=0, offset=2
	unsigned char segNumber; // size=0, offset=3
	void* callbackData; // size=0, offset=4
	unsigned short flags; // size=0, offset=8
	short duration; // size=0, offset=10
	short elapsedTime; // size=0, offset=12
	struct _G2AnimAlphaTable_Type* alphaTable; // size=4, offset=16
	union {
		struct {
			unsigned long (*function)(struct _G2AnimController_Type*, struct _G2AnimSegValue_Type*, struct _G2Matrix_Type*, struct _G2Matrix_Type*); // size=0, offset=0
			void* fnData; // size=0, offset=4
		} callback; // size=8, offset=0, found in object files: AnimG2.o, AnimG2.o
		struct {
			struct _G2SVector3_Type base; // size=6, offset=0
			struct _G2SVector3_Type offset; // size=6, offset=6
		} vector; // size=12, offset=0, found in object files: AnimG2.o, AnimG2.o
		struct {
			struct _G2Quat_Type src; // size=8, offset=0
			struct _G2Quat_Type dest; // size=8, offset=8
		} quat; // size=16, offset=0, found in object files: AnimG2.o, AnimG2.o
	} data; // size=16, offset=20, found in object files: AnimG2.o, AnimG2.o
};

struct ANIMG2_109fake // hashcode: 0x1C77A67E (dec: 477603454)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: AnimG2.o, AnimG2.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: AnimG2.o, AnimG2.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: AnimG2.o, AnimG2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: AnimG2.o, AnimG2.o
};

struct _G2AnimSegKeyflagInfo_Type // hashcode: 0x5786AAF7 (dec: 1468443383)
{
	unsigned long* stream; // size=0, offset=0
	unsigned long flags; // size=0, offset=4
	int bitCount; // size=0, offset=8
};

struct _G2AnimDecompressChannelInfo_Type // hashcode: 0x9B2AEA69 (dec: -1691686295)
{
	struct _G2AnimKeylist_Type* keylist; // size=20, offset=0
	unsigned short* chanData; // size=0, offset=4
	int storedKey; // size=0, offset=8
	int targetKey; // size=0, offset=12
};

struct _G2AnimSegValue_Type // hashcode: 0x862DA63B (dec: -2043828677)
{
	union {
		struct _G2SVector3_Type rot; // size=6, offset=0
		struct _G2Quat_Type quat; // size=8, offset=0
	} rotQuat; // size=8, offset=0, found in object files: AnimG2.o, AnimG2.o
	struct _G2SVector3_Type scale; // size=6, offset=8
	unsigned short pad; // size=0, offset=14
	struct _G2SVector3_Type trans; // size=6, offset=16
	unsigned short bIsQuat; // size=0, offset=22
};

struct _G2AnimChanStatusBlockPool_Type // hashcode: 0x795F01EF (dec: 2036269551)
{
	unsigned short blockSize; // size=0, offset=0
	unsigned short stackTop; // size=0, offset=2
	unsigned short stackSize; // size=0, offset=4
	unsigned short* stack; // size=0, offset=8
	struct _G2AnimChanStatusBlock_Type* blockPool; // size=36, offset=12
};

struct _G2AnimControllerPool_Type // hashcode: 0x7F1B4AF1 (dec: 2132495089)
{
	unsigned short blockSize; // size=0, offset=0
	unsigned short stackTop; // size=0, offset=2
	unsigned short stackSize; // size=0, offset=4
	unsigned short* stack; // size=0, offset=8
	struct _G2AnimController_Type* blockPool; // size=36, offset=12
};

struct _G2AnimInterpStateBlockPool_Type // hashcode: 0x71F99BDD (dec: 1912183773)
{
	unsigned short blockSize; // size=0, offset=0
	unsigned short stackTop; // size=0, offset=2
	unsigned short stackSize; // size=0, offset=4
	unsigned short* stack; // size=0, offset=8
	struct _G2AnimInterpStateBlock_Type* blockPool; // size=164, offset=12
};

extern struct _G2AnimChanStatusBlockPool_Type _chanStatusBlockPool;
extern struct _G2AnimInterpStateBlockPool_Type _interpStateBlockPool;
extern struct _G2AnimControllerPool_Type _controllerPool;

extern enum _G2Bool_Enum G2Anim_Install(); // 0x800929A4
extern void G2Anim_ResetInternalState(); // 0x80092A0C
extern void G2Anim_Init(struct _G2Anim_Type *anim, struct _Model *modelData); // 0x80092A64
extern struct _G2AnimSection_Type * G2Anim_AddSection(struct _G2Anim_Type *anim, int firstSegID, int segCount); // 0x80092AF8
extern void G2Anim_Free(struct _G2Anim_Type *anim); // 0x80092B94
extern void G2Anim_Restore(struct _G2Anim_Type *anim); // 0x80092C50
extern void G2Anim_BuildTransforms(struct _G2Anim_Type *anim); // 0x80092CD0
extern void G2Anim_UpdateStoredFrame(struct _G2Anim_Type *anim); // 0x80092D9C
extern struct _G2AnimSection_Type * G2Anim_GetSectionWithSeg(struct _G2Anim_Type *anim, int segNumber); // 0x80092F18
extern enum _G2Bool_Enum G2Anim_SegmentHasActiveChannels(struct _G2Anim_Type *anim, int segNumber, unsigned short chanMask); // 0x80092F84
extern void G2Anim_GetSegChannelValue(struct _G2Anim_Type *anim, int segIndex, unsigned short *valueTable, unsigned short channelMask); // 0x800930C4
extern void G2Anim_GetRootMotionFromTimeForDuration(struct _G2Anim_Type *anim, short durationStart, short duration, struct _G2SVector3_Type *motionVector); // 0x80093170
extern void G2AnimSection_SwitchToKeylistAtTime(struct _G2AnimSection_Type *section, struct _G2AnimKeylist_Type *keylist, int keylistID, short targetTime); // 0x80093448
extern void G2AnimSection_JumpToTime(struct _G2AnimSection_Type *section, short targetTime); // 0x80093600
extern short G2AnimSection_UpdateOverInterval(struct _G2AnimSection_Type *section, short interval); // 0x800936A0
extern short G2AnimSection_AdvanceOverInterval(struct _G2AnimSection_Type *section, short interval); // 0x80093894
extern short G2AnimSection_RewindOverInterval(struct _G2AnimSection_Type *section, short interval); // 0x80093C6C
extern void _G2Anim_BuildTransformsNoControllers(struct _G2Anim_Type *anim); // 0x80093EAC
extern void _G2Anim_BuildSegTransformNoControllers(struct _G2Matrix_Type *segMatrix, struct _G2Matrix_Type *parentMatrix, enum _G2Bool_Enum bRootTransUpdated, int segIndex); // 0x80094000
extern void _G2Anim_BuildSegLocalRotMatrix(struct _G2AnimSegValue_Type *segValue, struct _G2Matrix_Type *segMatrix); // 0x80094228
extern void wombat(unsigned char *segKeyList, int flagBitOffset, struct _G2AnimSegKeyflagInfo_Type *kfInfo); // 0x80094288
extern unsigned long kangaroo(struct _G2AnimSegKeyflagInfo_Type *kfInfo); // 0x800942C0
extern void _G2Anim_InitializeSegValue(struct _G2Anim_Type *anim, struct _G2AnimSegValue_Type *segValue, int segIndex); // 0x80094348
extern void _G2AnimSection_InitStatus(struct _G2AnimSection_Type *section, struct _G2Anim_Type *anim); // 0x8009439C
extern void FooBar(struct _G2AnimSection_Type *section, struct _G2Anim_Type *anim, int decompressedKey, int targetKey, long timeOffset); // 0x80094648
extern void _G2AnimSection_UpdateStoredFrameFromData(struct _G2AnimSection_Type *section, struct _G2Anim_Type *anim); // 0x80094AD0
extern struct _G2Anim_Type * _G2AnimSection_GetAnim(struct _G2AnimSection_Type *section); // 0x80094BA0
extern void _G2AnimSection_TriggerEffects(struct _G2AnimSection_Type *section, short startTime, short endTime); // 0x80094BC0
extern void _G2Anim_FreeChanStatusBlockList(struct _G2AnimChanStatusBlock_Type *block); // 0x80094CEC
extern long _G2AnimAlphaTable_GetValue(struct _G2AnimAlphaTable_Type *table, long trueAlpha); // 0x80094D2C

#endif
