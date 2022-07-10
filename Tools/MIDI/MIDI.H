#ifndef MIDI_H
#define MIDI_H

struct AadSoundBankHdr
{
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
	unsigned long sequenceID;
	unsigned long quarterNoteTime;
	unsigned short ppqn;
	unsigned short version;
	unsigned short numTracks;
	unsigned short unused;
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