#include "Game/CORE.H"
#include "AADSEQEV.H"
#include "AADLIB.H"
#include "AADSQCMD.H"
#include "AADVOICE.H"

#include <assert.h>

char midiDataByteCount[8] = { 2, 2, 2, 2, 1, 1, 2, 2 };

short aadStepsPerSemitone[85] = {
  0x0007,0x0008,0x0009,0x0009,0x0009,0x000b,0x000a,0x000c,0x000c,0x000d,0x000d,0x000f,0x000f,0x0010,0x0011,0x0012,
  0x0013,0x0015,0x0015,0x0017,0x0018,0x001a,0x001b,0x001d,0x001e,0x0020,0x0022,0x0025,0x0026,0x0029,0x002b,0x002d,
  0x0031,0x0033,0x0036,0x003a,0x003c,0x0041,0x0044,0x0049,0x004c,0x0052,0x0056,0x005b,0x0061,0x0066,0x006d,0x0073,
  0x0079,0x0081,0x0089,0x0091,0x0099,0x00a3,0x00ac,0x00b6,0x00c2,0x00cd,0x00d9,0x00e6,0x00f3,0x0102,0x0111,0x0122,
  0x0133,0x0145,0x0159,0x016c,0x0183,0x019a,0x01b2,0x01cc,0x01e7,0x0204,0x0222,0x0244,0x0266,0x028a,0x02b1,0x02d9,
  0x0306,0x0333,0x0364,0x0398,0x0000
};

void (*midiEventFunction[8])(struct AadSeqEvent* event, struct _AadSequenceSlot* slot) = {

	&midiNoteOff,
	&midiNoteOn,
	&midiPolyphonicAftertouch,
	&midiControlChange,
	&midiProgramChange,
	&midiChannelAftertouch,
	&midiPitchWheelControl,
	&midiMetaEvent
};

void (*midiControlFunction[16])(struct AadSeqEvent* event, struct _AadSequenceSlot* slot) = {
	&midiControlBankSelect,
	&midiControlDummy,
	&midiControlDummy,
	&midiControlDummy,
	&midiControlDummy,
	&midiControlDummy,
	&midiControlDummy,
	&midiControlVolume,
	&midiControlDummy,
	&midiControlDummy,
	&midiControlPan,
	&midiControlCallback,
	&midiControlDummy,
	&midiControlDummy,
	&midiControlDummy,
	&midiControlDummy
};

	
void (*midiMetaEventFunction[78])(struct AadSeqEvent* event, struct _AadSequenceSlot* slot) = {
	&metaCmdSelectChannel,
	&metaCmdSelectSlot,
	&metaCmdAssignSequence,
	&metaCmdUsePrimaryTempo,
	&metaCmdUseSecondaryTempo,
	&metaCmdSetTempo,
	&metaCmdSetTempoFromSequence,
	&metaCmdChangeTempo,
	&metaCmdStartSlot,
	&metaCmdStopSlot,
	&metaCmdPauseSlot,
	&metaCmdResumeSlot,
	&metaCmdSetSlotBendRange,
	&metaCmdSetChannelBendRange,
	&metaCmdSetSlotVolume,
	&metaCmdSetSlotPan,
	&metaCmdSetChannelVolume,
	&metaCmdSetChannelPan,
	&metaCmdMuteChannel,
	&metaCmdUnMuteChannel,
	&metaCmdMuteChannelList,
	&metaCmdUnMuteChannelList,
	&metaCmdChannelVolumeFade,
	&metaCmdChannelPanFade,
	&metaCmdSlotVolumeFade,
	&metaCmdSlotPanFade,
	&metaCmdSetChannelProgram,
	&metaCmdSetChannelBasePriority,
	&metaCmdSetChannelTranspose,
	&metaCmdIgnoreChannelTranspose,
	&metaCmdRespectChannelTranspose,
	&metaCmdSetChannelPitchMap,
	&metaCmdIgnoreChannelPitchMap,
	&metaCmdRespectChannelPitchMap,
	&metaCmdGetSequenceAssigned,
	&metaCmdGetTempo,
	&metaCmdGetSlotStatus,
	&metaCmdGetSlotVolume,
	&metaCmdGetSlotPan,
	&metaCmdGetChannelVolume,
	&metaCmdGetChannelPan,
	&metaCmdGetChannelMute,
	&metaCmdGetChannelBendRange,
	&metaCmdGetChannelTranspose,
	&metaCmdGetChannelProgram,
	&metaCmdGetChannelBasePriority,
	&metaCmdLoopStart,
	&metaCmdLoopEnd,
	&metaCmdLoopBreak,
	&metaCmdDefineLabel,
	&metaCmdGotoLabel,
	&metaCmdSetVariable,
	&metaCmdCopyVariable,
	&metaCmdAddVariable,
	&metaCmdSubtractVariable,
	&metaCmdSetVariableBits,
	&metaCmdClearVariableBits,
	&metaCmdBranchIfVarEqual,
	&metaCmdBranchIfVarNotEqual,
	&metaCmdBranchIfVarLess,
	&metaCmdBranchIfVarGreater,
	&metaCmdBranchIfVarLessOrEqual,
	&metaCmdBranchIfVarGreaterOrEqual,
	&metaCmdBranchIfVarBitsSet,
	&metaCmdBranchIfVarBitsClear,
	&metaCmdSubstituteVariableParam1,
	&metaCmdSubstituteVariableParam2,
	&metaCmdSubstituteVariableParam3,
	&metaCmdEndSequence,
	&metaCmdPlaySoundEffect,
	&metaCmdStopSoundEffect,
	&metaCmdSetSoundEffectVolumePan,
	&metaCmdSetSequencePosition,
	&metaCmdEnableSustainUpdate,
	&metaCmdDisableSustainUpdate,
	&metaCmdSetChannelMute,
	&metaCmdDelayMute,
	&metaCmdUpdateMute
};

