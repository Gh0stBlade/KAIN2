#ifndef SOUND_H
#define SOUND_H

#include "PSX/AADLIB.H"
#include "Game/INSTANCE.H"

struct _AnimSoundData_Type 
{
	short sfxToneID; // size=0, offset=0
	short volume; // size=0, offset=2
	short pitch; // size=0, offset=4
	short minVolDistance; // size=0, offset=6
};

struct ObjectOneShotSound 
{
	unsigned char type; // size=0, offset=0
	unsigned char numSfxIDs; // size=0, offset=1
	unsigned char soundInst; // size=0, offset=2
	unsigned char flags; // size=0, offset=3
	unsigned short minVolDistance; // size=0, offset=4
	short pitch; // size=0, offset=6
	unsigned short pitchVariation; // size=0, offset=8
	unsigned char maxVolume; // size=0, offset=10
	unsigned char maxVolVariation; // size=0, offset=11
	unsigned char initialDelay; // size=0, offset=12
	unsigned char initialDelayVariation; // size=0, offset=13
};

struct ObjectEventSound 
{
	unsigned char type; // size=0, offset=0
	unsigned char numSfxIDs; // size=0, offset=1
	unsigned char soundInst; // size=0, offset=2
	unsigned char flags; // size=0, offset=3
	unsigned short minVolDistance; // size=0, offset=4
	short pitch; // size=0, offset=6
	unsigned short pitchVariation; // size=0, offset=8
	unsigned char maxVolume; // size=0, offset=10
	unsigned char maxVolVariation; // size=0, offset=11
	unsigned char initialDelay; // size=0, offset=12
	unsigned char initialDelayVariation; // size=0, offset=13
};

struct ObjectPeriodicSound 
{
	unsigned char type; // size=0, offset=0
	unsigned char numSfxIDs; // size=0, offset=1
	unsigned char soundInst; // size=0, offset=2
	unsigned char flags; // size=0, offset=3
	unsigned short minVolDistance; // size=0, offset=4
	short pitch; // size=0, offset=6
	unsigned short pitchVariation; // size=0, offset=8
	unsigned char maxVolume; // size=0, offset=10
	unsigned char maxVolVariation; // size=0, offset=11
	unsigned char initialDelay; // size=0, offset=12
	unsigned char initialDelayVariation; // size=0, offset=13
	unsigned char onTime; // size=0, offset=14
	unsigned char onTimeVariation; // size=0, offset=15
	unsigned char offTime; // size=0, offset=16
	unsigned char offTimeVariation; // size=0, offset=17
};

struct MusicLoadCmd 
{
	int type; // size=0, offset=0
	int data; // size=0, offset=4
};

struct MusicLoadInfo 
{
	int state; // size=0, offset=0
	int nextState; // size=0, offset=4
	int errorStatus; // size=0, offset=8
	int bankLoaded; // size=0, offset=12
	int checkMusicDelay; // size=0, offset=16
	int currentMusicPlane; // size=0, offset=20
	char currentMusicName[8]; // size=8, offset=24
	struct MusicLoadCmd commandQueue[4]; // size=32, offset=32
	int commandIn; // size=0, offset=64
	int commandOut; // size=0, offset=68
	int numCmdsInQueue; // size=0, offset=72
};

struct SoundEffectChannel 
{
	unsigned char inUse; // size=0, offset=0
	unsigned char unused; // size=0, offset=1
	unsigned short unused1; // size=0, offset=2
	unsigned long handle; // size=0, offset=4
	short volume; // size=0, offset=8
	short volumeChangeTime; // size=0, offset=10
	short volumeChangeTimeSave; // size=0, offset=12
	short volumeChangePerUpdate; // size=0, offset=14
	short volumeChangeSign; // size=0, offset=16
	short volumeChangeErrPerUpdate; // size=0, offset=18
	short volumeChangeError; // size=0, offset=20
	short pitch; // size=0, offset=22
	short pitchChangeTime; // size=0, offset=24
	short pitchChangeTimeSave; // size=0, offset=26
	short pitchChangePerUpdate; // size=0, offset=28
	short pitchChangeSign; // size=0, offset=30
	short pitchChangeErrPerUpdate; // size=0, offset=32
	short pitchChangeError; // size=0, offset=34
};

extern char soundBuffer[sizeof(struct AadMemoryStruct) + (sizeof(struct _AadSequenceSlot) * 4)];

extern struct MusicLoadInfo musicInfo; // offset 0x800D1510

extern int gSramFreeBlocks; // offset 0x800D1500

extern int gSramFullAlarm; // offset 0x800CEA88

extern int gSramFullMsgCnt; // offset 0x800CEA8C

extern int gSramLargestFree; // offset 0x800D12BC

extern int gSramTotalFree; // offset 0x800D150C

extern int gSramTotalUsed; // offset 0x800D1508

extern int gSramUsedBlocks; // offset 0x800D1504

extern struct SoundEffectChannel * SndOpenSfxChannel(unsigned char *channelNum); // 0x8003E64C

