#include <windows.h>
#include <stdio.h>
#include <ddraw.h>
#include <d3d.h>
#include "../snd.h"
#include "d3dbuckt.h"
#include "d3dclip.h"

#undef near
#undef far

typedef struct D3D_FOGTBL
{
	float near;
	float far;
	int col;
} D3D_FOGTBL;

int __cdecl D3D_SetGammaNormalized(int level);

LPDIRECTDRAW lpDD;
LPDIRECTDRAW4 lpDD4;
LPDIRECT3D3 d3dobj;
LPDIRECT3DDEVICE3 d3ddev;
LPDIRECT3DVIEWPORT3 viewport;
LPDIRECTDRAWCLIPPER clipper;
LPDIRECTDRAWSURFACE4 primary, backbuffer, zbuffer;
LPDIRECTDRAWGAMMACONTROL gamma;

D3D_FOGTBL d3d_fogtbl[32];
float D3D_FogFar, D3D_FogNear, D3D_FogZScale;
DWORD D3D_FogColor, D3D_CurFogUnit, D3D_UseVertexFog, D3D_AdaptivePerspec, D3D_ClipPlaneMask;

void __cdecl D3D_InitBuckets();
void __cdecl D3D_FreeBuckets();

DWORD D3D_GammaLevel,
	D3D_GammaAdjust,
	D3D_SelectedDevice,
	D3D_bgcol;
int(__cdecl* TRANS_DoTransform)(DWORD, DWORD, DWORD, DWORD);

//0001:00074f20 ?ShutdownDevice@@YAXXZ     00475f20 f   rnd_d3d.obj
void __cdecl ShutdownDevice()
{
	if (D3D_InScene)
	{
		d3ddev->EndScene();
		D3D_InScene = 0;
	}

	if (clipper)
	{
		primary->SetClipper(nullptr);
		clipper->Release();
		clipper = nullptr;
	}
	
	if (viewport)
	{
		d3ddev->DeleteViewport(viewport);
		viewport->Release();
		viewport = nullptr;
	}

	if (d3ddev)
	{
		d3ddev->Release();
		d3ddev = nullptr;
	}

	if (d3dobj)
	{
		d3dobj->Release();
		d3dobj = nullptr;
	}

	if (D3D_GammaAdjust)
	{
		gamma->Release();
		gamma = nullptr;
		D3D_GammaAdjust = 0;
	}

	if (zbuffer)
	{
		zbuffer->Release();
		zbuffer = nullptr;
	}

	if (backbuffer)
	{
		backbuffer->Release();
		backbuffer = nullptr;
	}

	if (primary)
	{
		primary->Release();
		primary = nullptr;
	}

	if (lpDD4)
	{
		lpDD4->Release();
		lpDD4 = nullptr;
	}

	if (D3D_Windowed)
		SetWindowPos(hWnd, (HWND)1, 0, 0, 0, 0, 0x30B);
	while (ShowCursor(true) < 0);
}

//0001:00074410       _DBG_Print                 00475410 f   rnd_d3d.obj
void __cdecl DBG_Print(const char* fmt, ...)
{
	CHAR Text[256]; // [esp+0h] [ebp-100h] BYREF
	va_list va; // [esp+108h] [ebp+8h] BYREF

	va_start(va, fmt);
	vsprintf_s(Text, sizeof(Text), fmt, va);
	va_end(va);
	OutputDebugStringA(Text);
}

//0001:00074450       _D3D_FailAbort             00475450 f   rnd_d3d.obj
void __cdecl D3D_FailAbort(const char *fmt, ...)
{
	CHAR Text[256];
	va_list va;

	va_start(va, fmt);
	vsprintf_s(Text, sizeof(Text), fmt, va);
	va_end(va);

	D3D_FreeBuckets();
	ShutdownDevice();
	hWnd = 0;
	if (MessageBoxA(0, Text, "Kain 2 Error", MB_OKCANCEL) == IDCANCEL)
	{
		while (1);
	}
	ExitProcess(0);
}

//0001:000744b0 ?enumdepthbuf@@YGJPAU_DDPIXELFORMAT@@PAX@Z 004754b0 f   rnd_d3d.obj
//0001:000744e0 ?InitialiseDevice@@YAHXZ   004754e0 f   rnd_d3d.obj
//0001:00075080       _D3D_Init                  00476080 f   rnd_d3d.obj
_G2AppDataVM_Type* d3d_vm;
HWND hWnd;
int D3D_Windowed,
	D3D_XRes,
	D3D_YRes,
	D3D_BitDepth,
	D3D_Triplebuf,
	D3D_VSync,
	D3D_Filter,
	D3D_InScene;

