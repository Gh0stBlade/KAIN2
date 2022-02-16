#include <windows.h>
#include <stdio.h>

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
void __cdecl D3D_FailAbort(char *fmt, ...)
{
	CHAR Text[256]; // [esp+0h] [ebp-100h] BYREF
	va_list va; // [esp+108h] [ebp+8h] BYREF

	va_start(va, fmt);
	vsprintf_s(Text, sizeof(Text), fmt, va);
	va_end(va);
	//D3D_FreeBuckets();
	//ShutdownDevice();
	//hWnd = 0;
	if (MessageBoxA(0, Text, "Kain 2 Error", 1u) == 2)
	{
		while (1)
			;
	}
	ExitProcess(0);
}

//0001:000744b0 ? enumdepthbuf@@YGJPAU_DDPIXELFORMAT@@PAX@Z 004754b0 f   rnd_d3d.obj
//0001:000744e0 ? InitialiseDevice@@YAHXZ   004754e0 f   rnd_d3d.obj
//0001:00075080       _D3D_Init                  00476080 f   rnd_d3d.obj
//0001:00075130       _D3D_Pause                 00476130 f   rnd_d3d.obj
//0001:00075140       _D3D_ReInit                00476140 f   rnd_d3d.obj
//0001:000751d0       _D3D_Shutdown              004761d0 f   rnd_d3d.obj
//0001:000751f0       _D3D_SetGammaNormalized    004761f0 f   rnd_d3d.obj
//0001:000752b0       _D3D_SetBGColor            004762b0 f   rnd_d3d.obj
//0001:000752d0       _D3D_ActivateFogUnit       004762d0 f   rnd_d3d.obj
//0001:00075330       _D3D_SetFog                00476330 f   rnd_d3d.obj
//0001:000753f0       _D3D_Clear                 004763f0 f   rnd_d3d.obj
//0001:00075480       _D3D_ClearZBuffer          00476480 f   rnd_d3d.obj
//0001:00075510       _D3D_Flip                  00476510 f   rnd_d3d.obj
//0001:000758f0       _D3D_AddTri                004768f0 f   rnd_d3d.obj
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