#ifndef AADSFX_H
#define AADSFX_H


extern unsigned long aadPlaySfx(unsigned int toneID, int volume, int pan, int pitchOffset); // 0x800569DC

extern unsigned long aadStopSfx(unsigned long handle); // 0x80056A40

extern void aadStopAllSfx(); // 0x80056A7C

extern int aadIsSfxPlaying(unsigned long handle); // 0x80056AAC

extern int aadIsSfxPlayingOrRequested(unsigned long handle); // 0x80056B1C

extern int aadIsSfxTypePlaying(unsigned int toneID); // 0x80056BD4

extern int aadIsSfxTypePlayingOrRequested(unsigned int sfxToneID); // 0x80056C48

extern unsigned long aadSetSfxVolPanPitch(unsigned long handle, int volume, int pan, int pitch); // 0x80056D00

extern unsigned long createSfxHandle(unsigned int toneID); // 0x80056D44

extern void aadPutSfxCommand(int statusByte, int dataByte0, int dataByte1, unsigned long ulongParam, int shortParam); // 0x80056D8C

extern void aadExecuteSfxCommand(struct AadSfxCommand *sfxCmd); // 0x80056E80

extern void sfxCmdPlayTone(struct AadSfxCommand *sfxCmd); // 0x80056ECC

extern void sfxCmdStopTone(struct AadSfxCommand *sfxCmd); // 0x80057038

extern void sfxCmdStopAllTones(struct AadSfxCommand *sfxCmd); // 0x800570F0

extern void sfxCmdSetToneVolumeAndPan(struct AadSfxCommand *sfxCmd); // 0x80057194

extern void sfxCmdSetToneVolPanPitch(struct AadSfxCommand *sfxCmd); // 0x80057508

extern void sfxCmdLockVoice(struct AadSfxCommand *sfxCmd); // 0x80057974

extern void sfxCmdSetVoiceAttr(struct AadSfxCommand *sfxCmd); // 0x800579C0

extern void sfxCmdSetVoiceKeyOn(struct AadSfxCommand *sfxCmd); // 0x80057A4C

extern void sfxCmdSetVoiceKeyOff(struct AadSfxCommand *sfxCmd); // 0x80057A68


#endif
