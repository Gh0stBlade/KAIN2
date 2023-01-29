#include "Game/CORE.H"
#include "ANIMG2.H"
#include "POOLMMG2.H"
#include "ANMG2ILF.H"
#include "ANMINTRP.H"
#include "ANMCTRLR.H"
#include "Game/PSX/COLLIDES.H"
#include "Game/G2/ANMDECMP.H"

enum _G2Bool_Enum G2Anim_Install()
{
	struct _G2AnimController_Type *dummyController;

	G2PoolMem_InitPool(&_chanStatusBlockPool, 180, sizeof(struct _G2AnimChanStatusBlock_Type));
	G2PoolMem_InitPool(&_interpStateBlockPool, 96, sizeof(struct _G2AnimInterpStateBlock_Type));
	G2PoolMem_InitPool(&_controllerPool, 122, sizeof(struct _G2AnimController_Type));

	dummyController = (struct _G2AnimController_Type*)G2PoolMem_Allocate(&_controllerPool);

	dummyController->next = 0;
	dummyController->segNumber = 255;
	dummyController->type = 0;

	return (_G2Bool_Enum)1;
}

#if defined(PC_VERSION)
struct _G2PoolMemPool_Type stru_B08B60,
	stru_B08B70,
	stru_B08B80;
#elif defined(PSX_VERSION)
struct _G2AnimChanStatusBlockPool_Type _chanStatusBlockPool;
struct _G2AnimInterpStateBlockPool_Type _interpStateBlockPool;
struct _G2AnimControllerPool_Type _controllerPool;
#endif

void G2Anim_ResetInternalState()
{
#if defined(PSX_VERSION)
	struct _G2AnimController_Type* dummyController;

	G2PoolMem_ResetPool(&_chanStatusBlockPool);
	G2PoolMem_ResetPool(&_interpStateBlockPool);
	G2PoolMem_ResetPool(&_controllerPool);

	dummyController = (struct _G2AnimController_Type*)G2PoolMem_Allocate(&_controllerPool);
	dummyController->next = 0;
	dummyController->segNumber = 255;
	dummyController->type = 0;

#elif defined(PC_VERSION)
	struct _G2AnimController_Type* dummyController; // eax

	G2PoolMem_ResetPool(&stru_B08B70);
	G2PoolMem_ResetPool(&stru_B08B60);
	G2PoolMem_ResetPool(&stru_B08B80);
	dummyController = (struct _G2AnimController_Type*)G2PoolMem_Allocate(&stru_B08B80);
	dummyController->next = 0;
	dummyController->segNumber = -1;
	dummyController->type = 0;
#endif
}

void G2Anim_Init(struct _G2Anim_Type* anim, struct _Model* modelData)
{
#if defined(PSX_VERSION)
	struct _G2AnimSection_Type* section;
	int sectionID;

	anim->sectionCount = 1;
	anim->masterSection = 0;
	anim->controllerList = 0;
	anim->disabledControllerList = 0;
	anim->segMatrices = NULL;
	anim->modelData = modelData;

	memset(&anim->disabledBits[0], 0, sizeof(anim->disabledBits) + sizeof(anim->section));

	for (sectionID = 0; sectionID < 3; sectionID++, section++)
	{
		section = &anim->section[sectionID];

		section->storedTime = -1;
		section->swAlarmTable = NULL;
		section->speedAdjustment = 4096;
	}

	anim->section[0].segCount = modelData->numSegments;

#elif defined(PC_VERSION)
	__int16** p_swAlarmTable; // eax
	int v3; // ecx

	anim->sectionCount = 1;
	anim->masterSection = 0;
	anim->controllerList = 0;
	anim->disabledControllerList = 0;
	anim->segMatrices = 0;
	anim->modelData = modelData;
	memset(anim->disabledBits, 0, 0x9Cu);
	p_swAlarmTable = &anim->section[0].swAlarmTable;
	v3 = 3;
	do
	{
		*((WORD*)p_swAlarmTable - 3) = -1;
		*p_swAlarmTable = 0;
		p_swAlarmTable[1] = (__int16*)4096;
		p_swAlarmTable += 12;
		--v3;
	} while (v3);
	anim->section[0].segCount = modelData->numSegments;
#endif
}

struct _G2AnimSection_Type * G2Anim_AddSection(struct _G2Anim_Type *anim, int firstSegID, int segCount)
{ 
	struct _G2AnimSection_Type* section;

	section = &anim->section[anim->sectionCount];

	memset(section, 0, sizeof(struct _G2AnimSection_Type));

	section->storedTime = -1;
	section->firstSeg = firstSegID;
	section->segCount = segCount;
	section->swAlarmTable = NULL;
	section->speedAdjustment = 4096;
	section->sectionID = anim->sectionCount;

	anim->sectionCount++;

	return section;
}

void G2Anim_Free(struct _G2Anim_Type* anim)//Matching - 99.68%
{
	struct _G2AnimSection_Type* animSection;
	int sectionID;
	struct _G2AnimInterpInfo_Type* interpInfo;

	for (sectionID = 0; sectionID < anim->sectionCount; sectionID++)
	{
		animSection = &anim->section[sectionID];

		if (G2AnimSection_IsInInterpolation(animSection) != 0)
		{
			animSection->elapsedTime = animSection->interpInfo->targetTime;
		}

		_G2Anim_FreeChanStatusBlockList(animSection->chanStatusBlockList);

		interpInfo = animSection->interpInfo;

		animSection->chanStatusBlockList = NULL;

		if (interpInfo != NULL)
		{
			_G2Anim_FreeInterpStateBlockList(interpInfo->stateBlockList);

			interpInfo->stateBlockList = NULL;
		}
	}
}

void G2Anim_Restore(struct _G2Anim_Type* anim) // Matching - 99.84%
{
	struct _G2AnimSection_Type* section;
	int sectionID = 0;;

	for (sectionID = 0; sectionID < (int)anim->sectionCount; sectionID++)
	{
		section = &anim->section[sectionID];
		if (section->keylist != NULL)
		{
			section->storedTime = -section->keylist->timePerKey;
			G2AnimSection_JumpToTime(section, section->elapsedTime);
		}
	}
}

