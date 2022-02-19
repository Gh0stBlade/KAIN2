#include "../core.h"
#include "snd.h"

_G2AppDataVM_Type* pVM;

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
