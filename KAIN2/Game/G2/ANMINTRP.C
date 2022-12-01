#include "Game/CORE.H"
#include "ANMINTRP.H"
#include "ANIMG2.H"
#include "ANMG2ILF.H"
#include "Game/PSX/PSX_G2/QUATVM.H"
#include "POOLMMG2.H"
#include "Game/PSX/COLLIDES.H"

struct _G2AnimSegValue_Type _segValues[80];

void G2AnimSection_InterpToKeylistAtTime(struct _G2AnimSection_Type* section, struct _G2AnimKeylist_Type* keylist, int keylistID, short targetTime, int duration)
{
	struct _G2Anim_Type* anim;
	struct _G2AnimInterpInfo_Type* interpInfo;
	struct _G2AnimInterpStateBlock_Type* stateBlockList;
	struct _G2AnimQuatInfo_Type* quatInfo;
	unsigned long alarmFlags;
	short elapsedTime;
	int quatInfoChunkCount;
	int segCount;

	if (duration == 0)
	{
		G2AnimSection_SwitchToKeylistAtTime(section, keylist, keylistID, targetTime);
		return;
	}

	anim = _G2AnimSection_GetAnim(section);

	interpInfo = section->interpInfo;

	segCount = section->segCount;

	if (interpInfo->stateBlockList != NULL)
	{
		_G2AnimSection_InterpStateToQuat(section);
	}
	else if (_G2Anim_AllocateInterpStateBlockList(section) == NULL)
	{
		G2AnimSection_SwitchToKeylistAtTime(section, keylist, keylistID, targetTime);
		return;
	}
	else
	{
		section->interpInfo = NULL;

		_G2AnimSection_UpdateStoredFrameFromData(section, anim);

		section->interpInfo = interpInfo;

		_G2AnimSection_SegValueToQuat(section, 0);
	}

	elapsedTime = section->elapsedTime;

	alarmFlags = section->alarmFlags;

	section->interpInfo = NULL;

	G2AnimSection_SwitchToKeylistAtTime(section, keylist, keylistID, targetTime);

	_G2AnimSection_UpdateStoredFrameFromData(section, anim);

	section->interpInfo = interpInfo;
	section->alarmFlags = alarmFlags;
	section->elapsedTime = elapsedTime;

	_G2AnimSection_SegValueToQuat(section, 1);

	stateBlockList = interpInfo->stateBlockList;

	quatInfoChunkCount = 4;

	quatInfo = &stateBlockList->quatInfo[0];

	if ((quatInfo->destTrans.x | quatInfo->destTrans.y | quatInfo->destTrans.z) == 0)
	{
		quatInfo->srcTrans.x = 0;
		quatInfo->srcTrans.y = 0;
		quatInfo->srcTrans.z = 0;
	}

	while (segCount > 0)
	{
		quatInfoChunkCount--;

		quatInfo->destScale.x -= quatInfo->srcScale.x;
		quatInfo->destScale.y -= quatInfo->srcScale.y;
		quatInfo->destScale.z -= quatInfo->srcScale.z;

		quatInfo->destTrans.x -= quatInfo->srcTrans.x;
		quatInfo->destTrans.y -= quatInfo->srcTrans.y;
		quatInfo->destTrans.z -= quatInfo->srcTrans.z;

		if (quatInfoChunkCount == 0)
		{
			stateBlockList = stateBlockList->next;
			quatInfoChunkCount = 4;
			quatInfo = &stateBlockList->quatInfo[0];
		}
		quatInfo++;

		segCount--;
	}

	segCount++;

	interpInfo->targetTime = targetTime;
	interpInfo->duration = duration;

	if (!(section->flags & 0x2) && (alarmFlags & 0x3))
	{
		if (duration < (elapsedTime % section->keylist->s0TailTime) + 1)
		{
			elapsedTime = duration;
		}
		else
		{
			elapsedTime = (elapsedTime % section->keylist->s0TailTime) + 1;
		}

		section->elapsedTime = elapsedTime;
	}
	else
	{
		section->elapsedTime = 0;
	}

	section->keylist = keylist;
	section->keylistID = keylistID;

	keylist->timePerKey = -keylist->timePerKey;
	if ((section->flags & 0x2))
	{
		G2AnimSection_SetLoopRangeAll(section);
	}

	G2AnimSection_ClearAlarm(section, 0x3);

	section->flags &= 0x7F;

	G2AnimSection_SetUnpaused(section);

	section->swAlarmTable = NULL;
	section->interpInfo = interpInfo;

	anim->flags |= 0x1;
}