void G2Anim_BuildTransforms(struct _G2Anim_Type* anim)//Matching - 93.73%
{
	unsigned short z;
	unsigned long xy;

	G2Anim_UpdateStoredFrame(anim);

	if ((anim->section[0].flags & 0x88) != 0x80)
	{
		((int*)&anim->rootTrans.x)[0] = 0;
		anim->rootTrans.z = 0;
	}

	if ((anim->section[0].flags & 0x4))
	{
		anim->rootTrans.x = -anim->rootTrans.x;
		anim->rootTrans.y = -anim->rootTrans.y;
		anim->rootTrans.z = -anim->rootTrans.z;
	}

	z = anim->rootTrans.z;
	xy = ((unsigned long*)&anim->rootTrans.x)[0];

	_segValues[0].trans.z = z;
	((unsigned long*)&_segValues[0].trans.x)[0] = xy;

	if (anim->controllerList != 0)
	{
		_G2Anim_BuildTransformsWithControllers(anim);
	}
	else
	{
		_G2Anim_BuildTransformsNoControllers(anim);
	}

	((int*)&anim->rootTrans.x)[0] = 0;
	anim->rootTrans.z = 0;

	anim->section[0].flags &= 0x7F;
	anim->flags &= 0xFFFE;
}

void G2Anim_UpdateStoredFrame(struct _G2Anim_Type* anim)//Matching - 99.84%
{
	struct _G2AnimSection_Type* section;
	short storedTime;
	short elapsedTime;
	struct _G2SVector3_Type motionVector;
	int sectionCount;
	struct _G2AnimInterpInfo_Type* interpInfo;
	struct _G2SVector3_Type* vector;

	section = &anim->section[0];
	sectionCount = anim->sectionCount;
	storedTime = section->storedTime;
	elapsedTime = section->elapsedTime;

	for (; sectionCount > 0; sectionCount--)
	{
		interpInfo = section->interpInfo;

		if (interpInfo != NULL && interpInfo->stateBlockList != NULL)
		{
			_G2AnimSection_UpdateStoredFrameFromQuat(section);
		}
		else
		{
			_G2AnimSection_UpdateStoredFrameFromData(section, anim);
		}

		section++;
	}

	vector = &motionVector;

	if (storedTime < 0)
	{
		storedTime = 0;
	}

	((int*)&vector->x)[0] = 0;
	vector->z = 0;

	if (elapsedTime > storedTime)
	{
		G2Anim_GetRootMotionOverInterval(anim, storedTime, elapsedTime, vector);
	}
	else if (elapsedTime < storedTime)
	{
		G2Anim_GetRootMotionOverInterval(anim, elapsedTime, storedTime, vector);
	}

	if ((anim->section[0].flags & 0x4))
	{
		anim->rootTrans.x -= motionVector.x;
		anim->rootTrans.y -= motionVector.y;
		anim->rootTrans.z -= motionVector.z;
	}
	else
	{
		anim->rootTrans.x += motionVector.x;
		anim->rootTrans.y += motionVector.y;
		anim->rootTrans.z += motionVector.z;
	}
}

struct _G2AnimSection_Type* G2Anim_GetSectionWithSeg(struct _G2Anim_Type* anim, int segNumber)
{ 
	struct _G2AnimSection_Type* section;
	struct _G2AnimSection_Type* tempSection;
	struct _G2AnimSection_Type* endSection;
	int firstSeg;
	int lastSeg;

	tempSection = &anim->section[0];

	endSection = &anim->section[anim->sectionCount];
	
	section = NULL;

	while (tempSection < endSection)
	{
		firstSeg = tempSection->firstSeg;
		lastSeg = tempSection->segCount;

		if (segNumber >= firstSeg && segNumber < firstSeg + lastSeg)
		{
			section = tempSection;
			break;
		}

		tempSection++;
	}

	return section;
}

enum _G2Bool_Enum G2Anim_SegmentHasActiveChannels(struct _G2Anim_Type* anim, int segNumber, unsigned short chanMask)
{
	struct _G2AnimSection_Type* section;
	unsigned char* segChanFlagStream;
	unsigned char activeChanBits;
	unsigned short dataFlagBits;
	unsigned short segFlagBits;
	int flagBytesPerSeg;
	int a0;//?
	
	section = G2Anim_GetSectionWithSeg(anim, segNumber);

	a0 = ((anim->modelData->numSegments << 1) + anim->modelData->numSegments) + 7;

	if (a0 < 0)
	{
		a0 = ((anim->modelData->numSegments << 1) + anim->modelData->numSegments) + 14;
	}

	segNumber += (segNumber << 1);

	segFlagBits = segNumber;
	flagBytesPerSeg = a0 >> 3;

	segChanFlagStream = (unsigned char*)&section->keylist->sectionData[section->keylist->sectionCount];
	
	if (segNumber < 0)
	{
		segFlagBits = segNumber + 7;
	}

	segChanFlagStream = &segChanFlagStream[segFlagBits >> 3] + 1;

	segNumber -= segFlagBits;

	activeChanBits = ((unsigned char*)&section->keylist->sectionData[section->keylist->sectionCount])[0];

	if ((activeChanBits & 0x1))
	{
		segFlagBits = 0;

		segChanFlagStream += flagBytesPerSeg;

		segFlagBits = ((segChanFlagStream[segFlagBits >> 3] | (segChanFlagStream[0] << 8)) >> segNumber) & 0x7;
	}

	if ((activeChanBits & 0x2))
	{
		segFlagBits |= (((segChanFlagStream[0] | (segChanFlagStream[0] << 8)) >> segNumber) & 0x7) << 4;
	}

	if ((activeChanBits & 0x4))
	{
		segFlagBits |= (((segChanFlagStream[0] | (segChanFlagStream[1] << 8)) >> segNumber) & 0x7) << 8;
	}

	return (_G2Bool_Enum)(0 < (segFlagBits & chanMask));
}


void G2Anim_GetSegChannelValue(struct _G2Anim_Type* anim, int segIndex, unsigned short* valueTable, unsigned short channelMask) // Matching - 96.98%
{
	struct _G2AnimSegValue_Type* chanFinalValue;

	unsigned short z;
	unsigned long xy;

	G2Anim_UpdateStoredFrame(anim);

	xy = ((unsigned long*)&anim->rootTrans.x)[0];
	z = anim->rootTrans.z;
	((unsigned long*)&_segValues[0].trans.x)[0] = xy;
	_segValues[0].trans.z = z;
	_G2Anim_ApplyControllersToStoredFrame(anim);
	chanFinalValue = &_segValues[segIndex];
	if (channelMask != 0) {
		do {
			if ((channelMask & 1) != 0) {
				*valueTable = *(ushort*)&chanFinalValue->rotQuat;
				valueTable = valueTable + 1;
			}
			channelMask = channelMask >> 1;
			chanFinalValue = (struct _G2AnimSegValue_Type*)((int)&chanFinalValue->rotQuat + 2);
		} while (channelMask != 0);
	}
}

