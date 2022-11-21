#include "LIBGPU.H"
#include "EMULATOR_RENDER_COMMON.H"
#include "Core/EMULATOR_GLOBALS.H"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG_POLY_COUNT
int vram_need_update;

int g_DisableTouchUI;

unsigned short vram[VRAM_WIDTH * VRAM_HEIGHT];

unsigned char rgLUT[LUT_WIDTH * LUT_HEIGHT * sizeof(unsigned int)];

int splitAgain = FALSE;
unsigned int g_swapTime;
int g_wireframeMode = 0;
int g_swapInterval = SWAP_INTERVAL;
int g_PreviousBlendMode = BM_NONE;
int windowWidth = 0;
int windowHeight = 0;
unsigned int g_resetDeviceOnNextFrame = FALSE;
unsigned int g_closeOnNextFrame = FALSE;
unsigned int g_resettingDevice = FALSE;

int begin_scene_flag = FALSE;
int vbo_was_dirty_flag = FALSE;

#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
TextureID* g_lastBoundTexture[2];

ShaderID* g_gte_shader_4;
ShaderID* g_gte_shader_8;
ShaderID* g_gte_shader_16;
ShaderID* g_blit_shader;

TextureID* vramTexture;
TextureID* whiteTexture;
TextureID* rg8lutTexture;
#else
TextureID g_lastBoundTexture[2];

ShaderID g_gte_shader_4;
ShaderID g_gte_shader_8;
ShaderID g_gte_shader_16;
ShaderID g_blit_shader;

TextureID vramTexture;
TextureID whiteTexture;
TextureID rg8lutTexture;
#endif

typedef struct POLY_G3_SEMITRANS 
{
#if defined(USE_32_BIT_ADDR)
	unsigned long tag;
#if defined(PGXP)
	unsigned short len;
	unsigned short pgxp_index;
#else
	unsigned long len;
#endif
#else
	unsigned long tag;
#endif
	unsigned long dr_tpage; // size=0, offset=4
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
	short x0; // size=0, offset=12
	short y0; // size=0, offset=14
	unsigned char r1; // size=0, offset=16
	unsigned char g1; // size=0, offset=17
	unsigned char b1; // size=0, offset=18
	unsigned char pad1; // size=0, offset=19
	short x1; // size=0, offset=20
	short y1; // size=0, offset=22
	unsigned char r2; // size=0, offset=24
	unsigned char g2; // size=0, offset=25
	unsigned char b2; // size=0, offset=26
	unsigned char pad2; // size=0, offset=27
	short x2; // size=0, offset=28
	short y2; // size=0, offset=30
} POLY_G3_SEMITRANS;

typedef struct POLY_F4_SEMITRANS
{
#if defined(USE_32_BIT_ADDR)
	unsigned long tag;
#if defined(PGXP)
	unsigned short len;
	unsigned short pgxp_index;
#else
	unsigned long len;
#endif
#else
	unsigned long tag;
#endif

	unsigned long dr_tpage; // size=0, offset=4
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
	short x0; // size=0, offset=12
	short y0; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	short x2; // size=0, offset=20
	short y2; // size=0, offset=22
	short x3; // size=0, offset=24
	short y3; // size=0, offset=26
} POLY_F4_SEMITRANS;


#if defined(DEBUG_POLY_COUNT)
static int polygon_count = 0;
#endif

extern int g_emulatorPaused;

struct Vertex g_vertexBuffer[MAX_NUM_POLY_BUFFER_VERTICES];
unsigned short g_indexBuffer[MAX_NUM_POLY_BUFFER_VERTICES];
struct VertexBufferSplit g_splits[MAX_NUM_INDEX_BUFFERS];
int g_vertexIndex;
int g_indicesIndex;
int g_splitIndex;
int g_polygonSelected = 0;
unsigned short s_lastSemiTrans = 0xFFFF;
unsigned short s_lastPolyType = 0xFFFF;

#if defined(USE_32_BIT_ADDR)
unsigned int actualTerminator[2] = { 0xFFFFFFFF, 0};
unsigned int terminatorOT[2] = { (unsigned int)&actualTerminator, 0};

#if defined(_WIN64)
unsigned int terminatorAddr = 0;
#endif

#else
unsigned int actualTerminator = -1;
unsigned int terminatorOT = (unsigned int)&actualTerminator;
#endif

int IsValidCode(int code)
{
	return code >= 0x20 && code <= 0x80;
}

extern int splitAgain;

#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
void Emulator_AddSplit(int semiTrans, int page, TextureID* textureId)
#else
void Emulator_AddSplit(int semiTrans, int page, TextureID textureId)
#endif
{
	struct VertexBufferSplit* curSplit = &g_splits[g_splitIndex];
	BlendMode curBlendMode = semiTrans ? GET_TPAGE_BLEND(page) : BM_NONE;
	TexFormat curTexFormat = GET_TPAGE_FORMAT(page);

#if defined(VULKAN)
	if (curSplit->blendMode == curBlendMode && curSplit->texFormat == curTexFormat && curSplit->textureId.textureImage == textureId->textureImage)
#elif defined(D3D12)
	if (curSplit->blendMode == curBlendMode && curSplit->texFormat == curTexFormat && curSplit->textureId.m_textureResource == textureId.m_textureResource)
#elif defined(GXM)
	if (curSplit->blendMode == curBlendMode && curSplit->texFormat == curTexFormat && curSplit->textureId.Uid == textureId.Uid)
#else
	if (curSplit->blendMode == curBlendMode && curSplit->texFormat == curTexFormat && curSplit->textureId == textureId && !splitAgain)
#endif
	{
		return;
	}

	curSplit->vCount = g_vertexIndex - curSplit->vIndex;

	struct VertexBufferSplit* split = &g_splits[++g_splitIndex];

	split->textureId = textureId;
	split->vIndex = g_vertexIndex;
	split->iIndex = g_indicesIndex;
	split->vCount = 0;
	split->blendMode = curBlendMode;
	split->texFormat = curTexFormat;
}

void Emulator_MakeIndex(int indexCount)
{
	int bufferIndex = g_indicesIndex;

	for (int i = 0; i < indexCount; i += 3)
	{
		g_indexBuffer[g_indicesIndex + 0] = g_vertexIndex + i + 0;
		g_indexBuffer[g_indicesIndex + 1] = g_vertexIndex + i + 1;
		g_indexBuffer[g_indicesIndex + 2] = g_vertexIndex + i + 2;

		g_indicesIndex += 3;
	}
}

void Emulator_MakeTriangle()
{
	g_vertexBuffer[g_vertexIndex + 5] = g_vertexBuffer[g_vertexIndex + 3];
	g_vertexBuffer[g_vertexIndex + 3] = g_vertexBuffer[g_vertexIndex];
	g_vertexBuffer[g_vertexIndex + 4] = g_vertexBuffer[g_vertexIndex + 2];
}

// parses primitive and pushes it to VBO
// returns primitive size
// -1 means invalid primitive
int ParsePrimitive(uintptr_t primPtr)
{
	P_TAG* pTag = (P_TAG*)primPtr;

	int textured = (pTag->code & 0x4) != 0;

	int blend_mode = 0;

	int dr_tpage = 0;

	if (textured)
	{
		if ((pTag->code & 0x1) != 0)
		{
			blend_mode = 2;
		}
		else
		{
			blend_mode = 1;
		}
	}
	else
	{
		blend_mode = 0;
	}

	int code = 0;

	//BLK_FILL
	if (pTag->code != 2)
	{

		if (IsValidCode(pTag->code))
		{
			code = pTag->code;
		}
		else
		{
			if (IsValidCode(((POLY_F4_SEMITRANS*)pTag)->code))
			{
				dr_tpage = 1;
				code = ((POLY_F4_SEMITRANS*)pTag)->code;
			}
		}
	}
	else
	{
		//Do black fill
		typedef struct BLK_FILL
		{
#if defined(USE_32_BIT_ADDR)
			unsigned long tag; // size=0, offset=0
			unsigned long len; // size=0, offset=0
#else
			unsigned long tag; // size=0, offset=0
#endif
			unsigned char r0;
			unsigned char g0;
			unsigned char b0;
			unsigned char code;
			unsigned short x0;
			unsigned short y0;
			unsigned short w;
			unsigned short h;
		} BLK_FILL;

		BLK_FILL* poly = (BLK_FILL*)pTag;

		int r5 = poly->r0 * 31 / 255;
		int g5 = poly->g0 * 31 / 255;
		int b5 = poly->b0 * 31 / 255;
		int a1 = 0;

		unsigned short colour = (unsigned short)(r5 << 1 | g5 << 6 | b5 << 11 | a1);

		unsigned short* blackImage = (unsigned short*)malloc(poly->w * poly->h * sizeof(unsigned short));

		for (int i = 0; i < poly->w * poly->h; i++)
		{
			blackImage[i] = colour;
		}

		RECT16 r;
		r.x = poly->x0;
		r.y = poly->y0;
		r.w = poly->w;
		r.h = poly->h;

		LoadImagePSX(&r, (unsigned long*)blackImage);
		Emulator_UpdateVRAM();

		free(blackImage);

		int primitive_size = sizeof(BLK_FILL);
		return primitive_size;
	}

	int semi_transparent = (code & 2) != 0;

	int primitive_size = -1;	// -1

	switch (code & ~0x3)
	{
	case 0x0:
	{
		primitive_size = 4;
		break;
	}
	case 0x20:
	{
		POLY_F3* poly = (POLY_F3*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

		Emulator_GenerateVertexArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x2);
		Emulator_GenerateTexcoordArrayTriangleZero(&g_vertexBuffer[g_vertexIndex], 0);
		Emulator_GenerateColourArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0);

		Emulator_MakeIndex(3);

		g_vertexIndex += 3;

		primitive_size = sizeof(POLY_F3);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x24:
	{
		POLY_FT3* poly = (POLY_FT3*)pTag;
		activeDrawEnv.tpage = poly->tpage;

		Emulator_AddSplit(semi_transparent, poly->tpage, vramTexture);

		Emulator_GenerateVertexArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x2);
		Emulator_GenerateTexcoordArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->u0, &poly->u1, &poly->u2, poly->tpage, poly->clut, GET_TPAGE_DITHER(lastTpage));
		Emulator_GenerateColourArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0);

		Emulator_MakeIndex(3);

		g_vertexIndex += 3;

		primitive_size = sizeof(POLY_FT3);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x28:
	{
		if (dr_tpage)
		{
			POLY_F4_SEMITRANS* poly = (POLY_F4_SEMITRANS*)pTag;

			activeDrawEnv.tpage = (poly->dr_tpage & 0xFFFF);

			//Emulator_DoSplitHackQuad(&poly->x0, &poly->x1, &poly->x3, &poly->x2);

			Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

			Emulator_GenerateVertexArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x3, &poly->x2);
			Emulator_GenerateTexcoordArrayQuadZero(&g_vertexBuffer[g_vertexIndex], 0);
			Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, blend_mode == 2);

			Emulator_MakeTriangle();
			Emulator_MakeIndex(6);

			g_vertexIndex += 6;


			primitive_size = sizeof(POLY_F4_SEMITRANS);
#if defined(DEBUG_POLY_COUNT)
			polygon_count++;
#endif
		}
		else
		{
			POLY_F4* poly = (POLY_F4*)pTag;

			Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

			Emulator_GenerateVertexArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x3, &poly->x2);
			Emulator_GenerateTexcoordArrayQuadZero(&g_vertexBuffer[g_vertexIndex], 0);
			Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, blend_mode == 2);

			Emulator_MakeTriangle();
			Emulator_MakeIndex(6);

			g_vertexIndex += 6;
			primitive_size = sizeof(POLY_F4);
#if defined(DEBUG_POLY_COUNT)
			polygon_count++;
#endif
		}
		break;
	}
	case 0x2C:
	{
		POLY_FT4* poly = (POLY_FT4*)pTag;
		
		activeDrawEnv.tpage = poly->tpage;

		Emulator_AddSplit(semi_transparent, poly->tpage, vramTexture);

		Emulator_GenerateVertexArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x3, &poly->x2);
		Emulator_GenerateTexcoordArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->u0, &poly->u1, &poly->u3, &poly->u2, poly->tpage, poly->clut, GET_TPAGE_DITHER(lastTpage));
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, blend_mode == 2);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(POLY_FT4);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x30:
	{
		if (dr_tpage)
		{
			POLY_G3_SEMITRANS* poly = (POLY_G3_SEMITRANS*)pTag;

			activeDrawEnv.tpage = (poly->dr_tpage & 0xFFFF);

			Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

			Emulator_GenerateVertexArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x2);
			Emulator_GenerateTexcoordArrayTriangleZero(&g_vertexBuffer[g_vertexIndex], 1);
			Emulator_GenerateColourArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r1, &poly->r2);

			Emulator_MakeIndex(3);

			g_vertexIndex += 3;
			
			primitive_size = sizeof(POLY_G3_SEMITRANS);
#if defined(DEBUG_POLY_COUNT)
			polygon_count++;
#endif
		}
		else
		{
			POLY_G3* poly = (POLY_G3*)pTag;

			Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

			Emulator_GenerateVertexArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x2);
			Emulator_GenerateTexcoordArrayTriangleZero(&g_vertexBuffer[g_vertexIndex], 1);
			Emulator_GenerateColourArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r1, &poly->r2);

			Emulator_MakeIndex(3);

			g_vertexIndex += 3;

			primitive_size = sizeof(POLY_G3);
#if defined(DEBUG_POLY_COUNT)
			polygon_count++;
#endif
		}
		break;
	}
	case 0x34:
	{
		POLY_GT3* poly = (POLY_GT3*)pTag;

		activeDrawEnv.tpage = poly->tpage;

		Emulator_AddSplit(semi_transparent, poly->tpage, vramTexture);

		Emulator_GenerateVertexArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x2);
		Emulator_GenerateTexcoordArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->u0, &poly->u1, &poly->u2, poly->tpage, poly->clut, GET_TPAGE_DITHER(lastTpage));
		Emulator_GenerateColourArrayTriangle(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r1, &poly->r2);

		Emulator_MakeIndex(3);

		g_vertexIndex += 3;

		primitive_size = sizeof(POLY_GT3);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x38:
	{
		POLY_G4* poly = (POLY_G4*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

		Emulator_GenerateVertexArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x3, &poly->x2);
		Emulator_GenerateTexcoordArrayQuadZero(&g_vertexBuffer[g_vertexIndex], 1);
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r1, &poly->r3, &poly->r2, TRUE);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(POLY_G4);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x3C:
	{
		POLY_GT4* poly = (POLY_GT4*)pTag;
		activeDrawEnv.tpage = poly->tpage;

		Emulator_AddSplit(semi_transparent, poly->tpage, vramTexture);

		Emulator_GenerateVertexArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, &poly->x3, &poly->x2);
		Emulator_GenerateTexcoordArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->u0, &poly->u1, &poly->u3, &poly->u2, poly->tpage, poly->clut, GET_TPAGE_DITHER(lastTpage));
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r1, &poly->r3, &poly->r2, FALSE);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(POLY_GT4);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x40:
	{
		LINE_F2* poly = (LINE_F2*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

		Emulator_GenerateLineArray(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1);
		Emulator_GenerateTexcoordArrayLineZero(&g_vertexBuffer[g_vertexIndex], 0);
		Emulator_GenerateColourArrayLine(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0);

		Emulator_MakeTriangle();

		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(LINE_F2);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x48: // TODO (unused)
	{
		LINE_F3* poly = (LINE_F3*)pTag;
		/*
					for (int i = 0; i < 2; i++)
					{
						Emulator_AddSplit(POLY_TYPE_LINES, semi_transparent, activeDrawEnv.tpage, whiteTexture);

						if (i == 0)
						{
							//First line
							Emulator_GenerateLineArray(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1, NULL, NULL);
							Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, NULL, NULL, NULL);
							g_vertexIndex += 2;
						}
						else
						{
							//Second line
							Emulator_GenerateLineArray(&g_vertexBuffer[g_vertexIndex], &poly->x1, &poly->x2, NULL, NULL);
							Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, NULL, NULL, NULL);
							g_vertexIndex += 2;
						}
		#if defined(DEBUG_POLY_COUNT)
						polygon_count++;
		#endif
					}
		*/

		primitive_size = sizeof(LINE_F3);
		break;
	}
	case 0x50:
	{
		LINE_G2* poly = (LINE_G2*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

		Emulator_GenerateLineArray(&g_vertexBuffer[g_vertexIndex], &poly->x0, &poly->x1);
		Emulator_GenerateTexcoordArrayLineZero(&g_vertexBuffer[g_vertexIndex], 0);
		Emulator_GenerateColourArrayLine(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r1);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(LINE_G2);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x60:
	{
		TILE* poly = (TILE*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

		Emulator_GenerateVertexArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->x0, poly->w, poly->h);
		Emulator_GenerateTexcoordArrayQuadZero(&g_vertexBuffer[g_vertexIndex], 0);
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, FALSE);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(TILE);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif

		break;
	}
	case 0x64:
	{
		SPRT* poly = (SPRT*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, vramTexture);

		Emulator_GenerateVertexArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->x0, poly->w, poly->h);
		Emulator_GenerateTexcoordArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->u0, activeDrawEnv.tpage, poly->clut, poly->w, poly->h);
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, FALSE);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(SPRT);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x68:
	{
		TILE_1* poly = (TILE_1*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

		Emulator_GenerateVertexArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->x0, 1, 1);
		Emulator_GenerateTexcoordArrayQuadZero(&g_vertexBuffer[g_vertexIndex], 0);
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, FALSE);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(TILE_1);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x70:
	{
		TILE_8* poly = (TILE_8*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

		Emulator_GenerateVertexArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->x0, 8, 8);
		Emulator_GenerateTexcoordArrayQuadZero(&g_vertexBuffer[g_vertexIndex], 0);
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, FALSE);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(TILE_8);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x74:
	{
		SPRT_8* poly = (SPRT_8*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, vramTexture);

		Emulator_GenerateVertexArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->x0, 8, 8);
		Emulator_GenerateTexcoordArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->u0, activeDrawEnv.tpage, poly->clut, 8, 8);
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, FALSE);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(SPRT_8);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x78:
	{
		TILE_16* poly = (TILE_16*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, whiteTexture);

		Emulator_GenerateVertexArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->x0, 16, 16);
		Emulator_GenerateTexcoordArrayQuadZero(&g_vertexBuffer[g_vertexIndex], 0);
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, FALSE);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(TILE_16);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0x7C:
	{
		SPRT_16* poly = (SPRT_16*)pTag;

		Emulator_AddSplit(semi_transparent, activeDrawEnv.tpage, vramTexture);

		Emulator_GenerateVertexArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->x0, 16, 16);
		Emulator_GenerateTexcoordArrayRect(&g_vertexBuffer[g_vertexIndex], &poly->u0, activeDrawEnv.tpage, poly->clut, 16, 16);
		Emulator_GenerateColourArrayQuad(&g_vertexBuffer[g_vertexIndex], &poly->r0, &poly->r0, &poly->r0, &poly->r0, FALSE);

		Emulator_MakeTriangle();
		Emulator_MakeIndex(6);

		g_vertexIndex += 6;

		primitive_size = sizeof(SPRT_16);
