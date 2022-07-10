#include "MIDI.H"

#include <stdio.h>
#include <string>
#include <assert.h>

#define MIDI_MAGIC (0x4D546864)
#define MIDI_VERSION (1)
#define MIDI_TRK_MAGIC (0x4D54726B)

#define IS_MIDI_EVENT(x) ((x & 0x80) != 0)
#define IS_MIDI_NOTE_ON(x) (((x >> 4) & 0x7) == 1)

char midiDataByteCount[8] = { 2, 2, 2, 2, 1, 1, 2, 2 };

template <typename T>
T swap(T& u)
{
	static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

	union
	{
		T u;
		unsigned char u8[sizeof(T)];
	} source, dest;

	source.u = u;

	for (size_t k = 0; k < sizeof(T); k++)
		dest.u8[k] = source.u8[sizeof(T) - k - 1];


	u = dest.u;
	return dest.u;
}

void midiNoteOff(struct AadSeqEvent* event, FILE* f)
{

}

void midiNoteOn(struct AadSeqEvent* event, FILE* f)
{

}

void midiPolyphonicAftertouch(struct AadSeqEvent* event, FILE* f)
{

}

void midiControlChange(struct AadSeqEvent* event, FILE* f)
{

}

void midiProgramChange(struct AadSeqEvent* event, FILE* f)
{

}

void midiChannelAftertouch(struct AadSeqEvent* event, FILE* f)
{

}

void midiPitchWheelControl(struct AadSeqEvent* event, FILE* f)
{

}

void midiMetaEvent(struct AadSeqEvent* event, FILE* f)
{

}

void (*midiEventFunction[8])(AadSeqEvent* event, FILE* f) = {

	&midiNoteOff,
	&midiNoteOn,
	&midiPolyphonicAftertouch,
	&midiControlChange,
	&midiProgramChange,
	&midiChannelAftertouch,
	&midiPitchWheelControl,
	&midiMetaEvent
};

char* MIDI_ReadFile(char* midiFilePath, unsigned int* outSize)
{
	FILE* f = fopen(midiFilePath, "rb");

	if (f != NULL)
	{
		fseek(f, 0, SEEK_END);
		
		long fileSize = ftell(f);
		outSize[0] = fileSize;
		
		fseek(f, 0, SEEK_SET);

		char* fileData = new char[fileSize];
		
		fread(fileData, fileSize, 1, f);
		
		fclose(f);
	
		return fileData;
	}

	return NULL;
}

void MIDI_WriteUInt32(FILE* f, unsigned int value)
{
	if (f != NULL)
	{
		value = swap(value);
		fwrite(&value, sizeof(unsigned int), 1, f);
		fflush(f);
	}
}

void MIDI_WriteUInt16(FILE* f, unsigned short value)
{
	if (f != NULL)
	{
		value = swap(value);
		fwrite(&value, sizeof(unsigned short), 1, f);
		fflush(f);
	}
}

void MIDI_WriteUInt8(FILE* f, unsigned char value)
{
	if (f != NULL)
	{
		value = swap(value);
		fwrite(&value, sizeof(unsigned char), 1, f);
		fflush(f);
	}
}