void G2Anim_GetRootMotionFromTimeForDuration(struct _G2Anim_Type* anim, short durationStart, short duration, struct _G2SVector3_Type* motionVector)
{
	struct _G2AnimSection_Type* section;
	struct _G2AnimKeylist_Type* keylist;
	short storedKeyEndTime;
	short timePerKey;
	short keyTime;
	long alpha;
	struct _G2AnimInterpInfo_Type* interpInfo;
	struct _G2SVector3_Type* dest;
	struct _G2SVector3_Type* base;
	struct _G2SVector3_Type* offset;
	struct _G2SVector3_Type* vector;
	struct _G2Anim_Type dummyAnim;
	int off;

	dest = motionVector;

	section = &anim->section[0];

	interpInfo = section->interpInfo;

	if (interpInfo != NULL && interpInfo->stateBlockList != NULL)
	{
		alpha = _G2AnimAlphaTable_GetValue(interpInfo->alphaTable, (durationStart << 12) / interpInfo->duration);

		base = &interpInfo->stateBlockList->quatInfo->srcTrans;
		offset = &interpInfo->stateBlockList->quatInfo->destTrans;

		gte_ldlvnlsv(base);

		gte_ldsv(offset);

		gte_lddp(alpha);

		gte_gpl12();

		gte_stlvnlsv(motionVector);

		keylist = section->keylist;

		off = (duration << 12) / keylist->timePerKey;

		motionVector->x = (motionVector->x * off) >> 12;
		motionVector->y = (motionVector->y * off) >> 12;
		motionVector->z = (motionVector->z * off) >> 12;
	}
	else
	{
		keylist = section->keylist;

		timePerKey = keylist->timePerKey;

		storedKeyEndTime = timePerKey * ((durationStart / keylist->timePerKey) + 1);

		dummyAnim.sectionCount = 1;
		dummyAnim.section[0].sectionID = 0;
		dummyAnim.section[0].firstSeg = 0;
		dummyAnim.section[0].segCount = 1;
		dummyAnim.section[0].keylist = keylist;
		dummyAnim.section[0].chanStatusBlockList = NULL;
		dummyAnim.modelData = anim->modelData;
		dummyAnim.section[0].storedTime = -timePerKey;

		((int*)&motionVector->x)[0] = 0;
		motionVector->z = 0;

		while (duration != 0)
		{
			if (durationStart >= storedKeyEndTime)
			{
				timePerKey = keylist->s0TailTime;
			}

			dummyAnim.section[0].elapsedTime = durationStart;

			_G2AnimSection_UpdateStoredFrameFromData(&dummyAnim.section[0], &dummyAnim);

			keyTime = storedKeyEndTime - durationStart;

			if (duration < keyTime)
			{
				keyTime = duration;
			}

			if (keyTime < timePerKey)
			{
				alpha = (keyTime << 12) / timePerKey;
			}
			else
			{
				alpha = 4096;
			}

			gte_ldlvnlsv(motionVector);

			vector = &_segValues[0].trans;

			gte_ldsv(vector);

			gte_lddp(alpha);

			gte_gpl12();

			gte_stlvnlsv(motionVector);

			durationStart = storedKeyEndTime;

			duration -= keyTime;

			storedKeyEndTime += timePerKey;
		}

		_G2Anim_FreeChanStatusBlockList(dummyAnim.section[0].chanStatusBlockList);
	}
}

void G2AnimSection_SwitchToKeylistAtTime(struct _G2AnimSection_Type* section, struct _G2AnimKeylist_Type* keylist, int keylistID, short targetTime)//Matching - 98.20%
{
	struct _G2Anim_Type* anim;
	struct _G2SVector3_Type rootMotion;
	struct _G2AnimInterpInfo_Type* interpInfo;
	unsigned short z;
	unsigned long xy;

	anim = _G2AnimSection_GetAnim(section);

	if (section->firstSeg == 0)
	{
		anim->flags |= 0x1;

		if (section->keylist != NULL && section->storedTime >= 0)
		{
			G2Anim_GetRootMotionOverInterval(anim, section->storedTime, section->elapsedTime, &rootMotion);
		}
		else
		{
			rootMotion.x = 0;
			rootMotion.y = 0;
			rootMotion.z = 0;
		}

		rootMotion.x += anim->rootTrans.x;
		rootMotion.y += anim->rootTrans.y;
		rootMotion.z += anim->rootTrans.z;
	}

	interpInfo = section->interpInfo;

	if (interpInfo != NULL)
	{
		if (interpInfo->stateBlockList != NULL)
		{
			_G2Anim_FreeInterpStateBlockList(interpInfo->stateBlockList);

			interpInfo->stateBlockList = NULL;
		}
	}
	
	G2AnimSection_ClearAlarm(section, 0x3);

	if (keylist != section->keylist)
	{
		section->keylist = keylist;
		section->keylistID = keylistID;
		section->storedTime = -keylist->timePerKey;
	}
	
	G2AnimSection_JumpToTime(section, targetTime);
	
	if (section->firstSeg == 0)
	{
		section->flags |= 0x80;
		xy = ((unsigned long*)&rootMotion)[0];
		z = rootMotion.z;
		
		((unsigned long*)&anim->rootTrans)[0] = xy;
		anim->rootTrans.z = z;
	}

	if ((section->flags & 0x2))
	{
		G2AnimSection_SetLoopRangeAll(section);
	}

	G2AnimSection_SetUnpaused(section);

	section->swAlarmTable = NULL;
}

void G2AnimSection_JumpToTime(struct _G2AnimSection_Type* section, short targetTime)//Matching - 92.13%
{
	struct _G2Anim_Type* anim;

	anim = _G2AnimSection_GetAnim(section);

	if (targetTime < section->elapsedTime)
	{
		section->storedTime = -section->keylist->timePerKey;
	}

	section->elapsedTime = targetTime;

	_G2AnimSection_UpdateStoredFrameFromData(section, anim);

	G2AnimSection_ClearAlarm(section, 0x3);

	section->flags &= 0x7F;

	if (section->firstSeg == 0)
	{
		anim->rootTrans.x = 0;
		anim->rootTrans.y = 0;
		anim->rootTrans.z = 0;
	}
}