int aadQueueNextEvent(struct _AadSequenceSlot* slot, int track)//Matching - 98.60%
{
	struct AadSeqEvent seqEvent;
	unsigned char* seqData;
	unsigned long deltaTime;
	int c;
	int n;
	int i;

	if ((slot->trackFlags[track] & 0x18))
	{
		return -1;
	}

	if ((slot->trackFlags[track] & 0x20))
	{
		slot->lastEventExecutedTime[track] = slot->tempo.currentTick;
		slot->trackFlags[track] &= 0xDF;
	}

	seqData = slot->sequencePosition[track];

	if ((deltaTime = *seqData++) & 0x80)
	{
		deltaTime &= 0x7F | ((c = *seqData) & 0x7F);

		do
		{
			deltaTime = (deltaTime << 7) | ((c = *seqData++) & 0x7F);
		} while (c & 0x80);
	}

	seqEvent.track = track;
	seqEvent.deltaTime = deltaTime;

	if (*seqData == 0xFF)
	{
		seqData++;
		seqEvent.statusByte = *seqData++;
		n = *seqData++;

		if (seqEvent.statusByte == 0x44)
		{
			slot->trackFlags[track] |= 0x8;
		}
		else if (seqEvent.statusByte == 0x2E)
		{
			slot->trackFlags[track] |= 0x10;
		}
	}
	else
	{
		if (*seqData & 0x80)
		{
			seqEvent.statusByte = *seqData++;
			slot->runningStatus[track] = seqEvent.statusByte;
		}
		else
		{
			seqEvent.statusByte = slot->runningStatus[track];
		}

		n = midiDataByteCount[(seqEvent.statusByte >> 4) & 0x7];
	}

	i = 0;
	while (--n != -1)
	{
		seqEvent.dataByte[i++] = *seqData++;
	}

	slot->sequencePosition[track] = seqData;

	memcpy(&slot->eventQueue[slot->eventIn[track]][track], &seqEvent, sizeof(seqEvent));

	slot->eventsInQueue[track]++;
	slot->eventIn[track]++;

	if (slot->eventIn[track] == 4)
	{
		slot->eventIn[track] = 0;
	}

	return 0;
}

void aadExecuteEvent(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)//Matching - 82.11%
{
	int eventType;

	eventType = event->statusByte;

	if ((eventType & 0x80))
	{
		midiEventFunction[((eventType >> 4) & 0x7)](event, slot);
	}
	else
	{
		aadSubstituteVariables(event, slot);

		eventType = event->statusByte & 0x7F;

		if (event->statusByte < 78)
		{
			midiMetaEventFunction[eventType](event, slot);
		}
	}
}

void midiNoteOff(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
}

void midiNoteOn(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)//Matching - 99.66%
{
	struct AadProgramAtr* progAtr;
	struct AadToneAtr* toneAtrTbl;
	struct AadSynthVoice* voice;
	int channel;
	int midiNote;
	int transposedNote;
	int t;
	int dynBank;
	unsigned long waveStartAddr;

	channel = event->statusByte & 0xF;

	if (!((slot->channelMute >> channel) & 0x1) && slot->currentProgram[channel] != 255)
	{
		midiNote = (unsigned char)event->dataByte[0];

		if (event->dataByte[1] == 0)
		{
			for (t = 0; t < 24; t++)
			{
				voice = &aadMem->synthVoice[t];

				if (voice->voiceID == (slot->slotID | channel) && voice->note == midiNote && aadMem->voiceStatus[t] != 0 && aadMem->voiceStatus[t] != 2)
				{
					aadMem->voiceKeyOffRequest |= voice->voiceMask;

					aadMem->voiceKeyOnRequest &= ~voice->voiceMask;

					voice->voiceID = 255;
				}
			}
		}
		else
		{
			if (((slot->ignoreTranspose >> channel) & 0x1))
			{
				transposedNote = midiNote;
			}
			else
			{
				transposedNote = (midiNote + slot->transpose[channel]) & 0xFF;
			}

			dynBank = slot->currentDynamicBank[channel];

			if (aadMem->dynamicBankStatus[dynBank] == 2)
			{
				progAtr = aadMem->dynamicProgramAtr[dynBank] + slot->currentProgram[channel];
				toneAtrTbl = aadMem->dynamicToneAtr[dynBank];


				for (t = progAtr->firstTone; t < progAtr->firstTone + progAtr->numTones; t++)
				{
					if (midiNote >= (toneAtrTbl + t)->minNote &&
						(toneAtrTbl + t)->maxNote >= midiNote)
					{
						voice = aadAllocateVoice((toneAtrTbl + t)->priority);

						if (voice != NULL)
						{
							waveStartAddr = ((unsigned long*)aadMem->dynamicWaveAddr[dynBank])[(toneAtrTbl + t)->waveIndex];

							if ((toneAtrTbl + t)->pitchBendMax != 0 && slot->pitchWheel[channel] != 8192)
							{
								aadPlayTonePitchBend((toneAtrTbl + t), waveStartAddr, progAtr, transposedNote, event->dataByte[1], slot->volume[channel], slot->panPosition[channel], slot->slotVolume, slot->masterVolPtr[0], voice, slot->pitchWheel[channel]);

								voice->handle = 0;
							}
							else
							{
								aadPlayTone((toneAtrTbl + t), waveStartAddr, progAtr, transposedNote, event->dataByte[1], slot->volume[channel], slot->panPosition[channel], slot->slotVolume, slot->masterVolPtr[0], voice, 0);

								voice->handle = 0;
							}

							voice->voiceID = slot->slotID | channel;
							voice->priority = (toneAtrTbl + t)->priority;
							voice->note = midiNote;
							voice->program = slot->currentProgram[channel];
							voice->volume = event->dataByte[1];
							voice->updateVol = slot->volume[channel];
							voice->pan = slot->panPosition[channel];
							voice->progAtr = progAtr;
							voice->toneAtr = toneAtrTbl + t;
						}
					}
				}
			}
		}
	}
}

// autogenerated function stub: 
// void /*$ra*/ aadUpdateChannelVolPan(struct _AadSequenceSlot *slot /*$s4*/, int channel /*$s5*/)
void aadUpdateChannelVolPan(struct _AadSequenceSlot *slot, int channel)
{ // line 253, offset 0x80054ef0
	/* begin block 1 */
		// Start line: 254
		// Start offset: 0x80054EF0
		// Variables:
			struct AadSynthVoice *voice; // $s0
			int i; // $s2
			//struct AadVolume newVoiceVol; // stack offset -40

		/* begin block 1.1 */
			// Start line: 267
			// Start offset: 0x80054F90
			// Variables:
				unsigned long tmp; // $v0
		/* end block 1.1 */
		// End offset: 0x80054F90
		// End Line: 267

		/* begin block 1.2 */
			// Start line: 268
			// Start offset: 0x80054FF8
			// Variables:
				unsigned long masterVolumeSquared; // $v1
		/* end block 1.2 */
		// End offset: 0x80054FF8
		// End Line: 268

		/* begin block 1.3 */
			// Start line: 269
			// Start offset: 0x80055078
			// Variables:
				//unsigned long tmp; // $v0
		/* end block 1.3 */
		// End offset: 0x80055078
		// End Line: 269

		/* begin block 1.4 */
			// Start line: 270
			// Start offset: 0x800550E4
			// Variables:
				//unsigned long masterVolumeSquared; // $v1
		/* end block 1.4 */
		// End offset: 0x800550E4
		// End Line: 270

		/* begin block 1.5 */
			// Start line: 270
			// Start offset: 0x800550E4
			// Variables:
				//unsigned long masterVolumeSquared; // $v0
		/* end block 1.5 */
		// End offset: 0x800550E4
		// End Line: 270

		/* begin block 1.6 */
			// Start line: 270
			// Start offset: 0x800550E4
			// Variables:
				//unsigned long masterVolumeSquared; // $v0
		/* end block 1.6 */
		// End offset: 0x800550E4
		// End Line: 270

		/* begin block 1.7 */
			// Start line: 270
			// Start offset: 0x800550E4
			// Variables:
				//unsigned long masterVolumeSquared; // $v0
		/* end block 1.7 */
		// End offset: 0x800550E4
		// End Line: 270
	/* end block 1 */
	// End offset: 0x80055250
	// End Line: 282

	/* begin block 2 */
		// Start line: 583
	/* end block 2 */
	// End Line: 584
				UNIMPLEMENTED();
}

void aadUpdateSlotVolPan(struct _AadSequenceSlot* slot)
{
	struct AadSynthVoice* voice;
	int channel;
	int i;
	struct AadVolume newVoiceVol;
	unsigned long tmp;
	unsigned long masterVolumeSquared;

	for (i = 0; i < 24; i++)
	{
		voice = &aadMem->synthVoice[i];

		if ((voice->voiceID & 0xF0) == slot->slotID)
		{
			newVoiceVol.left = voice->volume * voice->volume;
			newVoiceVol.right = voice->volume * voice->volume;

			channel = voice->voiceID & 0xF;

			if (!(aadMem->flags & 0x1))
			{
				if (slot->panPosition[channel] >= 65)
				{
					masterVolumeSquared = (newVoiceVol.right * (((128 - slot->panPosition[channel]) * (128 - slot->panPosition[channel])) - 1)) >> 12;
					newVoiceVol.left = (short)masterVolumeSquared;
				}
				else if (slot->panPosition[channel] < 63)
				{
					newVoiceVol.right = (newVoiceVol.left * (((slot->panPosition[channel] + 1) * (slot->panPosition[channel] + 1)) + 1)) >> 12;
				}
			}

			newVoiceVol.left = (newVoiceVol.left * ((voice->toneAtr->volume + 1) * (voice->toneAtr->volume + 1) - 1)) >> 14;
			newVoiceVol.right = (newVoiceVol.right * ((voice->toneAtr->volume + 1) * (voice->toneAtr->volume + 1) - 1)) >> 14;

			if (!(aadMem->flags & 0x1))
			{
				if (voice->toneAtr->panPosition >= 65)
				{
					newVoiceVol.left = (newVoiceVol.right * (((128 - voice->toneAtr->panPosition) * (128 - voice->toneAtr->panPosition)) - 1)) >> 12;
				}
				else if (voice->toneAtr->panPosition < 63)
				{
					newVoiceVol.right = newVoiceVol.left * (((voice->toneAtr->panPosition + 1) * (voice->toneAtr->panPosition + 1)) + 1);
				}
			}

			newVoiceVol.left = (newVoiceVol.left * (((slot->volume[channel] + 1) * (slot->volume[channel] + 1)) - 1)) >> 14;
			newVoiceVol.right = (newVoiceVol.right * (((slot->volume[channel] + 1) * (slot->volume[channel] + 1)) - 1)) >> 14;

			newVoiceVol.left = (newVoiceVol.left * (((voice->progAtr->volume + 1) * (voice->progAtr->volume + 1)) - 1)) >> 14;
			newVoiceVol.right = (newVoiceVol.right * (((voice->progAtr->volume + 1) * (voice->progAtr->volume + 1)) - 1)) >> 14;

			newVoiceVol.left = (newVoiceVol.left * (((slot->slotVolume + 1) * (slot->slotVolume + 1)) - 1)) >> 14;
			newVoiceVol.right = (newVoiceVol.right * (((slot->slotVolume + 1) * (slot->slotVolume + 1)) - 1)) >> 14;

			newVoiceVol.left = (newVoiceVol.left * (((slot->masterVolPtr[0] + 1) * (slot->masterVolPtr[0] + 1)) - 1)) >> 14;
			newVoiceVol.right = (newVoiceVol.right * (((slot->masterVolPtr[0] + 1) * (slot->masterVolPtr[0] + 1)) - 1)) >> 14;

			SpuSetVoiceVolume(i, newVoiceVol.left, newVoiceVol.right);
		}
	}
}

