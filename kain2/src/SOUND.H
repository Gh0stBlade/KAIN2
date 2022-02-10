#ifndef SOUND_H
#define SOUND_H


extern struct SoundEffectChannel * SndOpenSfxChannel(unsigned char *channelNum); // 0x8003E64C

extern void SndCloseSfxChannel(int channelNum); // 0x8003E694

extern struct SoundEffectChannel * SndGetSfxChannel(int channelNum); // 0x8003E6BC

extern void SOUND_ProcessInstanceSounds(unsigned char *sfxFileData, struct SoundInstance *soundInstTbl, struct _Position *position, int livesInOnePlace, int inSpectral, int hidden, int burning, long *triggerFlags); // 0x8003E6E4

extern void SOUND_EndInstanceSounds(unsigned char *sfxFileData, struct SoundInstance *soundInstTbl); // 0x8003E86C

extern int isOkayToPlaySound(int flags, int spectralPlane, int hidden, int burning); // 0x8003E9D8

extern void setPeriodicSoundStateOff(struct SoundInstance *soundInst, struct ObjectPeriodicSound *sound); // 0x8003EAEC

extern void processPeriodicSound(struct _Position *position, int livesInOnePlane, int inSpectral, int hidden, int burning, struct SoundInstance *soundInst, struct ObjectPeriodicSound *sound); // 0x8003EB78

extern void processEventSound(struct _Position *position, struct SoundInstance *soundInst, struct ObjectEventSound *sound); // 0x8003F018

extern void SOUND_StartInstanceSound(struct SoundInstance *soundInst); // 0x8003F3BC

extern void SOUND_StopInstanceSound(struct SoundInstance *soundInst); // 0x8003F3DC

extern int SOUND_IsInstanceSoundLoaded(unsigned char *sfxFileData, long soundNumber); // 0x8003F438

extern void SOUND_SetInstanceSoundPitch(struct SoundInstance *soundInst, long pitchChangeAmt, long time); // 0x8003F534

extern void SOUND_SetInstanceSoundVolume(struct SoundInstance *soundInst, long volumeChangeAmt, long time); // 0x8003F5F4

extern void processOneShotSound(struct _Position *position, int hidden, int burning, long *triggerFlags, struct SoundInstance *soundInst, struct ObjectOneShotSound *sound); // 0x8003F6F8

extern unsigned long SOUND_Play3dSound(struct _Position *position, int sfxToneID, int pitch, int maxVolume, int minVolDist); // 0x8003FA40

extern unsigned long SOUND_Update3dSound(struct _Position *position, unsigned long handle, int pitch, int maxVolume, int minVolDist); // 0x8003FC7C

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
