#pragma once

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern LPDIRECTDRAW lpDD;
extern LPDIRECTDRAW4 lpDD4;
extern LPDIRECT3D3 d3dobj;
extern LPDIRECT3DDEVICE3 d3ddev;
extern LPDIRECT3DVIEWPORT3 viewport;
extern LPDIRECTDRAWCLIPPER clipper;
extern LPDIRECTDRAWSURFACE4 primary, backbuffer, zbuffer;
extern LPDIRECTDRAWGAMMACONTROL gamma;

void ShutdownDevice();
void DBG_Print(const char* fmt, ...);
void D3D_FailAbort(const char* fmt, ...);

HRESULT WINAPI enumdepthbuf(DDPIXELFORMAT* pixfmt, LPVOID lpContext);
int InitialiseDevice();

int D3D_Init(void* pvm);
void D3D_Pause();
int D3D_ReInit();
void D3D_Shutdown();
int D3D_SetGammaNormalized(int level);
void D3D_SetBGColor(int r, int g, int b);
void D3D_ActivateFogUnit(int index);
void D3D_SetFog(int r, int g, int b, float fogNear, float fogFar);
void D3D_Clear();
void D3D_ClearZBuffer();
void D3D_Flip();
void D3D_AddTri(int page, MYTRI* tri);
void D3D_DebugDrawLine(int a, int b);

unsigned int D3D_TimeDiff(unsigned int start);
unsigned int D3D_CurrentTime();
void D3D_Sleep(DWORD dwMilliseconds);

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif
