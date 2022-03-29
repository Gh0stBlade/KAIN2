#include "CORE.H"
#include "FONT.H"
#include "VRAM.H"
#include "STRMLOAD.H"
#include "LOAD3D.H"
#include "MEMPACK.H"
#include "GAMELOOP.H"

#include <stddef.h>

#pragma warning(disable: 4244)

unsigned short SpecialFogClut;
struct _BlockVramEntry* FONT_vramBlock; // offset 0x800D05E4
static WORD font_clut;
struct FontTracker fontTracker;
struct Object* fontsObject;

char fp_str[512];

unsigned char fontTransTable[128] = { 0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,
									  0x24,0x24,0x24,0x24,0x24,0x56,0x57,0x58,0x59,0x5A,0x5B,0x24,
									  0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x25,0x28,0x24,
									  0x24,0x29,0x24,0x33,0x2B,0x2C,0x2F,0x2D,0x32,0x27,0x24,0x26,
									  0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x2A,0x24,
									  0x55,0x2E,0x54,0x30,0x24,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,
									  0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,0x46,
									  0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x24,0x4F,0x52,0x24,
									  0x24,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,
									  0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
								      0x17,0x18,0x19,0x50,0x53,0x51,0x31,0x24 };

#if defined(PC_VERSION)
font_color_t the_font_color_table[5] =
{
	0x00, 0x00, 0x00,
	0x40, 0x40, 0x40,
	0x40, 0x40, 0xff,
	0x58, 0x58, 0x68,
	0xdc, 0xdc, 0x40
};

#elif defined(PSX_VERSION)
font_color_t the_font_color_table[5] =
{
	0x00, 0x00, 0x00,
	0x40, 0x40, 0x40,
	0x40, 0x40, 0xff,
	0x58, 0x58, 0x68,
	0xc0, 0xd2, 0xd2
};
#endif
FontPos fontPos[79] = { 0x00,0x00,0x0B,0x0C,0x0B,
						0x00,0x08,0x0C,0x14,0x00,
						0x08,0x0C,0x1C,0x00,0x09,
						0x0C,0x25,0x00,0x08,0x0C,
						0x2D,0x00,0x08,0x0C,0x35,
						0x00,0x0A,0x0C,0x00,0x0C,
						0x09,0x0C,0x09,0x0C,0x03,
						0x0C,0x2A,0x0C,0x05,0x0C,
						0x2F,0x0D,0x08,0x0B,0x01,
						0x6F,0x07,0x0C,0x00,0x1A,
						0x0A,0x0C,0x0B,0x1A,0x08,
						0x0C,0x13,0x1A,0x0A,0x0C,
						0x1D,0x1A,0x07,0x0C,0x2E,
						0x1A,0x08,0x0C,0x00,0x27,
						0x09,0x0C,0x0A,0x27,0x09,
						0x0C,0x13,0x27,0x08,0x0B,
						0x1B,0x27,0x09,0x0B,0x25,
						0x27,0x0A,0x0C,0x2F,0x27,
						0x08,0x0C,0x01,0x33,0x09,
						0x0D,0x0A,0x33,0x08,0x0D,
						0x12,0x33,0x08,0x0D,0x1B,
						0x33,0x07,0x0D,0x23,0x33,
						0x08,0x0D,0x2C,0x33,0x07,
						0x0D,0x34,0x33,0x07,0x0C,
						0x0C,0x0C,0x08,0x0C,0x14,
						0x0C,0x07,0x0C,0x21,0x0C,
						0x07,0x0C,0x3D,0x20,0x03,
						0x03,0x3D,0x33,0x02,0x0D,
						0x36,0x19,0x07,0x0C,0x30,
						0x05,0x05,0x07,0x1B,0x0C,
						0x05,0x0C,0x39,0x27,0x06,
						0x0B,0x3D,0x1A,0x03,0x0A,
						0x13,0x1A,0x05,0x0C,0x18,
						0x1A,0x05,0x0B,0x10,0x64,
						0x07,0x0A,0x31,0x66,0x08,
						0x07,0x31,0x25,0x05,0x0C,
						0x20,0x40,0x07,0x0C,0x24,
						0x19,0x09,0x05,0x14,0x46,
						0x05,0x04,0x00,0x40,0x08,
						0x0A,0x28,0x40,0x07,0x0C,
						0x3D,0x33,0x02,0xF3,0x25,
						0x1E,0x08,0x02,0x12,0x40,
						0x07,0x04,0x0A,0x40,0x06,
						0x05,0x0A,0x46,0x06,0x05,
						0x24,0x19,0x08,0x04,0x1B,
						0x47,0x05,0x04,0x2F,0x40,
						0x0B,0x0C,0x00,0x4D,0x0C,
						0x0C,0x0C,0x4D,0x0C,0x0C,
						0x18,0x4D,0x0C,0x0C,0x24,
						0x4D,0x0C,0x0C,0x30,0x4D,
						0x0A,0x0C,0x30,0x4B,0x0A,
						0xF4,0x00,0x59,0x0C,0x0B,
						0x00,0x59,0xF4,0x0B,0x0D,
						0x59,0x0E,0x0B,0x1C,0x59,
						0x0E,0x0B,0x2B,0x59,0x0E,
						0x0B,0x01,0x64,0x0E,0x0B,
						0x1A,0x64,0x09,0x0B,0x24,
						0x64,0x0C,0x0B  };