void _G2AnimSection_UpdateStoredFrameFromQuat(struct _G2AnimSection_Type* section)
{
	struct _G2AnimSegValue_Type* segValue;
	struct _G2AnimInterpInfo_Type* interpInfo;
	struct _G2AnimInterpStateBlock_Type* stateBlockList;
	struct _G2AnimQuatInfo_Type* quatInfo;
	long alpha;
	struct _G2Quat_Type newQuat;
	int quatInfoChunkCount;
	int segCount;
	struct _G2Quat_Type* source;
	struct _G2Quat_Type* dest;
	unsigned long zw;
	unsigned long xy;

#define GET_XY(a) *(long*)(&a->x);
#define SET_XY(a, c) *(long*)(&a.x) = c;
#define GET_ZW(b) *(long*)(&b->z);
#define SET_ZW(b, c) *(long*)(&b.z) = c;

	interpInfo = section->interpInfo;

	alpha = (section->elapsedTime * 4096) / interpInfo->duration;

	quatInfoChunkCount = 4;

	segCount = section->segCount;

	alpha = _G2AnimAlphaTable_GetValue(interpInfo->alphaTable, alpha);

	stateBlockList = interpInfo->stateBlockList;

	segValue = &_segValues[section->firstSeg];

	quatInfo = stateBlockList->quatInfo;

	if (segCount > 0)
	{
		struct _G2SVector3_Type* dest;
		struct _G2SVector3_Type* base;
		struct _G2SVector3_Type* offset;

		source = &newQuat;

		do
		{
			G2Quat_Slerp_VM(alpha, &quatInfo->srcQuat, &quatInfo->destQuat, &newQuat, 0);

			dest = &segValue->scale;
			base = &quatInfo->srcScale;
			offset = &quatInfo->destScale;

			xy = GET_XY(source);
			zw = GET_ZW(source);


			SET_XY(segValue->rotQuat.quat, xy);
			SET_ZW(segValue->rotQuat.quat, zw);

			gte_ldlvnlsv(base);

			gte_ldsv(offset);

			gte_lddp(alpha);

			gte_gpl12();

			gte_stlvnlsv(dest);

			dest = &segValue->trans;

			base = &quatInfo->srcTrans;

			offset = &quatInfo->destTrans;

			gte_ldlvnlsv(base);

			gte_ldsv(offset);

			gte_lddp(alpha);

			gte_gpl12();

			gte_stlvnlsv(dest);

			segCount--;
			quatInfoChunkCount--;
			quatInfo++;

			if (quatInfoChunkCount == 0)
			{
				stateBlockList = stateBlockList->next;
				quatInfoChunkCount = 4;
				quatInfo = stateBlockList->quatInfo;
			}

			segValue->bIsQuat = 1;
			segValue++;

		} while (segCount > 0);
	}

	section->storedTime = section->elapsedTime;
	section->flags |= 0x80;
}

void _G2AnimSection_InterpStateToQuat(struct _G2AnimSection_Type* section)//Matching - 83.15%
{
	struct _G2AnimInterpInfo_Type* interpInfo;
	struct _G2AnimInterpStateBlock_Type* stateBlockList;
	struct _G2AnimQuatInfo_Type* quatInfo;
	long alpha;
	struct _G2Quat_Type newQuat;
	int quatInfoChunkCount;
	int segCount;
	struct _G2Quat_Type* source;
	struct _G2Quat_Type* dest;
	unsigned long zw;
	unsigned long xy;

#define GET_XY(a) *(long*)(&a.x);
#define SET_XY(a, c) *(long*)(&a.x) = c;
#define GET_ZW(b) *(long*)(&b->z);
#define SET_ZW(b, c) *(long*)(&b.z) = c;

	interpInfo = section->interpInfo;
	alpha = (section->elapsedTime * 4096) / interpInfo->duration;
	quatInfoChunkCount = 4;
	segCount = section->segCount;

	alpha = _G2AnimAlphaTable_GetValue(interpInfo->alphaTable, alpha);
	stateBlockList = interpInfo->stateBlockList;

	quatInfo = &stateBlockList->quatInfo[0];

	if (segCount > 0)
	{
		source = &newQuat;

		do
		{
			struct _G2SVector3_Type* dest;
			struct _G2SVector3_Type* offset;

			G2Quat_Slerp_VM(alpha, &quatInfo->srcQuat, &quatInfo->destQuat, &newQuat, 0);

			zw = GET_ZW(source);
			SET_ZW(quatInfo->srcQuat, zw);

			xy = GET_XY(newQuat);
			SET_XY(quatInfo->srcQuat, xy);

			dest = &quatInfo->srcScale;
			offset = &quatInfo->destScale;

			gte_ldlvnlsv(dest);

			gte_ldsv(offset);

			gte_lddp(alpha);

			gte_gpl12();

			gte_stlvnlsv(dest);

			dest = &quatInfo->srcTrans;
			offset = &quatInfo->destTrans;

			gte_ldlvnlsv(dest);

			gte_ldsv(offset);

			gte_lddp(alpha);

			gte_gpl12();

			gte_stlvnlsv(dest);

			segCount--;

			quatInfoChunkCount--;

			quatInfo++;

			if (quatInfoChunkCount == 0)
			{
				stateBlockList = stateBlockList->next;

				quatInfoChunkCount = 4;

				quatInfo = stateBlockList->quatInfo;
			}

		} while (segCount > 0);
	}
}

