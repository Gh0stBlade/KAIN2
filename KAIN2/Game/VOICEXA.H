#ifndef VOICEXA_H
#define VOICEXA_H

#include "LOCAL/LOCALSTR.H"

struct XAVoiceListEntry // hashcode: 0xDBE9AB23 (dec: -605443293)
{
	unsigned short length; // size=0, offset=0
};

struct XAFileInfo // hashcode: 0xD7260012 (dec: -685375470)
{
	int startPos; // size=0, offset=0
};

struct CdCommand // hashcode: 0x042C215C (dec: 70000988)
{
	unsigned short flags; // size=0, offset=0
	unsigned char unused; // size=0, offset=2
	unsigned char cdCommand; // size=0, offset=3
	unsigned char cdCmdParam[4]; // size=4, offset=4
};

struct VoiceCommand // hashcode: 0xEEB8F864 (dec: -289867676)
{
	unsigned char voiceCmd; // size=0, offset=0
	unsigned char nextVoiceStatus; // size=0, offset=1
	unsigned short voiceCmdParam; // size=0, offset=2
};

struct XAVoiceTracker // hashcode: 0x77052065 (dec: 1996824677)
{
	CdlLOC currentPos; // size=4, offset=0
	int currentSector; // size=0, offset=4
	int endSector; // size=0, offset=8
	CdlCB prevCallback; // size=0, offset=12
	unsigned char cdResult[8]; // size=8, offset=16
	struct CdCommand cdCmdQueue[8]; // size=64, offset=24
	unsigned short requestQueue[4]; // size=8, offset=88
	struct VoiceCommand voiceCmdQueue[16]; // size=64, offset=96
	unsigned char cdCmdIn; // size=0, offset=160
	unsigned char cdCmdOut; // size=0, offset=161
	unsigned char cdCmdsQueued; // size=0, offset=162
	unsigned char reqIn; // size=0, offset=163
	unsigned char reqOut; // size=0, offset=164
	unsigned char reqsQueued; // size=0, offset=165
	unsigned char voiceCmdIn; // size=0, offset=166
	unsigned char voiceCmdOut; // size=0, offset=167
	unsigned char voiceCmdsQueued; // size=0, offset=168
	unsigned char unused1; // size=0, offset=169
	unsigned char voiceStatus; // size=0, offset=170
	unsigned char cdStatus; // size=0, offset=171
	short fileNum; // size=0, offset=172
	short unused2; // size=0, offset=174
	struct XAFileInfo xaFileInfo[30]; // size=120, offset=176
};

extern enum language_t the_language; // offset 0x800D5BFC

extern struct XAVoiceListEntry* voiceList; // offset 0x800D1EE4

extern struct XAVoiceTracker voiceTracker; // offset 0x800D7488

extern void VOICEXA_Init(); // 0x800B6E14

extern void putCdCommand(struct XAVoiceTracker *vt, unsigned char cdCommand, int numParams, unsigned char *params); // 0x800B6EE0

extern void VOICEXA_CdSyncCallback(unsigned char status, unsigned char *result); // 0x800B6F64

extern void processCdCommands(struct XAVoiceTracker *vt); // 0x800B6FD0

extern void putVoiceCommand(struct XAVoiceTracker *vt, unsigned char voiceCmd, unsigned char nextVoiceStatus, int voiceCmdParam); // 0x800B7078

extern void processVoiceCommands(struct XAVoiceTracker *vt); // 0x800B70F4

extern void voiceCmdPlay(struct XAVoiceTracker *vt, short voiceIndex); // 0x800B71A4

extern void voiceCmdStop(struct XAVoiceTracker *vt, short cmdParam); // 0x800B7314

extern void voiceCmdPause(struct XAVoiceTracker *vt, short cmdParam); // 0x800B7370

extern void voiceCmdResume(struct XAVoiceTracker *vt, short cmdParam); // 0x800B73AC

extern void voiceCmdNull(struct XAVoiceTracker *vt, short cmdParam); // 0x800B73E0

extern void VOICEXA_Play(int voiceIndex, int queueRequests); // 0x800B73E8

extern int VOICEXA_FinalStatus(struct XAVoiceTracker *vt); // 0x800B74AC

extern void VOICEXA_Pause(); // 0x800B74F0

extern void VOICEXA_Resume(); // 0x800B7560

extern void VOICEXA_Tick(); // 0x800B75D0

extern int VOICEXA_IsPlaying(); // 0x800B7728

extern int VOICEXA_IsPlayingOrPaused(); // 0x800B7768


#endif
