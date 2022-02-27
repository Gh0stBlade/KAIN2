#include "d3d.h"
#include "rnd_d3d.h"
#include <stdio.h>
#include "../async.h"

DDPIXELFORMAT sys_texture_fmt;
DWORD sys_texture_amask, sys_texture_rmask, sys_texture_gmask, sys_texture_bmask, sys_tex_status;
int sys_texture_cnt, dev_texture_cnt;
SystemTextureD3D sys_textures[32];
SystemTexturePool sys_texture_pool[64];
HANDLE sys_texture_handle;
RTL_CRITICAL_SECTION sys_tex_csec;

int sys_cluts[64],
	sys_clut_cnt;

void DBG_Print(const char* fmt, ...);
void LoadSystemTexture(int clut, unsigned __int16* data);

extern DWORD D3D_MipMapSupport;
extern LPDIRECTDRAW4 lpDD4;
extern LPDIRECT3DDEVICE3 d3ddev;

//0001:000717e0       _D3DTEX_SetTextureFormat   004727e0 f   d3dtextr.obj
void D3DTEX_SetTextureFormat(DDPIXELFORMAT* fmt)
{
	memcpy(&sys_texture_fmt, fmt, sizeof(sys_texture_fmt));
	sys_texture_rmask = (fmt->dwRBitMask << 8) / 0x7C00;
	sys_texture_gmask = (fmt->dwGBitMask << 8) / 0x3E0;
	sys_texture_bmask = (fmt->dwBBitMask << 8) / 0x1F;
	sys_texture_amask = fmt->dwRGBAlphaBitMask >> 15;
}
//0001:00071860       _D3DTEX_Save               00472860 f   d3dtextr.obj
void D3DTEX_Save()
{

}
//0001:000718b0       _D3DTEX_Restore            004728b0 f   d3dtextr.obj [unused]
//0001:00071940       _D3DTEX_Init               00472940 f   d3dtextr.obj
int D3DTEX_Init(int is_software)
{
	return 1;
}
//0001:00071a80       _D3DTEX_Shutdown           00472a80 f   d3dtextr.obj
int D3DTEX_Shutdown()
{
	if (d3ddev && sys_tex_status)
	{
		EnterCriticalSection(&sys_tex_csec);
		d3ddev->SetTexture(0, nullptr);

		sys_clut_cnt = 0;
		for (int i = 0; i < sys_texture_cnt; i++, sys_clut_cnt++)
		{
			if (sys_texture_pool[i].clut != -1)
			{
				sys_cluts[i] = sys_texture_pool[i].clut;
				sys_texture_pool[i].clut = -1;
			}
		}

		for (int i = 0; i < sys_texture_cnt; i++)
		{
			if (sys_texture_pool[i].clut != -1)
				sys_cluts[i] = sys_texture_pool[i].clut;
			sys_texture_pool[i].texture->Release();
			sys_texture_pool[i].texture = nullptr;
			sys_texture_pool[i].surface->Release();
			sys_texture_pool[i].surface = nullptr;
		}
		sys_texture_cnt = 0;

		for (int i = 0; i < dev_texture_cnt; i++)
		{
			sys_textures[i].texture->Release();
			sys_textures[i].texture = nullptr;
			sys_textures[i].surface->Release();
			sys_textures[i].surface = nullptr;
			sys_textures[i].clut = -1;
			sys_textures[i].linked = nullptr;
		}
		dev_texture_cnt = 0;
		sys_tex_status = 0;
		ASLD_CloseFile(sys_texture_handle);
		LeaveCriticalSection(&sys_tex_csec);
	}

	return 1;
}
//0001:00071bc0       _D3DTEX_CreateSystemTextures 00472bc0 f   d3dtextr.obj
int D3DTEX_CreateSystemTextures()
{
	sys_texture_cnt = 0;
	
	DDSURFACEDESC2 desc = { 0 };
	desc.dwSize = sizeof(desc);
	if (D3D_MipMapSupport)
	{
		desc.dwFlags = 0x21007;
		desc.ddsCaps.dwCaps = 0x401808;
		desc.dwMipMapCount = 1;
	}
	else
	{
		desc.dwFlags = 0x1007;
		desc.ddsCaps.dwCaps = 0x1800;
	}
	desc.dwWidth = 256;
	desc.dwHeight = 256;
	memcpy(&desc.ddpfPixelFormat, &sys_texture_fmt, sizeof(desc.ddpfPixelFormat));

	for (int i = 0; i < 64; i++)
	{
		if (FAILED(lpDD4->CreateSurface(&desc, &sys_texture_pool[i].surface, nullptr)))
			break;
		sys_texture_pool[i].surface->QueryInterface(IID_IDirect3DTexture2, (LPVOID*)&sys_texture_pool[i].texture);
		sys_texture_pool[i].age = 0;
		sys_texture_pool[i].clut = -1;
		sys_texture_cnt++;
	}

	DBG_Print("Number of systemtextures: %i\n", sys_texture_cnt);
	return sys_texture_cnt;
}
//0001:00071cd0       _D3DTEX_CreateDeviceTextures 00472cd0 f   d3dtextr.obj
int D3DTEX_CreateDeviceTextures()
{
	dev_texture_cnt = 0;

	DDSURFACEDESC2 desc = { 0 };
	desc.dwSize = sizeof(desc);
	if (D3D_MipMapSupport)
	{
		desc.dwFlags = 0x21007;
		desc.ddsCaps.dwCaps = 0x405008;
		desc.dwMipMapCount = 1;
	}
	else
	{
		desc.dwFlags = 0x1007;
		desc.ddsCaps.dwCaps = 0x5000;
	}
	desc.dwWidth = 256;
	desc.dwHeight = 256;
	memcpy(&desc.ddpfPixelFormat, &sys_texture_fmt, sizeof(desc.ddpfPixelFormat));

	for (int i = 0; i < 64; i++)
	{
		if (FAILED(lpDD4->CreateSurface(&desc, &sys_textures[i].surface, nullptr)))
			break;
		sys_textures[i].surface->QueryInterface(IID_IDirect3DTexture2, (LPVOID*)&sys_textures[i].texture);
		sys_textures[i].age = 0;
		sys_textures[i].clut = -1;
		dev_texture_cnt++;
	}

	DBG_Print("Number of devicetextures: %i\n", dev_texture_cnt);
	return dev_texture_cnt;
}
//0001:00071de0       ?TexturePreloaded@@YAXPAXH0@Z 00472de0 f   d3dtextr.obj
int dword_C40998;
void TexturePreloaded(unsigned __int16* data, signed int read_size, int clut)
{
	if (read_size >= 0x20000)
	{
		EnterCriticalSection(&sys_tex_csec);
		if (dword_C40998)
		{
			for (int i = 0; i < sys_texture_cnt; i++)
			{
				if (sys_texture_pool[i].clut == clut)
				{
					if (data)
						LoadSystemTexture(clut, data);
					break;
				}
			}
			ASLD_Free(data);
		}
		else
		{
			sys_cluts[sys_clut_cnt] = clut;
			sys_clut_cnt++;
			ASLD_Free(data);
		}
		LeaveCriticalSection(&sys_tex_csec);
	}
}
//0001:00071e70 ?LoadSystemTexture@@YAPAUSystemTextureD3D@@HPAG@Z 00472e70 f   d3dtextr.obj
void LoadSystemTexture(int clut, unsigned __int16* data)
{}
//0001:00072100       _D3DTEX_PreloadTexture     00473100 f   d3dtextr.obj
//extern int D3D_CurrentFrame;

