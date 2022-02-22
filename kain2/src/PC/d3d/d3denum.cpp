#include "d3d.h"
#include <stdlib.h>
#include <stdio.h>

DWORD D3D_NumTextureTypes, enumerated, D3D_NumDevices, dword_C3C27C, dword_C3C284;
int screenmode_cnt0, screenmode_cnt1;
TEXTURE_TYPE Texturetypelist[64];
D3D_RES Screenmodelist[512];
D3D_DEVLIST Devicelist[32];
void __cdecl D3D_FailAbort(const char* fmt, ...);
HRESULT WINAPI d3denumcallback(_GUID* lpGUID, LPSTR lpszDeviceDesc, LPSTR lpszDeviceName, LPD3DDEVICEDESC lpd3dHWDeviceDesc, LPD3DDEVICEDESC lpd3dSWDeviceDesc, void* ctx);

//0001:00070a40 ? EnumScrModesCallback@@YGJPAU_DDSURFACEDESC@@PAX@Z 00471a40 f   d3denum.obj
HRESULT WINAPI EnumScrModesCallback(DDSURFACEDESC* desc, LPVOID ctx)
{
	Screenmodelist[screenmode_cnt0].x = desc->dwWidth;
	Screenmodelist[screenmode_cnt0].y = desc->dwHeight;
	Screenmodelist[screenmode_cnt0].depth = desc->ddpfPixelFormat.dwRGBBitCount;
	screenmode_cnt0++;
	screenmode_cnt1++;
	return 1;
}

//0001:00070a90 ? enumtextures@@YGJPAU_DDPIXELFORMAT@@PAX@Z 00471a90 f   d3denum.obj
int count_bits(DWORD mask)
{
	int count;
	for (count = 0; mask; mask >>= 1)
		if ((mask & 1) != 0)
			++count;
	return count;
}

HRESULT WINAPI enumtextures(struct _DDPIXELFORMAT* fmt, LPVOID ctx)
{
	const DWORD mask0 = (DDPF_PALETTEINDEXED2 | DDPF_PALETTEINDEXED1);
	const DWORD mask1 = (DDPF_PALETTEINDEXEDTO8 | DDPF_PALETTEINDEXED8 | DDPF_PALETTEINDEXED4);
	const DWORD mask2 = (DDPF_RGB);

	if ((fmt->dwFlags & mask0) == 0 && (fmt->dwFlags & mask1) == 0 && (fmt->dwFlags & mask2) != 0)
	{
		auto t = &Texturetypelist[D3D_NumTextureTypes];
		memcpy(&t->pfmt, fmt, sizeof(*fmt));

		if (fmt->dwFlags & DDPF_ALPHAPIXELS)
			t->bits_alpha = count_bits(fmt->dwRGBAlphaBitMask);
		else t->bits_alpha = 0;

		t->bits_red = count_bits(fmt->dwRBitMask);
		t->bits_green = count_bits(fmt->dwGBitMask);
		t->bits_blue = count_bits(fmt->dwBBitMask);

		D3D_NumTextureTypes++;
	}

	return 1;
}

//0001:00070be0 ? DDEnumCallback@@YGHPAU_GUID@@PAD1PAX2@Z 00471be0 f   d3denum.obj
DWORD D3D_CanGamma;
char dev_name[256];
D3D_RES* pScreenmodelist;

