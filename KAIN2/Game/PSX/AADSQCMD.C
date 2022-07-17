#include "CORE.H"
#include "AADSQCMD.H"
#include "AADLIB.H"
#include "AADSEQEV.H"

void aadSubstituteVariables(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	unsigned char trackFlags;
	
	trackFlags = slot->trackFlags[event->track];

	if ((trackFlags & 0x7))
	{
		if ((unsigned int)(event->statusByte - 65) >= 3)
		{
			if ((trackFlags & 0x1))
			{
				trackFlags &= 0xFE;

				event->dataByte[0] = aadMem->userVariables[(unsigned char)event->dataByte[0]];
			}

			if ((trackFlags & 0x2))
			{
				trackFlags &= 0xFD;
				
				event->dataByte[1] = aadMem->userVariables[(unsigned char)event->dataByte[1]];
			}
			
			if ((trackFlags & 0x4))
			{
				trackFlags &= 0xFB;

				event->dataByte[2] = aadMem->userVariables[(unsigned char)event->dataByte[2]];
			}

			slot->trackFlags[event->track] = trackFlags;
		}
	}
}

void metaCmdSelectChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	int channelNumber;

	channelNumber = (unsigned char)event->dataByte[0];

	if (channelNumber < 16)
	{
		slot->selectedChannel = channelNumber;
	}

#elif defined(PC_VERSION)
	int v2; // eax

	v2 = event->dataByte[0];
	if (v2 < 16)
		slot->selectedChannel = v2;
#endif
}

void metaCmdSelectSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)

	int slotNumber;

	slotNumber = event->dataByte[0];//v1

	if (slotNumber < aadMem->numSlots)
	{
		slot->selectedSlotNum = slotNumber;
		
		slot->selectedSlotPtr = aadMem->sequenceSlots[slotNumber];
	}
	else if (slotNumber == 127)
	{
		slot->selectedSlotPtr = slot;

		slot->selectedSlotNum = slot->thisSlotNumber;
	}

#elif defined(PC_VERSION)
	int v2; // eax
	unsigned __int8 thisSlotNumber; // cl

	v2 = event->dataByte[0];
	if (v2 >= aadMem->numSlots)
	{
		if (v2 == 127)
		{
			thisSlotNumber = slot->thisSlotNumber;
			slot->selectedSlotPtr = slot;
			slot->selectedSlotNum = thisSlotNumber;
		}
	}
	else
	{
		slot->selectedSlotPtr = aadMem->sequenceSlots[v2];
		slot->selectedSlotNum = v2;
	}
#endif
}

void metaCmdAssignSequence(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
#if defined(PSX_VERSION)
	int sequenceNumber;
	int bank;

	bank = slot->selectedDynamicBank;
	sequenceNumber = (unsigned char)event->dataByte[0];

	if (aadMem->dynamicBankStatus[bank] == 2 && sequenceNumber < aadGetNumDynamicSequences(bank))
	{
		aadAssignDynamicSequence(bank, sequenceNumber, slot->selectedSlotNum);
	}

#elif defined(PC_VERSION)
	int selectedDynamicBank; // esi
	int v3; // edi

	selectedDynamicBank = slot->selectedDynamicBank;
	v3 = event->dataByte[0];
	if (aadMem->dynamicBankStatus[selectedDynamicBank] == 2 && v3 < aadGetNumDynamicSequences(slot->selectedDynamicBank))
		aadAssignDynamicSequence(selectedDynamicBank, v3, slot->selectedSlotNum);
#endif
}

void metaCmdUsePrimaryTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{

}


void metaCmdUseSecondaryTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{

}

void metaCmdSetTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	struct AadTempo tempo;

	tempo.quarterNoteTime = (unsigned char)event->dataByte[0] << 16;
	tempo.quarterNoteTime |= (unsigned char)event->dataByte[1] << 8;
	tempo.quarterNoteTime |= (unsigned char)event->dataByte[2];

	tempo.ppqn = slot->selectedSlotPtr->tempo.ppqn;

	aadSetSlotTempo(slot->selectedSlotNum, &tempo);

#elif defined(PC_VERSION)
	struct _AadSequenceSlot* selectedSlotPtr; // eax
	int v3; // [esp+0h] [ebp-8h] BYREF
	int ppqn; // [esp+4h] [ebp-4h]

	v3 = event->dataByte[2] | (event->dataByte[1] << 8) | (event->dataByte[0] << 16);
	selectedSlotPtr = slot->selectedSlotPtr;
	if (selectedSlotPtr->tempo.ticksPerUpdate)
		ppqn = selectedSlotPtr->tempo.ppqn;
	else
		ppqn = 480;
	aadSetSlotTempo(slot->selectedSlotNum, &v3);
