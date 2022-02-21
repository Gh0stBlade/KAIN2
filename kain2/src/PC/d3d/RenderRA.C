#include "../../core.h"
#include "../snd.h"

int RenderOptions,
	LastRenderOptions,
	RenderResolution,
	LastRenderResolution;

typedef struct D3D_RES
{
	int x;
	int y;
	int depth;
} D3D_RES;

typedef struct D3D_DEVLIST
{
	int field_0;
	GUID* pguid;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	GUID guid;
	char field_28[128];
	int field_A8;
	int is_software;
	int field_B0;
	int res_count;
	int is_ddraw6;
	D3D_RES* res_list;
} D3D_DEVLIST;

extern _G2AppDataVM_Type appDataVM;
extern D3D_DEVLIST Devicelist[];
extern void __cdecl D3D_FailAbort(char* fmt, ...);

#define ROPT_BILINEAR			0x01
#define ROPT_UNK1				0x02
#define ROPT_UNK2				0x04
#define ROPT_32BIT				0x08
#define ROPT_TRIPLEBUFFERING	0x10
#define ROPT_VSYNC				0x20

//0001 : 00073fa0       _RenderG2_Init             00474fa0 f   RenderRA.obj
int __cdecl RenderG2_Init(_G2AppDataVM_Type* vm)
{
	return D3D_Init(vm);
}
//0001 : 00073fb0       _RenderG2_ReInit           00474fb0 f   RenderRA.obj
int RenderG2_ReInit()
{
	return D3D_ReInit();
}
//0001 : 00073fc0       _RenderG2_ShutDown         00474fc0 f   RenderRA.obj
void RenderG2_ShutDown(void)
{
	D3D_Shutdown();
}
//0001 : 00073fd0       _RenderG2_Pause            00474fd0 f   RenderRA.obj
void RenderG2_Pause()
{
	D3D_Pause();
}
//0001 : 00073fe0       _RenderG2_GetRenderOptions 00474fe0 f   RenderRA.obj
BYTE __cdecl RenderG2_GetRenderOptions()
{
	BYTE result = 0;

	if (!Devicelist[appDataVM.Render_device_id].field_A8)
		result |= ROPT_UNK1;
	if (Devicelist[appDataVM.Render_device_id].field_0)
		result |= ROPT_UNK2;
	if (appDataVM.Filter)
		result |= ROPT_BILINEAR;
	if (appDataVM.Triple_buffer)
		result |= ROPT_TRIPLEBUFFERING;
	if (appDataVM.VSync)
		result |= ROPT_VSYNC;
	if (appDataVM.Screen_depth == 32)
		result |= ROPT_32BIT;

	return result;
}
//0001 : 00074040       _RenderG2_SetRenderOptions 00475040 f   RenderRA.obj
void __cdecl RenderG2_SetRenderOptions(BYTE a1)
{
}
//0001 : 000742a0       _RenderG2_SetResolution    004752a0 f   RenderRA.obj
int __cdecl RenderG2_SetResolution(int res)
{
	int res_count; // eax
	D3D_RES* res_list; // eax
	_G2AppDataVM_Type bk; // [esp+Ch] [ebp-34h] BYREF

	res_count = Devicelist[appDataVM.Render_device_id].res_count;
	qmemcpy(&bk, &appDataVM, sizeof(bk));
	if (res >= res_count)
		res = res_count - 1;
	D3D_Pause();
	res_list = Devicelist[appDataVM.Render_device_id].res_list;
	appDataVM.Screen_width = res_list[res].x;
	appDataVM.Screen_height = res_list[res].y;
	if (!D3D_ReInit())
	{
		qmemcpy(&appDataVM, &bk, sizeof(appDataVM));
		if (!D3D_ReInit())
			D3D_FailAbort("RNDD3D failed to reinitialise");
	}
	return res;
}
//0001 : 00074340       _RenderG2_GetResolution    00475340 f   RenderRA.obj
int RenderG2_GetResolution()
{
	return FindResolution(appDataVM.Render_device_id, appDataVM.Screen_width, appDataVM.Screen_height);
}
//0001 : 00074360       _RenderG2_Clear            00475360 f   RenderRA.obj
void __cdecl RenderG2_Clear()
{
	D3D_Clear();
}
//0001 : 00074370       _RenderG2_SetBGColor       00475370 f   RenderRA.obj
int __cdecl RenderG2_SetBGColor(int r, int g, int b)
{
	return D3D_SetBGColor(r, g, b);
}
//0001 : 00074390       _RenderG2_SetFog           00475390 f   RenderRA.obj
extern float D3D_FarClip;

void __cdecl RenderG2_SetFog(int r, int g, int b, int fogNear, int fogFar)
{
	D3D_SetFog(r, g, b, fogNear / 2000.f, fogFar / 2000.f);
	D3D_FarClip = fogFar / 2000.f;
}
//0001 : 000743e0       _RenderG2_GetScreenShot    004753e0 f   RenderRA.obj
void __cdecl RenderG2_GetScreenShot(BYTE* dest)
{
	D3D_GetScreenShot(dest);
}
//0001 : 000743f0       _RenderG2_Swap             004753f0 f   RenderRA.obj
void RenderG2_Swap(void)
{
	D3D_Flip();
}
//0001 : 00074400       _RenderG2_FlushDraw        00475400 f   RenderRA.obj
void __cdecl RenderG2_FlushDraw()
{
	D3D_DrawAllBuckets();
	D3D_ClearAllBuckets();
	D3D_DrawTransBucket();
}