char charMap[92][3] = { 0x00,0xFF,0xFF,0x01,0xFF,0xFF,0x02,0xFF,0xFF,0x03,0xFF,0xFF,0x04,0xFF,0xFF,0x05,0xFF,
						0xFF,0x06,0xFF,0xFF,0x07,0xFF,0xFF,0x08,0xFF,0xFF,0x09,0xFF,0xFF,0x0A,0xFF,0xFF,0x0B,
						0xFF,0xFF,0x0C,0xFF,0xFF,0x0D,0xFF,0xFF,0x0E,0xFF,0xFF,0x0F,0xFF,0xFF,0x39,0xFF,0xFF,
						0x10,0xFF,0xFF,0x11,0xFF,0xFF,0x12,0xFF,0xFF,0x13,0xFF,0xFF,0x14,0xFF,0xFF,0x15,0xFF,
						0xFF,0x16,0xFF,0xFF,0x17,0xFF,0xFF,0x18,0xFF,0xFF,0x0E,0xFF,0xFF,0x08,0xFF,0xFF,0x19,
						0xFF,0xFF,0x1A,0xFF,0xFF,0x1B,0xFF,0xFF,0x1C,0xFF,0xFF,0x1D,0xFF,0xFF,0x1E,0xFF,0xFF,
						0x1F,0xFF,0xFF,0x20,0xFF,0xFF,0x21,0xFF,0xFF,0x22,0xFF,0xFF,0x23,0xFF,0xFF,0x24,0xFF,
						0xFF,0x25,0xFF,0xFF,0x26,0xFF,0xFF,0x27,0xFF,0xFF,0x28,0xFF,0xFF,0x29,0xFF,0xFF,0x2A,
						0xFF,0xFF,0x2B,0xFF,0xFF,0x2C,0xFF,0xFF,0x2D,0xFF,0xFF,0x2E,0xFF,0xFF,0x2F,0xFF,0xFF,
						0xFF,0xFF,0x2F,0x02,0x2F,0xFF,0x13,0xFF,0x33,0x04,0xFF,0x35,0x00,0xFF,0x36,0x00,0xFF,
						0x33,0x00,0xFF,0x34,0x30,0xFF,0xFF,0x04,0xFF,0x36,0x04,0xFF,0x33,0x04,0xFF,0x34,0x08,
						0xFF,0x33,0x08,0xFF,0x36,0x08,0xFF,0x34,0x0E,0xFF,0x36,0x0E,0xFF,0x33,0x0E,0xFF,0x34,
						0x13,0xFF,0x36,0x13,0xFF,0x34,0x17,0xFF,0x33,0x00,0xFF,0x35,0x08,0xFF,0x35,0x0E,0xFF,
						0x35,0x13,0xFF,0x35,0x0D,0xFF,0x37,0x31,0xFF,0xFF,0x32,0xFF,0xFF,0x3A,0xFF,0xFF,0x3B,
						0xFF,0xFF,0x3C,0xFF,0xFF,0x3D,0xFF,0xFF,0x3E,0xFF,0xFF,0x3F,0xFF,0xFF,0x40,0xFF,0xFF,
						0x41,0xFF,0xFF,0x42,0xFF,0xFF,0x43,0xFF,0xFF,0x44,0xFF,0xFF,0x45,0xFF,0xFF,0x46,0xFF,
						0xFF,0x47,0xFF,0xFF };


void FONT_MakeSpecialFogClut(int x, int y)
{ 
#if defined(PSX_VERSION)
	int n;
	unsigned short cl[16];
	PSX_RECT myrect;

	for (n = 0; n < 15; n++)
	{
		cl[n] = 0x4210;
	}

	cl[15] = 0;

	myrect.w = 16;
	myrect.h = 1;
	myrect.x = x;
	myrect.y = y;
	
	SpecialFogClut = getClut(x, y);

	DrawSync(0);

	LoadImage(&myrect, (unsigned long*)&cl);
	
	DrawSync(0);
#else
	PSX_RECT rect; // [esp+4h] [ebp-28h] BYREF
	static WORD pal[16] =
	{
		0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 
		0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0x4210, 0
	}; // [esp+Ch] [ebp-20h] BYREF

	rect.x = x;
	rect.y = y;
	rect.w = 16;
	rect.h = 1;
	font_clut = getClut(x, y);
	DrawSync(0);
	LoadImage(&rect, (u_long*)pal);
	DrawSync(0);
#endif
}


void FONT_Init()
{
#ifdef PSX_VERSION

	unsigned long* timAddr;
	short x;
	short y;

	FONT_vramBlock = VRAM_CheckVramSlot(&x, &y, 16, 128, 3, -1);

	if (FONT_vramBlock != NULL)
	{
		timAddr = (unsigned long*)LOAD_ReadFile("\\kain2\\game\\font.tim", 5);

		LOAD_LoadTIM((long*)timAddr, x, y, x, (SCREEN_HEIGHT / 2) + 6);

		MEMPACK_Free((char*)timAddr);

		fontTracker.sprite_sort_push = 0;
		fontTracker.font_vramX = x;
		fontTracker.font_vramY = y;
		fontTracker.font_tpage = ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
		fontTracker.font_clut = ((y + 126) << 6) | ((x >> 4) & 0x3F);
		fontTracker.font_vramU = ((x & 0x3F) << 2);
		fontTracker.font_vramV = (y & 0xFF);

		FONT_MakeSpecialFogClut(x, y & 0x7F);
	}

	fontTracker.font_xpos = 10;
	fontTracker.font_ypos = 16;
	fontTracker.font_buffIndex = 0;
	fontTracker.sprite_sort_push = 0;
	fontTracker.color_global = 0;
	fontTracker.color_local = 0;

#else
	fontTracker.font_xpos = 10;
	fontTracker.font_ypos = 16;
	fontTracker.font_buffIndex = 0;
	fontTracker.sprite_sort_push = 0;
	fontTracker.color_global = 0;
	fontTracker.color_local = 0;
#endif
}

void FONT_ReloadFont()
{
	unsigned long* timAddr;
	timAddr = (unsigned long*)LOAD_ReadFile("\\kain2\\game\\font.tim", 5);
	LOAD_LoadTIM((long*)timAddr, fontTracker.font_vramX, fontTracker.font_vramY, fontTracker.font_vramX, fontTracker.font_vramY + 126);
	MEMPACK_Free((char*)timAddr);
	FONT_MakeSpecialFogClut(fontTracker.font_vramX, fontTracker.font_vramY + 127);
}

void FONT_DrawChar(struct FontChar *fontChar)
{
#if defined(PSX_VERSION)
	char c;
	long x;
	long y;
	
	c = fontChar->c;
	x = fontChar->x;
	y = fontChar->y;

	fontTracker.color_local = fontChar->color;

	FONT_DrawChar2D(c, x, y);

#elif defined(PC_VERSION)
	int y; // edx
	int x; // [esp-8h] [ebp-8h]
	char c; // [esp+4h] [ebp+4h]

	y = fontChar->y;
	c = fontChar->c;
	x = fontChar->x;
	fontTracker.color_local = fontChar->color;
	FONT_DrawChar2D(c, x, y);
#endif
}

long FONT_Get2DImageIndex(unsigned char c)
{
#if defined(PSX_VERSION)
	return fontTransTable[c];
#elif defined(PC_VERSION)
	int v2; // eax

	if (c >= 'a' && c <= 'z')
		return c - 97;
	if (c >= '0' && c <= '9')
		return c - 22;
	if (c >= 'A' && c <= 'Z')
		return c - 13;
	switch (c)
	{
	case '!':
		return 37;
	case '/':
		return 38;
	case '-':
		return 39;
	case '"':
		return 40;
	case '%':
		return 41;
	case ':':
		return 42;
	case '(':
		return 43;
	case ')':
		return 44;
	case '+':
		return 45;
	case '=':
		return 46;
	case '*':
		return 47;
	case '?':
		return 48;
	case '~':
		return 49;
	case ',':
		return 50;
	case '\'':
		return 51;
	case ';':
		return 42;
	case '.':
		return 36;
	case '\xC8':
		return 78;
	case '\xC9':
		return 79;
	case '\xCA':
		return 80;
	case '\xCB':
		return 81;
	case '\xD0':
		return 82;
	case '\xD1':
		return 83;
	case '\xD2':
		return 84;
	case '\xD3':
		return 85;
	case '\xCC':
		return 86;
	case '\xCD':
		return 87;
	case '\xCE':
		return 88;
	case '\xCF':
		return 89;
	case '\xD4':
		return 90;
	case '\xD5':
		return 91;
	case '\xD6':
		return 96;
	case '\xD7':
		return 97;
	case '`':
		return 92;
	case '\\':
		return 93;
	case '[':
		return 94;
	}
	v2 = -(c != 93);
	v2 = v2 & 0xC5;
	return v2 + 95;
#endif
}

