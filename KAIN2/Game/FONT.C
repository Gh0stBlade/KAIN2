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
//static WORD font_clut; //Probably PC stuff
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

font_color_t the_font_color_table[5] =
{
	{ 0x00, 0x00, 0x00 },
	{ 0x40, 0x40, 0x40 },
	{ 0x40, 0x40, 0xff },
	{ 0x58, 0x58, 0x68 },
	{ 0xc0, 0xd2, 0xd2 }
};

FontPos fontPos[] = {
{ 0x00,0x00,0x0b,0x0c },
{ 0x0b,0x00,0x08,0x0c },
{ 0x14,0x00,0x08,0x0c },
{ 0x1c,0x00,0x09,0x0c },
{ 0x25,0x00,0x08,0x0c },
{ 0x2d,0x00,0x08,0x0c },
{ 0x35,0x00,0x0a,0x0c },
{ 0x00,0x0c,0x09,0x0c },
{ 0x09,0x0c,0x03,0x0c },
{ 0x2a,0x0c,0x05,0x0c },
{ 0x2f,0x0d,0x08,0x0b },
{ 0x01,0x6f,0x07,0x0c },
{ 0x00,0x1a,0x0a,0x0c },
{ 0x0b,0x1a,0x08,0x0c },
{ 0x13,0x1a,0x0a,0x0c },
{ 0x1d,0x1a,0x07,0x0c },
{ 0x2e,0x1a,0x08,0x0c },
{ 0x00,0x27,0x09,0x0c },
{ 0x0a,0x27,0x09,0x0c },
{ 0x13,0x27,0x08,0x0b },
{ 0x1b,0x27,0x09,0x0b },
{ 0x25,0x27,0x0a,0x0c },
{ 0x2f,0x27,0x08,0x0c },
{ 0x01,0x33,0x09,0x0d },
{ 0x0a,0x33,0x08,0x0d },
{ 0x12,0x33,0x08,0x0d },
{ 0x1b,0x33,0x07,0x0d },
{ 0x23,0x33,0x08,0x0d },
{ 0x2c,0x33,0x07,0x0d },
{ 0x34,0x33,0x07,0x0c },
{ 0x0c,0x0c,0x08,0x0c },
{ 0x14,0x0c,0x07,0x0c },
{ 0x21,0x0c,0x07,0x0c },
{ 0x3d,0x20,0x03,0x03 },
{ 0x3d,0x33,0x02,0x0d },
{ 0x36,0x19,0x07,0x0c },
{ 0x30,0x05,0x05,0x07 },
{ 0x1b,0x0c,0x05,0x0c },
{ 0x39,0x27,0x06,0x0b },
{ 0x3d,0x1a,0x03,0x0a },
{ 0x13,0x1a,0x05,0x0c },
{ 0x18,0x1a,0x05,0x0b },
{ 0x10,0x64,0x07,0x0a },
{ 0x31,0x66,0x08,0x07 },
{ 0x31,0x25,0x05,0x0c },
{ 0x20,0x40,0x07,0x0c },
{ 0x24,0x19,0x09,0x05 },
{ 0x14,0x46,0x05,0x04 },
{ 0x00,0x40,0x08,0x0a },
{ 0x28,0x40,0x07,0x0c },
{ 0x3d,0x33,0x02,(char)0xf3 },
{ 0x25,0x1e,0x08,0x02 },
{ 0x12,0x40,0x07,0x04 },
{ 0x0a,0x40,0x06,0x05 },
{ 0x0a,0x46,0x06,0x05 },
{ 0x24,0x19,0x08,0x04 },
{ 0x1b,0x47,0x05,0x04 },
{ 0x2f,0x40,0x0b,0x0c },
{ 0x00,0x4d,0x0c,0x0c },
{ 0x0c,0x4d,0x0c,0x0c },
{ 0x18,0x4d,0x0c,0x0c },
{ 0x24,0x4d,0x0c,0x0c },
{ 0x30,0x4d,0x0a,0x0c },
{ 0x30,0x4b,0x0a,(char)0xf4 },
{ 0x00,0x59,0x0c,0x0b },
{ 0x00,0x59,(char)0xf4,0x0b },
{ 0x0d,0x59,0x0e,0x0b },
{ 0x1c,0x59,0x0e,0x0b },
{ 0x2b,0x59,0x0e,0x0b },
{ 0x01,0x64,0x0e,0x0b },
{ 0x1a,0x64,0x09,0x0b },
{ 0x24,0x64,0x0c,0x0b }

};

