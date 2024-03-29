#include "Game/CORE.H"
#include "AADSQCMD.H"
#include "AADLIB.H"
#include "AADSEQEV.H"

#undef eprintinf
#define eprintinf

void aadSubstituteVariables(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)//Matching - 91.96%
{
	unsigned char trackFlags = slot->trackFlags[event->track];

	if ((trackFlags & 0x7) && (unsigned char)(event->statusByte - 0x41) >= 3)
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

void metaCmdSelectChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int channelNumber;

	channelNumber = (unsigned char)event->dataByte[0];

	if (channelNumber < 16)
	{
		slot->selectedChannel = channelNumber;
	}

	eprintinf("[MIDI]: Set Channel Number: %d\n", channelNumber);
}

void metaCmdSelectSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int slotNumber;

	slotNumber = (unsigned char)event->dataByte[0];

	if (slotNumber < aadMem->numSlots)
	{
		slot->selectedSlotNum = slotNumber;
		
		slot->selectedSlotPtr = aadMem->sequenceSlots[slotNumber];

		eprintinf("[MIDI]: Set Slot Number: %d\n", slotNumber);
	}
	else if (slotNumber == 127)
	{
		slot->selectedSlotPtr = slot;

		slot->selectedSlotNum = slot->thisSlotNumber;

		eprintinf("[MIDI]: Set Slot Number: %d\n", slot->thisSlotNumber);
	}
}

void metaCmdAssignSequence(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int sequenceNumber;
	int bank;

	bank = slot->selectedDynamicBank;
	sequenceNumber = (unsigned char)event->dataByte[0];

	if (aadMem->dynamicBankStatus[bank] == 2 && sequenceNumber < aadGetNumDynamicSequences(bank))
	{
		aadAssignDynamicSequence(bank, sequenceNumber, slot->selectedSlotNum);
		eprintinf("[MIDI]: Assign dynamic bank: %d to sequence: %d\n", bank, sequenceNumber);
	}
}

void metaCmdUsePrimaryTempo(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdUseSecondaryTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
}

void metaCmdSetTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	struct AadTempo tempo;

	tempo.quarterNoteTime = (unsigned char)event->dataByte[0] << 16;
	tempo.quarterNoteTime |= (unsigned char)event->dataByte[1] << 8;
	tempo.quarterNoteTime |= (unsigned char)event->dataByte[2];

	tempo.ppqn = slot->selectedSlotPtr->tempo.ppqn;

	aadSetSlotTempo(slot->selectedSlotNum, &tempo);

	eprintinf("[MIDI]: Set Slot: %d Tempo: %d\n", slot->selectedSlotNum, tempo.quarterNoteTime);
}

void metaCmdChangeTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	struct AadTempo tempo;
	struct _AadSequenceSlot* selectedSlot;

	selectedSlot = slot->selectedSlotPtr;
	tempo.quarterNoteTime = selectedSlot->tempo.quarterNoteTime * 100;
	tempo.quarterNoteTime = tempo.quarterNoteTime / event->dataByte[0];
	tempo.ppqn = selectedSlot->tempo.ppqn;

	aadSetSlotTempo(slot->selectedSlotNum, &tempo);

	eprintinf("[MIDI]: Change Slot: %d Tempo: %d\n", slot->selectedSlotNum, tempo.quarterNoteTime);
}

void metaCmdSetTempoFromSequence(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int sequenceNumber;//s1
	struct AadTempo tempo;//-24
	int bank;//s0

	bank = slot->selectedDynamicBank;
	
	sequenceNumber = (unsigned char)event->dataByte[0];

	if (aadMem->dynamicBankStatus[bank] == 2 && sequenceNumber < aadGetNumDynamicSequences(bank))
	{
		aadGetTempoFromDynamicSequence(bank, sequenceNumber, &tempo);
		aadSetSlotTempo(slot->selectedSlotNum, &tempo);

		eprintinf("[MIDI]: Set Slot: %d Tempo: %d Sequence: %d\n", slot->selectedSlotNum, tempo.quarterNoteTime, sequenceNumber);
	}
}

void metaCmdStartSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	aadStartSlot(slot->selectedSlotNum);
	eprintinf("[MIDI]: Start Slot: %d\n", slot->selectedSlotNum);
}

void metaCmdStopSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	aadStopSlot(slot->selectedSlotNum);
	eprintinf("[MIDI]: Stop Slot: %d\n", slot->selectedSlotNum);
}

void metaCmdPauseSlot(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	aadPauseSlot(slot->selectedSlotNum);
}

void metaCmdResumeSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	aadResumeSlot(slot->selectedSlotNum);
	eprintinf("[MIDI]: Resume Slot: %d\n", slot->selectedSlotNum);
}

void metaCmdSetSlotBendRange(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdSetChannelBendRange(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdSetSlotVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int volume;

	volume = (unsigned char)event->dataByte[0];

	slot->selectedSlotPtr->slotVolume = volume;

	aadUpdateSlotVolPan(slot->selectedSlotPtr);

	eprintinf("[MIDI]: Update Slot:%d Vol: %d\n", slot->selectedSlotNum, volume);
}

void metaCmdSetSlotPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int pan;

	pan = (unsigned char)event->dataByte[0];

	aadUpdateSlotVolPan(slot->selectedSlotPtr);

	eprintinf("[MIDI]: Update Slot:%d Pan: %d\n", slot->selectedSlotNum, pan);
}

void metaCmdSetChannelVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int volume;

	volume = (unsigned char)event->dataByte[0];

	slot->selectedSlotPtr->volume[slot->selectedChannel] = volume;

	aadUpdateChannelVolPan(slot->selectedSlotPtr, slot->selectedChannel);

	eprintinf("[MIDI]: Set Channel: %d Volume: %d\n", slot->selectedChannel, volume);
}


void metaCmdSetChannelPan(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int pan;

	pan = event->dataByte[0];

	slot->selectedSlotPtr->panPosition[slot->selectedChannel] = pan;

	aadUpdateChannelVolPan(slot->selectedSlotPtr, slot->selectedChannel);
}

void metaCmdEnableSustainUpdate(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int channel;
	
	channel = slot->selectedChannel;

	slot->selectedSlotPtr->enableSustainUpdate = (1 << channel);

	eprintinf("[MIDI]: Set Channel: %d Sustain Update: %d\n", slot->selectedChannel, slot->selectedSlotPtr->enableSustainUpdate);
}

void metaCmdDisableSustainUpdate(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int channel;

	channel = slot->selectedChannel;
	
	slot->selectedSlotPtr->enableSustainUpdate &= ~(1 << channel);
}

void metaCmdMuteChannel(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	aadMuteChannels(slot->selectedSlotPtr, 1 << slot->selectedChannel);
}

void metaCmdUnMuteChannel(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	aadUnMuteChannels(slot->selectedSlotPtr, 1 << slot->selectedChannel);
}

void metaCmdMuteChannelList(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	aadMuteChannels(slot->selectedSlotPtr, (unsigned char)event->dataByte[0] | ((unsigned char)event->dataByte[1] << 8));
	eprintinf("[MIDI]: Set Muted Channel List: %d\n", (unsigned char)event->dataByte[0] | ((unsigned char)event->dataByte[1] << 8));
}

void metaCmdUnMuteChannelList(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	aadUnMuteChannels(slot->selectedSlotPtr, (((unsigned char)event->dataByte[1] << 8) | (unsigned char)event->dataByte[0]));
	eprintinf("[MIDI]: Set Un-Muted Channel List: %d\n", (((unsigned char)event->dataByte[1] << 8) | (unsigned char)event->dataByte[0]));
}