short G2AnimSection_UpdateOverInterval(struct _G2AnimSection_Type* section, short interval)//Matching - 94.87%
{
	struct _G2Anim_Type* anim;
	struct _G2AnimInterpInfo_Type* interpInfo;
	short elapsedTime;
	unsigned short z;
	unsigned long xy;
	struct _G2SVector3_Type motionVector;
	short s4;

	if ((section->flags & 0x1))
	{
		return 0;
	}

	interpInfo = section->interpInfo;

	if (interpInfo != NULL && interpInfo->stateBlockList != NULL)
	{
		anim = _G2AnimSection_GetAnim(section);

		anim->flags |= 0x1;

		elapsedTime = section->elapsedTime;

		s4 = (elapsedTime + ((interval * section->speedAdjustment) >> 12)) - interpInfo->duration;

		if (s4 >= 0)
		{
			section->storedTime = -section->keylist->timePerKey;

			G2AnimSection_JumpToTime(section, interpInfo->targetTime);

			if (section->firstSeg == 0)
			{
				G2Anim_GetRootMotionOverInterval(anim, section->elapsedTime, interpInfo->duration, &motionVector);

				xy = ((int*)&motionVector.x)[0];
				z = motionVector.z;

				((int*)&anim->rootTrans.x)[0] = xy;
				anim->rootTrans.z = z;

				section->flags |= 0x80;
			}

			_G2Anim_FreeInterpStateBlockList(interpInfo->stateBlockList);

			interpInfo->stateBlockList = NULL;

			if ((section->flags & 0x2))
			{
				G2AnimSection_SetLoopRangeAll(section);
			}

			section->alarmFlags |= 0x10;

			if (section->callback == NULL)
			{
				return 0;
			}
			else
			{
				section->callback(anim, section->sectionID, G2ANIM_MSG_SECTION_INTERPDONE, 0, 0, (struct _Instance*)section->callbackData);
				return s4;
			}
		}
		else
		{
			section->elapsedTime = elapsedTime + ((interval * section->speedAdjustment) >> 12);
			return 0;
		}
	}
	else
	{
		if (!(section->flags & 0x4))
		{
			return G2AnimSection_AdvanceOverInterval(section, interval);

		}
		else
		{
			return G2AnimSection_RewindOverInterval(section, interval);
		}
	}

	return 0;
}

short G2AnimSection_AdvanceOverInterval(struct _G2AnimSection_Type* section, short interval)//Matching - 97.60%
{
	struct _G2AnimKeylist_Type* keylist;
	short newTime;
	short extraTime;
	short elapsedTime;
	short endTime;
	short loopExtraTime;
	short* swAlarmTable;
	short swAlarmTime;
	struct _G2Anim_Type* anim;
	struct _G2SVector3_Type motionVector;
	unsigned long message;
	unsigned short z;
	unsigned long xy;

	loopExtraTime = 0;

	if ((section->flags & 0x1) || (section->alarmFlags & 0x1))
	{
		return 0;
	}

	anim = _G2AnimSection_GetAnim(section);

	anim->flags |= 0x1;

	G2AnimSection_ClearAlarm(section, 3);

	keylist = section->keylist;

	elapsedTime = section->elapsedTime;

	section->flags &= 0xFB;

	if ((section->flags & 0x2))
	{
		endTime = section->loopEndTime;
	}
	else
	{
		endTime = G2AnimKeylist_GetDuration(keylist);
	}

	newTime = elapsedTime + ((interval * section->speedAdjustment) >> 12);

	if (section->swAlarmTable != NULL)
	{
		swAlarmTable = section->swAlarmTable;

		swAlarmTime = swAlarmTable[0];

		while ((swAlarmTime = *swAlarmTable) != -1)
		{
			if (((elapsedTime < swAlarmTime != 0) && (newTime >= swAlarmTime)) || ((section->storedTime <= 0) && (elapsedTime == swAlarmTime)))
			{
				section->alarmFlags |= 0x20;

				if (section->callback != NULL)
				{
					section->callback(anim, section->sectionID, G2ANIM_MSG_SWALARMSET, elapsedTime, newTime, (struct _Instance*)section->callbackData);
				}
			}

			swAlarmTable++;
		}
	}
	else
	{
		G2AnimSection_ClearAlarm(section, 0x20);
	}

	_G2AnimSection_TriggerEffects(section, elapsedTime, newTime);
	extraTime = newTime - endTime;

	while (1)
	{
		if (extraTime >= 0)
		{
			if ((section->flags & 0x2))
			{
				message = 2;

				section->alarmFlags |= 0x4;

				G2AnimSection_JumpToTime(section, section->loopStartTime);

				newTime = section->loopStartTime + extraTime;

				loopExtraTime = newTime - endTime;

				_G2AnimSection_TriggerEffects(section, section->loopStartTime - 1, newTime);

				if (newTime >= endTime)
				{
					newTime = endTime - 1;
				}
			}
			else
			{
				message = 1;

				newTime = endTime - 1;

				section->alarmFlags |= 0x1;
			}

			if (section->firstSeg == 0)
			{
				anim = _G2AnimSection_GetAnim(section);

				G2Anim_GetRootMotionOverInterval(anim, elapsedTime, endTime, &motionVector);
			}

			if (section->callback != NULL)
			{
				swAlarmTime = section->callback((struct _G2Anim_Type*)_G2AnimSection_GetAnim(section), section->sectionID, (enum _G2AnimCallbackMsg_Enum)message, newTime, extraTime, (struct _Instance*)section->callbackData);

				if (swAlarmTime != newTime)
				{
					newTime = swAlarmTime;
					G2AnimSection_JumpToTime(section, swAlarmTime);
				}
				else if ((section->flags & 0x2))
				{
					G2AnimSection_JumpToTime(section, swAlarmTime);

					section->storedTime = section->loopStartTime;
				}
				else
				{
					((int*)&motionVector.x)[0] = 0;
					motionVector.z = 0;
				}
			}

			if (section->firstSeg == 0)
			{
				((int*)&anim->rootTrans.x)[0] = ((int*)&motionVector.x)[0];

				anim->rootTrans.z = motionVector.z;

				section->flags |= 0x80;
			}

			if (!(section->flags & 0x2))
			{
				break;
			}

			extraTime = loopExtraTime;
			endTime = section->loopEndTime;
		}
		else
		{
			extraTime = 0;
			break;
		}
	}


	section->elapsedTime = newTime;

	return extraTime;
}

