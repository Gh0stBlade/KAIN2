#include "../core.h"
#include <direct.h>
#include "snd.h"

_G2AppDataVM_Type* pVM;
static _G2AppDataVM_Type _appDataVM;
static LPSTR lpClass = (LPSTR)"";

//0001:00078b30       _PSXEmulation_CheckForTermination 00479b30 f   MainVM.obj
//0001:00078b40       _InitToolhelp32            00479b40 f   MainVM.obj
//0001:00078c10       _FailMsg                   00479c10 f   MainVM.obj
//0001:00078c60       _RetryMsg                  00479c60 f   MainVM.obj
//0001:00078cc0       _WinMain@16                00479cc0 f   MainVM.obj
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_appDataVM.hInstance = hInstance;
	_appDataVM.hWindow = 0;
	_appDataVM.Is_windowed = 1;
	_appDataVM.Render_device_id = 0;
	_appDataVM.Screen_width = 640;
	_appDataVM.Screen_height = 480;
	_appDataVM.Screen_depth = 16;
	_appDataVM.Gamma_level = 5;
	_appDataVM.Triple_buffer = 0;
	_appDataVM.Filter = 1;
	_appDataVM.Sound_device_id = 0;
	_appDataVM.VSync = 0;
	HKEY key;
	DWORD type, cbData;
	RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Crystal Dynamics\\Legacy of Kain: Soul Reaver\\1.00.000", 0, lpClass, 0, KEY_READ, nullptr, &key, nullptr);
	RegQueryValueExA(key, "Windowed",       0, &type, (LPBYTE)&_appDataVM.Is_windowed, &cbData);
	RegQueryValueExA(key, "RenderDeviceID", 0, &type, (LPBYTE)&_appDataVM.Render_device_id, &cbData);
	RegQueryValueExA(key, "SoundDeviceID",  0, &type, (LPBYTE)&_appDataVM.Sound_device_id, &cbData);
	RegQueryValueExA(key, "ScreenWidth",    0, &type, (LPBYTE)&_appDataVM.Screen_width, &cbData);
	RegQueryValueExA(key, "ScreenHeight",   0, &type, (LPBYTE)&_appDataVM.Screen_height, &cbData);
	RegQueryValueExA(key, "ScreenDepth",    0, &type, (LPBYTE)&_appDataVM.Screen_depth, &cbData);
	RegQueryValueExA(key, "TripleBuffer",   0, &type, (LPBYTE)&_appDataVM.Triple_buffer, &cbData);
	RegQueryValueExA(key, "Filter",         0, &type, (LPBYTE)&_appDataVM.Filter, &cbData);
	RegQueryValueExA(key, "VSync",          0, &type, (LPBYTE)&_appDataVM.VSync, &cbData);
	RegCloseKey(key);

	char path[MAX_PATH];
	GetModuleFileNameA(hInstance, path, MAX_PATH);
	*strrchr(path, '\\') = '\0';
	_chdir(path);
	DWORD PID = GetCurrentProcessId();

	return 0;
}
//0001:00079400       _MainG2_UpdateLoop         0047a400 f   MainVM.obj
DWORD dword_C65188;

int MainG2_UpdateLoop()
{
	MSG Msg;
	//InputG2_Update();

	if (!PeekMessageA(&Msg, 0, 0, 0, 0))
		return 1;
	while (1)
	{
		auto MessageA = GetMessageW(&Msg, 0, 0, 0);
		TranslateMessage(&Msg);
		DispatchMessageW(&Msg);
		if (!MessageA)
			break;
		if (!dword_C65188)
			WaitMessage();
		if (!PeekMessageW(&Msg, 0, 0, 0, 0))
			return 1;
	}
	return 0;
}
//0001:000794b0       _MainG2_TriggerExit        0047a4b0 f   MainVM.obj

LRESULT WINAPI ViewportW32_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_ACTIVATE:
		break;
	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case 0xE000:
		case 0xE001:
			break;
		}
		break;
	case WM_MOUSEFIRST:
		break;
	case WM_CLOSE:
		//g_shutdownInProgress = 1;
		ShowWindow(hWnd, SW_HIDE);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

//0001 : 0007a7e0       _ViewportG2_Init           0047b7e0 f   ViewVM.obj
int __cdecl ViewportG2_Init(_G2AppDataVM_Type* vm)
{
	WNDCLASSW WndClass = { 0 };

	pVM = vm;
	WndClass.lpfnWndProc = ViewportW32_WndProc;
	WndClass.hInstance = vm->hInstance;
	WndClass.hIcon = LoadIconW(vm->hInstance, (LPCWSTR)0x67);
	WndClass.hCursor = LoadCursorW(nullptr, (LPCWSTR)0x7F00);
	WndClass.hbrBackground = 0;
	WndClass.lpszClassName = L"G2WndClass";
	RegisterClassW(&WndClass);
	vm->hWindow = CreateWindowExW(WS_EX_TOPMOST, L"G2WndClass", L"Legacy of Kain: Soul Reaver",
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		vm->Screen_width, vm->Screen_height,
		nullptr, nullptr, vm->hInstance, nullptr);
	ShowWindow(vm->hWindow, 3);
	return 1;
}
//0001 : 0007a890       _ViewportG2_ShutDown       0047b890 f   ViewVM.obj
void __cdecl ViewportG2_ShutDown(_G2AppDataVM_Type* vm)
{
	DestroyWindow(vm->hWindow);
	vm->hWindow = nullptr;
}