#endif
}

void metaCmdChangeTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	struct AadTempo tempo;
	struct _AadSequenceSlot* selectedSlot;

	selectedSlot = slot->selectedSlotPtr;
	tempo.quarterNoteTime = selectedSlot->tempo.quarterNoteTime * 100;
	tempo.quarterNoteTime = tempo.quarterNoteTime / event->dataByte[0];
	tempo.ppqn = selectedSlot->tempo.ppqn;

	aadSetSlotTempo(slot->selectedSlotNum, &tempo);
#elif defined(PC_VERSION)
	struct _AadSequenceSlot* selectedSlotPtr; // ecx
	int selectedSlotNum; // edx
	int v4[2]; // [esp+8h] [ebp-8h] BYREF

	selectedSlotPtr = slot->selectedSlotPtr;
	selectedSlotNum = slot->selectedSlotNum;
	v4[0] = 100 * selectedSlotPtr->tempo.quarterNoteTime / event->dataByte[0];
	v4[1] = selectedSlotPtr->tempo.ppqn;
	aadSetSlotTempo(selectedSlotNum, v4);
#else
	UNIMPLEMENTED();
#endif
}

void metaCmdSetTempoFromSequence(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	int sequenceNumber;//s1
	struct AadTempo tempo;//-24
	int bank;//s0

	bank = slot->selectedDynamicBank;
	
	sequenceNumber = (unsigned char)event->dataByte[0];

	if (aadMem->dynamicBankStatus[bank] == 2 && sequenceNumber < aadGetNumDynamicSequences(bank))
	{
		aadGetTempoFromDynamicSequence(bank, sequenceNumber, &tempo);
		aadSetSlotTempo(slot->selectedSlotNum, &tempo);
	}

#elif defined(PC_VERSION)
	int selectedDynamicBank; // esi
	int v3; // edi
	char v4[8]; // [esp+Ch] [ebp-8h] BYREF

	selectedDynamicBank = slot->selectedDynamicBank;
	v3 = event->dataByte[0];
	if (aadMem->dynamicBankStatus[selectedDynamicBank] == 2 && v3 < aadGetNumDynamicSequences(slot->selectedDynamicBank))
	{
		aadGetTempoFromDynamicSequence(selectedDynamicBank, v3, v4);
		aadSetSlotTempo(slot->selectedSlotNum, v4);
	}
#endif
}

void metaCmdStartSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	aadStartSlot(slot->selectedSlotNum);
#elif defined(PC_VERSION)
	aadStartSlot(slot->selectedSlotNum);
#endif
}

void metaCmdStopSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	aadStopSlot(slot->selectedSlotNum);
#elif defined(PC_VERSION)
	aadStopSlot(slot->selectedSlotNum);
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdPauseSlot(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdPauseSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 199, offset 0x80055c8c
#if defined(PC_VERSION)
	aadPauseSlot(slot->selectedSlotNum);
#else
	UNIMPLEMENTED();
#endif
}

void metaCmdResumeSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	aadResumeSlot(slot->selectedSlotNum);
#elif defined(PC_VERSION)
	aadResumeSlot(slot->selectedSlotNum);
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetSlotBendRange(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetSlotBendRange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 213, offset 0x80055cd4
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetChannelBendRange(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetChannelBendRange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 226, offset 0x80055cdc
	UNIMPLEMENTED();
}

void metaCmdSetSlotVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	int volume;

	volume = (unsigned char)event->dataByte[0];

	slot->selectedSlotPtr->slotVolume = volume;

	aadUpdateSlotVolPan(slot->selectedSlotPtr);

#elif defined(PC_VERSION)
	slot->selectedSlotPtr->slotVolume = event->dataByte[0];
	aadUpdateSlotVolPan((int)slot->selectedSlotPtr, (int)slot->selectedSlotPtr);
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetSlotPan(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetSlotPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 244, offset 0x80055d18
#if defined(PC_VERSION)
	slot->selectedSlotPtr->slotPan = event->dataByte[0];
	aadUpdateSlotVolPan((int)slot->selectedSlotPtr, (int)slot->selectedSlotPtr);
#else
	UNIMPLEMENTED();
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetChannelVolume(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetChannelVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 256, offset 0x80055d4c
#if defined(PC_VERSION)
	slot->selectedSlotPtr->volume[slot->selectedChannel] = event->dataByte[0];
	aadUpdateChannelVolPan(slot->selectedSlotPtr, slot->selectedChannel);
#else
	UNIMPLEMENTED();
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetChannelPan(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetChannelPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 268, offset 0x80055d88
#if defined(PC_VERSION)
	slot->selectedSlotPtr->panPosition[slot->selectedChannel] = event->dataByte[0];
	aadUpdateChannelVolPan(slot->selectedSlotPtr, slot->selectedChannel);
#else
	UNIMPLEMENTED();
#endif
}

void metaCmdEnableSustainUpdate(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	int channel;
	
	channel = slot->selectedChannel;

	slot->selectedSlotPtr->enableSustainUpdate = (1 << channel);

#elif defined(PC_VERSION)
	slot->selectedSlotPtr->enableSustainUpdate |= 1 << slot->selectedChannel;
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdDisableSustainUpdate(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdDisableSustainUpdate(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 290, offset 0x80055de4
#if defined(PC_VERSION)
	slot->selectedSlotPtr->enableSustainUpdate &= ~(1 << slot->selectedChannel);
#else
	UNIMPLEMENTED();
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdMuteChannel(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdMuteChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 300, offset 0x80055e08
#if defined(PC_VERSION)
	aadMuteChannels(slot->selectedSlotPtr, 1 << slot->selectedChannel);
#else
	UNIMPLEMENTED();
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdUnMuteChannel(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdUnMuteChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 310, offset 0x80055e34
#if defined(PC_VERSION)
	aadUnMuteChannels(slot->selectedSlotPtr, 1 << slot->selectedChannel);
#else
	UNIMPLEMENTED();
#endif
}

void metaCmdMuteChannelList(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	aadMuteChannels(slot->selectedSlotPtr, (unsigned char)event->dataByte[0] | ((unsigned char)event->dataByte[1] << 8));
#elif defined(PC_VERSION)
	aadMuteChannels(slot->selectedSlotPtr, event->dataByte[0] | (event->dataByte[1] << 8));
#endif
}

void metaCmdUnMuteChannelList(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
#if defined(PSX_VERSION)
	aadUnMuteChannels(slot->selectedSlotPtr, (((unsigned char)event->dataByte[1] << 8) | (unsigned char)event->dataByte[0]));
#elif defined(PC_VERSION)
	aadUnMuteChannels(slot->selectedSlotPtr, (event->dataByte[1] << 8) | event->dataByte[0]);
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetChannelMute(struct AadSeqEvent *event /*$v0*/, struct _AadSequenceSlot *slot /*$s1*/)
void metaCmdSetChannelMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 340, offset 0x80055ec0
#if defined(PC_VERSION)
	int v2; // esi

	v2 = (event->dataByte[1] << 8) | event->dataByte[0];
	aadUnMuteChannels(slot->selectedSlotPtr, ~v2);
	aadMuteChannels(slot->selectedSlotPtr, v2);
#else
	UNIMPLEMENTED();
#endif
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdDelayMute(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdDelayMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 352, offset 0x80055f14
	/* begin block 1 */
		// Start line: 354
		// Start offset: 0x80055F14
		// Variables:
			unsigned long channelMask; // $v1
	/* end block 1 */
	// End offset: 0x80055F14
	// End Line: 356

	/* begin block 2 */
		// Start line: 733
	/* end block 2 */
	// End Line: 734

	/* begin block 3 */
		// Start line: 734
	/* end block 3 */
	// End Line: 735

	/* begin block 4 */
		// Start line: 736
	/* end block 4 */
	// End Line: 737
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdUpdateMute(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$s1*/)
void metaCmdUpdateMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 365, offset 0x80055f38
	/* begin block 1 */
		// Start line: 366
		// Start offset: 0x80055F38
		// Variables:
			unsigned long channelMask; // $s0
			unsigned long mask; // $a1
	/* end block 1 */
	// End offset: 0x80055FB8
	// End Line: 379

	/* begin block 2 */
		// Start line: 763
	/* end block 2 */
	// End Line: 764
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdChannelVolumeFade(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdChannelVolumeFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 382, offset 0x80055fcc
	/* begin block 1 */
		// Start line: 799
	/* end block 1 */
	// End Line: 800

	/* begin block 2 */
		// Start line: 809
	/* end block 2 */
	// End Line: 810
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdChannelPanFade(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdChannelPanFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 396, offset 0x80055fd4
	/* begin block 1 */
		// Start line: 827
	/* end block 1 */
	// End Line: 828

	/* begin block 2 */
		// Start line: 837
	/* end block 2 */
	// End Line: 838
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSlotVolumeFade(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSlotVolumeFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 410, offset 0x80055fdc
	/* begin block 1 */
		// Start line: 855
	/* end block 1 */
	// End Line: 856

	/* begin block 2 */
		// Start line: 864
	/* end block 2 */
	// End Line: 865
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSlotPanFade(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSlotPanFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 423, offset 0x80055fe4
	/* begin block 1 */
		// Start line: 881
	/* end block 1 */
	// End Line: 882

	/* begin block 2 */
		// Start line: 890
	/* end block 2 */
	// End Line: 891
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetChannelProgram(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetChannelProgram(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 436, offset 0x80055fec
	/* begin block 1 */
		// Start line: 438
		// Start offset: 0x80055FEC
		// Variables:
			int program; // $a0
	/* end block 1 */
	// End offset: 0x80055FEC
	// End Line: 441

	/* begin block 2 */
		// Start line: 907
	/* end block 2 */
	// End Line: 908

	/* begin block 3 */
		// Start line: 908
	/* end block 3 */
	// End Line: 909

	/* begin block 4 */
		// Start line: 911
	/* end block 4 */
	// End Line: 912
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetChannelBasePriority(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetChannelBasePriority(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 446, offset 0x80056004
	/* begin block 1 */
		// Start line: 928
	/* end block 1 */
	// End Line: 929

	/* begin block 2 */
		// Start line: 948
	/* end block 2 */
	// End Line: 949
	UNIMPLEMENTED();
}

void metaCmdSetChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int channel;
	int transpose;
	
	channel = slot->selectedChannel;
	transpose = event->dataByte[0];

	slot->selectedSlotPtr->transpose[channel] = transpose;
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdIgnoreChannelTranspose(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdIgnoreChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 481, offset 0x80056024
	/* begin block 1 */
		// Start line: 483
		// Start offset: 0x80056024
		// Variables:
			int channel; // $a0
	/* end block 1 */
	// End offset: 0x80056024
	// End Line: 486

	/* begin block 2 */
		// Start line: 999
	/* end block 2 */
	// End Line: 1000

	/* begin block 3 */
		// Start line: 1000
	/* end block 3 */
	// End Line: 1001

	/* begin block 4 */
		// Start line: 1003
	/* end block 4 */
	// End Line: 1004
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdRespectChannelTranspose(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdRespectChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 491, offset 0x80056044
	/* begin block 1 */
		// Start line: 493
		// Start offset: 0x80056044
		// Variables:
			int channel; // $v0
	/* end block 1 */
	// End offset: 0x80056044
	// End Line: 496

	/* begin block 2 */
		// Start line: 1020
	/* end block 2 */
	// End Line: 1021

	/* begin block 3 */
		// Start line: 1021
	/* end block 3 */
	// End Line: 1022

	/* begin block 4 */
		// Start line: 1024
	/* end block 4 */
	// End Line: 1025
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetChannelPitchMap(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetChannelPitchMap(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 501, offset 0x80056068
	/* begin block 1 */
		// Start line: 1041
	/* end block 1 */
	// End Line: 1042

	/* begin block 2 */
		// Start line: 1052
	/* end block 2 */
	// End Line: 1053
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdIgnoreChannelPitchMap(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdIgnoreChannelPitchMap(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 516, offset 0x80056070
	/* begin block 1 */
		// Start line: 1071
	/* end block 1 */
	// End Line: 1072

	/* begin block 2 */
		// Start line: 1079
	/* end block 2 */
	// End Line: 1080
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdRespectChannelPitchMap(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdRespectChannelPitchMap(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 528, offset 0x80056078
	/* begin block 1 */
		// Start line: 1095
	/* end block 1 */
	// End Line: 1096

	/* begin block 2 */
		// Start line: 1103
	/* end block 2 */
	// End Line: 1104
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetSequenceAssigned(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetSequenceAssigned(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 540, offset 0x80056080
	/* begin block 1 */
		// Start line: 542
		// Start offset: 0x80056080
		// Variables:
			int variableNum; // $a0
	/* end block 1 */
	// End offset: 0x800560A8
	// End Line: 549

	/* begin block 2 */
		// Start line: 1119
	/* end block 2 */
	// End Line: 1120

	/* begin block 3 */
		// Start line: 1120
	/* end block 3 */
	// End Line: 1121

	/* begin block 4 */
		// Start line: 1122
	/* end block 4 */
	// End Line: 1123
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetTempo(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 552, offset 0x800560b0
	/* begin block 1 */
		// Start line: 554
		// Start offset: 0x800560B0
		// Variables:
			int variableNum1; // $a2
			int variableNum2; // $a3
			int variableNum3; // $t0
			unsigned long quarterNoteTime; // $a0
	/* end block 1 */
	// End offset: 0x8005610C
	// End Line: 570

	/* begin block 2 */
		// Start line: 1143
	/* end block 2 */
	// End Line: 1144

	/* begin block 3 */
		// Start line: 1144
	/* end block 3 */
	// End Line: 1145

	/* begin block 4 */
		// Start line: 1147
	/* end block 4 */
	// End Line: 1148
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetSlotStatus(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetSlotStatus(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 573, offset 0x80056114
	/* begin block 1 */
		// Start line: 575
		// Start offset: 0x80056114
		// Variables:
			int variableNum; // $a0
	/* end block 1 */
	// End offset: 0x8005613C
	// End Line: 582

	/* begin block 2 */
		// Start line: 1186
	/* end block 2 */
	// End Line: 1187

	/* begin block 3 */
		// Start line: 1187
	/* end block 3 */
	// End Line: 1188

	/* begin block 4 */
		// Start line: 1189
	/* end block 4 */
	// End Line: 1190
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetChannelMute(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetChannelMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 585, offset 0x80056144
	/* begin block 1 */
		// Start line: 587
		// Start offset: 0x80056144
		// Variables:
			int variableNum1; // $a2
			int variableNum2; // $a0
	/* end block 1 */
	// End offset: 0x8005618C
	// End Line: 598

	/* begin block 2 */
		// Start line: 1210
	/* end block 2 */
	// End Line: 1211

	/* begin block 3 */
		// Start line: 1211
	/* end block 3 */
	// End Line: 1212

	/* begin block 4 */
		// Start line: 1213
	/* end block 4 */
	// End Line: 1214
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetChannelVolume(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetChannelVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 601, offset 0x80056194
	/* begin block 1 */
		// Start line: 603
		// Start offset: 0x80056194
		// Variables:
			int variableNum; // $a0
			int channel; // $v1
	/* end block 1 */
	// End offset: 0x800561C4
	// End Line: 611

	/* begin block 2 */
		// Start line: 1242
	/* end block 2 */
	// End Line: 1243

	/* begin block 3 */
		// Start line: 1243
	/* end block 3 */
	// End Line: 1244

	/* begin block 4 */
		// Start line: 1245
	/* end block 4 */
	// End Line: 1246
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetChannelPan(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetChannelPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 614, offset 0x800561cc
	/* begin block 1 */
		// Start line: 616
		// Start offset: 0x800561CC
		// Variables:
			int variableNum; // $a0
			int channel; // $v1
	/* end block 1 */
	// End offset: 0x800561FC
	// End Line: 623

	/* begin block 2 */
		// Start line: 1268
	/* end block 2 */
	// End Line: 1269

	/* begin block 3 */
		// Start line: 1269
	/* end block 3 */
	// End Line: 1270

	/* begin block 4 */
		// Start line: 1271
	/* end block 4 */
	// End Line: 1272
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetChannelTranspose(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 626, offset 0x80056204
	/* begin block 1 */
		// Start line: 1292
	/* end block 1 */
	// End Line: 1293

	/* begin block 2 */
		// Start line: 1302
	/* end block 2 */
	// End Line: 1303
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetChannelProgram(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetChannelProgram(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 640, offset 0x8005620c
	/* begin block 1 */
		// Start line: 642
		// Start offset: 0x8005620C
		// Variables:
			int variableNum; // $a0
			int channel; // $v1
	/* end block 1 */
	// End offset: 0x8005623C
	// End Line: 649

	/* begin block 2 */
		// Start line: 1320
	/* end block 2 */
	// End Line: 1321

	/* begin block 3 */
		// Start line: 1321
	/* end block 3 */
	// End Line: 1322

	/* begin block 4 */
		// Start line: 1323
	/* end block 4 */
	// End Line: 1324
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetChannelBasePriority(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetChannelBasePriority(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 652, offset 0x80056244
	/* begin block 1 */
		// Start line: 1344
	/* end block 1 */
	// End Line: 1345

	/* begin block 2 */
		// Start line: 1361
	/* end block 2 */
	// End Line: 1362
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetChannelBendRange(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetChannelBendRange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 673, offset 0x8005624c
	/* begin block 1 */
		// Start line: 1386
	/* end block 1 */
	// End Line: 1387

	/* begin block 2 */
		// Start line: 1395
	/* end block 2 */
	// End Line: 1396
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetSlotVolume(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetSlotVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 686, offset 0x80056254
	/* begin block 1 */
		// Start line: 688
		// Start offset: 0x80056254
		// Variables:
			int variableNum; // $a0
	/* end block 1 */
	// End offset: 0x8005627C
	// End Line: 695

	/* begin block 2 */
		// Start line: 1412
	/* end block 2 */
	// End Line: 1413

	/* begin block 3 */
		// Start line: 1413
	/* end block 3 */
	// End Line: 1414

	/* begin block 4 */
		// Start line: 1415
	/* end block 4 */
	// End Line: 1416
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdGetSlotPan(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdGetSlotPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 698, offset 0x80056284
	/* begin block 1 */
		// Start line: 700
		// Start offset: 0x80056284
		// Variables:
			int variableNum; // $a0
	/* end block 1 */
	// End offset: 0x800562AC
	// End Line: 706

	/* begin block 2 */
		// Start line: 1436
	/* end block 2 */
	// End Line: 1437

	/* begin block 3 */
		// Start line: 1437
	/* end block 3 */
	// End Line: 1438

	/* begin block 4 */
		// Start line: 1439
	/* end block 4 */
	// End Line: 1440
			UNIMPLEMENTED();
}

void metaCmdSetVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int value;
	int destVariable;

	destVariable = (unsigned char)event->dataByte[1];
	value = (unsigned char)event->dataByte[0];

	if (destVariable < 128)
	{
		aadMem->userVariables[destVariable] = value;
	}
}

void metaCmdCopyVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int srcVariable;
	int destVariable;
	
	srcVariable = (unsigned char)event->dataByte[0];
	destVariable = (unsigned char)event->dataByte[1];

	if (srcVariable < 128 && destVariable < 128)
	{
		aadMem->userVariables[destVariable] = aadMem->userVariables[srcVariable];
	}
}

void metaCmdAddVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int value;
	int destVariable;
	
	destVariable = (unsigned char)event->dataByte[1];
	value = (unsigned char)event->dataByte[0];

	if (destVariable < 128)
	{
		aadMem->userVariables[destVariable] += value;
	}
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSubtractVariable(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSubtractVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 746, offset 0x80056354
	/* begin block 1 */
		// Start line: 748
		// Start offset: 0x80056354
		// Variables:
			int value; // $a0
			int destVariable; // $a1
	/* end block 1 */
	// End offset: 0x80056384
	// End Line: 755

	/* begin block 2 */
		// Start line: 1534
	/* end block 2 */
	// End Line: 1535

	/* begin block 3 */
		// Start line: 1535
	/* end block 3 */
	// End Line: 1536

	/* begin block 4 */
		// Start line: 1538
	/* end block 4 */
	// End Line: 1539
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetVariableBits(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetVariableBits(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 758, offset 0x8005638c
	/* begin block 1 */
		// Start line: 760
		// Start offset: 0x8005638C
		// Variables:
			int value; // $a0
			int destVariable; // $a1
	/* end block 1 */
	// End offset: 0x800563BC
	// End Line: 767

	/* begin block 2 */
		// Start line: 1559
	/* end block 2 */
	// End Line: 1560

	/* begin block 3 */
		// Start line: 1560
	/* end block 3 */
	// End Line: 1561

	/* begin block 4 */
		// Start line: 1563
	/* end block 4 */
	// End Line: 1564
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdClearVariableBits(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdClearVariableBits(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 770, offset 0x800563c4
	/* begin block 1 */
		// Start line: 772
		// Start offset: 0x800563C4
		// Variables:
			int value; // $a0
			int destVariable; // $v1
	/* end block 1 */
	// End offset: 0x800563F4
	// End Line: 779

	/* begin block 2 */
		// Start line: 1584
	/* end block 2 */
	// End Line: 1585

	/* begin block 3 */
		// Start line: 1585
	/* end block 3 */
	// End Line: 1586

	/* begin block 4 */
		// Start line: 1588
	/* end block 4 */
	// End Line: 1589
			UNIMPLEMENTED();
}

void aadGotoSequencePosition(struct _AadSequenceSlot *slot, int track, unsigned char *newPosition)
{
	slot->sequencePosition[track] = newPosition;

	while (slot->eventsInQueue[track] != 0)
	{
		slot->eventsInQueue[track]--;
		slot->eventOut[track]++;

		if (slot->eventOut[track] == 4)
		{
			slot->eventOut[track] = 0;
		}
	}

	slot->trackFlags[track] &= 0xE7;
}

void aadGotoSequenceLabel(struct _AadSequenceSlot *slot, int track, int labelNumber)
{
	struct AadSequenceHdr* seqHdr;
	unsigned long trackOffset;
	int bank;

	bank = slot->sequenceAssignedDynamicBank;
	seqHdr = (struct AadSequenceHdr*)aadMem->dynamicSequenceAddressTbl[bank][slot->sequenceNumberAssigned];
	trackOffset = ((unsigned long*)(seqHdr + 1))[track];
	slot->sequencePosition[track] = (unsigned char*)(char*)seqHdr + trackOffset + aadMem->dynamicSequenceLabelOffsetTbl[bank][labelNumber];

	if (slot->eventsInQueue[track] != 0)
	{
		do
		{
			slot->eventsInQueue[track]--;
			slot->eventOut[track]++;

			if (slot->eventOut[track] == 4)
			{
				slot->eventOut[track] = 0;
			}

		} while (slot->eventsInQueue[track] != 0);
	}

	slot->trackFlags[track] &= 0xE7;
	
	return;
}

void metaCmdLoopStart(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int nestLevel; // $a2
	int track; // $a3

	track = event->track;
	nestLevel = slot->loopCurrentNestLevel[track];

	if (nestLevel < 4)
	{
		slot->loopSequencePosition[nestLevel][track] = slot->sequencePosition[track];

		slot->loopCounter[nestLevel][track] = (unsigned char)event->dataByte[0];

		slot->loopCurrentNestLevel[track]++;
	}

	slot->trackFlags[track] &= 0xEF;
}

void metaCmdLoopEnd(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int prevNestLevel;
	int track;

	track = event->track;
	prevNestLevel = slot->loopCurrentNestLevel[track] - 1;

	if (slot->loopCurrentNestLevel[track] != 0)
	{
		if (slot->loopCounter[prevNestLevel][track] == 0 || --slot->loopCounter[prevNestLevel][track] != 0)
		{
			aadGotoSequencePosition(slot, track, slot->loopSequencePosition[prevNestLevel][track]);
		}
		else
		{
			slot->loopCurrentNestLevel[track]--;
		}
	}
}

void metaCmdLoopBreak(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ 

}

void metaCmdDefineLabel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
}

void metaCmdGotoLabel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ 
	aadGotoSequenceLabel(slot, event->track, (unsigned char)event->dataByte[0]);
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetSequencePosition(struct AadSeqEvent *event /*$v0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetSequencePosition(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 922, offset 0x80056660
	/* begin block 1 */
		// Start line: 923
		// Start offset: 0x80056660
	/* end block 1 */
	// End offset: 0x80056660
	// End Line: 923

	/* begin block 2 */
		// Start line: 1914
	/* end block 2 */
	// End Line: 1915
	UNIMPLEMENTED();
}

void metaCmdBranchIfVarEqual(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int variableNum;
	int value;
	int labelNum;

	variableNum = (unsigned char)event->dataByte[0];
	value = (unsigned char)event->dataByte[1];
	labelNum = (unsigned char)event->dataByte[2];

	if (aadMem->userVariables[variableNum] == value)
	{
		aadGotoSequenceLabel(slot, event->track, labelNum);
	}
}

void metaCmdBranchIfVarNotEqual(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int variableNum;
	int value;
	int labelNum;

	variableNum = (unsigned char)event->dataByte[0];
	value = (unsigned char)event->dataByte[1];
	labelNum = (unsigned char)event->dataByte[2];

	if (aadMem->userVariables[variableNum] != value)
	{
		aadGotoSequenceLabel(slot, event->track, labelNum);
	}
}

void metaCmdBranchIfVarLess(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int variableNum;
	int value;
	int labelNum;
	
	variableNum = (unsigned char)event->dataByte[0];
	value = (unsigned char)event->dataByte[1];
	labelNum = (unsigned char)event->dataByte[2];

	if (aadMem->userVariables[variableNum] < value)
	{
		aadGotoSequenceLabel(slot, event->track, labelNum);
	}
}

void metaCmdBranchIfVarGreater(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int variableNum;
	int value;
	int labelNum;

	variableNum = (unsigned char)event->dataByte[0];
	value = (unsigned char)event->dataByte[1];
	labelNum = (unsigned char)event->dataByte[2];

	if (value < aadMem->userVariables[variableNum])
	{
		aadGotoSequenceLabel(slot, event->track, labelNum);
	}
}

void metaCmdBranchIfVarLessOrEqual(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)//Matching - 99.0%
{
	int variableNum;
	int value;
	int labelNum;

	variableNum = event->dataByte[0];
	value = event->dataByte[1];
	labelNum = event->dataByte[2];

	if (aadMem->userVariables[variableNum] <= value)
	{
		aadGotoSequenceLabel(slot, event->track, labelNum);
	}
}

void metaCmdBranchIfVarGreaterOrEqual(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int variableNum;
	int value;
	int labelNum;

	variableNum = event->dataByte[0];
	value = event->dataByte[1];
	labelNum = event->dataByte[2];

	if (value <= aadMem->userVariables[variableNum])
	{
		aadGotoSequenceLabel(slot, event->track, labelNum);
	}
}

void metaCmdBranchIfVarBitsSet(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int variableNum;
	int mask;
	int labelNum;
	
	variableNum = (unsigned char)event->dataByte[0];
	mask = (unsigned char)event->dataByte[1];
	labelNum = (unsigned char)event->dataByte[2];

	if ((aadMem->userVariables[variableNum] & mask) != 0)
	{
		aadGotoSequenceLabel(slot, event->track, labelNum);
	}
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdBranchIfVarBitsClear(struct AadSeqEvent *event /*$a3*/, struct _AadSequenceSlot *slot /*$a0*/)
void metaCmdBranchIfVarBitsClear(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 1029, offset 0x800568b8
	/* begin block 1 */
		// Start line: 1030
		// Start offset: 0x800568B8
		// Variables:
			int variableNum; // $v1
			int mask; // $v1
			int labelNum; // $a2
	/* end block 1 */
	// End offset: 0x800568F8
	// End Line: 1039

	/* begin block 2 */
		// Start line: 2209
	/* end block 2 */
	// End Line: 2210
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSubstituteVariableParam1(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSubstituteVariableParam1(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 1042, offset 0x80056908
	/* begin block 1 */
		// Start line: 2244
	/* end block 1 */
	// End Line: 2245

	/* begin block 2 */
		// Start line: 2245
	/* end block 2 */
	// End Line: 2246
	UNIMPLEMENTED();
}

void metaCmdSubstituteVariableParam2(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ 
	slot->trackFlags[event->track] |= 0x2;
}

void metaCmdSubstituteVariableParam3(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	slot->trackFlags[event->track] |= 0x4;
}

void metaCmdEndSequence(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ 
	aadInitSequenceSlot(slot);

	aadAllNotesOff(slot->thisSlotNumber);

	if (aadMem->endSequenceCallback != NULL)
	{
		aadMem->endSequenceCallback(aadMem->endSequenceCallbackData, slot->thisSlotNumber, 0);
	}
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdPlaySoundEffect(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdPlaySoundEffect(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 1180, offset 0x800569c4
	/* begin block 1 */
		// Start line: 2520
	/* end block 1 */
	// End Line: 2521

	/* begin block 2 */
		// Start line: 2521
	/* end block 2 */
	// End Line: 2522
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdStopSoundEffect(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdStopSoundEffect(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 1183, offset 0x800569cc
	/* begin block 1 */
		// Start line: 2526
	/* end block 1 */
	// End Line: 2527

	/* begin block 2 */
		// Start line: 2527
	/* end block 2 */
	// End Line: 2528
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ metaCmdSetSoundEffectVolumePan(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a1*/)
void metaCmdSetSoundEffectVolumePan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 1186, offset 0x800569d4
	/* begin block 1 */
		// Start line: 2532
	/* end block 1 */
	// End Line: 2533

	/* begin block 2 */
		// Start line: 2533
	/* end block 2 */
	// End Line: 2534
	UNIMPLEMENTED();
}




