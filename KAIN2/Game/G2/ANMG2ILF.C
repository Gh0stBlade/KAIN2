#include "Game/CORE.H"
#include "ANMG2ILF.H"
#include "ANIMG2.H"
#include "ANMINTRP.H"
#include "Game/G2/TIMERG2.H"

short G2Anim_GetElapsedTime(struct _G2Anim_Type* anim)
{
	struct _G2AnimSection_Type* section;
	section = &anim->section[anim->masterSection];
	return section->elapsedTime;
}

struct _G2AnimKeylist_Type* G2Anim_GetKeylist(struct _G2Anim_Type* anim)
{
	struct _G2AnimSection_Type* section;
	section = &anim->section[anim->masterSection];
	return section->keylist;
}

void G2Anim_GetRootMotionOverInterval(struct _G2Anim_Type* anim, short intervalStart, short intervalEnd, struct _G2SVector3_Type* motionVector)
{
	G2Anim_GetRootMotionFromTimeForDuration(anim, intervalStart, intervalEnd - intervalStart, motionVector);
}

void G2Anim_InterpToKeylistFrame(struct _G2Anim_Type* anim, struct _G2AnimKeylist_Type* keylist, int keylistID, int targetFrame, int duration)//Matching - 89.12%
{
	int i;

	for (i = 0; i < anim->sectionCount; i++)
	{
		G2AnimSection_InterpToKeylistFrame(&anim->section[i], keylist, keylistID, targetFrame, (short)duration);
	}
}

void G2Anim_SetAlphaTable(struct _G2Anim_Type* anim, struct _G2AnimAlphaTable_Type* table)//Matching - 99.82%
{
	int i;

	for (i = 0; i < anim->sectionCount; i++)
	{
		G2AnimSection_SetAlphaTable(&anim->section[i], table);
	}
}

void G2Anim_SetCallback(struct _G2Anim_Type *anim, unsigned long (*func)(struct _G2Anim_Type*, int, enum _G2AnimCallbackMsg_Enum, long, long, struct _Instance*), void *data)
{
	int i;

	if (anim->sectionCount != 0)
	{
		for (i = 0; i < anim->sectionCount; i++)
		{
			anim->section[i].callback = func;
			anim->section[i].callbackData = data;
		}
	}
}

void G2Anim_SetLooping(struct _G2Anim_Type* anim)  // Matching - 100%
{
	int i;

	for (i = 0; i < anim->sectionCount; i++)
	{
		G2AnimSection_SetLooping(&anim->section[i]);
	}
}

void G2Anim_SetNoLooping(struct _G2Anim_Type* anim)//Matching - 99.79%
{
	int i;

	for (i = 0; i < anim->sectionCount; i++)
	{
		G2AnimSection_SetNoLooping(&anim->section[i]);
	}
}

void G2Anim_SetPaused(struct _G2Anim_Type* anim)
{
	UNIMPLEMENTED();
}

void G2Anim_SetSpeedAdjustment(struct _G2Anim_Type* anim, long adjustment)
{
	int i;

	for (i = 0; i < anim->sectionCount; i++)
	{
		anim->section[i].speedAdjustment = adjustment;
	}
}

void G2Anim_SetUnpaused(struct _G2Anim_Type* anim)//Matching - 99.79%
{
	int i;

	for (i = 0; i < anim->sectionCount; i++)
	{
		G2AnimSection_SetUnpaused(&anim->section[i]);
	}
}

void G2Anim_SwitchToKeylist(struct _G2Anim_Type *anim, struct _G2AnimKeylist_Type *keylist, int keylistID)
{
	int i;

	if (anim->sectionCount != 0)
	{
		for (i = 0; i < anim->sectionCount; i++)
		{
			G2AnimSection_SwitchToKeylist(&anim->section[i], keylist, keylistID);
		}
	}
}

short G2AnimKeylist_GetDuration(struct _G2AnimKeylist_Type* keylist)//Matching - 100%
{
	return keylist->s0TailTime + (keylist->timePerKey * (keylist->keyCount - 1));
}

