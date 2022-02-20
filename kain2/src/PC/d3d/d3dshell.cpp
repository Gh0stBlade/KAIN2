#include <Windows.h>
#include <ddraw.h>

struct BLOCK
{
	int width;
	int height;
	int red_cnt, red_diff;
	int green_cnt, green_diff;
	int blue_cnt, blue_diff;
	int dwRBitMask;
	int dwGBitMask;
	int dwBBitMask;
	int dwRGBAlphaBitMask;
	int dwRGBBitCount;
	DWORD* ptr;
};

BLOCK *Block;
DWORD invalid_ptrs[5];

//0001:00070ff0 ?D3DSHL_StretchBlitToBuffer@@YAXPAXHHHPAUBufferSurface@@@Z 00471ff0 f   d3dshell.obj
void __cdecl D3DSHL_StretchBlitToBuffer(void* surf, int w, int h, int pitch, struct BLOCK* block)
{
	if (w < 2 || h < 2) return;

	if (block->dwRGBBitCount == 32)
	{

	}
	else if (block->dwRGBBitCount == 16)
	{

	}
}

//0001:000711c0       _D3DSHL_Initialize         004721c0 f   d3dshell.obj
static void fill_diffcnt(DWORD colMask, DWORD bitmask, int *cnt, int *diff)
{
	int BITS_COL = 0;
	if (colMask)
		do { BITS_COL++; } while (colMask);

	int I_COL = 0;
	do { I_COL++; } while (bitmask >> I_COL);

	int COL_CNT;
	if (I_COL <= BITS_COL) COL_CNT = 0;
	else COL_CNT = I_COL - BITS_COL;
	*cnt = COL_CNT;

	int COL_DIFF;
	if (BITS_COL <= I_COL) COL_DIFF = 0;
	else COL_DIFF = BITS_COL - I_COL;
	*diff = COL_DIFF;
}

void __cdecl D3DSHL_Initialize(LPDIRECTDRAWSURFACE4 lpSurf, int is_software)
{
	if (Block)
	{
		free(Block->ptr);
		free(Block);
	}

	Block = nullptr;
	invalid_ptrs[4] = NULL;

	if (lpSurf == nullptr)
		return;

	DDPIXELFORMAT pfmt = { 0 };
	pfmt.dwSize = sizeof(pfmt);
	if (FAILED(lpSurf->GetPixelFormat(&pfmt)))
		return;

	invalid_ptrs[0] = 0x12345678;
	invalid_ptrs[1] = 0xFACEDEED;
	invalid_ptrs[2] = 0xCAFEBABE;
	invalid_ptrs[3] = 0xDEADBEEF;
	invalid_ptrs[4] = NULL;

	Block = (BLOCK*)malloc(sizeof(*Block));
	if (Block)
	{
		Block->width = 512;
		Block->height = 480;
		Block->ptr = (DWORD*)malloc(0xf0000);

		fill_diffcnt(pfmt.dwRBitMask, 0x1f, &Block->red_cnt, &Block->red_diff);
		fill_diffcnt(pfmt.dwGBitMask, 0x3e0, &Block->green_cnt, &Block->green_diff);
		fill_diffcnt(pfmt.dwBBitMask, 0x7c00, &Block->blue_cnt, &Block->blue_diff);

		Block->dwRBitMask = pfmt.dwRBitMask;
		Block->dwGBitMask = pfmt.dwGBitMask;
		Block->dwBBitMask = pfmt.dwBBitMask;
		Block->dwRGBAlphaBitMask = pfmt.dwRGBAlphaBitMask;
		Block->dwRGBBitCount = pfmt.dwRGBBitCount;
	}
}
//0001:000713b0       _D3DSHL_Shutdown           004723b0 f   d3dshell.obj
void __cdecl D3DSHL_Shutdown()
{
	if (Block)
	{
		free(Block->ptr);
		free(Block);
	}
	Block = nullptr;
	invalid_ptrs[4] = NULL;
}
//0001:000713f0       _D3DSHL_Trip               004723f0 f   d3dshell.obj
void __cdecl D3DSHL_Trip()
{
	invalid_ptrs[3] = 0xDEADBEEF;
	invalid_ptrs[2] = 0xCAFEBABE;
	invalid_ptrs[1] = 0xFACEDEED;
	invalid_ptrs[0] = 0x12345678;
}
//0001:00071420       _D3DSHL_Blit               00472420 f   d3dshell.obj
void __cdecl D3DSHL_Blit(DWORD* data, int w, int h, int x, int y)
{
	if (Block == nullptr)
		return;

	invalid_ptrs[4] = 1;
	// framebuffer pic?
	if (w == 512 && h == 480)
	{
		// avoid updating the same picture twice?
		if (data[1000] == invalid_ptrs[3] && data[2000] == invalid_ptrs[2] && data[3000] == invalid_ptrs[1] && data[4000] == invalid_ptrs[0])
			return;
		invalid_ptrs[3] = data[1000];
		invalid_ptrs[2] = data[2000];
		invalid_ptrs[1] = data[3000];
		invalid_ptrs[0] = data[4000];
	}


}
//0001:000716c0       _D3DSHL_BlitToBuffer       004726c0 f   d3dshell.obj
void __cdecl D3DSHL_BlitToBuffer(LPDIRECTDRAWSURFACE4 backsurf, int is_software)
{
	DDSURFACEDESC2 desc = { 0 };
	desc.dwSize = sizeof(desc);
	if (FAILED(backsurf->Lock(nullptr, &desc, 0x821, nullptr)))
		return;
	D3DSHL_StretchBlitToBuffer(desc.lpSurface, desc.dwWidth, desc.dwHeight, desc.lPitch, Block);
	backsurf->Unlock(nullptr);
}
//0001:00071790       _D3DSHL_Clear              00472790 f   d3dshell.obj
void __cdecl D3DSHL_Clear()
{
	if (Block) memset(Block->ptr, 0, 4 * Block->width * Block->height);
}
//0001:000717c0       _D3DSHL_UsedThisFrame      004727c0 f   d3dshell.obj
int __cdecl D3DSHL_UsedThisFrame(int mode)
{
	int result;

	result = invalid_ptrs[4];
	if (mode)
		invalid_ptrs[4] = 0;
	return result;
}