#pragma once

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

extern BLOCK* Block;
extern DWORD invalid_ptrs[5];

void __cdecl D3DSHL_StretchBlitToBuffer(void* surf, int w, int h, int pitch, struct BLOCK* block);
void __cdecl D3DSHL_Initialize(LPDIRECTDRAWSURFACE4 lpSurf, int is_software);
void __cdecl D3DSHL_Shutdown();
void __cdecl D3DSHL_Trip();
void __cdecl D3DSHL_Blit(DWORD* data, int w, int h, int x, int y);
void __cdecl D3DSHL_BlitToBuffer(LPDIRECTDRAWSURFACE4 backsurf, int is_software);
void __cdecl D3DSHL_Clear();
int  __cdecl D3DSHL_UsedThisFrame(int mode);