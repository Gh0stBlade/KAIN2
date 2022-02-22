#include "d3d.h"
#include <stdio.h>
#include "../snd.h"
#include "d3dbuckt.h"
#include "d3dclip.h"
#include "d3dshell.h"

#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

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

D3DDEVICEDESC D3D_DeviceDesc;
DDCAPS DD_Caps;

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

D3D_FOGTBL d3d_fogtbl[32];
float D3D_FogFar, D3D_FogNear, D3D_FogZScale;
DWORD D3D_FogColor, D3D_CurFogUnit, D3D_UseVertexFog, D3D_AdaptivePerspec;
float D3D_XCenter, D3D_YCenter;

void __cdecl D3D_EnumTextureTypes(IDirect3DDevice3* dev);
void __cdecl D3D_Flip();
void __cdecl D3D_Clear();

void __cdecl D3DTEX_SetTextureFormat(DDPIXELFORMAT* fmt);
int __cdecl D3DTEX_Init(int is_software);

void __cdecl ASLD_ReportStatus();
void __cdecl FONT_SetCursor(__int16 x, __int16 y);

DWORD D3D_GammaLevel,
	D3D_GammaAdjust,
	D3D_MipMapSupport,
	D3D_SoftwareDevice,
	D3D_SelectedDevice,
	D3D_bgcol,
	D3D_CurrentFrame;
int(__cdecl* TRANS_DoTransform)(DWORD, DWORD, DWORD, DWORD);

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)		if((x)) { (x)->Release(); (x) = nullptr; }
#endif

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

	SAFE_RELEASE(d3ddev);
	SAFE_RELEASE(d3dobj);

	if (D3D_GammaAdjust)
	{
		gamma->Release();
		gamma = nullptr;
		D3D_GammaAdjust = 0;
	}

	SAFE_RELEASE(zbuffer);
	SAFE_RELEASE(backbuffer);
	SAFE_RELEASE(primary);
	SAFE_RELEASE(lpDD4);

	if (D3D_Windowed)
		SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, 0x30B);
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
#if 0
	if (MessageBoxA(0, Text, "Kain 2 Error", MB_OKCANCEL) == IDCANCEL)
	{
		// questionable code
		// it makes the program stagger if you click on CANCEL
		while (1);
	}
#else
	// more sane like this, just display an error message and quit
	MessageBoxA(0, Text, "Kain 2 Error", MB_OK);
#endif
	ExitProcess(0);
}

//0001:000744b0 ?enumdepthbuf@@YGJPAU_DDPIXELFORMAT@@PAX@Z 004754b0 f   rnd_d3d.obj
HRESULT WINAPI enumdepthbuf(DDPIXELFORMAT* pixfmt, LPVOID lpContext)
{
	if (pixfmt->dwRGBBitCount == 16)
		memcpy(lpContext, pixfmt, sizeof(DDPIXELFORMAT));

	return 1;
}

