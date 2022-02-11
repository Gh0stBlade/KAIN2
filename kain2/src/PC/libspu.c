#include "libspu.h"

//0001:00031520       _SpuSetCommonMasterVolume  00432520 f   libspu.obj
void SpuSetCommonMasterVolume(short mvol_left, short mvol_right){}
//0001 : 00031530       _SpuClearReverbWorkArea    00432530 f   libspu.obj
long SpuClearReverbWorkArea(long mode) { return 0; }
//0001 : 00031540       _SpuGetAllKeysStatus       00432540 f   libspu.obj
void SpuGetAllKeysStatus(char* status){}
//0001 : 00031550       _SpuGetVoicePitch          00432550 f   libspu.obj
void SpuGetVoicePitch(int vNum, unsigned short* pitch){}
//0001 : 00031560       _SpuInit                   00432560 f   libspu.obj
void SpuInit (void){}
//0001 : 00031570       _SpuIsTransferCompleted    00432570 f   libspu.obj
long SpuIsTransferCompleted(long flag) { return 1; }
//0001 : 00031580       _SpuQuit                   00432580 f   libspu.obj
void SpuQuit(void) {}
//0001:00031590       _SpuSetCommonAttr          00432590 f   libspu.obj
void SpuSetCommonAttr(SpuCommonAttr* attr){}
//0001:000315a0       _SpuSetKey                 004325a0 f   libspu.obj
void SpuSetKey(long on_off, unsigned long voice_bit){}
//0001 : 000315b0       _SpuSetMute                004325b0 f   libspu.obj
long SpuSetMute(long on_off) { return 0; }
//0001 : 000315c0       _SpuSetReverb              004325c0 f   libspu.obj
long SpuSetReverb(long on_off) { return 0; }
//0001 : 000315d0       _SpuSetReverbDepth         004325d0 f   libspu.obj
long SpuSetReverbDepth(SpuReverbAttr* attr) { return 0; }
//0001 : 000315e0       _SpuSetReverbModeParam     004325e0 f   libspu.obj
long SpuSetReverbModeParam(SpuReverbAttr* attr) { return 0; }
//0001 : 000315f0       _SpuSetReverbVoice         004325f0 f   libspu.obj
unsigned long SpuSetReverbVoice(long on_off, unsigned long voice_bit) { return 0; }
//0001 : 00031600       _SpuSetTransferStartAddr   00432600 f   libspu.obj
unsigned long SpuSetTransferStartAddr(unsigned long addr) { return 0; }
//0001 : 00031610       _SpuSetTransferCallback    00432610 f   libspu.obj
SpuTransferCallbackProc SpuSetTransferCallback(SpuTransferCallbackProc func) { return 0; }
//0001 : 00031620       _SpuSetVoiceADSRAttr       00432620 f   libspu.obj
void SpuSetVoiceADSRAttr(int vNum, unsigned short AR, unsigned short DR, unsigned short SR, unsigned short RR, unsigned short SL, long ARmode, long SRmode, long RRmode) {}
//0001 : 00031630       _SpuSetVoicePitch          00432630 f   libspu.obj
void SpuSetVoicePitch(int vNum, unsigned short pitch) {}
//0001 : 00031640       _SpuSetVoiceStartAddr      00432640 f   libspu.obj
void SpuSetVoiceStartAddr(int vNum, unsigned long startAddr) {}
//0001 : 00031650       _SpuSetVoiceVolume         00432650 f   libspu.obj
void SpuSetVoiceVolume(int vNum, short volL, short volR) {}
//0001 : 00031660       _SpuWrite                  00432660 f   libspu.obj
unsigned long SpuWrite(unsigned char* addr, unsigned long size) { return 0; }
//0001 : 00031670       _SpuSetCommonCDMix         00432670 f   libspu.obj
void SpuSetCommonCDMix(long cd_mix) {}
//0001 : 00031680       _SpuSetReverbModeDepth     00432680 f   libspu.obj
void SpuSetReverbModeDepth(short depth_left, short depth_right){}
//0001:00031690       _SpuSetReverbModeType      00432690 f   libspu.obj
long SpuSetReverbModeType(long mode) { return 0; }
