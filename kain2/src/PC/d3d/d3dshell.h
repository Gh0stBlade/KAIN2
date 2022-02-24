#pragma once

typedef struct BLOCK
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
} BLOCK;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif
extern BLOCK* Block;
extern DWORD invalid_ptrs[5];

extern void D3DSHL_StretchBlitToBuffer(void* surf, int w, int h, int pitch, struct BLOCK* block);
extern void D3DSHL_Initialize(LPDIRECTDRAWSURFACE4 lpSurf, int is_software);
extern void D3DSHL_Shutdown();
extern void D3DSHL_Trip();
extern void D3DSHL_Blit(DWORD* data, int w, int h, int x, int y);
extern void D3DSHL_BlitToBuffer(LPDIRECTDRAWSURFACE4 backsurf, int is_software);
extern void D3DSHL_Clear();
extern int  D3DSHL_UsedThisFrame(int mode);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif