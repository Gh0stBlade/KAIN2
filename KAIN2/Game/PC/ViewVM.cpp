#include "../core.h"
#include <direct.h>
#include "snd.h"
#include "d3d/d3d.h"

_G2AppDataVM_Type* pVM;
_G2AppDataVM_Type appDataVM;
static LPSTR lpClass = (LPSTR)"";

//0001:00078b30       _PSXEmulation_CheckForTermination 00479b30 f   MainVM.obj
void PSXEmulation_CheckForTermination()
{
}
//0001:00078b40       _InitToolhelp32            00479b40 f   MainVM.obj
//0001:00078c10       _FailMsg                   00479c10 f   MainVM.obj
void FailMsg(const char* fmt, ...)
{
	CHAR Text[256]; // [esp+0h] [ebp-100h] BYREF
	va_list va; // [esp+108h] [ebp+8h] BYREF

	va_start(va, fmt);
	vsprintf_s(Text, sizeof(Text), fmt, va);
	va_end(va);

	MessageBoxA(0, Text, "Kain2 Error", MB_OK);
	ExitProcess(0);
}
//0001:00078c60       _RetryMsg                  00479c60 f   MainVM.obj
int RetryMsg(const char* fmt, ...)
{
	CHAR Text[256]; // [esp+0h] [ebp-100h] BYREF
	va_list va; // [esp+108h] [ebp+8h] BYREF

	va_start(va, fmt);
	vsprintf_s(Text, sizeof(Text), fmt, va);
	va_end(va);

	if (MessageBoxA(0, Text, "Kain2 Error", MB_OKCANCEL) == IDCANCEL)
		ExitProcess(0);
	return 1;
}
//0001:00078cc0       _WinMain@16                00479cc0 f   MainVM.obj
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	appDataVM.hInstance = hInstance;
	appDataVM.hWindow = 0;
	appDataVM.Is_windowed = 1;
	appDataVM.Render_device_id = 0;
	appDataVM.Screen_width = 640;
	appDataVM.Screen_height = 480;
	appDataVM.Screen_depth = 16;
	appDataVM.Gamma_level = 5;
	appDataVM.Triple_buffer = 0;
	appDataVM.Filter = 1;
	appDataVM.Sound_device_id = 0;
	appDataVM.VSync = 0;
	HKEY key;
	DWORD type = 0, cbData = 0;
	RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Crystal Dynamics\\Legacy of Kain: Soul Reaver\\1.00.000", 0, lpClass, 0, KEY_READ, nullptr, &key, nullptr);
	RegQueryValueExA(key, "Windowed",       0, &type, (LPBYTE)&appDataVM.Is_windowed, &cbData);
	RegQueryValueExA(key, "RenderDeviceID", 0, &type, (LPBYTE)&appDataVM.Render_device_id, &cbData);
	RegQueryValueExA(key, "SoundDeviceID",  0, &type, (LPBYTE)&appDataVM.Sound_device_id, &cbData);
	RegQueryValueExA(key, "ScreenWidth",    0, &type, (LPBYTE)&appDataVM.Screen_width, &cbData);
	RegQueryValueExA(key, "ScreenHeight",   0, &type, (LPBYTE)&appDataVM.Screen_height, &cbData);
	RegQueryValueExA(key, "ScreenDepth",    0, &type, (LPBYTE)&appDataVM.Screen_depth, &cbData);
	RegQueryValueExA(key, "TripleBuffer",   0, &type, (LPBYTE)&appDataVM.Triple_buffer, &cbData);
	RegQueryValueExA(key, "Filter",         0, &type, (LPBYTE)&appDataVM.Filter, &cbData);
	RegQueryValueExA(key, "VSync",          0, &type, (LPBYTE)&appDataVM.VSync, &cbData);
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
