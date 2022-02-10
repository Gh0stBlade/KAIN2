#ifndef AADVOICE_H
#define AADVOICE_H


extern struct AadSynthVoice * aadAllocateVoice(int priority); // 0x80057A98

extern void SpuSetVoiceADSR1ADSR2(int vNum, unsigned short adsr1, unsigned short adsr2); // 0x80057B8C

extern void aadPlayTone(struct AadToneAtr *toneAtr, unsigned long waveStartAddr, struct AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, struct AadSynthVoice *voice, int pitchOffset); // 0x80057C34

extern void aadPlayTonePitchBend(struct AadToneAtr *toneAtr, unsigned long waveStartAddr, struct AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, struct AadSynthVoice *voice, int pitchWheelPos); // 0x80058064


#endif