void D3DTEX_PreloadTexture(int clut)
{
	int i;
	for (i = 0; i < sys_texture_cnt; i++)
	{
		if (sys_texture_pool[i].clut == clut)
		{
			ASLD_RequestFileDataFP(sys_texture_handle, (clut << 17) + 4096, 0x20000, (void(*)(HGLOBAL, DWORD, int))TexturePreloaded, clut, 1);
		}
	}

	sys_texture_pool[i].age = D3D_CurrentFrame;
}
//0001:00072160       _D3DTEX_DumpTexture        00473160 f   d3dtextr.obj
void D3DTEX_DumpTexture(int clut)
{}
//0001:000721e0       ?GetSystemTexture@@YAPAUSystemTextureD3D@@H@Z 004731e0 f   d3dtextr.obj
SystemTexturePool* GetSystemTexture(int clut)
{
	return nullptr;
}
//0001:000722a0       _D3DTEX_SetActiveTexture   004732a0 f   d3dtextr.obj
void D3DTEX_SetActiveTexture(int clut)
{}
//0001:000726d0 ?LoadAndActivateIntoDevice@@YAXPAUSystemTextureD3D@@@Z 004736d0 f   d3dtextr.obj
void LoadAndActivateIntoDevice(SystemTextureD3D* a1)
{}