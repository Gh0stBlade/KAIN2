#pragma once

#include <windows.h>
#include <ddraw.h>
#include <d3d.h>
#include "d3dbuckt.h"
#include "d3dclip.h"
#include "rnd_d3d.h"

typedef struct _SystemTextureD3D
{
	DWORD age;
	DWORD clut;
	LPDIRECTDRAWSURFACE4 surface;
	IDirect3DTexture2* texture;
	struct _SystemTextureD3D* linked;
} SystemTextureD3D;

typedef struct SystemTexturePool
{
	int age;
	int clut;
	LPDIRECTDRAWSURFACE4 surface;
	IDirect3DTexture2* texture;
} SystemTexturePool;

typedef struct TEXTURE_TYPE
{
	DDPIXELFORMAT pfmt;
	int field_20;
	int bits_alpha;
	int bits_red;
	int bits_green;
	int bits_blue;
} TEXTURE_TYPE;

typedef struct D3D_RES
{
	int x;
	int y;
	int depth;
} D3D_RES;

typedef struct D3D_DEVLIST
{
	GUID* pguid0;
	GUID* pguid;
	GUID guid0;
	GUID guid;
	char desc[128];
	int tri_caps;
	int is_software;
	int field_B0;
	int res_count;
	int can_gamma;
	D3D_RES* res_list;
} D3D_DEVLIST;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern DWORD D3D_NumTextureTypes, enumerated, D3D_NumDevices, dword_C3C27C, dword_C3C284, D3D_CurrentFrame;
extern int screenmode_cnt0, screenmode_cnt1;
extern TEXTURE_TYPE Texturetypelist[64];
extern D3D_RES Screenmodelist[512];
extern D3D_DEVLIST Devicelist[32];

extern DWORD D3D_ClipPlaneMask, D3D_InverseClipPlanes;
extern float D3D_LeftClip, D3D_BottomClip, D3D_TopClip, D3D_RightClip,
	D3D_FarClip, D3D_NearClip;
extern int D3D_Windowed,
	D3D_XRes,
	D3D_YRes,
	D3D_BitDepth,
	D3D_Triplebuf,
	D3D_VSync,
	D3D_Filter,
	D3D_InScene;

extern DDPIXELFORMAT sys_texture_fmt;
extern DWORD sys_texture_amask, sys_texture_rmask, sys_texture_gmask, sys_texture_bmask;
extern int sys_texture_cnt, dev_texture_cnt;
extern SystemTextureD3D sys_textures[32];
extern SystemTexturePool sys_texture_pool[64];
extern HANDLE sys_texture_handle;
extern RTL_CRITICAL_SECTION sys_tex_csec;
extern int sys_cluts[64], sys_clut_cnt;

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif