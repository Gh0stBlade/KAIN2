#include "CORE.H"
#include "AADVOICE.H"
#include "AADLIB.H"

unsigned long aadStepsPerCent[85];
unsigned short aadPitchTable[85];

struct AadSynthVoice* aadAllocateVoice(int priority)
{
	int i;
	int lowestPriSus;
	int lowestPriRel;
	struct AadSynthVoice* lowestPriSusVoice;
	struct AadSynthVoice* lowestPriRelVoice;
	struct AadSynthVoice* voice;

	lowestPriRel = 0x7FFF;
	lowestPriSus = 0x7FFF;

	lowestPriRelVoice = NULL;
	lowestPriSusVoice = NULL;

	voice = &aadMem->synthVoice[0];

	for (i = 0; i < 24; i++, voice++)
	{
		if (!(voice->flags & 0x1))
		{
			if (aadMem->voiceStatus[i] != 0)
			{
				if (aadMem->voiceStatus[i] != 2)
				{
					if (voice->priority < lowestPriRel)
					{
						lowestPriRel = voice->priority;
						lowestPriRelVoice = voice;
					}
				}
				else
				{
					if (voice->priority < lowestPriSus)
					{
						lowestPriSus = voice->priority;
						lowestPriSusVoice = voice;
					}
				}
			}
			else
			{
				aadMem->voiceStatus[i] = 1;
				voice->flags |= 0x2;
				return voice;
			}

		}
	}

	if (priority >= lowestPriRel)
	{
		lowestPriRelVoice->flags |= 2;
		return lowestPriRelVoice;
	}
	else if (priority >= lowestPriSus)
	{
		lowestPriSusVoice->flags |= 2;
		return lowestPriSusVoice;
	}
	else
	{
		return NULL;
	}
}

void SpuSetVoiceADSR1ADSR2(int vNum, unsigned short adsr1, unsigned short adsr2)
{ 
	unsigned short sl;
	unsigned short dr;
	unsigned short ar;
	unsigned short arm;
	unsigned short rr;
	unsigned short rrm;
	unsigned short sr;
	unsigned short srm;

	sl = adsr1 & 0xF;
	dr = ((adsr1 & 0xFFFF) >> 4) & 0xF;
	ar = adsr1 & 0x7F;
	arm = 1;
	sr = adsr2;

	if ((adsr1 & 0x8000))
	{
		arm = 5;
	}

	rr = adsr2 & 0x1F;
	rrm = 3;
	if ((adsr2 & 0x20))
	{
		rrm = 7;
	}

	sr = (sr >> 6) & 0x7F;

	if (!(adsr2 & 0x4000))
	{
		if (!(adsr2 & 0x8000))
		{
			srm = 1;
		}
		else
		{
			srm = 5;
		}
	}
	else
	{
		if (!(adsr2 & 0x8000))
		{
			srm = 3;
		}
		else
		{
			srm = 7;
		}
	}

	SpuSetVoiceADSRAttr(vNum, ar, dr, sr, rr, sl, arm, srm, rrm);
}