#if defined(DEBUG_POLY_COUNT)
		polygon_count++;
#endif
		break;
	}
	case 0xE0:
	{
		switch (pTag->code)
		{
		case 0xE1:
		{
#if defined(USE_32_BIT_ADDR)
			unsigned short tpage = ((unsigned short*)pTag)[4];
#else
			unsigned short tpage = ((unsigned short*)pTag)[2];
#endif
			//if (tpage != 0)
			{
				activeDrawEnv.tpage = tpage;
			}

			primitive_size = 4;//sizeof(DR_TPAGE);
#if defined(DEBUG_POLY_COUNT)
			polygon_count++;
#endif

			break;
		}
		default:
		{
			eprinterr("Primitive type error");
			assert(FALSE);
			break;
		}
		}
		break;
	}
	case 0x80: {

		DR_MOVE* poly = (DR_MOVE*)pTag;
		RECT16 r;
		
		int x = poly->code[2] & 0xFFFF;
		int y = ((poly->code[2] >> 16) & 0xFFFF) & 0x1FF;

		r.x = poly->code[3] & 0xFFFF;
		r.y = ((poly->code[3] >> 16) & 0xFFFF) & 0x1FF;
		
		r.w = poly->code[4] & 0xFFFF;
		r.h = ((poly->code[4] >> 16) & 0xFFFF);
		
		MoveImage(&r, x, y);

		primitive_size = sizeof(DR_MOVE);

		break;
	}
	default:
	{
		//Unhandled poly type
		eprinterr("Unhandled primitive type: %02X type2:%02X\n", pTag->code, pTag->code & ~3);
		break;
	}
	}

	return primitive_size;
}


int ParseLinkedPrimitiveList(uintptr_t packetStart, uintptr_t packetEnd)
{
	uintptr_t currentAddress = packetStart;

	int lastSize = -1;

	while (currentAddress != packetEnd)
	{
		lastSize = ParsePrimitive(currentAddress);

		if (lastSize == -1)	// not valid packets submitted
			break;

		currentAddress += lastSize;
	}

	g_splits[g_splitIndex].vCount = g_vertexIndex - g_splits[g_splitIndex].vIndex;

	return lastSize;
}


void Emulator_ClearVBO()
{
	g_vertexIndex = 0;
	g_indicesIndex = 0;
	g_splitIndex = 0;
	g_splits[g_splitIndex].texFormat = (enum TexFormat)0xFFFF;
}

void Emulator_ResetPolyState()
{
	s_lastSemiTrans = 0xFFFF;
	s_lastPolyType = 0xFFFF;
}

#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
ShaderID* Emulator_GetGTEShader(TexFormat texFormat)
#else
ShaderID Emulator_GetGTEShader(TexFormat texFormat)
#endif
{
	switch (texFormat)
	{
	case TF_4_BIT:
		return g_gte_shader_4;
		break;
	case TF_8_BIT:
		return g_gte_shader_8;
		break;
	case TF_16_BIT:
		return g_gte_shader_16;
		break;
	}

	return g_blit_shader;
}

void Emulator_DrawSplit(const struct VertexBufferSplit* split)
{
	Emulator_SetBlendMode(split->blendMode);
	Emulator_SetTextureAndShader(split->textureId, Emulator_GetGTEShader(split->texFormat));

	Emulator_DrawTriangles(split->vIndex, split->iIndex, split->vCount / 3);
}

void Emulator_DrawAggregatedSplits()
{
	if (g_emulatorPaused)
	{
		for (int i = 0; i < 3; i++)
		{
			struct Vertex* vert = &g_vertexBuffer[g_polygonSelected + i];
			vert->r = 255;
			vert->g = 0;
			vert->b = 0;

			eprintf("==========================================\n");
			eprintf("POLYGON: %d\n", i);
			eprintf("X: %d Y: %d\n", vert->x, vert->y);
			eprintf("U: %d V: %d\n", vert->u, vert->v);
			eprintf("TP: %d CLT: %d\n", vert->page, vert->clut);
			eprintf("==========================================\n");
		}

		Emulator_UpdateInput(1);
	}

	// next code ideally should be called before EndScene
	Emulator_UpdateVertexBuffer(g_vertexBuffer, g_vertexIndex);
	Emulator_UpdateIndexBuffer(g_indexBuffer, g_vertexIndex);

	for (int i = 1; i <= g_splitIndex; i++)
		Emulator_DrawSplit(&g_splits[i]);

	Emulator_ClearVBO();
}

void Emulator_AggregatePTAGsToSplits(unsigned long* p, int singlePrimitive)
{
	if (!p)
		return;

	if (singlePrimitive)
	{
		// single primitive
		ParsePrimitive((uintptr_t)p);
		g_splits[g_splitIndex].vCount = g_vertexIndex - g_splits[g_splitIndex].vIndex;
	}
	else
	{
		P_TAG* pTag = (P_TAG*)p;

		// P_TAG as primitive list
		//do
#if defined(_WIN64)
		while ((uintptr_t)pTag != (uintptr_t)terminatorAddr)
#else
		while ((uintptr_t)pTag != (uintptr_t)&terminatorOT)
#endif
		{
			if (pTag->len > 0)
			{
				int lastSize = ParseLinkedPrimitiveList((uintptr_t)pTag, (uintptr_t)pTag + (uintptr_t)(pTag->len * 4) + 4 + LEN_OFFSET);
				if (lastSize == -1)
					break; // safe bailout
			}

			pTag = (P_TAG*)pTag->addr;
		}
	}
}

void* Emulator_GenerateRG8LUT()
{
	for (unsigned short y = 0; y < LUT_HEIGHT; y++)
	{
		unsigned char* row = rgLUT + y * (LUT_HEIGHT * 4);

		for (unsigned short x = 0; x < LUT_WIDTH; x++)
		{
			unsigned short c = (y << 8) | x;

			unsigned char* pixel = row + x * 4;
			pixel[0] = (unsigned char)((c & 0x1F)) << 3;
			pixel[1] = (unsigned char)((c & 0x3E0) >> 5) << 3;
			pixel[2] = (unsigned char)(((c & 0x7C00) >> 10) << 3);
			pixel[3] = (unsigned char)0xFF;
		}
	}

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && defined(DEBUG_RG8LUT)
	FILE* f = fopen("RG8LUT.TGA", "wb");
	unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
	unsigned char header[6];
	header[0] = (LUT_WIDTH % 256);
	header[1] = (LUT_WIDTH / 256);
	header[2] = (LUT_HEIGHT % 256);
	header[3] = (LUT_HEIGHT / 256);
	header[4] = 32;
	header[5] = 0;

	fwrite(TGAheader, sizeof(unsigned char), 12, f);
	fwrite(header, sizeof(unsigned char), 6, f);
	fwrite(rgLUT, sizeof(rgLUT), 1, f);
	fclose(f);
#endif

	return rgLUT;
}