void _G2AnimSection_SegValueToQuat(struct _G2AnimSection_Type* section, int zeroOne)//Matching - 98.79%
{
	struct _G2AnimSegValue_Type* segValue;
	struct _G2AnimInterpInfo_Type* interpInfo;
	struct _G2AnimInterpStateBlock_Type* stateBlockList;
	struct _G2AnimQuatInfo_Type* quatInfo;
	struct _G2EulerAngles_Type preQuat;
	int quatInfoChunkCount;
	int segCount;

	quatInfoChunkCount = 4;
	segCount = section->segCount;
	interpInfo = section->interpInfo;
	stateBlockList = interpInfo->stateBlockList;
	segValue = &_segValues[section->firstSeg];
	quatInfo = stateBlockList->quatInfo;

	while (segCount > 0)
	{
		preQuat.x = segValue->rotQuat.quat.x & 0xFFF;
		preQuat.y = segValue->rotQuat.quat.y & 0xFFF;
		preQuat.z = segValue->rotQuat.quat.z & 0xFFF;
		preQuat.order = 0;

		if (zeroOne == 0)
		{
			G2Quat_FromEuler_S(&quatInfo->srcQuat, &preQuat);

			quatInfo->srcScale.x = segValue->scale.x;
			quatInfo->srcScale.y = segValue->scale.y;
			quatInfo->srcScale.z = segValue->scale.z;

			quatInfo->srcTrans.x = segValue->trans.x;
			quatInfo->srcTrans.y = segValue->trans.y;
			quatInfo->srcTrans.z = segValue->trans.z;
		}
		else
		{
			G2Quat_FromEuler_S(&quatInfo->destQuat, &preQuat);

			quatInfo->destScale.x = segValue->scale.x;
			quatInfo->destScale.y = segValue->scale.y;
			quatInfo->destScale.z = segValue->scale.z;

			quatInfo->destTrans.x = segValue->trans.x;
			quatInfo->destTrans.y = segValue->trans.y;
			quatInfo->destTrans.z = segValue->trans.z;
		}

		segValue++;
		segCount--;
		quatInfo++;

		if (--quatInfoChunkCount == 0)
		{
			stateBlockList = stateBlockList->next;
			quatInfoChunkCount = 4;
			quatInfo = stateBlockList->quatInfo;
		}
	}
}

struct _G2AnimInterpStateBlock_Type* _G2Anim_AllocateInterpStateBlockList(struct _G2AnimSection_Type* section)//Matching - 93.55%
{
	struct _G2AnimInterpInfo_Type* interpInfo;
	struct _G2AnimInterpStateBlock_Type* newBlock;
	int segCount;

	segCount = section->segCount;

	interpInfo = section->interpInfo;

	interpInfo->stateBlockList = (struct _G2AnimInterpStateBlock_Type*)G2PoolMem_Allocate(&_interpStateBlockPool);

	if (interpInfo->stateBlockList != NULL)
	{
		newBlock = interpInfo->stateBlockList;

		segCount -= 4;

		while (segCount > 0)
		{
			newBlock->next = (struct _G2AnimInterpStateBlock_Type*)G2PoolMem_Allocate(&_interpStateBlockPool);

			if (newBlock->next == NULL)
			{
				_G2Anim_FreeInterpStateBlockList(interpInfo->stateBlockList);

				interpInfo->stateBlockList = NULL;

				return NULL;
			}

			newBlock = newBlock->next;
			segCount -= 4;
		}
	}

	newBlock->next = NULL;

	return interpInfo->stateBlockList;
}

void _G2Anim_FreeInterpStateBlockList(struct _G2AnimInterpStateBlock_Type* block)//Matching - 92.81%
{
	struct _G2AnimInterpStateBlock_Type* nextBlock;
	
	while (block != NULL)
	{
		nextBlock = block->next;
		
		G2PoolMem_Free(&_interpStateBlockPool, block);

		block = nextBlock;
	}
}