void aadUpdateChannelPitchBend(struct _AadSequenceSlot* slot, int channel)
{
	struct AadSynthVoice* voice;
	int i;
	int finePitch;
	int newPitch;
	int pitchWheelPos;
	int pitchIndex;
	long pitchValueBendAmount;

	pitchWheelPos = slot->pitchWheel[channel] - 8192;

	for (i = 0; i < 24; i++)
	{
		voice = &aadMem->synthVoice[i];

		if (voice->voiceID == (slot->slotID | channel))
		{
			if (voice->toneAtr->pitchBendMax != 0)
			{
				finePitch = (slot->pitchWheel[channel] - 8192) / (8192 / voice->toneAtr->pitchBendMax);
		
				pitchValueBendAmount = (slot->pitchWheel[channel] - 8192) % (8192 / voice->toneAtr->pitchBendMax);
		
				pitchIndex = voice->note - (voice->toneAtr->centerNote - 60);

				pitchValueBendAmount = (aadStepsPerSemitone[pitchIndex + finePitch] * pitchValueBendAmount) / (8192 / voice->toneAtr->pitchBendMax);

				if ((voice->toneAtr->centerFine & 0x80))
				{
					newPitch = aadPitchTable[pitchIndex] - (((aadStepsPerCent[pitchIndex] * 100) * 256) >> 23);
				}
				else
				{
					newPitch = aadPitchTable[pitchIndex] + (((aadStepsPerCent[pitchIndex] * 100) * voice->toneAtr->centerFine) >> 23);
				}

				SpuSetVoicePitch(i, (unsigned short)(newPitch + pitchValueBendAmount));
			}
		}
	}
}

void midiPolyphonicAftertouch(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void midiControlChange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int controlNumber;
	
	controlNumber = (unsigned char)event->dataByte[0] & 0xF;

	midiControlFunction[controlNumber](event, slot);
}

void midiProgramChange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int channel;

	channel = event->statusByte & 0xF;
	
	slot->currentProgram[channel] = (unsigned char)event->dataByte[0];
}

void midiChannelAftertouch(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void midiPitchWheelControl(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int channel;

	channel = event->statusByte & 0xF;

	slot->pitchWheel[channel] = ((unsigned char)event->dataByte[0] | ((unsigned char)event->dataByte[1] << 7));
	
	aadUpdateChannelPitchBend(slot, channel);
}

void midiMetaEvent(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void midiControlBankSelect(struct AadSeqEvent* event, struct _AadSequenceSlot* slot)
{
}

void midiControlVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
	int channel;

	channel = event->statusByte & 0xF;
	slot->volume[channel] = (unsigned char)event->dataByte[1];

	if (((slot->enableSustainUpdate >> channel) & 0x1) != 0)
	{
		aadUpdateChannelVolPan(slot, channel);
	}
}


// autogenerated function stub: 
// void /*$ra*/ midiControlPan(struct AadSeqEvent *event /*$a0*/, struct _AadSequenceSlot *slot /*$a2*/)
void midiControlPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 461, offset 0x8005588c
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ midiControlCallback(struct AadSeqEvent *event /*$a3*/, struct _AadSequenceSlot *slot /*$a1*/)
void midiControlCallback(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ // line 476, offset 0x800558dc
	UNIMPLEMENTED();
}

void midiControlDummy(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{
}




