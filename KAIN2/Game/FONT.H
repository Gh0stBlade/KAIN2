#ifndef FONT_H
#define FONT_H

#include "G2/QUATG2.H"
#include "CORE.H"

typedef struct FontChar // hashcode: 0x5A2F9F5B (dec: 1513070427)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	char c; // size=0, offset=4
	char color; // size=0, offset=5
} FontChar;

struct FontTracker // hashcode: 0xE32D3F0A (dec: -483574006)
{
	struct FontChar font_buffer[256]; // size=1536, offset=0
	long font_xpos; // size=0, offset=1536
	long font_ypos; // size=0, offset=1540
	long font_buffIndex; // size=0, offset=1544
	short font_tpage; // size=0, offset=1548
	short font_clut; // size=0, offset=1550
	short font_vramU; // size=0, offset=1552
	short font_vramV; // size=0, offset=1554
	short font_width; // size=0, offset=1556
	short sprite_sort_push; // size=0, offset=1558
	short font_vramX; // size=0, offset=1560
	short font_vramY; // size=0, offset=1562
	char color_global; // size=0, offset=1564
	char color_local; // size=0, offset=1565
};

typedef struct font_color_t // hashcode: 0x5F515B79 (dec: 1599167353)
{
	int r; // size=0, offset=0
	int g; // size=0, offset=4
	int b; // size=0, offset=8
} font_color_t;

typedef struct FontPos // hashcode: 0x0084D7DD (dec: 8706013)
{
	char x; // size=0, offset=0
	char y; // size=0, offset=1
	char w; // size=0, offset=2
	char h; // size=0, offset=3
} FontPos;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern unsigned short SpecialFogClut;
extern FontPos fontPos[79];
extern char charMap[92][3];
extern struct Object* fontsObject;
extern struct _BlockVramEntry* FONT_vramBlock; // offset 0x800D05E4
extern struct FontTracker fontTracker;

extern void FONT_MakeSpecialFogClut(int x, int y); // 0x8002CB74
extern void FONT_Init(); // 0x8002CBF8
extern void FONT_ReloadFont(); // 0x8002CD18
extern void FONT_DrawChar(struct FontChar *fontChar); // 0x8002CD7C
extern long FONT_Get2DImageIndex(unsigned char c); // 0x8002CDB0
extern void drawChar2DPoly(long fpi, long x, long y); // 0x8002CDC8
extern void FONT_DrawChar2D(unsigned char c, long x, long y); // 0x8002CFC8
extern long FONT_CharSpacing(char c, long fontXSize); // 0x8002D1FC
extern void FONT_AddCharToBuffer(char c, long x, long y); // 0x8002D340
extern void FONT_Print(const char *fmt, ...); // 0x8002D3AC
extern void FONT_Print2(const char *fmt, ...); // 0x8002D43C
extern int FONT_GetStringWidth(char *str); // 0x8002D48C
extern void FONT_Flush(); // 0x8002D500
extern void FONT_SetCursor(short x, short y); // 0x8002D590
extern void FONT_VaReallyPrint(const char *fmt, va_list ap); // 0x8002D5B0
extern void FONT_FontPrintCentered(char *text, long y); // 0x8002D740
extern void FONT_SetColorIndex(int color); // 0x8002D798
extern void FONT_SetColorIndexCol(int color, int r, int g, int b); // 0x8002D7A4

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
