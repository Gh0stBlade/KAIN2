#include "Game/CORE.H"
#include "ANMDECMP.H"

int _stepSizeTable[] = {
	0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0x10,
    0x12, 0x14, 0x16, 0x18, 0x1A, 0x1D,
    0x20, 0x23, 0x26, 0x29, 0x2D, 0x31,
    0x35, 0x39, 0x3E, 0x43, 0x48, 0x4F,
    0x55, 0x5B, 0x61, 0x68, 0x6F, 0x76,
    0x7E, 0x6B, 0x73, 0x7C, 0x85, 0x8E,
    0x98, 0xA2, 0xAD, 0xB8, 0xC4, 0xD0,
    0xDD, 0xEA, 0xF8, 0x106, 0x115, 0x125,
    0x135, 0x146, 0x157, 0x169, 0x17C,
    0x190, 0x1A5, 0x1BB, 0x1D2, 0x1EB,
    0x206, 0x223, 0x243,
};

int _indexTable[] = {
	 -1, -2, -1,
     -1, 1, 2, 4, 7, -1,
     -2, -1, -1,
     1, 2, 4, 7,
};

void _G2Anim_DecompressChannel_AdaptiveDelta(struct _G2AnimDecompressChannelInfo_Type* dcInfo, struct _G2AnimChanStatus_Type* status)
{
#if defined(PSX_VERSION)
	unsigned short* chanData; // $t7
	int index; // $t2
	int keyData; // $t5
	unsigned short step; // $v1
	unsigned short predictedDelta; // $a2
	int targetKey; // $t6
	int storedKey; // $t4
	int keyCount; // $t8

	storedKey = dcInfo->storedKey;
	keyData = status->keyData;
	index = status->index;
	keyCount = dcInfo->keylist->keyCount + 3;
	targetKey = dcInfo->targetKey;
	chanData = &dcInfo->chanData[2];

	while (storedKey < targetKey)
	{
		storedKey++;

		step = _stepSizeTable[index];
		index += _indexTable[chanData[(storedKey >> 2)] >> ((storedKey & 0x3) << 2) & 0xF];

		if (index < 0)
		{
			index = 0;
		}

		if (index >= 64)
		{
			index = 63;
		}

		predictedDelta = step >> 3;

		if ((chanData[(storedKey >> 2)] >> ((storedKey & 0x3) << 2) & 0x4))
		{
			predictedDelta = step + predictedDelta;
		}

		if ((chanData[(storedKey >> 2)] >> ((storedKey & 0x3) << 2) & 0x2))
		{
			predictedDelta += (step >> 1);
		}

		if ((chanData[(storedKey >> 2)] >> ((storedKey & 0x3) << 2) & 0x1))
		{
			predictedDelta = predictedDelta + (step >> 2) + (step & 0x1);
		}

		if ((chanData[(storedKey >> 2)] >> ((storedKey & 0x3) << 2) & 0x8))
		{
			keyData -= predictedDelta;
		}
		else
		{
			keyData += predictedDelta;
		}

		storedKey++;
	}

	storedKey--;

	status->index = index;
	status->keyData = keyData;
	dcInfo->chanData = &chanData[keyCount >> 2];

#elif defined(PC_VERSION)
	struct _G2AnimChanStatus_Type* v2; // ebx
	struct _G2AnimDecompressChannelInfo_Type* v3; // edi
	int storedKey; // edx
	u_short* v5; // eax
	int v6; // ecx
	int index; // esi
	unsigned __int16 v8; // bp
	unsigned __int16 v9; // ax
	unsigned __int16 v10; // cx
	int v11; // eax
	int keyData; // [esp+14h] [ebp-10h]

	v2 = status;
	v3 = dcInfo;
	storedKey = dcInfo->storedKey;
	v5 = dcInfo->chanData + 2;
	v6 = dcInfo->keylist->keyCount + 3;
	keyData = status->keyData;
	index = status->index;
	if (storedKey < dcInfo->targetKey)
	{
		while (1)
		{
			++storedKey;
			v8 = word_4F4AE0[2 * index];
			v9 = (v5[storedKey >> 2] >> (4 * (storedKey & 3))) & 0xF;
			index += dword_4F4AA0[v9];
			if (index < 0)
				index = 0;
			if (index > 63)
				index = 63;
			v10 = v8 >> 3;
			if ((v9 & 4) != 0)
				v10 += v8;
			if ((v9 & 2) != 0)
				v10 += v8 >> 1;
			if ((v9 & 1) != 0)
				v10 += (v8 >> 2) + (v8 & 1);
			v11 = (v9 & 8) != 0 ? keyData - v10 : v10 + keyData;
			keyData = v11;
			if (storedKey >= dcInfo->targetKey)
				break;
			v5 = dcInfo->chanData + 2;
		}
		v2 = status;
		v6 = dcInfo->keylist->keyCount + 3;
		v5 = dcInfo->chanData + 2;
		v3 = dcInfo;
	}
	v2->index = index;
	v2->keyData = keyData;
	v3->chanData = &v5[v6 >> 2];
#endif
}