void aadPlayTone(struct AadToneAtr *toneAtr, unsigned long waveStartAddr, struct AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, struct AadSynthVoice *voice, int pitchOffset)
{ 
	struct AadVolume voiceVol;
	int pitch;
	int finePitch;
	int pitchIndex;
	unsigned long tmp;
	unsigned long masterVolumeSquared;

#define GET_VOLUME_SQUARED(x) (x) * (x) 
#define GET_MASTER_PAN_LEFT(x, y) (y * (y + (GET_VOLUME_SQUARED((128 - x)) - 1))) >> 12
#define GET_MASTER_PAN_RIGHT(x, y) (y * (y + (GET_VOLUME_SQUARED((x + 1)) + 1))) >> 12
#define GET_MASTER_VOL(x, y) (y * (x - 1)) >> 14

	masterVolumeSquared = GET_VOLUME_SQUARED(volume + 1) - 1;
	voiceVol.left = masterVolumeSquared;
	voiceVol.right = masterVolumeSquared;
	
	if (!(aadMem->flags & 0x1))
	{
		if (masterPan >= 65)
		{
			voiceVol.left = GET_MASTER_PAN_LEFT(masterPan, masterVolumeSquared);
		}
		else if (masterPan < 63)
		{
			voiceVol.right = GET_MASTER_PAN_RIGHT(masterPan, masterVolumeSquared);
		}
	}

	masterVolumeSquared = GET_VOLUME_SQUARED(toneAtr->volume + 1) - 1;
	voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
	voiceVol.right = (voiceVol.right * masterVolumeSquared) >> 14;

	if (!(aadMem->flags & 0x1))
	{
		if (toneAtr->panPosition >= 65)
		{
			voiceVol.left = GET_MASTER_PAN_LEFT(voiceVol.left, toneAtr->panPosition);
		}
		else if (toneAtr->panPosition < 63)
		{
			voiceVol.right = GET_MASTER_PAN_RIGHT(voiceVol.right, toneAtr->panPosition);
		}
	}

	masterVolumeSquared = GET_VOLUME_SQUARED(masterVolume + 1);
	voiceVol.left = GET_MASTER_VOL(masterVolumeSquared, voiceVol.left);
	voiceVol.right = GET_MASTER_VOL(masterVolumeSquared, voiceVol.right);

	tmp = GET_VOLUME_SQUARED(progAtr->volume + 1);
	pitch = tmp - 1;
	masterVolumeSquared = voiceVol.left * pitch;

	tmp = GET_VOLUME_SQUARED(slotVolume + 1);
	int a2 = (voiceVol.right * pitch) >> 14;
	pitchIndex = masterVolumeSquared >> 14;
	masterVolumeSquared = tmp - 1;
	int a1 = (pitchIndex * masterVolumeSquared) >> 14;

	int a0 = GET_VOLUME_SQUARED(masterMasterVol + 1);
	int t2 = a2 * masterVolumeSquared;
	int t0 = a1 * (a0 - 1);
	int v1 = tmp *  masterVolumeSquared;

	voiceVol.left = pitchIndex;
	voiceVol.right = a2;
	voiceVol.left = a1;
	voiceVol.right = t2 >> 14;

	a1 = (t0 >> 14) << 16;
	voiceVol.left = a1 >> 16;

	v1 = (t2 >> 14) * (a0 - 1);
	voiceVol.right = v1 >> 14;

	SpuSetVoiceVolume(voice->voiceNum, voiceVol.left, voiceVol.right);

	pitchIndex = midiNote - (toneAtr->centerNote - 60);
	if ((toneAtr->centerFine & 0x80))
	{
		finePitch = 256;
		masterMasterVol = aadStepsPerCent[pitchIndex];
		finePitch -= toneAtr->centerFine;
		finePitch = aadPitchTable[pitchIndex] - ((masterMasterVol * 100) >> 23);
	}
	else
	{
		masterMasterVol = aadStepsPerCent[pitchIndex];
		finePitch = toneAtr->centerFine;
		tmp = (masterMasterVol * 100) * finePitch;
		finePitch = aadPitchTable[pitchIndex] + (tmp >> 23);
	}

	SpuSetVoicePitch(voice->voiceNum, pitchOffset);
	SpuSetVoiceStartAddr(voice->voiceNum, waveStartAddr);
	SpuSetVoiceADSR1ADSR2(voice->voiceNum, toneAtr->adsr1, toneAtr->adsr2);

	if (toneAtr->centerNote == 4)
	{
		aadMem->voiceReverbRequest |= voice->voiceMask;
	}
	else
	{
		aadMem->voiceReverbRequest = aadMem->voiceReverbRequest & ~voice->voiceMask;
	}

	aadMem->voiceKeyOnRequest |= voice->voiceMask;
}


// autogenerated function stub: 
// void /*$ra*/ aadPlayTonePitchBend(struct AadToneAtr *toneAtr /*$s0*/, unsigned long waveStartAddr /*$s4*/, struct AadProgramAtr *progAtr /*$a2*/, int midiNote /*$s3*/, int voll /*stack 16*/, int masterVolume /*stack 20*/, int masterPan /*stack 24*/, int slotVolume /*stack 28*/, int masterMasterVol /*stack 32*/, struct AadSynthVoice *voice /*stack 36*/, int pitchWheelPos /*stack 40*/)
void aadPlayTonePitchBend(struct AadToneAtr *toneAtr, unsigned long waveStartAddr, struct AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, struct AadSynthVoice *voice, int pitchWheelPos)
{ // line 246, offset 0x80058064
	/* begin block 1 */
		// Start line: 247
		// Start offset: 0x80058064
		// Variables:
			//struct AadVolume voiceVol; // stack offset -32
			int pitch; // $a1
			int finePitch; // $a1
			int pitchIndex; // $a3
			long pitchValueBendAmount; // $t0

		/* begin block 1.1 */
			// Start line: 258
			// Start offset: 0x800580D4
			// Variables:
				unsigned long tmp; // $v0
		/* end block 1.1 */
		// End offset: 0x800580D4
		// End Line: 258

		/* begin block 1.2 */
			// Start line: 259
			// Start offset: 0x80058130
			// Variables:
				unsigned long masterVolumeSquared; // $v1
		/* end block 1.2 */
		// End offset: 0x80058130
		// End Line: 259

		/* begin block 1.3 */
			// Start line: 260
			// Start offset: 0x800581A0
			// Variables:
				//unsigned long tmp; // $v0
		/* end block 1.3 */
		// End offset: 0x800581A0
		// End Line: 260

		/* begin block 1.4 */
			// Start line: 262
			// Start offset: 0x80058208
			// Variables:
				//unsigned long masterVolumeSquared; // $v1
		/* end block 1.4 */
		// End offset: 0x80058208
		// End Line: 262

		/* begin block 1.5 */
			// Start line: 262
			// Start offset: 0x80058208
			// Variables:
				//unsigned long masterVolumeSquared; // $a1
		/* end block 1.5 */
		// End offset: 0x80058208
		// End Line: 262

		/* begin block 1.6 */
			// Start line: 262
			// Start offset: 0x80058208
			// Variables:
				//unsigned long masterVolumeSquared; // $v1
		/* end block 1.6 */
		// End offset: 0x80058208
		// End Line: 262

		/* begin block 1.7 */
			// Start line: 262
			// Start offset: 0x80058208
			// Variables:
				//unsigned long masterVolumeSquared; // $v1
		/* end block 1.7 */
		// End offset: 0x80058208
		// End Line: 262
	/* end block 1 */
	// End offset: 0x800584AC
	// End Line: 313

	/* begin block 2 */
		// Start line: 620
	/* end block 2 */
	// End Line: 621

}