char charMap[92][3] = {
  { 0x00,(char)0xff,(char)0xff },
  { 0x01,(char)0xff,(char)0xff },
  { 0x02,(char)0xff,(char)0xff },
  { 0x03,(char)0xff,(char)0xff },
  { 0x04,(char)0xff,(char)0xff },
  { 0x05,(char)0xff,(char)0xff },
  { 0x06,(char)0xff,(char)0xff },
  { 0x07,(char)0xff,(char)0xff },
  { 0x08,(char)0xff,(char)0xff },
  { 0x09,(char)0xff,(char)0xff },
  { 0x0a,(char)0xff,(char)0xff },
  { 0x0b,(char)0xff,(char)0xff },
  { 0x0c,(char)0xff,(char)0xff },
  { 0x0d,(char)0xff,(char)0xff },
  { 0x0e,(char)0xff,(char)0xff },
  { 0x0f,(char)0xff,(char)0xff },
  { 0x39,(char)0xff,(char)0xff },
  { 0x10,(char)0xff,(char)0xff },
  { 0x11,(char)0xff,(char)0xff },
  { 0x12,(char)0xff,(char)0xff },
  { 0x13,(char)0xff,(char)0xff },
  { 0x14,(char)0xff,(char)0xff },
  { 0x15,(char)0xff,(char)0xff },
  { 0x16,(char)0xff,(char)0xff },
  { 0x17,(char)0xff,(char)0xff },
  { 0x18,(char)0xff,(char)0xff },
  { 0x0e,(char)0xff,(char)0xff },
  { 0x08,(char)0xff,(char)0xff },
  { 0x19,(char)0xff,(char)0xff },
  { 0x1a,(char)0xff,(char)0xff },
  { 0x1b,(char)0xff,(char)0xff },
  { 0x1c,(char)0xff,(char)0xff },
  { 0x1d,(char)0xff,(char)0xff },
  { 0x1e,(char)0xff,(char)0xff },
  { 0x1f,(char)0xff,(char)0xff },
  { 0x20,(char)0xff,(char)0xff },
  { 0x21,(char)0xff,(char)0xff },
  { 0x22,(char)0xff,(char)0xff },
  { 0x23,(char)0xff,(char)0xff },
  { 0x24,(char)0xff,(char)0xff },
  { 0x25,(char)0xff,(char)0xff },
  { 0x26,(char)0xff,(char)0xff },
  { 0x27,(char)0xff,(char)0xff },
  { 0x28,(char)0xff,(char)0xff },
  { 0x29,(char)0xff,(char)0xff },
  { 0x2a,(char)0xff,(char)0xff },
  { 0x2b,(char)0xff,(char)0xff },
  { 0x2c,(char)0xff,(char)0xff },
  { 0x2d,(char)0xff,(char)0xff },
  { 0x2e,(char)0xff,(char)0xff },
  { 0x2f,(char)0xff,(char)0xff },
  { (char)0xff,(char)0xff,0x2f },
  { 0x02,0x2f,(char)0xff },
  { 0x13,(char)0xff,0x33 },
  { 0x04,(char)0xff,0x35 },
  { 0x00,(char)0xff,0x36 },
  { 0x00,(char)0xff,0x33 },
  { 0x00,(char)0xff,0x34 },
  { 0x30,(char)0xff,(char)0xff },
  { 0x04,(char)0xff,0x36 },
  { 0x04,(char)0xff,0x33 },
  { 0x04,(char)0xff,0x34 },
  { 0x08,(char)0xff,0x33 },
  { 0x08,(char)0xff,0x36 },
  { 0x08,(char)0xff,0x34 },
  { 0x0e,(char)0xff,0x36 },
  { 0x0e,(char)0xff,0x33 },
  { 0x0e,(char)0xff,0x34 },
  { 0x13,(char)0xff,0x36 },
  { 0x13,(char)0xff,0x34 },
  { 0x17,(char)0xff,0x33 },
  { 0x00,(char)0xff,0x35 },
  { 0x08,(char)0xff,0x35 },
  { 0x0e,(char)0xff,0x35 },
  { 0x13,(char)0xff,0x35 },
  { 0x0d,(char)0xff,0x37 },
  { 0x31,(char)0xff,(char)0xff },
  { 0x32,(char)0xff,(char)0xff },
  { 0x3a,(char)0xff,(char)0xff },
  { 0x3b,(char)0xff,(char)0xff },
  { 0x3c,(char)0xff,(char)0xff },
  { 0x3d,(char)0xff,(char)0xff },
  { 0x3e,(char)0xff,(char)0xff },
  { 0x3f,(char)0xff,(char)0xff },
  { 0x40,(char)0xff,(char)0xff },
  { 0x41,(char)0xff,(char)0xff },
  { 0x42,(char)0xff,(char)0xff },
  { 0x43,(char)0xff,(char)0xff },
  { 0x44,(char)0xff,(char)0xff },
  { 0x45,(char)0xff,(char)0xff },
  { 0x46,(char)0xff,(char)0xff },
  { 0x47,(char)0xff,(char)0xff }
};

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

	LoadImage(&myrect, (unsigned int*)&cl);
	
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
	LoadImage(&rect, (unsigned int*)pal);
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

		LOAD_LoadTIM((int*)timAddr, x, y, x, (SCREEN_HEIGHT / 2) + 6);

		MEMPACK_Free((char*)timAddr);

		fontTracker.sprite_sort_push = 0;
		fontTracker.font_vramX = x;
		fontTracker.font_vramY = y;
		fontTracker.font_tpage = ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
		fontTracker.font_clut = ((y + 126) << 6) | ((x >> 4) & 0x3F);
		fontTracker.font_vramU = ((x & 0x3F) << 2);
		fontTracker.font_vramV = (y & 0xFF);

		FONT_MakeSpecialFogClut(x, y & 0x7F);