void drawChar2DPoly(long fpi, long x, long y)
{
#if defined(PSX_VERSION)

	unsigned long** drawOT; // $s1
	POLY_FT4* textPoly; // $a3
	long xl; // $t8
	long xr; // $s0
	long yt; // $a0
	long yb; // $a2
	long w; // $a0
	long h; // $v0
	int u; // $v1
	int v; // $a0
	int u0; // $t3
	int v0; // $t2
	int u1; // $t5
	int v1; // $t1
	int u2; // $t7
	int v2; // $t0
	int u3; // $t6
	int v3; // $t4
	int holdu; // $v0
	int holdv; // $v1
	struct font_color_t* color; // $v0

	u = ABS(fontPos[fpi].x);
	xl = x;
	v = ABS(fontPos[fpi].y);
	u0 = u + fontTracker.font_vramU;
	u2 = u0;
	drawOT = gameTrackerX.drawOT;
	v0 = v + fontTracker.font_vramV;
	w = ABS(fontPos[fpi].w);

	u1 = u0 + w;
	v1 = v0;
	h = ABS(fontPos[fpi].h);

	v2 = v1 + h;
	u3 = u1;
	v3 = v2;

	xr = xl + w;
	yt = y - (h - 12);
	yb = y + 12;

	textPoly = (POLY_FT4*)gameTrackerX.primPool->nextPrim;

	if (fontPos[fpi].h < 0)
	{
		v0 = v3;
		v2 = v1;
		v1 = v0;
		v3 = v2;
	}

	if (fontPos[fpi].w < 0)
	{
		holdu = u0;
		holdv = v0;
		
		u0 = u1;
		v0 = v1;
		
		u1 = holdu;
		v1 = holdv;

		holdu = u3;
		holdv = v3;

		u3 = u2;
		v3 = v2;

		u2 = holdu;
		v2 = holdv;
	}

	setPolyFT4(textPoly);

	if (fontTracker.color_local == 0)
	{
		textPoly->code = 0x2D;
	}
	else
	{
		textPoly->code = 0x2C;
		color = &the_font_color_table[fontTracker.color_local];

		setRGB0(textPoly, color->r, color->g, color->b);
	}
	
	textPoly->u0 = u0;
	textPoly->v0 = v0;
	textPoly->u1 = u1;
	textPoly->v1 = v1;
	textPoly->u2 = u2;
	textPoly->v2 = v2;
	textPoly->u3 = u3;
	textPoly->v3 = v3;

	textPoly->x0 = xl;
	textPoly->y0 = yt;
	textPoly->x1 = xr;
	textPoly->y1 = yt;
	textPoly->x2 = xl;
	textPoly->y2 = yb;
	textPoly->x3 = xr;
	textPoly->y3 = yb;

	textPoly->code &= 0xFD;
	textPoly->tpage = fontTracker.font_tpage;
	textPoly->clut = fontTracker.font_clut;
	
	addPrim(drawOT, textPoly);

	gameTrackerX.primPool->nextPrim = (unsigned long*)(textPoly + 1);

#elif defined(PC_VERSION)
	__int16 v3; // ax
	int v4; // ecx
	__int16 v5; // ax
	int v6; // edx
	__int16 w; // ax
	int v8; // ebp
	__int16 h; // ax
	int v10; // edi
	char v11; // cl
	char v12; // dl
	__int16 v13; // si
	__int16 v14; // ax
	__int16 v15; // si
	u_char v16; // di
	u_char v17; // cl
	POLY_FT4* nextPrim; // eax
	u_char v19; // dl
	u_char v20; // bp
	u_char v21; // di
	u_char v22; // bp
	u_char v23; // di
	u_char v24; // bp
	u_long tag; // edi
	char color_local; // bl
	char v27; // bl
	u_char v28; // [esp+10h] [ebp-20h]
	u_char v29; // [esp+14h] [ebp-1Ch]
	u_char v30; // [esp+18h] [ebp-18h]
	u_char v31; // [esp+1Ch] [ebp-14h]
	__int16 v32; // [esp+20h] [ebp-10h]
	__int16 v33; // [esp+24h] [ebp-Ch]
	__int16 v34; // [esp+28h] [ebp-8h]
	__int16 v35; // [esp+2Ch] [ebp-4h]
	u_char fpia; // [esp+34h] [ebp+4h]
	u_char ya; // [esp+3Ch] [ebp+Ch]

	v3 = fontPos[fpi].x;
	v4 = v3;
	if (v3 < 0)
		v4 = -v3;
	v5 = fontPos[fpi].y;
	v6 = v5;
	if (v5 < 0)
		v6 = -v5;
	w = fontPos[fpi].w;
	v33 = w;
	v8 = w;
	if (w < 0)
		v8 = -w;
	h = fontPos[fpi].h;
	v32 = h;
	v10 = h;
	if (h < 0)
		v10 = -h;
	v11 = fontTracker.font_vramU + v4;
	v12 = fontTracker.font_vramV + v6;
	v34 = v8 + x;
	v13 = y - v10;
	v14 = y + 12;
	ya = v12 + v10;
	v15 = v13 + 12;
	v16 = v11 + v8;
	v35 = v14;
	v17 = v11 + 1;
	fpia = v16;
	nextPrim = (POLY_FT4*)gameTrackerX.primPool->nextPrim;
	v29 = v16;
	v19 = v12 + 1;
	v30 = ya;
	v28 = v19;
	v31 = v17;
	if (v32 < 0)
	{
		v31 = v17;
		fpia = v16;
		v20 = v19;
		v19 = ya;
		ya = v20;
		v28 = v30;
		v29 = v16;
		v30 = v20;
	}
	if (v33 < 0)
	{
		v21 = v17;
		v17 = fpia;
		v22 = v19;
		v19 = v28;
		fpia = v21;
		v23 = v29;
		v28 = v22;
		v24 = v30;
		v29 = v31;
		v31 = v23;
		v30 = ya;
		ya = v24;
	}
	tag = nextPrim->tag;
	*(&nextPrim->b0 + 1) = 44;
	nextPrim->tag = tag & 0xFFFFFF | 0x9000000;
	color_local = fontTracker.color_local;
	if (fontTracker.color_local)
	{
		*(&nextPrim->b0 + 1) = 0x2C;
		nextPrim->r0 = the_font_color_table[color_local].r;
		nextPrim->g0 = the_font_color_table[color_local].g;
		nextPrim->b0 = the_font_color_table[color_local].b;
	}
	else
	{
		*(&nextPrim->b0 + 1) = 0x2D;
	}
	nextPrim->u0 = v17;
	v27 = *(&nextPrim->b0 + 1);
	nextPrim->v0 = v19;
	nextPrim->u1 = fpia;
	nextPrim->v1 = v28;
	nextPrim->u2 = v31;
	nextPrim->v2 = ya;
	nextPrim->u3 = v29;
	nextPrim->v3 = v30;
	nextPrim->x0 = x;
	nextPrim->x2 = x;
	nextPrim->y0 = v15;
	*(&nextPrim->b0 + 1) = v27 & 0xFD;
	nextPrim->x1 = v34;
	nextPrim->y1 = v15;
	nextPrim->y2 = v35;
	nextPrim->x3 = v34;
	nextPrim->y3 = v35;
	nextPrim->tpage = fontTracker.font_tpage;
	nextPrim->clut = fontTracker.font_clut;
	if (!fontTracker.color_local)
	{
		nextPrim->r0 = 0x80;
		nextPrim->g0 = 0x80;
		nextPrim->b0 = 0x80;
	}
	//D3D_RenderLetter(nextPrim);
#endif
}

void FONT_DrawChar2D(unsigned char c, long x, long y)
{
#if defined(PSX_VERSION)
	long w;
	long h;
	long w1;
	long w2;
	long w3;
	long xoff;
	long yoff;
	int i1;
	int i2;
	int i3;
	long index;

	if (gameTrackerX.primPool->lastPrim - 12 >= gameTrackerX.primPool->nextPrim)
	{
		index = FONT_Get2DImageIndex(c);

		i1 = charMap[index][0];
		i2 = charMap[index][1];
		i3 = charMap[index][2];

		if (i1 >= 0)
		{
			w1 = ABS(fontPos[i1].w);

		}
		else
		{
			w1 = 8;
		}

		if (i2 >= 0)
		{
			w2 = ABS(fontPos[i2].w);
		}
		else
		{
			w2 = w1;
		}

		if (i3 >= 0)
		{
			w3 = ABS(fontPos[i3].w);
		}
		else
		{
			w3 = w1;
		}

		if (w2 < w1)
		{
			if (w2 < w1)
			{
				w = w1;
			}
			else
			{
				w = w2;
			}
		}
		else 
		{
			if (w3 < w2)
			{
				w = w2;
			}
			else
			{
				w = w3;
			}
		}

		if (i1 >= 0)
		{
			h = ABS(fontPos[i1].h);

		}
		else
		{
			h = 12;
		}

		if (i1 >= 0)
		{
			xoff = (((w - w1) + ((w - w1) >> 31)) >> 1) + x;
			yoff = y;

			drawChar2DPoly(i1, xoff, yoff);
		}

		xoff = x + (((w - w2) >> 31) + ((w - w2)) >> 1);
		yoff = y;

		if (c == 'A')
		{
			yoff = y + 2;
		}

		if (c == '\'')
		{
			y += 3;
		}

		if (i2 >= 0)
		{
			drawChar2DPoly(i2, xoff, yoff);
		}

		if (i3 >= 0)
		{
			xoff = x + (((w - w3) >> 31) + (w - w3) >> 1);
			yoff = (y + 1) - h;
			drawChar2DPoly(i3, xoff, yoff);
		}
	}

#elif defined(PC_VERSION)
	int v3; // eax
	int v4; // ecx
	int v5; // esi
	int v6; // ebx
	__int16 w; // ax
	int v8; // edx
	__int16 v9; // ax
	int v10; // edi
	__int16 v11; // ax
	int v12; // ebx
	int h; // eax
	int v14; // esi
	int v15; // ecx
	int v16; // [esp+10h] [ebp-10h]
	int v17; // [esp+14h] [ebp-Ch]
	int fpi; // [esp+18h] [ebp-8h]
	int v19; // [esp+1Ch] [ebp-4h]

	if (gameTrackerX.primPool->nextPrim > gameTrackerX.primPool->lastPrim - 12)
		return;
	v3 = FONT_Get2DImageIndex(c);
	v4 = charMap[v3][0];
	v5 = charMap[v3][1];
	v6 = charMap[v3][2];
	fpi = v5;
	v19 = v6;
	if (v4 < 0)
	{
		v8 = 8;
	}
	else
	{
		w = fontPos[v4].w;
		v8 = w;
		if (w < 0)
			v8 = -w;
	}
	if (v5 < 0)
	{
		v16 = v8;
	}
	else
	{
		v9 = fontPos[v5].w;
		if (v9 >= 0)
		{
			v10 = v9;
			v16 = v9;
			goto LABEL_12;
		}
		v16 = -v9;
	}
	v10 = v16;
LABEL_12:
	if (v6 < 0)
	{
		v12 = v8;
	}
	else
	{
		v11 = fontPos[v6].w;
		v12 = v11;
		if (v11 < 0)
			v12 = -v11;
	}
	if (v8 <= v10)
	{
		if (v10 > v12)
			goto LABEL_21;
	LABEL_20:
		v10 = v12;
		goto LABEL_21;
	}
	if (v8 <= v12)
		goto LABEL_20;
	v10 = v8;
LABEL_21:
	if (v4 < 0)
	{
		v17 = 12;
	}
	else
	{
		h = fontPos[v4].h;
		if ((h & 0x8000u) != 0)
			h = -(__int16)h;
		v17 = h;
	}
	v14 = y;
	if (v4 >= 0)
		drawChar2DPoly(v4, x + (v10 - v8) / 2, y);
	v15 = y + 2;
	if (c != 65)
		v15 = y;
	if (c == 39)
		v14 = y + 3;
	if (fpi >= 0)
		drawChar2DPoly(fpi, x + (v10 - v16) / 2, v15);
	if (v19 >= 0)
		drawChar2DPoly(v19, x + (v10 - v12) / 2, v14 - v17 + 1);
#endif
}

long FONT_CharSpacing(char c, long fontXSize)
{
#if defined(PSX_VERSION)
	long index;
	long w;
	long w1;

#if defined(PSXPC_VERSION)
	w1 = 0;//UWP compile hack
#endif

	long w2;
	long w3;
	char i1;
	char i2;
	char i3;
	int holdw;
	int holdw2;

	if (c != ' ')
	{
		index = FONT_Get2DImageIndex(c);

		if (index != -1)
		{
			i1 = charMap[index][0];
			i2 = charMap[index][1];
			i3 = charMap[index][2];
			
			holdw = ABS(fontPos[i1].w);

			if (fontPos[i1].x < 0)
			{
				holdw = fontPos[i1].h;
			}
			
			if (i1 >= 0)
			{
				w = holdw;
			}
			else
			{
				w = 8;
			}
			
			holdw = ABS(fontPos[i2].w);
			
			if (i2 >= 0)
			{
				w2 = holdw;
			}
			else
			{
				w2 = w;
			}
			
			holdw = ABS(fontPos[i3].w);
	
			if (i3 >= 0)
			{
				w3 = holdw;
			}
			else
			{
				w3 = w;
			}

			if (w2 >= w3)
			{
				if (w >= w2)
				{
					return w + 1;
				}
				else
				{
					return w2 + 1;
				}
			}
			else if (w >= w1)
			{
				return w + 1;
			}
			else
			{
				return w1 + 1;
			}
		}
	}

	return fontXSize;

#elif defined(PC_VERSION)
	int v2; // eax
	int v3; // eax
	char v4; // dl
	char v5; // bl
	int w; // ecx
	int v7; // esi
	int v8; // eax
	int v9; // edx
	int v10; // eax
	char fontXSizea; // [esp+10h] [ebp+8h]

	if (c == 32)
		return fontXSize;
	v2 = FONT_Get2DImageIndex(c);
	if (v2 == -1)
		return fontXSize;
	v3 = v2;
	v4 = charMap[v3][0];
	v5 = charMap[v3][1];
	fontXSizea = charMap[v3][2];
	w = fontPos[v4].w;
	if ((w & 0x8000u) != 0)
		w = -(__int16)w;
	if (fontPos[v4].x < 0)
		w = fontPos[v4].h;
	v7 = w;
	if (v4 < 0)
		v7 = 8;
	v8 = fontPos[v5].w;
	if ((v8 & 0x8000u) != 0)
		v8 = -(__int16)v8;
	v9 = v8;
	if (v5 < 0)
		v9 = v7;
	v10 = fontPos[fontXSizea].w;
	if ((v10 & 0x8000u) != 0)
		v10 = -(__int16)v10;
	if (fontXSizea < 0)
		v10 = v7;
	if (v7 <= v9)
	{
		if (v9 > v10)
			v10 = v9;
	}
	else if (v7 > v10)
	{
		return v7 + 1;
	}
	return v10 + 1;
#endif
}

void FONT_AddCharToBuffer(char c, long x, long y)
{
#if defined(PSX_VERSION)
	struct FontChar* fontChar;
	
	fontChar = &fontTracker.font_buffer[fontTracker.font_buffIndex];
	
	if (fontTracker.font_buffIndex < 255)
	{
		if (c == '@')
		{
			fontChar->x = x & 0xFF;
			fontChar->y = y & 0xFF;
		}
		else
		{
			fontChar->x = x;
			fontChar->y = y;
		}

		fontChar->c = c;

		fontTracker.font_buffIndex++;

		fontChar->color = fontTracker.color_global;
	}

#elif defined(PC_VERSION)
	FontChar* v3; // eax
	__int16 v4; // dx

	v3 = &fontTracker.font_buffer[fontTracker.font_buffIndex];
	if (fontTracker.font_buffIndex < 2047)
	{
		if (c == '@')
		{
			v3->x = (unsigned __int8)x;
			v4 = (unsigned __int8)y;
		}
		else
		{
			v3->x = x;
			v4 = y;
		}
		v3->y = v4;
		v3->c = c;
		++fontTracker.font_buffIndex;
		v3->color = fontTracker.color_global;
	}
#endif
}