extern void SndCloseSfxChannel(int channelNum); // 0x8003E694

extern struct SoundEffectChannel * SndGetSfxChannel(int channelNum); // 0x8003E6BC

extern void SOUND_ProcessInstanceSounds(unsigned char *sfxFileData, struct SoundInstance *soundInstTbl, _Position *position, int livesInOnePlace, int inSpectral, int hidden, int burning, long *triggerFlags); // 0x8003E6E4

extern void SOUND_EndInstanceSounds(unsigned char *sfxFileData, struct SoundInstance *soundInstTbl); // 0x8003E86C

extern int isOkayToPlaySound(int flags, int spectralPlane, int hidden, int burning); // 0x8003E9D8

extern void setPeriodicSoundStateOff(struct SoundInstance *soundInst, struct ObjectPeriodicSound *sound); // 0x8003EAEC

extern void processPeriodicSound(_Position *position, int livesInOnePlane, int inSpectral, int hidden, int burning, struct SoundInstance *soundInst, struct ObjectPeriodicSound *sound); // 0x8003EB78

extern void processEventSound(_Position *position, struct SoundInstance *soundInst, struct ObjectEventSound *sound); // 0x8003F018

extern void SOUND_StartInstanceSound(struct SoundInstance *soundInst); // 0x8003F3BC

extern void SOUND_StopInstanceSound(struct SoundInstance *soundInst); // 0x8003F3DC

extern int SOUND_IsInstanceSoundLoaded(unsigned char *sfxFileData, long soundNumber); // 0x8003F438

extern void SOUND_SetInstanceSoundPitch(struct SoundInstance *soundInst, long pitchChangeAmt, long time); // 0x8003F534

extern void SOUND_SetInstanceSoundVolume(struct SoundInstance *soundInst, long volumeChangeAmt, long time); // 0x8003F5F4

extern void processOneShotSound(_Position *position, int hidden, int burning, long *triggerFlags, struct SoundInstance *soundInst, struct ObjectOneShotSound *sound); // 0x8003F6F8

extern unsigned long SOUND_Play3dSound(_Position *position, int sfxToneID, int pitch, int maxVolume, int minVolDist); // 0x8003FA40

extern unsigned long SOUND_Update3dSound(_Position *position, unsigned long handle, int pitch, int maxVolume, int minVolDist); // 0x8003FC7C

extern void SOUND_HandleGlobalValueSignal(int name, long data); // 0x8003FEB0

extern void SOUND_Init(); // 0x8003FEB8

extern void SOUND_Free(); // 0x8003FF6C

extern void SOUND_SetMusicVariable(int variable, int value); // 0x8003FF90

extern void SOUND_SetMusicVolume(int newVolume); // 0x8003FFB0

extern void SOUND_SetSfxVolume(int newVolume); // 0x8003FFE4

extern void SOUND_SetVoiceVolume(int newVolume); // 0x80040018

extern void SOUND_PauseAllSound(); // 0x80040038

extern void SOUND_ResumeAllSound(); // 0x80040068

extern void SOUND_StopAllSound(); // 0x80040098

extern void SOUND_ResetAllSound(); // 0x800400D8

extern void SOUND_MusicOff(); // 0x80040108

extern void SOUND_MusicOn(); // 0x8004016C

extern void SOUND_SfxOff(); // 0x800401A4

extern void SOUND_SfxOn(); // 0x800401C4

extern int SndIsPlaying(unsigned long handle); // 0x800401CC

extern int SndIsPlayingOrRequested(unsigned long handle); // 0x800401EC

extern int SndTypeIsPlayingOrRequested(unsigned int sfxToneID); // 0x8004020C

extern unsigned long SndPlay(unsigned int sample); // 0x8004022C

extern void SndEndLoop(unsigned long handle); // 0x80040264

extern unsigned long SndPlayVolPan(unsigned int sample, unsigned short vol, unsigned short pan, short pitch); // 0x80040284

extern unsigned long SndUpdateVolPanPitch(unsigned long handle, unsigned short vol, unsigned short pan, short pitch); // 0x800402C0

extern void musicLoadReturnFunc(int dynamicBankIndex, int errorStatus); // 0x80040308

extern void musicFadeoutReturnFunc(); // 0x8004031C

extern void musicEndCallbackFunc(long userData, int slot, int loopFlag); // 0x80040330

extern void SOUND_PutMusicCommand(int cmdType, int cmdData); // 0x80040390

extern void SOUND_MusicInit(); // 0x800403D8

extern int SOUND_IsMusicLoading(); // 0x80040404

extern void SOUND_ProcessMusicLoad(); // 0x80040434

extern void SOUND_UpdateSound(); // 0x80040930

extern void SOUND_PlaneShift(int newPlane); // 0x800409DC

extern void SOUND_ShutdownMusic(); // 0x80040A18

extern void SOUND_SetMusicModifier(long modifier); // 0x80040ACC

extern void SOUND_ResetMusicModifier(long modifier); // 0x80040B8C


#endif
