#include <Windows.h>
#include "snd.h"

_G2AppDataVM_Type* pVM_Setup;

extern void __cdecl D3D_EnumerateDevices();
extern void __cdecl SND_EnumerateDevices();


INT_PTR CALLBACK SetupDialogProc(HWND hWnd, UINT, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int __cdecl SETUP_DoSetupDialog(_G2AppDataVM_Type* vm)
{
	int result; // eax
	HWND hWnd; // eax
	int v3; // edi

	D3D_EnumerateDevices();
	SND_EnumerateDevices();
	result = (int)localstr_load_language_pc();
	if (result)
	{
		pVM_Setup = vm;
		if (!vm->Sound_device_id)
			vm->Sound_device_id = 1;
		hWnd = GetActiveWindow();
		v3 = DialogBoxParamA(0, (LPCSTR)0x71, hWnd, SetupDialogProc, 0);
		if (v3)
		{
			HKEY key;
			if (!RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Crystal Dynamics\\Legacy of Kain: Soul Reaver\\1.00.000", 0, 0x20019u, &key))
			{
				RegSetValueExA(key, "Windowed",       0, REG_DWORD, (const BYTE*)&pVM_Setup->Is_windowed, 4);
				RegSetValueExA(key, "RenderDeviceID", 0, REG_DWORD, (const BYTE*)&pVM_Setup->Render_device_id, 4);
				RegSetValueExA(key, "SoundDeviceID",  0, REG_DWORD, (const BYTE*)&pVM_Setup->Sound_device_id, 4);
				RegSetValueExA(key, "ScreenWidth",    0, REG_DWORD, (const BYTE*)&pVM_Setup->Screen_width, 4);
				RegSetValueExA(key, "ScreenHeight",   0, REG_DWORD, (const BYTE*)&pVM_Setup->Screen_height, 4);
				RegSetValueExA(key, "ScreenDepth",    0, REG_DWORD, (const BYTE*)&pVM_Setup->Screen_depth, 4);
				RegSetValueExA(key, "TripleBuffer",   0, REG_DWORD, (const BYTE*)&pVM_Setup->Triple_buffer, 4);
				RegSetValueExA(key, "Filter",         0, REG_DWORD, (const BYTE*)&pVM_Setup->Filter, 4);
				RegSetValueExA(key, "VSync",          0, REG_DWORD, (const BYTE*)&pVM_Setup->VSync, 4);
				RegCloseKey(key);
			}
		}
		localstr_clear_language_pc();
		return v3;
	}
	return result;
}