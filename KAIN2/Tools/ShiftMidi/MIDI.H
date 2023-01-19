#ifndef MIDI_H
#define MIDI_H

//#define AKUJI

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
#if defined(AKUJI)
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