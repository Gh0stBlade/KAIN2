#ifndef __PSYQ_H
#define __PSYQ_H

typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned short u_short;
typedef unsigned char u_char;

char* MemW32_GetMemBase();
int MemW32_GetSize();
char* getScratchAddr(int a1);

typedef struct TCB // hashcode: 0x6A70BBF4 (dec: 1785773044)
{
	long status; // size=0, offset=0
	long mode; // size=0, offset=4
	unsigned long reg[40]; // size=160, offset=8
	long system[6]; // size=24, offset=168
} TCB;

#undef s_addr

typedef struct EXEC // hashcode: 0x4291A2AD (dec: 1116840621)
{
	unsigned long pc0; // size=0, offset=0
	unsigned long gp0; // size=0, offset=4
	unsigned long t_addr; // size=0, offset=8
	unsigned long t_size; // size=0, offset=12
	unsigned long d_addr; // size=0, offset=16
	unsigned long d_size; // size=0, offset=20
	unsigned long b_addr; // size=0, offset=24
	unsigned long b_size; // size=0, offset=28
	unsigned long s_addr; // size=0, offset=32
	unsigned long s_size; // size=0, offset=36
	unsigned long sp; // size=0, offset=40
	unsigned long fp; // size=0, offset=44
	unsigned long gp; // size=0, offset=48
	unsigned long ret; // size=0, offset=52
	unsigned long base; // size=0, offset=56
} EXEC;

typedef struct DIRENTRY // hashcode: 0xD989A944 (dec: -645289660)
{
	char name[20]; // size=20, offset=0
	long attr; // size=0, offset=20
	long size; // size=0, offset=24
	struct DIRENTRY *next; // size=40, offset=28
	long head; // size=0, offset=32
	char system[4]; // size=4, offset=36
} DIRENTRY;

#ifndef PSX_VERSION
typedef struct CdlLOC // hashcode: 0x449289F8 (dec: 1150454264)
{
	unsigned char minute; // size=0, offset=0
	unsigned char second; // size=0, offset=1
	unsigned char sector; // size=0, offset=2
	unsigned char track; // size=0, offset=3
} CdlLOC;
#endif

#endif