void metaCmdSetChannelMute(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)//Matching - 99.52%
{
	int muteChannelMask;

	muteChannelMask = ((unsigned char)event->dataByte[1] << 8) | (unsigned char)event->dataByte[0];
	aadUnMuteChannels(slot->selectedSlotPtr, ~muteChannelMask);
	aadMuteChannels(slot->selectedSlotPtr, muteChannelMask);
}

void metaCmdDelayMute(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	unsigned long channelMask;

	channelMask = ((unsigned char)event->dataByte[1] << 8) | (unsigned char)event->dataByte[0];

	slot->selectedSlotPtr->delayedMuteMode |= channelMask;
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

void metaCmdChannelVolumeFade(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdChannelPanFade(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdSlotVolumeFade(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdSlotPanFade(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdSetChannelProgram(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int program;

	program = event->dataByte[0];

	slot->selectedSlotPtr[slot->selectedChannel].currentProgram[0] = program;
}

void metaCmdSetChannelBasePriority(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
}

void metaCmdSetChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int channel;
	int transpose;
	
	channel = slot->selectedChannel;
	transpose = (unsigned char)event->dataByte[0];

	slot->selectedSlotPtr->transpose[channel] = transpose;

	eprintinf("[MIDI]: Set Channel: %d Transpose: %d\n", channel, transpose);
}

void metaCmdIgnoreChannelTranspose(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int channel;
	
	channel = slot->selectedChannel;

	slot->selectedSlotPtr->ignoreTranspose |= (1 << channel);
}

void metaCmdRespectChannelTranspose(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int channel;

	channel = slot->selectedChannel;

	slot->selectedSlotPtr->ignoreTranspose &= ~(1 << channel);
}

void metaCmdSetChannelPitchMap(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdIgnoreChannelPitchMap(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdRespectChannelPitchMap(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdGetSequenceAssigned(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int variableNum;

	variableNum = (unsigned char)event->dataByte[0];

	if (variableNum != 0)
	{
		aadMem->userVariables[variableNum] = slot->selectedSlotPtr->sequenceNumberAssigned;
	}
}

void metaCmdGetTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ 
	int variableNum1;
	int variableNum2;
	int variableNum3;
	unsigned long quarterNoteTime;

	variableNum1 = (unsigned char)event->dataByte[0];
	variableNum2 = (unsigned char)event->dataByte[1];
	variableNum3 = (unsigned char)event->dataByte[2];

	if (variableNum1 < 128 && variableNum2 < 128 && variableNum3 < 128)
	{
		quarterNoteTime = slot->selectedSlotPtr->tempo.quarterNoteTime;

		aadMem->userVariables[variableNum1] = (unsigned char)quarterNoteTime;
		aadMem->userVariables[variableNum2] = (unsigned char)(quarterNoteTime >> 8);
		aadMem->userVariables[variableNum3] = (unsigned char)(quarterNoteTime >> 16);

		eprintinf("[MIDI]: Set Variable: %d Value: %d\n", variableNum1, aadMem->userVariables[variableNum1]);
		eprintinf("[MIDI]: Set Variable: %d Value: %d\n", variableNum2, aadMem->userVariables[variableNum2]);
		eprintinf("[MIDI]: Set Variable: %d Value: %d\n", variableNum3, aadMem->userVariables[variableNum3]);
	}
}

void metaCmdGetSlotStatus(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int variableNum;

	variableNum = (unsigned char)event->dataByte[0];

	if (variableNum < 128)
	{
		aadMem->userVariables[variableNum] = (unsigned char)slot->selectedSlotPtr->status;
		eprintinf("[MIDI]: Set Variable: %d\n", variableNum, slot->selectedSlotPtr->status);
	}
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

void metaCmdGetChannelVolume(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int variableNum;
	int channel;
	
	variableNum = (unsigned char)event->dataByte[0];

	channel = slot->selectedChannel;

	if (variableNum < 128)
	{
		aadMem->userVariables[variableNum] = slot->selectedSlotPtr->volume[channel];
	}
}

void metaCmdGetChannelPan(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int variableNum;
	int channel;

	variableNum = (unsigned char)event->dataByte[0];

	channel = slot->selectedChannel;

	if (variableNum < 128)
	{
		aadMem->userVariables[variableNum] = slot->selectedSlotPtr->panPosition[channel];
	}
}

void metaCmdGetChannelTranspose(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdGetChannelProgram(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{ 
	int variableNum;
	int channel;

	variableNum = (unsigned char)event->dataByte[0];

	channel = slot->selectedChannel;

	if (variableNum < 128)
	{
		aadMem->userVariables[variableNum] = slot->selectedSlotPtr->currentProgram[channel];
	}
}

void metaCmdGetChannelBasePriority(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdGetChannelBendRange(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdGetSlotVolume(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int variableNum;
	
	variableNum = (unsigned char)event->dataByte[0];

	if (variableNum < 128)
	{
		aadMem->userVariables[variableNum] = slot->selectedSlotPtr->slotVolume;
	}
}

void metaCmdGetSlotPan(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int variableNum;

	variableNum = (unsigned char)event->dataByte[0];

	if (variableNum < 128)
	{
		aadMem->userVariables[variableNum] = slot->selectedSlotPtr->slotPan;
	}
}

void metaCmdSetVariable(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int value;
	int destVariable;

	destVariable = (unsigned char)event->dataByte[1];
	value = (unsigned char)event->dataByte[0];

	if (destVariable < 128)
	{
		aadMem->userVariables[destVariable] = value;

		eprintinf("[MIDI]: Set Variable: %d Value: %d\n", destVariable, value);
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
		eprintinf("[MIDI]: Set Variable: %d  = Variable %d Value: %d\n", destVariable, srcVariable, aadMem->userVariables[srcVariable]);

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
		eprintinf("[MIDI]: Add Variable: %d Value: %d\n", destVariable, value);
	}
}

void metaCmdSubtractVariable(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int value;
	int destVariable;

	destVariable = (unsigned char)event->dataByte[1];
	value = (unsigned char)event->dataByte[0];

	if (destVariable < 128)
	{
		aadMem->userVariables[destVariable] -= value;
		eprintinf("[MIDI]: Sub Variable: %d Value: %d\n", destVariable, value);
	}
}

void metaCmdSetVariableBits(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int value;
	int destVariable;

	destVariable = (unsigned char)event->dataByte[1];
	value = (unsigned char)event->dataByte[0];

	if (destVariable < 128)
	{
		aadMem->userVariables[destVariable] |= value;
		eprintinf("[MIDI]: Or Variable: %d Value: %d\n", destVariable, value);
	}
}

void metaCmdClearVariableBits(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	int value;
	int destVariable;

	destVariable = (unsigned char)event->dataByte[1];
	value = (unsigned char)event->dataByte[0];

	if (destVariable < 128)
	{
		aadMem->userVariables[destVariable] &= ~value;
		eprintinf("[MIDI]: NOr Variable: %d Value: %d\n", destVariable, value);
	}
}

void aadGotoSequencePosition(struct _AadSequenceSlot *slot, int track, unsigned char *newPosition)//Matching - 100%
{
	slot->sequencePosition[track] = newPosition;

	eprintinf("[MIDI]: Track: %d Goto Sequence Position: %x\n", track, newPosition);


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

void aadGotoSequenceLabel(struct _AadSequenceSlot *slot, int track, int labelNumber)//Matching - 99.67%
{
	struct AadSequenceHdr* seqHdr;
	unsigned long trackOffset;
	int bank;

	bank = slot->sequenceAssignedDynamicBank;
	seqHdr = (struct AadSequenceHdr*)aadMem->dynamicSequenceAddressTbl[bank][slot->sequenceNumberAssigned];
	
#if defined(AKUJI)
	trackOffset = sizeof(struct AadSequenceHdr);
#else
	trackOffset = ((unsigned long*)(seqHdr + 1))[track];
#endif	

	slot->sequencePosition[track] = (unsigned char*)(char*)seqHdr + trackOffset + aadMem->dynamicSequenceLabelOffsetTbl[bank][labelNumber];

	eprintinf("[MIDI]: Track: %d Goto Sequence Position: %x Label: %d\n", track, slot->sequencePosition[track], labelNumber);


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

void metaCmdLoopStart(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)//Matching - 100%
{
	int nestLevel;
	int track;

	track = event->track;
	nestLevel = slot->loopCurrentNestLevel[track];

	if (nestLevel < 4)
	{
		slot->loopSequencePosition[nestLevel][track] = slot->sequencePosition[track];

		slot->loopCounter[nestLevel][track] = (unsigned char)event->dataByte[0];

		slot->loopCurrentNestLevel[track]++;
	}

	slot->trackFlags[track] &= 0xEF;

	eprintinf("[MIDI]: Loop Start Track: %d\n", track);

}

void metaCmdLoopEnd(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)//Matching - 93.19%
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
			slot->loopCurrentNestLevel[track] = prevNestLevel;
		}
	}

	eprintinf("[MIDI]: Loop End Track: %d\n", track);

}

void metaCmdLoopBreak(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{ 
}

void metaCmdDefineLabel(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdGotoLabel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ 
	aadGotoSequenceLabel(slot, event->track, (unsigned char)event->dataByte[0]);
}

void metaCmdSetSequencePosition(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	aadGotoSequenceLabel(slot->selectedSlotPtr, event->track, (unsigned char)event->dataByte[0]);
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
		eprintinf("[MIDI]: if Variable_%d == %d\n", variableNum, value);
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
		eprintinf("[MIDI]: if Variable_%d != %d\n", variableNum, value);

	}
}

void metaCmdBranchIfVarLess(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)//Matching - 99%
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
		eprintinf("[MIDI]: if Variable_%d < %d\n", variableNum, value);

	}
}

void metaCmdBranchIfVarGreater(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)//Matching - 99.25%
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
		eprintinf("[MIDI]: if %d < Variable_%d\n", value, variableNum);

	}
}

void metaCmdBranchIfVarLessOrEqual(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)//Matching - 99.0%
{
	int variableNum;
	int value;
	int labelNum;

	variableNum = (unsigned char)event->dataByte[0];
	value = (unsigned char)event->dataByte[1];
	labelNum = (unsigned char)event->dataByte[2];

	if (aadMem->userVariables[variableNum] <= value)
	{
		aadGotoSequenceLabel(slot, event->track, labelNum);
		eprintinf("[MIDI]: if Variable_%d <= %d\n", variableNum, value);

	}
}

void metaCmdBranchIfVarGreaterOrEqual(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)//Matching - 99%
{
	int variableNum;
	int value;
	int labelNum;

	variableNum = (unsigned char)event->dataByte[0];
	value = (unsigned char)event->dataByte[1];
	labelNum = (unsigned char)event->dataByte[2];

	if (value <= aadMem->userVariables[variableNum])
	{
		aadGotoSequenceLabel(slot, event->track, labelNum);
		eprintinf("[MIDI]: if %d <= Variable_%d\n", value, variableNum);
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
		eprintinf("[MIDI]: Variable_%d (%d) & %x\n", variableNum, aadMem->userVariables[variableNum], mask);

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

void metaCmdSubstituteVariableParam1(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
	slot->trackFlags[event->track] |= 0x1;
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

	eprintinf("[MIDI]: End Sequence Track: %d\n", event->track);

}

void metaCmdPlaySoundEffect(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdStopSoundEffect(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void metaCmdSetSoundEffectVolumePan(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}