char byte_C549E0[512];

void FONT_Print(const char *fmt, ...)
{
#ifdef PSX_VERSION
	//-8+arg_0 = a0
	//-8+arg_4 = a1
	//-8+arg_8 = a2
	//-8+arg_c = a3

	//a1 = fmt
	//0x10+arg_0 = a0
	//a0 = &fp_str[0];
	//s0 = 0x10+arg_4
#else

	char* v1; // ecx
	char v2; // al
	va_list ap; // [esp+8h] [ebp+8h] BYREF

	va_start(ap, fmt);
	vsprintf_s(byte_C549E0, sizeof(byte_C549E0), fmt, ap);
	va_end(ap);

	v1 = byte_C549E0;
	if (byte_C549E0)
	{
		do
		{
			v2 = *v1;
			if (*v1 >= 'A' && v2 <= 'Z')
				*v1 = v2 + ' ';
		} while (*++v1);
	}
	FONT_VaReallyPrint(byte_C549E0, ap);
#endif
}

void FONT_Print2(const char *fmt, ...)
{
#if defined(PSX_VERSION)
	va_list ap;
	va_start(ap, fmt);
	vsprintf(fp_str, fmt, ap);
	FONT_VaReallyPrint(fmt, ap);
#elif defined(PC_VERSION)
	va_list ap; // [esp+8h] [ebp+8h] BYREF

	va_start(ap, fmt);
	vsprintf_s(byte_C549E0, sizeof(byte_C549E0), fmt, ap);
	va_end(ap);

	FONT_VaReallyPrint(byte_C549E0, ap);
#endif
}

int FONT_GetStringWidth(char *str)
{ 
#if defined(PSX_VERSION)
	int w;
	int len;
	int i;

	i = 0;
	w = 0;
	len = strlen(str);

	if (len > 0)
	{
		for (i = 0; i < len; i++)
		{
			w += FONT_CharSpacing(str[i], 8);
		}
	}

	return w;
#elif defined(PC_VERSION)
	unsigned int v1; // kr04_4
	signed int v2; // esi
	int i; // ebx

	v1 = strlen(str) + 1;
	v2 = 0;
	for (i = 0; v2 < (int)(v1 - 1); ++v2)
		i += FONT_CharSpacing(str[v2], 8);
	return i;
#endif
}

void FONT_Flush()
{
#if defined(PSX_VERSION)

	long i;
	struct FontChar* fontChar;

	fontTracker.font_xpos = 10;
	fontTracker.font_ypos = 16;

	for (i = 0; i < fontTracker.font_buffIndex; i++)
	{
		fontChar = &fontTracker.font_buffer[i];

		if (fontChar->c != ' ' && fontChar->c != '@')
		{
			FONT_DrawChar(fontChar);
		}
	}

	fontTracker.font_buffIndex = 0;

#else//PC
	FontChar* p_c; // esi
	__int32 font_buffIndex; // edi
	//int y; // ecx
	//int x; // [esp-10h] [ebp-14h]
	//char c; // [esp+0h] [ebp-4h]

	fontTracker.font_xpos = 10;
	fontTracker.font_ypos = 16;
	if (fontTracker.font_buffIndex)
	{
		p_c = &fontTracker.font_buffer[0];
		font_buffIndex = fontTracker.font_buffIndex;
		do
		{
			if (p_c->c != ' ' && p_c->c != '@')
			{
				fontTracker.color_local = p_c->color;
				FONT_DrawChar2D(p_c->c, p_c->x, p_c->y);
			}
			++p_c;
			--font_buffIndex;
		} while (font_buffIndex);
		fontTracker.font_buffIndex = 0;
	}
#endif
}

void FONT_SetCursor(short x, short y)
{
#if defined(PSX_VERSION)
	fontTracker.font_xpos = x;
	fontTracker.font_ypos = y;
#elif defined(PC_VERSION)
	if (x > 0)
		fontTracker.font_xpos = x;
	if (y > 0)
		fontTracker.font_ypos = y;
#endif
}

void FONT_VaReallyPrint(const char *fmt, void *ap)
{
#if defined(PSX_VERSION)
	char* p;
	long* xpos;
	long* ypos;
	unsigned char w;
	unsigned char h;

	p = (char*)fmt;
	xpos = &fontTracker.font_xpos;
	ypos = &fontTracker.font_ypos;

	while (p[0] != 0)
	{
		if (p[0] == '\xA')
		{
			xpos[0] = p[0];
			ypos[0] += 12;
		}
		else if (p[0] == '@')
		{
			h = p[1];
			w = p[2];
			p += 2;

			FONT_AddCharToBuffer('@', xpos[0], ypos[0]);
			h -= 64;
			w -= 64;

			FONT_AddCharToBuffer('@', w, h);
			p += 2;
		}
		else if (p[0] == '$')
		{
			xpos[0] = 10;
			ypos[0] = 16;
		}
		else if (p[0] == '\xD')
		{
			xpos[0] = 10;
		}
		else if (p[0] == '\x9')
		{
			xpos[0] += 32;
		}
		else if (p[0] == ' ')
		{
			xpos[0] += 8;
		}
		else if (p[0] != '\x5F')
		{
			FONT_AddCharToBuffer(p[0], xpos[0], ypos[0]);
			xpos[0] += FONT_CharSpacing(p[0], 8);
		}

		p++;
	}

#elif defined(PC_VERSION)
	const char* v2; // edi
	__int32 font_buffIndex; // esi
	char v4; // dl
	__int16 font_xpos_low; // dx
	__int32 v6; // eax
	__int16 font_ypos; // cx
	int v8; // eax
	unsigned __int8 v10; // [esp+4h] [ebp-4h]
	unsigned __int8 fmta; // [esp+Ch] [ebp+4h]

	v2 = fmt;
	if (*fmt)
	{
		font_buffIndex = fontTracker.font_buffIndex;
		do
		{
			v4 = *v2;
			if (*v2 == 10)
			{
				fontTracker.font_xpos = 10;
				fontTracker.font_ypos += 12;
			}
			else if (v4 == 64)
			{
				fmta = v2[1] - 64;
				v10 = v2[2] - 64;
				if (font_buffIndex < 2047)
				{
					font_xpos_low = LOBYTE(fontTracker.font_xpos);
					fontTracker.font_buffer[font_buffIndex].y = LOBYTE(fontTracker.font_ypos);
					fontTracker.font_buffer[font_buffIndex].x = font_xpos_low;
					fontTracker.font_buffer[font_buffIndex].c = 64;
					++fontTracker.font_buffIndex;
					fontTracker.font_buffer[font_buffIndex].color = fontTracker.color_global;
					font_buffIndex = fontTracker.font_buffIndex;
				}
				if (font_buffIndex < 2047)
				{
					fontTracker.font_buffer[font_buffIndex].x = v10;
					fontTracker.font_buffer[font_buffIndex].y = fmta;
					fontTracker.font_buffer[font_buffIndex].c = 64;
					++fontTracker.font_buffIndex;
					fontTracker.font_buffer[font_buffIndex].color = fontTracker.color_global;
					font_buffIndex = fontTracker.font_buffIndex;
				}
				v2 += 2;
			}
			else if (v4 == 36)
			{
				fontTracker.font_xpos = 10;
				fontTracker.font_ypos = 16;
			}
			else if (v4 == 13)
			{
				fontTracker.font_xpos = 10;
			}
			else
			{
				if (v4 == 9)
				{
					v6 = fontTracker.font_xpos + 32;
				}
				else
				{
					if (v4 != 32 && v4 != 95)
					{
						font_ypos = fontTracker.font_ypos;
						if (font_buffIndex < 2047)
						{
							fontTracker.font_buffer[font_buffIndex].x = fontTracker.font_xpos;
							fontTracker.font_buffer[font_buffIndex].y = font_ypos;
							fontTracker.font_buffer[font_buffIndex].c = v4;
							++fontTracker.font_buffIndex;
							fontTracker.font_buffer[font_buffIndex].color = fontTracker.color_global;
						}
						v8 = FONT_CharSpacing(*v2, 8);
						font_buffIndex = fontTracker.font_buffIndex;
						fontTracker.font_xpos += v8;
						continue;
					}
					v6 = fontTracker.font_xpos + 8;
				}
				fontTracker.font_xpos = v6;
			}
		} while (*++v2);
	}
#endif
}

void FONT_FontPrintCentered(char *text, long y)
{
#if defined(PSX_VERSION)
	FONT_SetCursor(0x100 - (FONT_GetStringWidth(text) >> 1), y);
	FONT_Print2(text);
#elif defined(PC_VERSION)
	unsigned int v2; // kr04_4
	signed int v3; // esi
	int i; // edi
	__int16 v5; // ax

	v2 = strlen(text) + 1;
	v3 = 0;
	for (i = 0; v3 < (int)(v2 - 1); ++v3)
		i += FONT_CharSpacing(text[v3], 8);
	v5 = 256 - (i >> 1);
	if (v5 > 0)
		fontTracker.font_xpos = v5;
	if ((__int16)y > 0)
		fontTracker.font_ypos = (__int16)y;
	FONT_Print2(text);
#endif
}

void FONT_SetColorIndex(int color)
{
	fontTracker.color_global = color;
}

void FONT_SetColorIndexCol(int color, int r, int g, int b)
{
	struct font_color_t* fcol;

	fcol = &the_font_color_table[color];
	fcol->r = r;
	fcol->g = g;
	fcol->b = b;

#if defined(PC_VERSION)
	font_color_t* v4; // eax

	v4 = &the_font_color_table[color];
	v4->r = r;
	v4->g = g;
	v4->b = b;
#endif
}