#if defined(PSXPC_VERSION)
		for (int i = 0; i < 10; i++)
		{
			short xc[4];
			short yc[4];
			short xOff = 0;

			switch (i)
			{
			case 0:
				xOff = 0;
				break;
			case 1:
				xOff = 11;
				break;
			case 2:
				xOff = 24;
				break;
			case 3:
				xOff = 35;
				break;
			}

			xc[0] = x + xOff;
			yc[0] = y + 77;

			xc[1] = x + xOff + 11;
			yc[1] = y + 77;

			xc[2] = x + xOff;
			yc[2] = y + 77 + 11;

			xc[3] = x + xOff + 11;
			yc[3] = y + 77 + 11;

			Emulator_HintTouchFontUIButton(fontTracker.font_tpage, fontTracker.font_clut, &xc[0], &yc[0], i);
		}
#endif
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
	LOAD_LoadTIM((int*)timAddr, fontTracker.font_vramX, fontTracker.font_vramY, fontTracker.font_vramX, fontTracker.font_vramY + 126);
	MEMPACK_Free((char*)timAddr);
	FONT_MakeSpecialFogClut(fontTracker.font_vramX, fontTracker.font_vramY + 127);
}

void FONT_DrawChar(struct FontChar *fontChar)
{
	char c;
	long x;
	long y;
	
	c = fontChar->c;
	x = fontChar->x;
	y = fontChar->y;

	fontTracker.color_local = fontChar->color;

	FONT_DrawChar2D(c, x, y);
}

long FONT_Get2DImageIndex(unsigned char c)
{
	return fontTransTable[c];
}

void drawChar2DPoly(long fpi, long x, long y)
{
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

	gameTrackerX.primPool->nextPrim = (unsigned int*)(textPoly + 1);
}

void FONT_DrawChar2D(unsigned char c, long x, long y)
{
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
}

long FONT_CharSpacing(char c, long fontXSize)
{
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
}

void FONT_AddCharToBuffer(char c, long x, long y)
{
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
}

char byte_C549E0[512];

void FONT_Print(const char *fmt, ...)
{
	char* hold;
	va_list ap;

	va_start(ap, fmt);
	vsprintf(fp_str, fmt, ap);
	va_end(ap);

	hold = fp_str;

	while (*hold != 0)
	{
		if ((unsigned int)(*hold - 65) < 26)
		{
			*hold += 32;
		}

		hold++;
	}

	FONT_VaReallyPrint(fp_str, ap);
}

void FONT_Print2(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsprintf(fp_str, fmt, ap);
	FONT_VaReallyPrint(fmt, ap);
}

int FONT_GetStringWidth(char *str)
{ 
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
}

void FONT_Flush()
{
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
}

void FONT_SetCursor(short x, short y)
{
	fontTracker.font_xpos = x;
	fontTracker.font_ypos = y;
}

void FONT_VaReallyPrint(const char *fmt, va_list ap)
{
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
}

void FONT_FontPrintCentered(char *text, long y)
{
	FONT_SetCursor((SCREEN_WIDTH/2) - (FONT_GetStringWidth(text) >> 1), y);
	FONT_Print2(text);
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
}
