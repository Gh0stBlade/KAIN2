#include "kain2.h"

extern void (*cb_vsync)(void);
void (*_psxEmuState)(void);
u_long primBase;

//0001:00028b10       _ClearImage                00429b10 f   libgpu.obj
int ClearImage(RECT* rect, u_char r, u_char g, u_char b)
{
	return 0;
}
//0001:00028b20       _ClearOTagR                00429b20 f   libgpu.obj
u_long* ClearOTagR(u_long* ot, int n)
{
	u_long* result; // eax
	int v3; // ecx
	int v4; // esi
	unsigned int v5; // edi

	result = ot;
	if (n > 1)
	{
		v3 = n;
		v4 = n - 1;
		do
		{
			v5 = (u_long)&ot[v3 - 2] - primBase;
			--v3;
			--v4;
			ot[v3] = v5 & 0xFFFFFF;
		} while (v4);
	}
	*ot = -1;
	return result;
}
//0001:00028b70       _DrawOTag                  00429b70 f   libgpu.obj
int DrawOTag(u_long* p)
{
	PSXEmulation_CheckForTermination();
	D3D_ParseAndDrawOTag(p);
	RendererAPSX_Flush();
	if (_psxEmuState)
		_psxEmuState();
	if (cb_vsync)
		cb_vsync();
}
//0001:00028ba0       _BreakDraw                 00429ba0 f   libgpu.obj
u_long* BreakDraw(void) { return 0; }
//0001:00028bb0       _DrawPrim                  00429bb0 f   libgpu.obj
void DrawPrim(void* p){}
//0001:00028bc0       _DrawSync                  00429bc0 f   libgpu.obj
int DrawSync(int mode) { return 0; }
//0001:00028bd0       _DrawSyncCallback          00429bd0 f   libgpu.obj
u_long DrawSyncCallback(void (*func)(void))
{
	_psxEmuState = func;
	return 1;
}
//0001:00028be0       _DumpOTag                  00429be0 f   libgpu.obj
void DumpOTag(u_long* p) {}		// this has code on pc, but it's never referenced
//0001:00028cc0       _GetTPage                  00429cc0 f   libgpu.obj
u_short GetTPage(int tp, int abr, int x, int y)
{
	return ((y & 0x100 | (x >> 2) & 0xF0) >> 4) | (4 * (y & 0x200 | (8 * ((4 * (tp & 3)) | abr & 3))));
}
//0001:00028d10       _LoadImage                 00429d10 f   libgpu.obj
int LoadImage(RECT* rect, u_long* p)
{
	return 1;
}
//0001:00028d20       _MoveImage                 00429d20 f   libgpu.obj
int MoveImage2(RECT* rect, int x, int y) { return 0; }
//0001:00028d30       _PutDispEnv                00429d30 f   libgpu.obj
DISPENV* PutDispEnv(DISPENV* env) { return 0; }
//0001:00028d40       _PutDrawEnv                00429d40 f   libgpu.obj
DRAWENV* PutDrawEnv(DRAWENV* env) { return 0; }
//0001:00028d50       _ResetGraph                00429d50 f   libgpu.obj
int ResetGraph(int mode) { return 0; }
//0001:00028d60       _SetDefDispEnv             00429d60 f   libgpu.obj
DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h) { return 0; }
//0001:00028d70       _SetDefDrawEnv             00429d70 f   libgpu.obj
DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h) { return 0; }
//0001:00028d80       _SetDispMask               00429d80 f   libgpu.obj
void SetDispMask(int mask) {}
//0001:00028d90       _SetDrawArea               00429d90 f   libgpu.obj
void SetDrawArea(DR_AREA* p, RECT* r) {}
//0001:00028da0       _SetDrawTPage              00429da0 f   libgpu.obj
void SetDrawTPage(DR_TPAGE* p, int dfe, int dtd, int tpage) {}
//0001:00028db0       _SetGraphDebug             00429db0 f   libgpu.obj
int SetGraphDebug(int level) { return 0; }
//0001:00028dc0       _SetPolyFT4                00429dc0 f   libgpu.obj
void SetPolyFT4(POLY_FT4* p) {}
//0001:00028dd0       _StoreImage                00429dd0 f   libgpu.obj
int StoreImage(RECT* rect, u_long* p) { return 0; }