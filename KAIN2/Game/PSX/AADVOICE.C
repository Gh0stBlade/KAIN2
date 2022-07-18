#include "CORE.H"
#include "AADVOICE.H"
#include "AADLIB.H"

unsigned long aadStepsPerCent[85] =
{
  0x000011eb,0x0000147a,0x0000170a,0x0000170a,0x0000170a,0x00001c28,0x00001999,0x00001eb8,0x00001eb8,0x00002147,0x00002147,0x00002666,0x00002666,0x000028f5,0x00002b85,0x00002e14,
  0x000030a3,0x000035c2,0x000035c2,0x00003ae1,0x00003d70,0x0000428f,0x0000451e,0x00004a3d,0x00004ccc,0x000051eb,0x0000570a,0x00005eb8,0x00006147,0x000068f5,0x00006e14,0x00007333,
  0x00007d70,0x0000828f,0x00008a3d,0x0000947a,0x00009999,0x0000a666,0x0000ae14,0x0000bae1,0x0000c28f,0x0000d1eb,0x0000dc28,0x0000e8f5,0x0000f851,0x0001051e,0x0001170a,0x00012666,
  0x000135c2,0x00014a3d,0x00015eb8,0x00017333,0x000187ae,0x0001a147,0x0001b851,0x0001d1eb,0x0001f0a3,0x00020ccc,0x00022b85,0x00024ccc,0x00026e14,0x0002947a,0x0002bae1,0x0002e666,
  0x000311eb,0x00034000,0x00037333,0x0003a3d7,0x0003deb8,0x00041999,0x0004570a,0x00049999,0x0004deb8,0x000528f5,0x000575c2,0x0005cccc,0x000623d7,0x00068000,0x0006e3d7,0x00074a3d,
  0x0007bd70,0x000830a3,0x0008ae14,0x00093333,0x00000000
};

short aadPitchTable[85] = {
  0x0080,0x0087,0x008f,0x0098,0x00a1,0x00aa,0x00b5,0x00bf,0x00cb,0x00d7,0x00e4,0x00f1,0x0100,0x010f,0x011f,0x0130,
  0x0142,0x0155,0x016a,0x017f,0x0196,0x01ae,0x01c8,0x01e3,0x0200,0x021e,0x023e,0x0260,0x0285,0x02ab,0x02d4,0x02ff,
  0x032c,0x035d,0x0390,0x03c6,0x0400,0x043c,0x047d,0x04c1,0x050a,0x0556,0x05a8,0x05fe,0x0659,0x06ba,0x0720,0x078d,
  0x0800,0x0879,0x08fa,0x0983,0x0a14,0x0aad,0x0b50,0x0bfc,0x0cb2,0x0d74,0x0e41,0x0f1a,0x1000,0x10f3,0x11f5,0x1306,
  0x1428,0x155b,0x16a0,0x17f9,0x1965,0x1ae8,0x1c82,0x1e34,0x2000,0x21e7,0x23eb,0x260d,0x2851,0x2ab7,0x2d41,0x2ff2,
  0x32cb,0x35d1,0x3904,0x3c68,0x3fff
};

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