BOOL WINAPI DDEnumCallback(GUID *guid, LPSTR name, LPSTR, LPVOID ctx, HMONITOR)
{
	strcpy_s(dev_name, sizeof(dev_name), name);
	screenmode_cnt1 = 0;
	pScreenmodelist = &Screenmodelist[screenmode_cnt0];

	LPDIRECTDRAW lpDD;
	if (SUCCEEDED(DirectDrawCreate(guid, &lpDD, nullptr)))
	{
		D3D_CanGamma = 0;
		// test if gamma ramp is available (so much code, oof)
		LPDIRECTDRAW4 lpDD4;
		if (SUCCEEDED(lpDD->QueryInterface(IID_IDirectDraw4, (LPVOID*)&lpDD4)))
		{
			DDCAPS caps4 = { 0 };
			caps4.dwSize = sizeof(caps4);
			if (SUCCEEDED(lpDD4->GetCaps(&caps4, nullptr)))
			{
				if ((caps4.dwCaps2 & DDCAPS2_PRIMARYGAMMA) != 0)
					D3D_CanGamma = 1;
			}
			lpDD4->Release();
		}

		// enumerate resolutions
		DDSURFACEDESC ddesc = { 0 };
		ddesc.dwSize = sizeof(ddesc);
		ddesc.dwFlags = DDSD_CAPS;
		ddesc.ddsCaps.dwCaps = DDSCAPS_3DDEVICE;
		lpDD->EnumDisplayModes(0, &ddesc, nullptr, EnumScrModesCallback);

		// enumerate 3d devices
		LPDIRECT3D3 lpD3D3;
		if (SUCCEEDED(lpDD->QueryInterface(IID_IDirect3D3, (LPVOID*)&lpD3D3)))
		{
			if(guid)
				lpD3D3->EnumDevices(d3denumcallback, guid);
			lpD3D3->Release();
		}

		lpDD->Release();
	}
	return 1;
}
//0001:00070d80 ? d3denumcallback@@YGJPAU_GUID@@PAD1PAU_D3DDeviceDesc@@2PAX@Z 00471d80 f   d3denum.obj
HRESULT WINAPI d3denumcallback(_GUID* lpGUID, LPSTR lpszDeviceDesc, LPSTR lpszDeviceName, LPD3DDEVICEDESC lpd3dHWDeviceDesc, LPD3DDEVICEDESC lpd3dSWDeviceDesc, void* ctx)
{
	if (((lpd3dHWDeviceDesc->dcmColorModel & D3DCOLOR_RGB) != 0 || (lpd3dSWDeviceDesc->dcmColorModel & D3DCOLOR_RGB) != 0)
		&& (lpd3dHWDeviceDesc->dwFlags & D3DDD_TRICAPS) != 0)
	{
		auto dev = &Devicelist[D3D_NumDevices];
		if (ctx)
		{
			dev->guid0 = *(GUID*)ctx;
			dev->pguid0 = &dev->guid0;
		}
		else dev->pguid0 = nullptr;

		if ((lpd3dHWDeviceDesc->dwFlags & D3DDD_TRICAPS) != 0)
			dev->tri_caps = 0;
		else
		{
			lpd3dHWDeviceDesc = lpd3dSWDeviceDesc;
			dev->tri_caps = 1;
		}

		dev->field_B0 = 1;
		dev->is_software = false;
		dev->can_gamma = D3D_CanGamma;
		dev->guid = *lpGUID;
		dev->pguid = &dev->guid;
		dev->res_list = &Screenmodelist[screenmode_cnt0];
		auto dev_res = dev->res_list;
		if (screenmode_cnt1 > 0)
		{
			auto bitDepth = lpd3dHWDeviceDesc->dwDeviceRenderBitDepth;
			auto res = pScreenmodelist;
			for (int i = 0; i < screenmode_cnt1; i++, res++)
			{
				auto depth = res->depth;
				if ((depth != 32 || (bitDepth & 0x100) != 0)
					&& (depth != 24 || (bitDepth & 0x200) != 0)
					&& (depth != 16 || (bitDepth & 0x400) != 0)
					&& depth != 8)
				{
					dev->res_count++;
					dev_res->x = res->x;
					dev_res->y = res->y;
					dev_res->depth = res->depth;
					dev_res++;
					++screenmode_cnt0;
				}
			}
		}

		sprintf_s(dev->desc, sizeof(dev->desc), "%s : %s", dev_name, lpszDeviceDesc);
		D3D_NumDevices++;
	}
	return 1;
}

//0001:00070b60       _D3D_EnumTextureTypes      00471b60 f   d3denum.obj
void __cdecl D3D_EnumTextureTypes(IDirect3DDevice3* dev)
{
	D3D_NumTextureTypes = 0;
	dev->EnumTextureFormats(enumtextures, nullptr);
}
//0001:00070b80       _D3D_EnumerateDevices      00471b80 f   d3denum.obj
void __cdecl D3D_EnumerateDevices()
{
	if (!enumerated)
	{
		dword_C3C27C = 1;
		D3D_NumDevices = 0;
		dword_C3C284 = 0;
		DirectDrawEnumerateExA(DDEnumCallback, nullptr, DDENUM_ATTACHEDSECONDARYDEVICES | DDENUM_DETACHEDSECONDARYDEVICES | DDENUM_NONDISPLAYDEVICES);
		if (!D3D_NumDevices)
			D3D_FailAbort("This game requires DirectX6");
		enumerated = 1;
	}
}
