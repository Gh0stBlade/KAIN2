#ifndef CINEPSX_H
#define CINEPSX_H

struct cinema_fn_table_t // hashcode: 0xBFD79C7E (dec: -1076388738)
{
	int (*play)(); // size=0, offset=0
	char* versionID; // size=0, offset=4
};


extern int CINE_CDIntrQuery(); // 0x800B777C

extern unsigned short CINE_Pad(int pad); // 0x800B77A4

extern void CINE_Play(char *strfile, unsigned short mask, int buffers); // 0x800B77C8

extern int CINE_Load(); // 0x800B782C

extern int CINE_Loaded(); // 0x800B78D0

extern void CINE_Unload(); // 0x800B78E0

extern void CINE_PlayIngame(int number); // 0x800B7928


#endif