// autogenerated function stub: 
// short /*$ra*/ G2AnimSection_RewindOverInterval(struct _G2AnimSection_Type *section /*$s1*/, short interval /*$a1*/)
short G2AnimSection_RewindOverInterval(struct _G2AnimSection_Type *section, short interval)
{ // line 1671, offset 0x80093c6c
	/* begin block 1 */
		// Start line: 1672
		// Start offset: 0x80093C6C
		// Variables:
			short newTime; // $s3
			short extraTime; // $s6
			short elapsedTime; // $s7
			short endTime; // $s2
			struct _G2Anim_Type *anim; // $s4
			struct _G2SVector3_Type motionVector; // stack offset -48
			unsigned long message; // $s5

		/* begin block 1.1 */
			// Start line: 1832
			// Start offset: 0x80093E34
		/* end block 1.1 */
		// End offset: 0x80093E3C
		// End Line: 1832

		/* begin block 1.2 */
			// Start line: 1841
			// Start offset: 0x80093E4C

			/* begin block 1.2.1 */
				// Start line: 1841
				// Start offset: 0x80093E4C
				// Variables:
					unsigned short z; // $v1
					unsigned long xy; // $v0
			/* end block 1.2.1 */
			// End offset: 0x80093E4C
			// End Line: 1841
		/* end block 1.2 */
		// End offset: 0x80093E4C
		// End Line: 1841
	/* end block 1 */
	// End offset: 0x80093E80
	// End Line: 1867

	/* begin block 2 */
		// Start line: 3631
	/* end block 2 */
	// End Line: 3632
					UNIMPLEMENTED();
	return 0;
}

void _G2Anim_BuildTransformsNoControllers(struct _G2Anim_Type* anim)//Matching - 81.39%
{
	struct _Segment* segment;
	struct _G2Matrix_Type* segMatrix;
	enum _G2Bool_Enum bRootTransUpdated;
	int segIndex;
	int segCount;
#if defined(PSXPC_VERSION)
	unsigned long disabledBits[3];
#endif
	unsigned long disabledMask;
	unsigned long parentMask;
	unsigned long parentIndex;
	unsigned long* disabledBitsArray;
	unsigned short* parentIndexPtr;

	segMatrix = anim->segMatrices;

	segment = anim->modelData->segmentList;

	disabledBits[0] = anim->disabledBits[0];

	disabledBits[1] = anim->disabledBits[1];

	disabledBits[2] = anim->disabledBits[2];

	bRootTransUpdated = (enum _G2Bool_Enum)(((anim->section[0].flags & 0x88) == 0x80));

	segCount = anim->modelData->numSegments;

	disabledMask = 1;

	disabledBitsArray = &disabledBits[0];

	for (segIndex = 0, parentIndexPtr = (unsigned short*)&segment->parent; segIndex < segCount;)
	{
		parentIndex = parentIndexPtr[0];

		if ((short)parentIndex != -1 && (disabledBits[((short)parentIndex >> 5)] & ((1 << (parentIndex & 0x1F)))))
		{
			disabledBitsArray[0] |= disabledMask;
		}


		if (!(disabledBitsArray[0] & disabledMask))
		{
			_G2Anim_BuildSegTransformNoControllers(segMatrix, &anim->segMatrices[(short)parentIndexPtr[0]], bRootTransUpdated, segIndex);
		}

		bRootTransUpdated = (enum _G2Bool_Enum)0;

		parentIndexPtr += 12;

		segMatrix++;

		disabledMask <<= 1;

		segIndex++;

		if ((segIndex % 32) == 0)
		{
			int testing = 0;
			testing++;
		}

		if (disabledMask == 0)
		{
			disabledBitsArray++;

			disabledMask = 1;
		}
	}
}

void _G2Anim_BuildSegTransformNoControllers(struct _G2Matrix_Type* segMatrix, struct _G2Matrix_Type* parentMatrix, enum _G2Bool_Enum bRootTransUpdated, int segIndex)
{
	struct _G2AnimSegValue_Type* segValue;
	struct _G2LVector3_Type scale;
	struct _G2SVector3_Type* svector;
	struct _G2LVector3_Type* lvector;

	segValue = &_segValues[segIndex];

	scale.x = segValue->scale.x;
	scale.y = segValue->scale.y;
	scale.z = segValue->scale.z;

	_G2Anim_BuildSegLocalRotMatrix(segValue, segMatrix);

	if (0 < ((scale.x | scale.y | scale.z) ^ 4096))
	{
		ScaleMatrix((MATRIX*)segMatrix, (VECTOR*)&scale);

		segMatrix->scaleFlag = 1;
	}

	gte_SetRotMatrix(parentMatrix);

	gte_ldclmv((((short*)segMatrix) + 0));
	gte_rtir();
	gte_stclmv((((short*)segMatrix) + 0));

	gte_ldclmv((((short*)segMatrix) + 1));
	gte_rtir();
	gte_stclmv((((short*)segMatrix) + 1));

	gte_ldclmv((((short*)segMatrix) + 2));
	gte_rtir();
	gte_stclmv((((short*)segMatrix) + 2));

	svector = &segValue->trans;
	lvector = &segMatrix->trans;

	gte_ldv0(svector);
	gte_rtv0();
	gte_stlvnl(lvector);

	if (bRootTransUpdated != 0)
	{
		parentMatrix->trans.x += segMatrix->trans.x;
		parentMatrix->trans.y += segMatrix->trans.y;
		parentMatrix->trans.z += segMatrix->trans.z;
		
		segMatrix->trans.x = 0;
		segMatrix->trans.y = 0;
		segMatrix->trans.z = 0;
	}

	segMatrix->trans.x += parentMatrix->trans.x;
	segMatrix->trans.y += parentMatrix->trans.y;
	segMatrix->trans.z += parentMatrix->trans.z;
}

void _G2Anim_BuildSegLocalRotMatrix(struct _G2AnimSegValue_Type* segValue, struct _G2Matrix_Type* segMatrix)
{
	struct _G2SVector3_Type rot;
	struct _G2SVector3_Type* source;
	struct _G2SVector3_Type* dest;
	unsigned long mask;
	unsigned short z;
	unsigned long xy;

	source = &segValue->rotQuat.rot;
	
	if (segValue->bIsQuat != 0)
	{
		G2Quat_ToMatrix_S(&segValue->rotQuat.quat, segMatrix);
	}
	else
	{
		mask = 0xFFF0FFF;

		dest = &rot;
		z = segValue->rotQuat.rot.z & 0xFFF;
		xy = ((unsigned long*)&segValue->rotQuat.rot.x)[0] & mask;

		rot.z = z;
		((unsigned long*)&rot.x)[0] = xy;

		RotMatrixZYX((SVECTOR*)dest, (MATRIX*)segMatrix);
	}
}

