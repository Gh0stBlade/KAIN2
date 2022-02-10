#ifndef VOICEXA_H
#define VOICEXA_H


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