void MIDI_Save(char* midiFilePath, unsigned int dataLength, AadSoundBankHdr* soundBankHeader, AadProgramAtr* programAtr, AadToneAtr* toneAtr, unsigned long* waveAddr, unsigned long* sequenceOffsetTbl, unsigned long* sequenceLabelOffsetTbl, unsigned char* sequenceBase)
{
	//Every midi sequence
	for (int i = 0; i < soundBankHeader->numSequences; i++)
	{
		AadSequenceHdr* seqHdr = (AadSequenceHdr*)&sequenceBase[sequenceOffsetTbl[i]];
		AadSequenceHdr* nextSeqHdr = NULL;
		
		if (i + 1 == soundBankHeader->numSequences)
		{
			unsigned int offset = (char*)&sequenceBase[sequenceOffsetTbl[0]] - (char*)soundBankHeader;
			nextSeqHdr = (AadSequenceHdr*)&sequenceBase[dataLength - sequenceOffsetTbl[i] - offset];
		}
		else
		{
			nextSeqHdr = (AadSequenceHdr*)&sequenceBase[sequenceOffsetTbl[i + 1]];
		}

		if (i == 3)
		{
			int testing = 0;
			testing++;
		}

		unsigned int sequenceLength = ((char*)nextSeqHdr - (char*)seqHdr) - sizeof(AadSequenceHdr) - (seqHdr->numTracks * sizeof(unsigned int));

		long runningStatus[16] = {};

		char nameBuff[256];
		sprintf(nameBuff, "%s_%d_.MID", midiFilePath, i);
		FILE* f = fopen(nameBuff, "wb+");

		MIDI_WriteUInt32(f, MIDI_MAGIC);
		MIDI_WriteUInt32(f, 6);
		MIDI_WriteUInt16(f, MIDI_VERSION);
		MIDI_WriteUInt16(f, seqHdr->numTracks);
		MIDI_WriteUInt16(f, seqHdr->ppqn);

		//Every track
		for (int t = 0; t < seqHdr->numTracks; t++)
		{
			MIDI_WriteUInt32(f, MIDI_TRK_MAGIC);
			MIDI_WriteUInt32(f, 0);//Dummy

			unsigned int trkStart = ftell(f);

#if 0
			// every SMF should have GM Reset
			MIDI_WriteUInt8(f, 0x00);
			MIDI_WriteUInt8(f, 0xF0);
			MIDI_WriteUInt8(f, 0x05);
			MIDI_WriteUInt8(f, 0x7E);
			MIDI_WriteUInt8(f, 0x7F);
			MIDI_WriteUInt8(f, 0x09);
			MIDI_WriteUInt8(f, 0x01);
			MIDI_WriteUInt8(f, 0xF7);
			// and GM2 Reset
			MIDI_WriteUInt8(f, 0x00);
			MIDI_WriteUInt8(f, 0xF0);
			MIDI_WriteUInt8(f, 0x05);
			MIDI_WriteUInt8(f, 0x7E);
			MIDI_WriteUInt8(f, 0x7F);
			MIDI_WriteUInt8(f, 0x09);
			MIDI_WriteUInt8(f, 0x02);
			MIDI_WriteUInt8(f, 0xF7);
#endif

			AadSeqEvent seqEvent = {};

			unsigned int trkStartOffset = ((int*)seqHdr)[4 + t];
			unsigned int trkNextStartOffset = ((int*)seqHdr)[4 + t + 1];
			unsigned int trkLength = trkNextStartOffset - trkStartOffset;

			unsigned char* sequencePosition = (unsigned char*)(char*)seqHdr + trkStartOffset;

			if (t + 1 >= seqHdr->numTracks)
			{
				trkLength = (char*)nextSeqHdr - (char*)sequencePosition;
			}

			if (i == 3 && t == 3)
			{
				int testing = 0;
				testing++;
			}


			unsigned char* pData = &sequencePosition[trkLength-1];

			if (*pData == 0)
			{
				while (*pData == 0)
				{
					pData--;
				}

				pData += 2;

			}

			fwrite(sequencePosition, pData-sequencePosition, 1, f);

			//Write end of track just incase
#if 1
			MIDI_WriteUInt8(f, 0);
			MIDI_WriteUInt8(f, 0xFF);
			MIDI_WriteUInt8(f, 0x2F);
			MIDI_WriteUInt8(f, 0x0);
#endif

			unsigned int trkEnd = ftell(f);

			unsigned int trkLen = trkEnd - trkStart;


			fseek(f, trkStart - sizeof(unsigned int), SEEK_SET);

			MIDI_WriteUInt32(f, trkLen);

			fseek(f, trkEnd, SEEK_SET);
		}

		fclose(f);
	}
}


void MIDI_Open(char* midiFilePath)
{
	unsigned int outSize = 0;
	char* pFileData = MIDI_ReadFile(midiFilePath, &outSize);

	if (pFileData != NULL)
	{
		AadSoundBankHdr* soundBankHeader = (AadSoundBankHdr*)pFileData;
		AadProgramAtr* programAtr = (struct AadProgramAtr*)((char*)soundBankHeader + soundBankHeader->headerSize);
		AadToneAtr* toneAtr = (struct AadToneAtr*)((char*)programAtr + soundBankHeader->numPrograms * sizeof(struct AadProgramAtr));
		
		unsigned long* waveAddr = (unsigned long*)((char*)toneAtr + soundBankHeader->numTones * sizeof(struct AadToneAtr));
		unsigned long* sequenceOffsetTbl = (unsigned long*)((char*)waveAddr + soundBankHeader->numWaves * sizeof(unsigned int));
		unsigned long* sequenceLabelOffsetTbl = (unsigned long*)((char*)sequenceOffsetTbl + soundBankHeader->numSequences * sizeof(unsigned int));
		unsigned char* sequenceBase = (unsigned char*)((char*)sequenceLabelOffsetTbl + soundBankHeader->numLabels * sizeof(unsigned int));

		MIDI_Save(midiFilePath, outSize, soundBankHeader, programAtr, toneAtr, waveAddr, sequenceOffsetTbl, sequenceLabelOffsetTbl, sequenceBase);

		delete[] pFileData;
	}
}
