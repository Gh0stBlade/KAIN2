#ifndef FONT_H
#define FONT_H


extern void FONT_MakeSpecialFogClut(int x, int y); // 0x8002CB74

extern void FONT_Init(); // 0x8002CBF8

extern void FONT_ReloadFont(); // 0x8002CD18

extern void FONT_DrawChar(struct FontChar *fontChar); // 0x8002CD7C

extern long FONT_Get2DImageIndex(unsigned char c); // 0x8002CDB0

extern void drawChar2DPoly(long fpi, long x, long y); // 0x8002CDC8

extern void FONT_DrawChar2D(unsigned char c, long x, long y); // 0x8002CFC8

extern long FONT_CharSpacing(char c, long fontXSize); // 0x8002D1FC

extern void FONT_AddCharToBuffer(char c, long x, long y); // 0x8002D340

extern void FONT_Print(char *fmt, ...); // 0x8002D3AC

extern void FONT_Print2(char *fmt); // 0x8002D43C

extern int FONT_GetStringWidth(char *str); // 0x8002D48C

extern void FONT_Flush(); // 0x8002D500

extern void FONT_SetCursor(short x, short y); // 0x8002D590

extern void FONT_VaReallyPrint(char *fmt, void *ap); // 0x8002D5B0

extern void FONT_FontPrintCentered(char *text, long y); // 0x8002D740

extern void FONT_SetColorIndex(int color); // 0x8002D798

extern void FONT_SetColorIndexCol(int color, int r, int g, int b); // 0x8002D7A4


#endif
