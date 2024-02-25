#ifndef MENUFACE_H
#define MENUFACE_H

#include "../VRAM.H"

struct _ButtonTexture 
{
	short tpage; // size=0, offset=0
	short clut; // size=0, offset=2
	short textureW; // size=0, offset=4
	short textureH; // size=0, offset=6
	struct _BlockVramEntry* vramBlock; // size=0, offset=8
	long xshift; // size=0, offset=12
};

enum bdepth // Hashcode: 0xB297A406 (dec: -1298684922)
{
	TIM_4BIT = 0,
	TIM_8BIT = 1,
	TIM_16BIT = 2,
};

struct menuface_t // hashcode: 0xC2732FD8 (dec: -1032638504)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	char w; // size=0, offset=4
	char h; // size=0, offset=5
	char frames; // size=0, offset=6
	short curFrame; // size=0, offset=8
	short transitionDir; // size=0, offset=10
	unsigned short delay; // size=0, offset=12
	unsigned short loaded; // size=0, offset=14
};

typedef enum bdepth bdepth;
typedef struct menuface_t menuface_t;

extern char * NextTimAddr(char *addr, int w, int h, enum bdepth bpp); // 0x800B9538
extern void menuface_initialize(); // 0x800B9580
extern void menuface_terminate(); // 0x800B96AC
extern void MENUFACE_ChangeStateRandomly(int index); // 0x800B9758
extern void MENUFACE_RefreshFaces(); // 0x800B987C

#endif