int G2AnimKeylist_GetKeyframeCount(struct _G2AnimKeylist_Type* keylist)
{
	return (((keylist->timePerKey * (keylist->keyCount - 1))) + (keylist->s0TailTime * 2 - 1)) / keylist->s0TailTime;
}

void G2AnimSection_ClearAlarm(struct _G2AnimSection_Type *section, unsigned long flag)
{
	section->alarmFlags &= ~flag;
}

int G2AnimSection_GetKeyframeNumber(struct _G2AnimSection_Type* section)//Matching - 60.65%
{
	if (G2AnimSection_IsInInterpolation(section) != 0)
	{
		return section->interpInfo->targetTime / section->keylist->s0TailTime;
	}
	else
	{
		return section->elapsedTime / section->keylist->s0TailTime;
	}
}

int G2AnimSection_GetStoredKeyframeNumber(struct _G2AnimSection_Type* section)
{
	return section->storedTime / section->keylist->s0TailTime;
}

void G2AnimSection_InterpToKeylistFrame(struct _G2AnimSection_Type* section, struct _G2AnimKeylist_Type* keylist, int keylistID, int targetFrame, int duration)//Matching - 99.69%
{
	G2AnimSection_InterpToKeylistAtTime(section, keylist, keylistID, (short)(targetFrame * keylist->s0TailTime), (short)duration);
}

enum _G2Bool_Enum G2AnimSection_IsInInterpolation(struct _G2AnimSection_Type* section)
{
	struct _G2AnimInterpInfo_Type* interpInfo;

	interpInfo = section->interpInfo;

	if (interpInfo != NULL && interpInfo->stateBlockList != NULL)
	{
		return (enum _G2Bool_Enum)1;
	}

	return G2FALSE;
}

short G2AnimSection_NextKeyframe(struct _G2AnimSection_Type* section)
{
	if (!(section->flags & 0x1))
	{
		G2AnimSection_SetNotRewinding(section);

		return G2AnimSection_UpdateOverInterval(section, G2Timer_GetFrameTime());
	}

	return 0;
}

void G2AnimSection_SetAlphaTable(struct _G2AnimSection_Type *section, struct _G2AnimAlphaTable_Type *table)
{
	if (section->interpInfo != NULL)
	{
		section->interpInfo->alphaTable = table;
	}
}

void G2AnimSection_SetInterpInfo(struct _G2AnimSection_Type *section, struct _G2AnimInterpInfo_Type *newInfoPtr)
{
	section->interpInfo = newInfoPtr;
	
	if (section->interpInfo != NULL)
	{
		memset(section->interpInfo, 0, sizeof(struct _G2AnimInterpInfo_Type));
	}
}

void G2AnimSection_SetLooping(struct _G2AnimSection_Type *section)
{
	G2AnimSection_ClearAlarm(section, 0x3);

	G2AnimSection_SetLoopRangeAll(section);

	section->flags |= 0x2;
}

void G2AnimSection_SetLoopRangeAll(struct _G2AnimSection_Type *section)
{
	section->loopStartTime = 0;
	section->loopEndTime = G2AnimKeylist_GetDuration(section->keylist);
}

void G2AnimSection_SetNoLooping(struct _G2AnimSection_Type *section)
{
	section->flags &= ~0x2;
}

void G2AnimSection_SetNotRewinding(struct _G2AnimSection_Type* section)
{
	section->flags &= ~0x4;
}

void G2AnimSection_SetPaused(struct _G2AnimSection_Type *section)
{
	section->flags |= 0x1;
}

void G2AnimSection_SetUnpaused(struct _G2AnimSection_Type *section)
{
	section->flags &= ~0x1;
}

void G2AnimSection_SwitchToKeylist(struct _G2AnimSection_Type* section, struct _G2AnimKeylist_Type* keylist, int keylistID)
{
	G2AnimSection_SwitchToKeylistAtTime(section, keylist, keylistID, 0);
}
