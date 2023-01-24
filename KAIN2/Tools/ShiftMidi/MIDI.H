#ifndef MIDI_H
#define MIDI_H

//#define AKUJI
//#define KAIN2_ALPHA

struct AadSoundBankHdr
{
#if defined(AKUJI)
	unsigned long bankID;
	unsigned short headerSize;
	unsigned char unknown;
	unsigned char numPrograms;
	unsigned short numTones;
	unsigned short numWaves;
	unsigned short numSequences;
	unsigned short numLabels;
	unsigned short reverbMode;
	unsigned short reverbDepth;
	unsigned short unknown3[19];
#elif defined(KAIN2_ALPHA)
	unsigned long bankID;
	unsigned long headerSize;
	unsigned short bankVersion;
	unsigned char numBanks;
	unsigned char numPrograms;
	unsigned short numTones;
	unsigned short numWaves;
	unsigned char numSequences;
	unsigned char numPitchMaps;
	unsigned short numLabels;
	unsigned char reverbMode;
	unsigned char unused1;
	short reverbDepth;
	unsigned short objectID;
	unsigned short unused2;
	unsigned long unused3;
	unsigned long unused4;
	unsigned long unused5;
	unsigned long unused6;
	unsigned long unused7;
	unsigned long unused8;
	unsigned long unused9;
	unsigned long unused10;
	unsigned long unused11;
	unsigned long unused12;
	unsigned long unused13;
	unsigned long unused14;
	unsigned long unused15;
	unsigned char baseProgramTbl[16];
	unsigned short baseToneTbl[16];
#else
	unsigned long bankID;
	unsigned long headerSize;
	int bankVersion;
	int numPrograms;
	int numTones;
	int numWaves;
	int numSequences;
	int numLabels;
	int reverbMode;
	int reverbDepth;
#endif
};

struct AadSeqEvent
{
	unsigned long deltaTime;
	unsigned char track;
	unsigned char statusByte;
	char dataByte[6];
};

struct AadSequenceHdr
{
#if defined(AKUJI)|| defined(KAIN2_ALPHA)
	unsigned long sequenceID;
	unsigned long quarterNoteTime;
	unsigned short ppqn;
	unsigned short version;
#else
	unsigned long sequenceID;
	unsigned long quarterNoteTime;
	unsigned short ppqn;
	unsigned short version;
	unsigned short numTracks;
	unsigned short unused;
#endif
};

struct AadProgramAtr
{
	unsigned short numTones;
	unsigned short firstTone;
	unsigned char volume;
	unsigned char panPosition;
	unsigned short unused;
};

struct AadToneAtr
{
	unsigned char priority;
	unsigned char parentProgram;
	unsigned char volume;
	unsigned char panPosition;
	unsigned char centerNote;
	char centerFine;
	unsigned char minNote;
	unsigned char maxNote;
	unsigned char mode;
	unsigned char pitchBendMax;
	unsigned short adsr1;
	unsigned short adsr2;
	short waveIndex;
};

extern void MIDI_Open(char* midiFilePath);

#endif