//0001:000744e0 ?InitialiseDevice@@YAHXZ   004754e0 f   rnd_d3d.obj
int __cdecl InitialiseDevice()
{
	D3D_CurrentFrame = 0;
	if(FAILED(DirectDrawCreate(Devicelist[D3D_SelectedDevice].pguid, &lpDD, nullptr)))
		return 0;
	
	lpDD->QueryInterface(IID_IDirectDraw4, (LPVOID*)&lpDD4);
	lpDD->Release();
	if (D3D_XRes == 0 || D3D_YRes == 0)
		return 1;

	if (D3D_Windowed)
	{
		if (Devicelist[D3D_SelectedDevice].field_B0)
			return 0;
		if (FAILED(lpDD4->SetCooperativeLevel(hWnd, DDSCL_NORMAL)))
			return 0;

		DDSURFACEDESC2 scap = { 0 };
		scap.dwSize = sizeof(scap);
		scap.dwFlags = DDSD_CAPS;
		scap.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		lpDD4->CreateSurface(&scap, &primary, 0);
		if (primary == nullptr)
			return 0;
		scap.dwWidth = D3D_XRes;
		scap.dwHeight = D3D_YRes;
		scap.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		scap.ddsCaps.dwCaps = Devicelist[D3D_SelectedDevice].tri_caps != 0 ? DDSCAPS_3DDEVICE | DDSCAPS_SYSTEMMEMORY : DDSCAPS_3DDEVICE | DDSCAPS_VIDEOMEMORY;
		lpDD4->CreateSurface(&scap, &backbuffer, 0);
		if (!backbuffer)
			return 0;
		lpDD4->QueryInterface(IID_IDirect3D3, (LPVOID*)&d3dobj);
		if (!d3dobj)
			return 0;

		if (!Devicelist[D3D_SelectedDevice].is_software)
		{
			DDSURFACEDESC2 scap2 = { 0 };
			scap2.dwSize = sizeof(scap2);
			d3dobj->EnumZBufferFormats((const IID&)Devicelist[D3D_SelectedDevice].guid, enumdepthbuf, (LPVOID)&scap2.ddpfPixelFormat);
			scap2.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
			if (!Devicelist[D3D_SelectedDevice].tri_caps)
				scap2.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | DDSCAPS_VIDEOMEMORY;
			else scap2.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | DDSCAPS_SYSTEMMEMORY;
			scap2.dwWidth = D3D_XRes;
			scap2.dwHeight = D3D_YRes;
			lpDD4->CreateSurface(&scap2, &zbuffer, 0);
			if (!zbuffer)
				return 0;
			if (FAILED(backbuffer->AddAttachedSurface(zbuffer)))
				return 0;
		}

		lpDD4->CreateClipper(0, &clipper, nullptr);
		primary->SetClipper(clipper);
	}
	else
	{
		while (ShowCursor(0) >= 0);
		SetCursor(0);
		if (FAILED(lpDD4->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_FULLSCREEN)))
			return 0;
		if (FAILED(lpDD4->SetDisplayMode(D3D_XRes, D3D_YRes, D3D_BitDepth, 0, 0)))
			return 0;

		DDSURFACEDESC2 scap = { 0 };
		scap.dwBackBufferCount = (D3D_Triplebuf != 0) + 1;
		scap.dwSize = sizeof(scap);
		scap.dwFlags = 0x21;
		scap.ddsCaps.dwCaps = 0x2218;
		lpDD4->CreateSurface(&scap, &primary, nullptr);
		if (!primary)
			return 0;

		DDSCAPS2 caps = { 0 };
		caps.dwCaps = 4;
		primary->GetAttachedSurface(&caps, &backbuffer);
		if (!backbuffer)
			return 0;
		lpDD4->QueryInterface(IID_IDirect3D3, (LPVOID*)&d3dobj);
		if (!d3dobj)
			return 0;

		DDSURFACEDESC2 scap2 = { 0 };
		scap2.dwSize = 124;
		d3dobj->EnumZBufferFormats((const IID&)Devicelist[D3D_SelectedDevice].guid, enumdepthbuf, (LPVOID)&scap2.ddpfPixelFormat);
		scap2.dwFlags = 0x1007;
		scap2.dwWidth = D3D_XRes;
		scap2.dwHeight = D3D_YRes;
		scap2.ddsCaps.dwCaps = (-(Devicelist[D3D_SelectedDevice].tri_caps != 0) & ~0x37FF) | DDSCAPS_ZBUFFER | DDSCAPS_VIDEOMEMORY;
		lpDD4->CreateSurface(&scap2, &zbuffer, 0);
		if (!zbuffer)
			return 0;
		if (FAILED(backbuffer->AddAttachedSurface(zbuffer)))
			return 0;
	}

	D3D_GammaAdjust = 0;
	if (Devicelist[D3D_SelectedDevice].can_gamma)
	{
		if (SUCCEEDED(primary->QueryInterface(IID_IDirectDrawGammaControl, (LPVOID*)&gamma)))
			D3D_GammaAdjust = 1;
	}

	d3dobj->CreateDevice((const IID&)Devicelist[D3D_SelectedDevice].pguid, backbuffer, &d3ddev, nullptr);
	if (d3ddev == nullptr)
		return 0;

	D3DDEVICEDESC desc = {0};
	desc.dwSize = sizeof(desc);
	memset(&D3D_DeviceDesc, 0, sizeof(desc));
	D3D_DeviceDesc.dwSize = sizeof(desc);

	LPDDCAPS pcaps;
	if (Devicelist[D3D_SelectedDevice].tri_caps)
	{
		d3ddev->GetCaps(&desc, &D3D_DeviceDesc);
		pcaps = nullptr;
	}
	else
	{
		d3ddev->GetCaps(&D3D_DeviceDesc, &desc);
		pcaps = &DD_Caps;
		memset(&DD_Caps, 0, sizeof(DD_Caps));
		DD_Caps.dwSize = sizeof(DD_Caps);
	}

	DDCAPS caps = { 0 };
	caps.dwSize = sizeof(caps);
	lpDD4->GetCaps(&caps, pcaps);

	if (!(D3D_DeviceDesc.dpcTriCaps.dwRasterCaps & 0x80))
		D3D_UseVertexFog = 0;
	else
	{
		DBG_Print("Using vertex fog.\n");
		D3D_UseVertexFog = 1;
	}
	if ((DD_Caps.ddsCaps.dwCaps & 0x400000) != 0)
	{
		DBG_Print("Device has mipmap support.\n");
		D3D_MipMapSupport = 1;
	}
	else
	{
		D3D_MipMapSupport = 0;
	}
	if (Devicelist[D3D_SelectedDevice].tri_caps)
	{
		D3D_AdaptivePerspec = 1;
		D3D_SoftwareDevice = 1;
	}
	else
	{
		D3D_AdaptivePerspec = 0;
		D3D_SoftwareDevice = 0;
	}

	D3D_EnumTextureTypes(d3ddev);
	TEXTURE_TYPE* t = Texturetypelist;
	int rgb = -1, found = -1;
	for (DWORD i = 0; i < D3D_NumTextureTypes; i++)
	{
		if (t->bits_alpha)
		{
			int _rgb = t->bits_red + t->bits_green + t->bits_blue;
			if (rgb == -1 || (_rgb <= 15 || rgb < 12) && (_rgb >= 12 && rgb > 16 || _rgb >= rgb))
			{
				rgb = _rgb;
				found = i;
			}
		}
	}

	D3DTEX_SetTextureFormat(&Texturetypelist[found].pfmt);
	if (D3DTEX_Init(D3D_SoftwareDevice))
	{
		d3dobj->CreateViewport(&viewport, nullptr);
		d3ddev->AddViewport(viewport);

		D3DVIEWPORT2 vp2 = { 0 };
		vp2.dwSize = sizeof(vp2);
		vp2.dwWidth = D3D_XRes;
		vp2.dwHeight = D3D_YRes;
		vp2.dwSize = 44;
		vp2.dvClipX = -1.f;
		vp2.dvClipWidth = 2.f;
		vp2.dvClipY = 1.f;
		vp2.dvClipHeight = 2.f;
		vp2.dvMaxZ = 1.f;
		viewport->SetViewport2(&vp2);
		d3ddev->SetCurrentViewport(viewport);

		d3ddev->SetRenderState(D3DRENDERSTATE_CULLMODE, 1);
		d3ddev->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
		d3ddev->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, 1);
		d3ddev->SetRenderState(D3DRENDERSTATE_ZFUNC, 2);
		if (D3D_UseVertexFog)
			d3ddev->SetRenderState(D3DRENDERSTATE_FOGENABLE, 1);
		d3ddev->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, 1);

		if (D3D_Filter)
			d3ddev->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
		else d3ddev->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_POINT);

		D3DSHL_Initialize(backbuffer, Devicelist[D3D_SelectedDevice].is_software);

		D3D_NearClip = 0.000099999997f;
		D3D_FarClip = 1.f;
		D3D_LeftClip = 0.f;
		D3D_TopClip = 0.f;
		D3D_RightClip = (float)D3D_XRes;
		D3D_BottomClip = (float)D3D_YRes;
		D3D_ClipPlaneMask = 0;
		D3D_XCenter = D3D_RightClip * 0.5f;
		D3D_YCenter = D3D_BottomClip * 0.5f;

		D3D_Clear();
		D3D_Flip();
		D3D_Clear();
		return 1;
	}

	return 0;
}
//0001:00075080       _D3D_Init                  00476080 f   rnd_d3d.obj
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
		if (SUCCEEDED(d3ddev->BeginScene()))
			D3D_InScene = 1;
		return 1;
	}
	return result;
}
//0001:00075130       _D3D_Pause                 00476130 f   rnd_d3d.obj
void __cdecl D3D_Pause()
{
	if (hWnd) ShutdownDevice();
}
//0001:00075140       _D3D_ReInit                00476140 f   rnd_d3d.obj
void __cdecl D3D_ReInit()
{
	if (hWnd)
	{
		ShutdownDevice();
		D3D_Windowed = d3d_vm->Is_windowed;
		D3D_XRes = d3d_vm->Screen_width;
		D3D_YRes = d3d_vm->Screen_height;
		D3D_BitDepth = d3d_vm->Screen_depth;
		D3D_SelectedDevice = d3d_vm->Render_device_id;
		D3D_Triplebuf = d3d_vm->Triple_buffer;
		D3D_VSync = d3d_vm->VSync;
		D3D_Filter = d3d_vm->Filter;
		if (InitialiseDevice())
			d3d_vm->Gamma_level = D3D_SetGammaNormalized(d3d_vm->Gamma_level);
	}
}
//0001:000751d0       _D3D_Shutdown              004761d0 f   rnd_d3d.obj
void __cdecl D3D_Shutdown()
{
	D3D_FreeBuckets();
	ShutdownDevice();
	hWnd = nullptr;
}
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
void __cdecl D3D_SetFog(int r, int g, int b, float fogNear, float fogFar)
{
	if (d3ddev)
	{
		d3d_fogtbl[D3D_CurFogUnit].near = fogNear;
		d3d_fogtbl[D3D_CurFogUnit].far = fogFar;
		d3d_fogtbl[D3D_CurFogUnit].col = b | ((g | (r << 8)) << 8);
		D3D_CurFogUnit++;
		D3D_DrawAllBuckets();
		D3D_ClearAllBuckets();

		D3D_FogColor = d3d_fogtbl[D3D_CurFogUnit].col;
		D3D_FogFar = d3d_fogtbl[D3D_CurFogUnit].far;
		D3D_FogNear = d3d_fogtbl[D3D_CurFogUnit].near;
		if (D3D_UseVertexFog)
		{
			d3ddev->SetRenderState(D3DRENDERSTATE_FOGCOLOR, D3D_FogColor);
			D3D_FogZScale = 254.f / (D3D_FogFar - D3D_FogNear);
		}
	}
}
//0001:000753f0       _D3D_Clear                 004763f0 f   rnd_d3d.obj
void __cdecl D3D_Clear()
{
	D3DRECT rect;
	rect.x1 = 0;
	rect.y1 = 0;
	rect.x2 = D3D_XRes;
	rect.y2 = D3D_YRes;

	if (d3ddev)
		viewport->Clear2(1, &rect, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3D_bgcol, 1.f, 0);
}
//0001:00075480       _D3D_ClearZBuffer          00476480 f   rnd_d3d.obj
void __cdecl D3D_ClearZBuffer()
{
	D3DRECT rect;
	rect.x1 = 0;
	rect.y1 = 0;
	rect.x2 = D3D_XRes;
	rect.y2 = D3D_YRes;

	if (d3ddev)
		viewport->Clear2(1, &rect, D3DCLEAR_ZBUFFER, D3D_bgcol, 1.f, 0);
}
//0001:00075510       _D3D_Flip                  00476510 f   rnd_d3d.obj
void __cdecl D3D_Flip()
{
	if (d3ddev == nullptr) return;

	if (D3DSHL_UsedThisFrame(1))
	{
		if (D3D_InScene)
		{
			d3ddev->EndScene();
			D3D_InScene = 0;
		}
		D3DSHL_BlitToBuffer(backbuffer, Devicelist[D3D_SelectedDevice].is_software);
		if (D3D_InScene == 0 && SUCCEEDED(d3ddev->BeginScene()))
			D3D_InScene = 1;
	}

	D3D_CurFogUnit = 0;
	D3D_DrawAllBuckets();
	D3D_ClearAllBuckets();
	D3D_DrawTransBucket();
	++D3D_CurrentFrame;
	if (D3D_InScene)
	{
		d3ddev->EndScene();
		D3D_InScene = 0;
	}
	ASLD_ReportStatus();

	// useless GDI stuff for debugging, not needed for now
	//if ( backbuffer && fontTracker.font_buffIndex && gUseWin32Font && !backbuffer->lpVtbl->GetDC(backbuffer, &hdc) )
	//{}

	FONT_SetCursor(0, 0);
	if (primary->IsLost() == DDERR_SURFACELOST)
		primary->Restore();
	if(backbuffer->IsLost() == DDERR_SURFACELOST)
	{
		backbuffer->Restore();
		D3D_Clear();
	}

	if (primary->IsLost() != DDERR_SURFACELOST && backbuffer->IsLost() != DDERR_SURFACELOST)
	{
		if (D3D_Windowed)
		{
			clipper->SetHWnd(0, hWnd);
			RECT r0, r1;
			GetClientRect(hWnd, &r0);
			if (r0.right > r0.left && r0.bottom > r0.top)
			{
				ClientToScreen(hWnd, (LPPOINT)&r0.left);
				ClientToScreen(hWnd, (LPPOINT)&r0.right);
				r0.bottom = D3D_YRes + r0.top;
				r0.right = D3D_XRes + r0.left;
				r1.left = 0;
				r1.top = 0;
				r1.right = D3D_XRes;
				r1.bottom = D3D_YRes;
				primary->Blt(&r0, backbuffer, &r1, 0, 0);
			}
		}
		else primary->Flip(nullptr, 1);

		if (!D3D_InScene && SUCCEEDED(d3ddev->BeginScene()))
			D3D_InScene = 1;
		if (!D3D_Filter || D3D_SoftwareDevice)
		{
			d3ddev->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_POINT);
			d3ddev->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFG_POINT);
		}
		else
		{
			d3ddev->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
			d3ddev->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFG_LINEAR);
		}
	}
}
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
void __cdecl D3D_DebugDrawLine(int a, int b)
{
	if (d3ddev)
	{
		d3ddev->SetTexture(0, nullptr);
		d3ddev->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
		//d3ddev->DrawPrimitive(D3DPRIMITIVETYPE::D3DPT_LINESTRIP, 0x1C4, b, v5, 0);
	}
}
//0001:000759c0       _D3D_GetScreenShot         004769c0 f   rnd_d3d.obj
//0001:00075bc0       _DrawTPage                 00476bc0 f   rnd_d3d.obj [unused]
//0001:00075dd0       _D3D_TimeDiff              00476dd0 f   rnd_d3d.obj
unsigned int __cdecl D3D_TimeDiff(unsigned int start)
{
	LARGE_INTEGER Frequency, PerformanceCount;

	QueryPerformanceCounter(&PerformanceCount);
	QueryPerformanceFrequency(&Frequency);
	return (unsigned int)(PerformanceCount.QuadPart / (Frequency.QuadPart / 1000000) - start);
}
//0001:00075e30       _D3D_CurrentTime           00476e30 f   rnd_d3d.obj
unsigned int D3D_CurrentTime()
{
	LARGE_INTEGER Frequency, PerformanceCount;

	QueryPerformanceCounter(&PerformanceCount);
	QueryPerformanceFrequency(&Frequency);
	return (unsigned int)(PerformanceCount.QuadPart / (Frequency.QuadPart / 1000000));
}
//0001:00075e80       _D3D_Sleep                 00476e80 f   rnd_d3d.obj
void __cdecl D3D_Sleep(DWORD dwMilliseconds)
{
	Sleep(dwMilliseconds);
}

//0003:007509b8       _TRANS_DoTransform         00c409b8     rnd_d3d.obj
//0003:00750b78       _D3D_XCenter               00c40b78     rnd_d3d.obj
//0003:00750b7c       _D3D_AdaptivePerspec       00c40b7c     rnd_d3d.obj
//0003:00750b80       _D3D_Filter                00c40b80     rnd_d3d.obj
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
//0003:00750cdc       _D3D_GammaAdjust           00c40cdc     rnd_d3d.obj
//0003:00750ce0       _D3D_InScene               00c40ce0     rnd_d3d.obj