void wombat(unsigned char* segKeyList, int flagBitOffset, struct _G2AnimSegKeyflagInfo_Type* kfInfo)//Matching - 100%
{
#if defined(PSX_VERSION)
	int flagDWordOffset;
	int flagBitShift;

	flagDWordOffset = flagBitOffset >> 5;

	segKeyList = &segKeyList[flagDWordOffset << 2];

	flagBitShift = flagBitOffset - (flagDWordOffset << 5);

	kfInfo->stream = (unsigned long*)segKeyList;

	kfInfo->flags = kfInfo->stream[0] >> flagBitShift;

	kfInfo->bitCount = 32 - (flagBitOffset & 0x1F);

#elif defined(PC_VERSION)
	ulong* v3; // esi
	ulong v4; // eax

	v3 = (ulong*)&segKeyList[4 * (flagBitOffset >> 5)];
	kfInfo->stream = v3;
	v4 = *v3 >> (flagBitOffset - 32 * (flagBitOffset >> 5));
	kfInfo->bitCount = 32 - (flagBitOffset & 0x1F);
	kfInfo->flags = v4;
#endif
}

unsigned long kangaroo(struct _G2AnimSegKeyflagInfo_Type* kfInfo)
{
#if defined(PSX_VERSION)

	unsigned long keyflags;
	unsigned long tempFlags;

	keyflags = 0;

	if (kfInfo->stream != NULL)
	{
		tempFlags = kfInfo->flags;

		keyflags = tempFlags & 0x7;

		kfInfo->flags = tempFlags >> 3;

		kfInfo->bitCount -= 3;

		if (kfInfo->bitCount <= 0)
		{
			kfInfo->stream = &kfInfo->stream[1];

			kfInfo->flags = kfInfo->stream[0];

			if (kfInfo->bitCount < 0)
			{
				tempFlags = (kfInfo->flags << (kfInfo->bitCount + 3)) & 0x7;

				keyflags |= tempFlags;

				kfInfo->flags = kfInfo->flags >> -kfInfo->bitCount;
			}

			kfInfo->bitCount += 32;
		}
	}

	return keyflags;

#elif defined(PC_VERSION)

	ulong* stream; // esi
	ulong flags; // ecx
	int v3; // edi
	unsigned int result; // eax
	ulong v5; // esi

	stream = kfInfo->stream;
	if (!kfInfo->stream)
		return 0;
	flags = kfInfo->flags;
	v3 = kfInfo->bitCount - 3;
	result = flags & 7;
	kfInfo->bitCount = v3;
	kfInfo->flags = flags >> 3;
	if (v3 <= 0)
	{
		kfInfo->stream = stream + 1;
		v5 = stream[1];
		kfInfo->flags = v5;
		if (v3 < 0)
		{
			result |= (v5 << (v3 + 3)) & 7;
			kfInfo->flags = v5 >> -(char)v3;
		}
		kfInfo->bitCount = v3 + 32;
	}
	return result;
#endif
}

void _G2Anim_InitializeSegValue(struct _G2Anim_Type* anim, struct _G2AnimSegValue_Type* segValue, int segIndex)
{
	struct _Segment* segment;
	struct _G2Quat_Type* quat;
	unsigned long zpad;
	unsigned long xy;

	quat = &segValue->rotQuat.quat;

	segValue->scale.x = 0x1000;
	segValue->scale.y = 0x1000;
	segValue->scale.z = 0x1000;

	segValue->rotQuat.quat.x = 0;
	segValue->rotQuat.quat.y = 0;
	segValue->rotQuat.quat.z = 0;
	segValue->rotQuat.quat.w = 0x1000;

	segment = &anim->modelData->segmentList[segIndex];

	xy = ((unsigned long*)&segment->px)[0];
	zpad = ((unsigned long*)&segment->pz)[0] & 0xFFFF;

	((unsigned long*)(&segValue->trans.x))[0] = xy;
	((unsigned long*)(&segValue->trans.z))[0] = zpad;
}

void _G2AnimSection_InitStatus(struct _G2AnimSection_Type* section, struct _G2Anim_Type* anim)//Matching - 97.98%
{
	struct _G2AnimDecompressChannelInfo_Type dcInfo;
	struct _G2AnimSegValue_Type* segValue;
	struct _G2AnimChanStatusBlock_Type** chanStatusNextBlockPtr;
	struct _G2AnimChanStatusBlock_Type* chanStatusBlock;
	struct _G2AnimChanStatus_Type* chanStatus;
	struct _G2AnimSegKeyflagInfo_Type rotKfInfo;
	struct _G2AnimSegKeyflagInfo_Type scaleKfInfo;
	struct _G2AnimSegKeyflagInfo_Type transKfInfo;
	int type;
	unsigned long segChanFlags;
	int segIndex;
	int lastSeg;
	int flagBitOffset;
	unsigned long activeChanBits;
	unsigned char* segKeyList;
	int bitsPerFlagType;
	int chanStatusChunkCount;

	flagBitOffset = ((section->firstSeg << 1) + section->firstSeg) + 8;

	bitsPerFlagType = ((anim->modelData->numSegments << 1) + anim->modelData->numSegments) + 7;

	segKeyList = (unsigned char*)&section->keylist->sectionData[section->keylist->sectionCount];

	activeChanBits = segKeyList[0];

	bitsPerFlagType &= 0xFFFFFFF8;

	rotKfInfo.stream = NULL;
	scaleKfInfo.stream = NULL;
	transKfInfo.stream = NULL;

	if ((activeChanBits & 0x1))
	{
		wombat(segKeyList, flagBitOffset, &rotKfInfo);

		flagBitOffset += bitsPerFlagType;
	}

	if ((activeChanBits & 0x2))
	{
		wombat(segKeyList, flagBitOffset, &scaleKfInfo);

		flagBitOffset += bitsPerFlagType;
	}

	if ((activeChanBits & 0x4))
	{
		wombat(segKeyList, flagBitOffset, &transKfInfo);
	}

	chanStatus = NULL;

	_G2Anim_FreeChanStatusBlockList(section->chanStatusBlockList);

	section->chanStatusBlockList = NULL;

	chanStatusChunkCount = 0;

	dcInfo.keylist = section->keylist;

	dcInfo.chanData = section->keylist->sectionData[section->sectionID];

	segIndex = section->firstSeg;

	lastSeg = segIndex + section->segCount;

	segValue = &_segValues[segIndex];

	chanStatusNextBlockPtr = &section->chanStatusBlockList;

	while (segIndex < lastSeg)
	{
		segChanFlags = kangaroo(&rotKfInfo);
		segChanFlags |= kangaroo(&scaleKfInfo) << 4;
		segChanFlags |= kangaroo(&transKfInfo) << 8;

		_G2Anim_InitializeSegValue(anim, segValue, segIndex);

		while (segChanFlags != 0)
		{
			if ((segChanFlags & 0x1))
			{
				type = (((unsigned char*)dcInfo.chanData)[1] & 0xE0);

				if (type == 0xE0)
				{
					type = 0;
				}

				switch (type)
				{
				case 0:
				{
					dcInfo.chanData = &dcInfo.chanData[dcInfo.keylist->keyCount];
					break;
				}
				case 0x20:
				{
					dcInfo.chanData = &dcInfo.chanData[2];
					break;
				}
				default:
				{
					if (chanStatusChunkCount == 0)
					{
						chanStatusChunkCount = 8;

						chanStatusBlock = (struct _G2AnimChanStatusBlock_Type*)G2PoolMem_Allocate(&_chanStatusBlockPool);

						chanStatusBlock->next = NULL;

						chanStatusNextBlockPtr[0] = chanStatusBlock;

						chanStatusNextBlockPtr = (struct _G2AnimChanStatusBlock_Type**)chanStatusBlock;

						chanStatus = ((struct _G2AnimChanStatusBlock_Type*)chanStatusNextBlockPtr)->chunks;
					}

					switch (type)
					{
					case 0x40:
					{
						_G2Anim_InitializeChannel_AdaptiveDelta(&dcInfo, chanStatus);
						break;
					}
					case 0x60:
					{
						_G2Anim_InitializeChannel_Linear(&dcInfo, chanStatus);

						break;
					}
					}
					chanStatus++;

					chanStatusChunkCount--;
					break;
				}
				}
			}
			segChanFlags >>= 1;
		}

		segIndex++;
		segValue++;
	}

	section->storedTime = -section->keylist->timePerKey;
}

void FooBar(struct _G2AnimSection_Type* section, struct _G2Anim_Type* anim, int decompressedKey, int targetKey, long timeOffset)//Matching - 99.12%
{
	struct _G2AnimDecompressChannelInfo_Type dcInfo;
	struct _G2AnimSegValue_Type* segValue;
	short* chanValue;
	struct _G2AnimChanStatusBlock_Type* chanStatusBlock;
	struct _G2AnimChanStatus_Type* chanStatus;
	int chanStatusChunkCount;
	struct _G2AnimSegKeyflagInfo_Type rotKfInfo;
	struct _G2AnimSegKeyflagInfo_Type scaleKfInfo;
	struct _G2AnimSegKeyflagInfo_Type transKfInfo;
	int type;
	unsigned long segChanFlags;
	int segIndex;
	int lastSeg;
	struct _G2AnimDecompressChannelInfo_Type nextDCInfo;
	struct _G2AnimDecompressChannelInfo_Type initDCInfo;
	struct _G2AnimChanStatus_Type nextChanStatus;
	int bitsPerFlagType;
	int flagBitOffset;
	unsigned long activeChanBits;
	unsigned char* segKeyList;

	flagBitOffset = ((section->firstSeg << 1) + section->firstSeg) + 8;

	bitsPerFlagType = ((anim->modelData->numSegments << 1) + anim->modelData->numSegments) + 7;

	segKeyList = (unsigned char*)&section->keylist->sectionData[section->keylist->sectionCount];

	activeChanBits = segKeyList[0];

	bitsPerFlagType &= 0xFFFFFFF8;

	rotKfInfo.stream = NULL;
	scaleKfInfo.stream = NULL;
	transKfInfo.stream = NULL;

	if ((activeChanBits & 0x1))
	{
		wombat(segKeyList, flagBitOffset, &rotKfInfo);

		flagBitOffset += bitsPerFlagType;
	}

	if ((activeChanBits & 0x2))
	{
		wombat(segKeyList, flagBitOffset, &scaleKfInfo);

		flagBitOffset += bitsPerFlagType;
	}

	if ((activeChanBits & 0x4))
	{
		wombat(segKeyList, flagBitOffset, &transKfInfo);
	}

	chanStatusBlock = section->chanStatusBlockList;

	chanStatusChunkCount = 8;

	chanStatus = &chanStatusBlock->chunks[0];

	dcInfo.keylist = section->keylist;

	dcInfo.chanData = section->keylist->sectionData[section->sectionID];

	dcInfo.storedKey = decompressedKey;

	dcInfo.targetKey = targetKey;

	if (timeOffset != 0)
	{
		nextDCInfo.keylist = dcInfo.keylist;
		nextDCInfo.chanData = dcInfo.chanData;
		nextDCInfo.storedKey = targetKey;
		nextDCInfo.targetKey = targetKey + 1;

		if (targetKey + 1 >= section->keylist->keyCount)
		{
			if ((section->flags & 0x2))
			{
				initDCInfo.keylist = dcInfo.keylist;
				initDCInfo.chanData = dcInfo.chanData;
				nextDCInfo.storedKey = -1;
				nextDCInfo.targetKey = 0;
			}
			else
			{
				timeOffset = 0;
			}
		}
	}

	segIndex = section->firstSeg;

	segValue = &_segValues[segIndex];

	lastSeg = segIndex + section->segCount;

	while (segIndex < lastSeg)
	{
		_G2Anim_InitializeSegValue(anim, segValue, segIndex);

		segChanFlags = kangaroo(&rotKfInfo);

		segChanFlags |= kangaroo(&scaleKfInfo) << 4;

		segChanFlags |= kangaroo(&transKfInfo) << 8;

		chanValue = (short*)segValue;

		while (segChanFlags != 0)
		{
			if ((segChanFlags & 0x1))
			{
				type = ((unsigned char*)dcInfo.chanData)[1] & 0xE0;

				if (type == 0xE0)
				{
					type = 0;
				}

				switch (type)
				{
				case 0:
				{
					chanValue[0] = dcInfo.chanData[targetKey];

					dcInfo.chanData += dcInfo.keylist->keyCount;

					break;
				}
				case 0x20:
				{
					chanValue[0] = dcInfo.chanData[1];

					dcInfo.chanData += 2;

					break;
				}
				default:
					switch (type)
					{
					case 0x40:
					{
						_G2Anim_DecompressChannel_AdaptiveDelta(&dcInfo, chanStatus);
						break;
					}
					case 0x60:
					{
						_G2Anim_DecompressChannel_Linear(&dcInfo, chanStatus);
						break;
					}
					}
					chanStatusChunkCount--;

					chanValue[0] = chanStatus->keyData;

					nextChanStatus = chanStatus[0];

					chanStatus++;

					if (chanStatusChunkCount == 0)
					{
						chanStatusBlock = chanStatusBlock->next;

						chanStatusChunkCount = 8;

						chanStatus = chanStatusBlock->chunks;
					}

					break;
				}

				if (timeOffset != 0)
				{
					switch (type)
					{
					case 0:
					{
						nextChanStatus.keyData = nextDCInfo.chanData[nextDCInfo.targetKey];

						nextDCInfo.chanData = dcInfo.chanData;

						initDCInfo.chanData = dcInfo.chanData;
						break;
					}
					case 0x20:
					{
						nextChanStatus.keyData = chanValue[0];

						nextDCInfo.chanData = dcInfo.chanData;

						initDCInfo.chanData = dcInfo.chanData;
						break;
					}
					case 0x40:
					{
						if (nextDCInfo.storedKey == -1)
						{
							_G2Anim_InitializeChannel_AdaptiveDelta(&initDCInfo, &nextChanStatus);
						}

						_G2Anim_DecompressChannel_AdaptiveDelta(&nextDCInfo, &nextChanStatus);
						break;
					}
					case 0x60:
					{
						if (nextDCInfo.storedKey == -1)
						{
							_G2Anim_InitializeChannel_Linear(&initDCInfo, &nextChanStatus);
						}

						_G2Anim_DecompressChannel_Linear(&nextDCInfo, &nextChanStatus);
						break;
					}
					}

					nextChanStatus.keyData -= chanValue[0];

					if (nextChanStatus.keyData >= 2048)
					{
						nextChanStatus.keyData -= 4096;
					}

					if (nextChanStatus.keyData < -2047)
					{
						nextChanStatus.keyData += 4096;
					}

					if (segIndex != 0)
					{
						chanValue[0] += (nextChanStatus.keyData * timeOffset) >> 12;
					}
				}
			}

			segChanFlags >>= 1;

			chanValue++;
		}

		segValue++;

		segIndex++;
	}
}