extern int __cdecl TRANS_Init();
int InitialiseDevice();

int __cdecl D3D_Init(_G2AppDataVM_Type* vm)
{
	int result; // eax

	d3d_vm = vm;
	hWnd = vm->hWindow;
	D3D_Windowed = vm->Is_windowed;
	D3D_XRes = vm->Screen_width;
	D3D_YRes = vm->Screen_height;
	D3D_BitDepth = vm->Screen_depth;
	D3D_SelectedDevice = vm->Render_device_id;
	D3D_Triplebuf = vm->Triple_buffer;
	D3D_VSync = vm->VSync;
	D3D_Filter = vm->Filter;
	D3D_InitBuckets();
	TRANS_Init();
	result = InitialiseDevice();
	if (result)
	{
		d3d_vm->Gamma_level = D3D_SetGammaNormalized(d3d_vm->Gamma_level);
		if (!d3ddev->BeginScene())
			D3D_InScene = 1;
		return 1;
	}
	return result;
}
//0001:00075130       _D3D_Pause                 00476130 f   rnd_d3d.obj
//0001:00075140       _D3D_ReInit                00476140 f   rnd_d3d.obj
//0001:000751d0       _D3D_Shutdown              004761d0 f   rnd_d3d.obj
//0001:000751f0       _D3D_SetGammaNormalized    004761f0 f   rnd_d3d.obj
int __cdecl D3D_SetGammaNormalized(int level)
{
	return 1;
}
//0001:000752b0       _D3D_SetBGColor            004762b0 f   rnd_d3d.obj
void __cdecl D3D_SetBGColor(int r, int g, int b)
{
	D3D_bgcol = b | ((g | (r << 8)) << 8);
}
//0001:000752d0       _D3D_ActivateFogUnit       004762d0 f   rnd_d3d.obj
void __cdecl D3D_ActivateFogUnit(int index)
{
	D3D_FogColor = d3d_fogtbl[index].col;
	D3D_FogFar = d3d_fogtbl[index].far;
	D3D_FogNear = d3d_fogtbl[index].near;
	if (D3D_UseVertexFog)
	{
		d3ddev->SetRenderState(D3DRENDERSTATE_FOGCOLOR, D3D_FogColor);
		D3D_FogZScale = 254.f / (D3D_FogFar - D3D_FogNear);
	}
}
//0001:00075330       _D3D_SetFog                00476330 f   rnd_d3d.obj
//0001:000753f0       _D3D_Clear                 004763f0 f   rnd_d3d.obj
//0001:00075480       _D3D_ClearZBuffer          00476480 f   rnd_d3d.obj
void __cdecl D3D_ClearZBuffer()
{
	D3DRECT rect;
	rect.x1 = 0;
	rect.y1 = 0;
	rect.x2 = D3D_XRes;
	rect.y2 = D3D_YRes;

	if (d3ddev)
		viewport->Clear2(1, &rect, 2, D3D_bgcol, 1.f, 0);
}
//0001:00075510       _D3D_Flip                  00476510 f   rnd_d3d.obj
//0001:000758f0       _D3D_AddTri                004768f0 f   rnd_d3d.obj
void __cdecl D3D_AddTri(int page, MYTRI* tri)
{
	MATBUCKET* bucket = D3D_GetBucket(page);
	if ((tri[2].col & tri[1].col & tri->col) == 0)
	{
		if (D3D_ClipPlaneMask)
		{
			//AddPlaneClippedTri(bucket, tri);
		}
		else if (tri->col || tri[1].col || tri[2].col)
		{
			//AddClippedTri(bucket, tri);
		}
		else
		{
			D3D_AddTriToBucket(bucket, tri);
		}
	}
}
//0001:00075960       _D3D_DebugDrawLine         00476960 f   rnd_d3d.obj
//0001:000759c0       _D3D_GetScreenShot         004769c0 f   rnd_d3d.obj
//0001:00075bc0       _DrawTPage                 00476bc0 f   rnd_d3d.obj
//0001:00075dd0       _D3D_TimeDiff              00476dd0 f   rnd_d3d.obj
unsigned __int64 __cdecl D3D_TimeDiff(unsigned int start)
{
	LARGE_INTEGER Frequency; // [esp+4h] [ebp-10h] BYREF
	LARGE_INTEGER PerformanceCount; // [esp+Ch] [ebp-8h] BYREF

	QueryPerformanceCounter(&PerformanceCount);
	QueryPerformanceFrequency(&Frequency);
	return PerformanceCount.QuadPart / (Frequency.QuadPart / 1000000) - start;
}
//0001:00075e30       _D3D_CurrentTime           00476e30 f   rnd_d3d.obj
unsigned __int64 D3D_CurrentTime()
{
	LARGE_INTEGER Frequency; // [esp+0h] [ebp-10h] BYREF
	LARGE_INTEGER PerformanceCount; // [esp+8h] [ebp-8h] BYREF

	QueryPerformanceCounter(&PerformanceCount);
	QueryPerformanceFrequency(&Frequency);
	return PerformanceCount.QuadPart / (Frequency.QuadPart / 1000000);
}
//0001:00075e80       _D3D_Sleep                 00476e80 f   rnd_d3d.obj
void __cdecl D3D_Sleep(DWORD dwMilliseconds)
{
	Sleep(dwMilliseconds);
}

