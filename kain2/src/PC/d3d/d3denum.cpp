#include <windows.h>
#include <ddraw.h>
#include <d3d.h>

DWORD D3D_NumTextureTypes, enumerated, D3D_NumDevices, dword_C3C27C, dword_C3C284;
void __cdecl D3D_FailAbort(const char* fmt, ...);

//0001:00070a40 ? EnumScrModesCallback@@YGJPAU_DDSURFACEDESC@@PAX@Z 00471a40 f   d3denum.obj
//0001:00070a90 ? enumtextures@@YGJPAU_DDPIXELFORMAT@@PAX@Z 00471a90 f   d3denum.obj
HRESULT WINAPI enumtextures(struct _DDPIXELFORMAT*, LPVOID ctx)
{
	return 1;
}

//0001:00070be0 ? DDEnumCallback@@YGHPAU_GUID@@PAD1PAX2@Z 00471be0 f   d3denum.obj
BOOL WINAPI DDEnumCallback(GUID *guid, LPSTR name, LPSTR desc, LPVOID ctx, HMONITOR)
{
	return 1;
}

//0001:00070b60       _D3D_EnumTextureTypes      00471b60 f   d3denum.obj
int __cdecl D3D_EnumTextureTypes(IDirect3DDevice3* dev)
{
	D3D_NumTextureTypes = 0;
	return dev->EnumTextureFormats(enumtextures, nullptr);
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