void _G2AnimSection_UpdateStoredFrameFromData(struct _G2AnimSection_Type* section, struct _G2Anim_Type* anim)//Matching - 47.88%
{
	short timePerKey;
	long storedKey;
	long targetKey;
	long timeOffset;

	storedKey = section->storedTime / section->keylist->timePerKey;
	timePerKey = section->keylist->timePerKey;
	targetKey = section->elapsedTime / timePerKey;

	if (storedKey < 0 || targetKey < storedKey)
	{
		_G2AnimSection_InitStatus(section, anim);
		storedKey = -1;
	}

	timeOffset = ((section->elapsedTime - (targetKey * timePerKey)) << 12) / timePerKey;

#if !defined(EDITOR)
	FooBar(section, anim, storedKey, targetKey, timeOffset);
#endif

	section->storedTime = section->elapsedTime;

	section->flags |= 0x80;
}

struct _G2Anim_Type* _G2AnimSection_GetAnim(struct _G2AnimSection_Type* section)
{
	return (struct _G2Anim_Type*)((char*)(section) - (section->sectionID * sizeof(struct _G2AnimSection_Type) + 0x24));
}

// autogenerated function stub: 
// void /*$ra*/ _G2AnimSection_TriggerEffects(struct _G2AnimSection_Type *section /*$s1*/, short startTime /*$a1*/, short endTime /*$a2*/)
void _G2AnimSection_TriggerEffects(struct _G2AnimSection_Type *section, short startTime, short endTime)
{ // line 2576, offset 0x80094bc0
#if defined(PSX_VERSION)
	UNIMPLEMENTED();
#elif defined(PC_VERSION)
	struct _G2AnimFxHeader_Type* fxHeader; // esi
	signed __int8 i; // dl
	int sectionID; // ecx
	unsigned int sizeAndSection; // edi
	int v8; // eax
	int v9; // edi
	__int16 v10; // ax
	int(__stdcall * callback)(); // ebp
	struct _G2AnimKeylist_Type* keylist; // [esp+Ch] [ebp+4h]

	keylist = section->keylist;
	fxHeader = keylist->fxList;
	if (fxHeader)
	{
		for (i = fxHeader->type; i != -1; fxHeader = (struct _G2AnimFxHeader_Type*)((char*)fxHeader + v9))
		{
			sectionID = section->sectionID;
			sizeAndSection = fxHeader->sizeAndSection;
			v8 = sizeAndSection & 0xF;
			v9 = (sizeAndSection >> 2) & 0x3C;
			if (v8 == sectionID)
			{
				if ((v10 = fxHeader->keyframeID * keylist->s0TailTime, startTime < v10) && endTime >= v10
					|| !v10 && startTime <= 0 && endTime >= 0)
				{
					callback = section->callback;
					if (callback)
						((void(__cdecl*)(__int16**, int, int, DWORD, struct _G2AnimFxHeader_Type*, void*))callback)(
							&section[-sectionID - 1].swAlarmTable,
							sectionID,
							6,
							i,
							&fxHeader[1],
							section->callbackData);
				}
			}
			i = *(&fxHeader->type + v9);
		}
	}
#endif
}

void _G2Anim_FreeChanStatusBlockList(struct _G2AnimChanStatusBlock_Type* block)//Matching - 92.81%
{
	struct _G2AnimChanStatusBlock_Type* nextBlock;
	
	while (block != NULL)
	{
		nextBlock = block->next;

		G2PoolMem_Free(&_chanStatusBlockPool, block);

		block = nextBlock;
	}
}

long _G2AnimAlphaTable_GetValue(struct _G2AnimAlphaTable_Type* table, long trueAlpha)//Matching - 98.80%
{
	long position;
	long positionInt;
	long positionFrac;
	long value;

	if (table == NULL)
	{
		return trueAlpha;
	}
	else
	{
		position = (table->size - 1) * trueAlpha;

		positionInt = position >> 12;

		value = table->data[positionInt + 1];

		trueAlpha = table->data[positionInt];

		positionFrac = position & 0xFFF;

		return value + (((trueAlpha - value) * positionFrac) >> 12);
	}
}