void aadPlayTone(struct AadToneAtr* toneAtr, unsigned long waveStartAddr, struct AadProgramAtr* progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, struct AadSynthVoice* voice, int pitchOffset)
{
	struct AadVolume voiceVol;
	int pitch;
	int finePitch;
	int pitchIndex;
	unsigned long tmp;
	unsigned long masterVolumeSquared;

#define GET_VOLUME_SQUARED(x) (x) * (x) 
#define GET_MASTER_PAN_LEFT(x, y) ((short)x * (GET_VOLUME_SQUARED((128 - y)) - 1)) >> 12
#define GET_MASTER_PAN_RIGHT(x, y) ((short)x * (GET_VOLUME_SQUARED((y + 1)) + 1)) >> 12
#define GET_MASTER_VOL_SHIFT(x, y) (y * (x)) >> 14
#define GET_MASTER_VOL(x, y) (y * (x))

	voiceVol.right = GET_VOLUME_SQUARED(volume + 1) - 1;
	voiceVol.left = GET_VOLUME_SQUARED(volume + 1) - 1;

	if (!(aadMem->flags & 0x1))
	{
		if (masterPan >= 65)
		{
			voiceVol.left = GET_MASTER_PAN_LEFT(voiceVol.left, masterPan);
		}
		else if (masterPan < 63)
		{
			voiceVol.right = GET_MASTER_PAN_RIGHT(voiceVol.right, masterPan);
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

	masterVolumeSquared = GET_VOLUME_SQUARED(masterVolume + 1) - 1;
	voiceVol.left = GET_MASTER_VOL_SHIFT(masterVolumeSquared, voiceVol.left);
	voiceVol.right = GET_MASTER_VOL_SHIFT(masterVolumeSquared, voiceVol.right);

	tmp = GET_VOLUME_SQUARED(progAtr->volume + 1);
	pitch = tmp - 1;

	tmp = GET_VOLUME_SQUARED(slotVolume + 1);
	masterVolumeSquared = tmp - 1;

	voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
	voiceVol.right = ((voiceVol.right * pitch) >> 14);

	voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
	voiceVol.right = (voiceVol.right * pitch) >> 14;

	voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
	voiceVol.right = (voiceVol.right * pitch) >> 14;

	SpuSetVoiceVolume(voice->voiceNum, voiceVol.left, voiceVol.right);

	pitchIndex = midiNote - (toneAtr->centerNote - 60);

	if ((toneAtr->centerFine & 0x80))
	{
		pitchIndex = 256 - toneAtr->centerFine;
		finePitch = toneAtr->centerFine;
		tmp = (aadStepsPerCent[pitchIndex] * 100) * finePitch;
		finePitch = aadPitchTable[pitchIndex * pitchIndex] + (tmp >> 23);
	}
	else
	{
		finePitch = toneAtr->centerFine;
		tmp = (aadStepsPerCent[pitchIndex] * 100) * finePitch;
		finePitch = aadPitchTable[pitchIndex] + (tmp >> 23);
	}

	SpuSetVoicePitch(voice->voiceNum, (finePitch + pitchOffset) & 0xFFFF);
	SpuSetVoiceStartAddr(voice->voiceNum, waveStartAddr);
	SpuSetVoiceADSR1ADSR2(voice->voiceNum, toneAtr->adsr1, toneAtr->adsr2);

	if (toneAtr->mode == 4)
	{
		aadMem->voiceReverbRequest |= voice->voiceMask;
	}
	else
	{
		aadMem->voiceReverbRequest = aadMem->voiceReverbRequest & ~voice->voiceMask;
	}

	aadMem->voiceKeyOnRequest |= voice->voiceMask;
}


void aadPlayTonePitchBend(struct AadToneAtr *toneAtr, unsigned long waveStartAddr, struct AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, struct AadSynthVoice *voice, int pitchWheelPos)//Matching - 73.28%
{
	struct AadVolume voiceVol; // stack offset -32
	int pitch; // $a1
	int finePitch; // $a1
	int pitchIndex; // $a3
	long pitchValueBendAmount; // $t0
	unsigned long tmp; // $v0
	unsigned long masterVolumeSquared; // $v1

#define GET_VOLUME_SQUARED(x) (x) * (x) 
#define GET_MASTER_PAN_LEFT(x, y) (x * (GET_VOLUME_SQUARED((128 - y)) - 1)) >> 12
#define GET_MASTER_PAN_RIGHT(x, y) (x * (GET_VOLUME_SQUARED((y + 1)) + 1)) >> 12
#define GET_MASTER_VOL(x, y) (y * (x - 1)) >> 14

	//s0 = toneAtr
	//v0 = 1

	masterVolumeSquared = GET_VOLUME_SQUARED(volume + 1) - 1;
	//s1 = pitchWheelPos
	//a0 = masterPan
	//s2 = voice
	//s4 = waveStartAddr
	//v0 = aadMem
	voiceVol.left = masterVolumeSquared;
	voiceVol.right = masterVolumeSquared;
	//s3 = midiNote
	if (!(aadMem->flags & 0x1))
	{
		if (masterPan >= 65)
		{
			voiceVol.right = GET_MASTER_PAN_LEFT(masterPan, masterVolumeSquared);
		}
		else if (masterPan < 63)
		{
			voiceVol.left = GET_MASTER_PAN_RIGHT(masterPan, masterVolumeSquared);
		}
	}
	//loc_8005864C
	masterVolumeSquared = GET_VOLUME_SQUARED(toneAtr->volume + 1) - 1;

	//t0 = voiceVol.right * masterVolumeSquared
	voiceVol.right = (voiceVol.right * masterVolumeSquared) >> 14;
	voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
	//v1 = voiceVol.left * masterVolumeSquared
	//a0 = (voiceVol.left * masterVolumeSquared) >> 14
	//v0 = aadMem

	if (!(aadMem->flags & 0x1))
	{
		if (toneAtr->panPosition >= 65)
		{
			voiceVol.right = GET_MASTER_PAN_LEFT(voiceVol.left, toneAtr->panPosition);
		}
		else if (toneAtr->panPosition < 63)
		{
			voiceVol.left = GET_MASTER_PAN_RIGHT(voiceVol.right, toneAtr->panPosition);
		}
	}
	//loc_80058724
	//v0 = masterVolume

	masterVolumeSquared = GET_VOLUME_SQUARED(masterVolume + 1);
	voiceVol.right = GET_MASTER_VOL(masterVolumeSquared, voiceVol.right);
	voiceVol.left = GET_MASTER_VOL(masterVolumeSquared, voiceVol.left);

	tmp = GET_VOLUME_SQUARED(progAtr->volume + 1);
	pitch = tmp - 1;
	masterVolumeSquared = voiceVol.right * pitch;

	tmp = GET_VOLUME_SQUARED(slotVolume + 1);
	int a2 = (voiceVol.left * pitch) >> 14;
	pitchIndex = masterVolumeSquared >> 14;
	masterVolumeSquared = tmp - 1;
	int a1 = (pitchIndex * masterVolumeSquared) >> 14;

	int a0 = GET_VOLUME_SQUARED(masterMasterVol + 1);
	int t2 = a2 * masterVolumeSquared;
	int t0 = a1 * (a0 - 1);
	int v1 = tmp * masterVolumeSquared;

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

	SpuSetVoicePitch(voice->voiceNum, finePitch);
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




