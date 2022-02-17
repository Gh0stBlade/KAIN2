#ifndef __PSYQ_H
#define __PSYQ_H

typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned char u_char;

struct TCB // hashcode: 0x6A70BBF4 (dec: 1785773044)
{
	long status; // size=0, offset=0
	long mode; // size=0, offset=4
	unsigned long reg[40]; // size=160, offset=8
	long system[6]; // size=24, offset=168
};

#undef s_addr

struct EXEC // hashcode: 0x4291A2AD (dec: 1116840621)
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
};

struct DIRENTRY // hashcode: 0xD989A944 (dec: -645289660)
{
	char name[20]; // size=20, offset=0
	long attr; // size=0, offset=20
	long size; // size=0, offset=24
	struct DIRENTRY *next; // size=40, offset=28
	long head; // size=0, offset=32
	char system[4]; // size=4, offset=36
};

struct MATRIX // hashcode: 0x610186A2 (dec: 1627489954)
{
	short m[3][3]; // size=18, offset=0
	long t[3]; // size=12, offset=20
};

struct VECTOR // hashcode: 0xE0DB0D68 (dec: -522515096)
{
	long vx; // size=0, offset=0
	long vy; // size=0, offset=4
	long vz; // size=0, offset=8
	long pad; // size=0, offset=12
};

struct SVECTOR // hashcode: 0x55473CEB (dec: 1430732011)
{
	short vx; // size=0, offset=0
	short vy; // size=0, offset=2
	short vz; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct CVECTOR // hashcode: 0xDE4B0C81 (dec: -565506943)
{
	unsigned char r; // size=0, offset=0
	unsigned char g; // size=0, offset=1
	unsigned char b; // size=0, offset=2
	unsigned char cd; // size=0, offset=3
};

struct DVECTOR // hashcode: 0x5BF12E56 (dec: 1542532694)
{
	short vx; // size=0, offset=0
	short vy; // size=0, offset=2
};

struct RVECTOR // hashcode: 0xEC34D5C3 (dec: -332081725)
{
	struct SVECTOR v; // size=8, offset=0
	unsigned char uv[2]; // size=2, offset=8
	unsigned short pad; // size=0, offset=10
	struct CVECTOR c; // size=4, offset=12
	struct DVECTOR sxy; // size=4, offset=16
	unsigned long sz; // size=0, offset=20
};

struct CRVECTOR3 // hashcode: 0x9A564233 (dec: -1705622989)
{
	struct RVECTOR r01; // size=24, offset=0
	struct RVECTOR r12; // size=24, offset=24
	struct RVECTOR r20; // size=24, offset=48
	struct RVECTOR *r0; // size=24, offset=72
	struct RVECTOR *r1; // size=24, offset=76
	struct RVECTOR *r2; // size=24, offset=80
	unsigned long *rtn; // size=0, offset=84
};

struct CRVECTOR4 // hashcode: 0x56AED614 (dec: 1454298644)
{
	struct RVECTOR r01; // size=24, offset=0
	struct RVECTOR r02; // size=24, offset=24
	struct RVECTOR r31; // size=24, offset=48
	struct RVECTOR r32; // size=24, offset=72
	struct RVECTOR rc; // size=24, offset=96
	struct RVECTOR *r0; // size=24, offset=120
	struct RVECTOR *r1; // size=24, offset=124
	struct RVECTOR *r2; // size=24, offset=128
	struct RVECTOR *r3; // size=24, offset=132
	unsigned long *rtn; // size=0, offset=136
};

struct RECT // hashcode: 0xDFC821CB (dec: -540532277)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short w; // size=0, offset=4
	short h; // size=0, offset=6
};

struct DR_ENV // hashcode: 0xD77E7FD5 (dec: -679575595)
{
	unsigned long tag; // size=0, offset=0
	unsigned long code[15]; // size=60, offset=4
};

struct P_TAG // hashcode: 0x962C4103 (dec: -1775484669)
{
	unsigned int addr; // size=24, offset=0
	unsigned int len; // size=8, offset=24
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
};

struct CdlLOC // hashcode: 0x449289F8 (dec: 1150454264)
{
	unsigned char minute; // size=0, offset=0
	unsigned char second; // size=0, offset=1
	unsigned char sector; // size=0, offset=2
	unsigned char track; // size=0, offset=3
};

#endif