#if defined(TOUCH_UI)

extern unsigned short resultTouchKeysPressed;

void Emulator_DrawTouchUI()
{
	if (g_DisableTouchUI)
	{
		return;
	}

	unsigned short mapper[4] = {
		0x8,
		0x1,
		0x2,
		0x4,
	};

	unsigned long OT[4];
	char polygonBuffer[sizeof(POLY_F4) * 32];
	char* p = &polygonBuffer[0];

	ClearOTagR(OT, 4 / 2);

	int dist = 16;
	int cx = 32;
	int cy = 172;

	for (int i = 0; i < 4; i++)
	{
		int dx = (i % 2) ? 0 : 1;
		int dy = dx ? 0 : 1;
		int ndist = (i >= 2) ? dist : -dist;
		
		int mx = dx ? ndist * 2 : 0;
		int my = dy ? ndist * 2 : 0;
		int pressed = (resultTouchKeysPressed & mapper[i] << 4) != 0;
		
		setPolyF4(p);
		setSemiTrans(p, 1);
		setRGB0((POLY_F4*)p, 127, pressed ? 0 : 127, pressed ? 0 : 127);
		//((POLY_FT4*)p)->clut = touchButtonsUI[i].clut;
		//((POLY_FT4*)p)->tpage = touchButtonsUI[i].tpage;
		//setUV4((POLY_FT4*)p, touchButtonsUI[i].u[0], touchButtonsUI[i].v[0], touchButtonsUI[i].u[1], touchButtonsUI[i].v[1], touchButtonsUI[i].u[2], touchButtonsUI[i].v[2], touchButtonsUI[i].u[3], touchButtonsUI[i].v[3]);
		setXY4((POLY_F4*)p, cx + mx, cy + my, cx + mx + 32, cy + my, cx + mx, cy + my + 32, cx + mx + 32, cy + my + 32);
		addPrim(OT, p);
		p += sizeof(POLY_F4);
	}
	
	cx = 512-64;
	cy = 172;

	for (int i = 0; i < 4; i++)
	{
		int dx = (i % 2) ? 0 : 1;
		int dy = dx ? 0 : 1;
		int ndist = (i >= 2) ? dist : -dist;

		int mx = dx ? ndist * 2 : 0;
		int my = dy ? ndist * 2 : 0;

		int pressed = (resultTouchKeysPressed & mapper[i] << 12) != 0;
		setPolyF4(p);
		setSemiTrans(p, 1);
		setRGB0((POLY_F4*)p, 127, pressed ? 0 : 127, pressed ? 0 : 127);
		setXY4((POLY_F4*)p, cx + mx, cy + my, cx + mx + 32, cy + my, cx + mx, cy + my + 32, cx + mx + 32, cy + my + 32);
		addPrim(OT, p);
		p += sizeof(POLY_F4);
	}

	cx = 512 / 2;
	cy = 240 - 32;
	dist = 32;

	for (int i = 0; i < 2; i++)
	{
		int dx = (i % 2) ? 0 : 1;
		int ndist = (i != 0) ? dist : -dist;

		int mx = dx ? ndist * 2 : 0;

		int pressed = (resultTouchKeysPressed & mapper[!i]) != 0;
		setPolyF4(p);
		setSemiTrans(p, 1);
		setRGB0((POLY_F4*)p, 127, pressed ? 0 : 127, pressed ? 0 : 127);
		//setRGB1((POLY_F4*)p, 127, pressed ? 0 : 127, pressed ? 0 : 127);
		//setRGB2((POLY_F4*)p, 127, pressed ? 0 : 127, pressed ? 0 : 127);
		//setRGB3((POLY_F4*)p, 127, pressed ? 0 : 127, pressed ? 0 : 127);
		setXY4((POLY_F4*)p, cx + mx, cy, cx + mx + 32, cy, cx + mx, cy + 16, cx + mx + 32, cy + 16);
		addPrim(OT, p);
		p += sizeof(POLY_F4);
	}

	Emulator_AggregatePTAGsToSplits(OT, FALSE);
}
#endif