//0003:007509b0       _D3D_GammaLevel            00c409b0     rnd_d3d.obj
//0003:007509b4       _D3D_SelectedDevice        00c409b4     rnd_d3d.obj
//0003:007509b8       _TRANS_DoTransform         00c409b8     rnd_d3d.obj
//0003:007509c0       _D3D_FogNear               00c409c0     rnd_d3d.obj
//0003:007509c4       _d3ddev                    00c409c4     rnd_d3d.obj
//0003:007509c8 ? lpDD@@3PAUIDirectDraw@@A  00c409c8     rnd_d3d.obj
//0003:007509cc       _D3D_CurrentFrame          00c409cc     rnd_d3d.obj
//0003:007509d0       _D3D_BitDepth              00c409d0     rnd_d3d.obj
//0003:007509d4       _D3D_XRes                  00c409d4     rnd_d3d.obj
//0003:007509d8 ? d3dobj@@3PAUIDirect3D3@@A 00c409d8     rnd_d3d.obj
//0003:007509e0 ? DD_Caps@@3U_DDCAPS_DX6@@A 00c409e0     rnd_d3d.obj
//0003:00750b5c       _D3D_YRes                  00c40b5c     rnd_d3d.obj
//0003:00750b60 ? D3D_Triplebuf@@3HA        00c40b60     rnd_d3d.obj
//0003:00750b64 ? lpDD4@@3PAUIDirectDraw4@@A 00c40b64     rnd_d3d.obj
//0003:00750b68 ? clipper@@3PAUIDirectDrawClipper@@A 00c40b68     rnd_d3d.obj
//0003:00750b6c ? D3D_VSync@@3HA            00c40b6c     rnd_d3d.obj
//0003:00750b74 ? viewport@@3PAUIDirect3DViewport3@@A 00c40b74     rnd_d3d.obj
//0003:00750b78       _D3D_XCenter               00c40b78     rnd_d3d.obj
//0003:00750b7c       _D3D_AdaptivePerspec       00c40b7c     rnd_d3d.obj
//0003:00750b80       _D3D_Filter                00c40b80     rnd_d3d.obj
//0003:00750b84 ? backbuffer@@3PAUIDirectDrawSurface4@@A 00c40b84     rnd_d3d.obj
//0003:00750b94 ? zbuffer@@3PAUIDirectDrawSurface4@@A 00c40b94     rnd_d3d.obj
//0003:00750b98       _D3D_DeviceDesc            00c40b98     rnd_d3d.obj
//0003:00750c94       _D3D_UseVertexFog          00c40c94     rnd_d3d.obj
//0003:00750c98       _D3D_CurFogUnit            00c40c98     rnd_d3d.obj
//0003:00750c9c       _D3D_YCenter               00c40c9c     rnd_d3d.obj
//0003:00750ca0       _D3D_FogZScale             00c40ca0     rnd_d3d.obj
//0003:00750ca4       _D3D_Windowed              00c40ca4     rnd_d3d.obj
//0003:00750ca8       _D3D_SoftwareDevice        00c40ca8     rnd_d3d.obj
//0003:00750cac       _D3D_FogColor              00c40cac     rnd_d3d.obj
//0003:00750cb0       _D3D_MipMapSupport         00c40cb0     rnd_d3d.obj
//0003:00750cc0       _D3D_FogFar                00c40cc0     rnd_d3d.obj
//0003:00750cd8 ? primary@@3PAUIDirectDrawSurface4@@A 00c40cd8     rnd_d3d.obj
//0003:00750cdc       _D3D_GammaAdjust           00c40cdc     rnd_d3d.obj
//0003:00750ce0       _D3D_InScene               00c40ce0     rnd_d3d.obj
//0003:00750e68 ? gamma@@3PAUIDirectDrawGammaControl@@A 00c40e68     rnd_d3d.obj