void _G2Anim_DecompressChannel_Linear(struct _G2AnimDecompressChannelInfo_Type* dcInfo, struct _G2AnimChanStatus_Type* status)
{
	unsigned short* chanData;
	short rangeBase;
	short rangeInfo;
	int rangeLength;
	int rangeOffset;
	int targetKey;

	chanData = dcInfo->chanData;
	targetKey = dcInfo->targetKey;

	dcInfo->chanData = &chanData[((chanData[0] & 0xFFF)) + 1];
	rangeBase = chanData[1];
	rangeInfo = chanData[2];
	chanData += 1;

loop_2:
	rangeLength = (rangeInfo & 0xF800) >> 0xB;
	rangeOffset = (rangeInfo << 0x15) >> 0x15;
	if (rangeLength < targetKey)
	{
		rangeInfo = chanData[2];
		targetKey -= rangeLength;
		rangeBase += rangeOffset;
		chanData += 1;
		goto loop_2;
	}

	status->keyData = rangeBase + ((rangeOffset * targetKey) / rangeLength);
}

void _G2Anim_InitializeChannel_AdaptiveDelta(struct _G2AnimDecompressChannelInfo_Type* dcInfo, struct _G2AnimChanStatus_Type* status)
{
#if defined(PSX_VERSION)
	unsigned short* chanData;
	int keyCount;

	keyCount = dcInfo->keylist->keyCount;

	chanData = dcInfo->chanData;

	status->index = ((unsigned char*)chanData)[0];

	status->keyData = chanData[1];

	dcInfo->chanData = &chanData[((keyCount + 3) >> 2)] + 2;

#elif defined(PC_VERSION)
	u_short* chanData; // eax
	int v3; // ecx

	chanData = dcInfo->chanData;
	v3 = (dcInfo->keylist->keyCount + 3) >> 2;
	status->index = *(unsigned __int8*)chanData;
	status->keyData = chanData[1];
	dcInfo->chanData = &chanData[v3 + 2];
#endif
}

void _G2Anim_InitializeChannel_Linear(struct _G2AnimDecompressChannelInfo_Type* dcInfo, struct _G2AnimChanStatus_Type* status)//Matching - 100%
{
#if defined(PSX_VERSION)
	unsigned short* chanData;
	int chanLength;

	chanData = dcInfo->chanData;

	chanLength = (chanData[0] & 0xFFF) + 1;

	status->keyData = chanData[1];

	dcInfo->chanData = &chanData[chanLength];

#elif defined(PC_VERSION)
	u_short* chanData; // ecx
	int v3; // eax

	chanData = dcInfo->chanData;
	v3 = *chanData & 0xFFF;
	status->keyData = chanData[1];
	dcInfo->chanData = &chanData[v3 + 1];
#endif
}
