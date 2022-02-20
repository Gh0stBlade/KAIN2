#include <windows.h>
#include <ddraw.h>
#include <d3d.h>

struct TEXTURE_TYPE
{
	DDPIXELFORMAT pfmt;
	int field_20;
	int bits_alpha;
	int bits_red;
	int bits_green;
	int bits_blue;
};

typedef struct D3D_RES
{
	int x;
	int y;
	int depth;
} D3D_RES;

DWORD D3D_NumTextureTypes, enumerated, D3D_NumDevices, dword_C3C27C, dword_C3C284;
int screenmode_cnt0, screenmode_cnt1;
TEXTURE_TYPE Texturetypelist[64];
D3D_RES Screenmodelist[512];
void __cdecl D3D_FailAbort(const char* fmt, ...);

//0001:00070a40 ? EnumScrModesCallback@@YGJPAU_DDSURFACEDESC@@PAX@Z 00471a40 f   d3denum.obj
int WINAPI EnumScrModesCallback(DDSURFACEDESC* desc, LPVOID ctx)
{
	Screenmodelist[screenmode_cnt0].x = desc->dwWidth;
	Screenmodelist[screenmode_cnt0].y = desc->dwHeight;
	Screenmodelist[screenmode_cnt0].depth = desc->ddpfPixelFormat.dwRGBBitCount;
	screenmode_cnt0++;
	screenmode_cnt1++;
	return 1;
}

//0001:00070a90 ? enumtextures@@YGJPAU_DDPIXELFORMAT@@PAX@Z 00471a90 f   d3denum.obj
int count_bits(DWORD mask)
{
	int count;
	for (count = 0; mask; mask >>= 1)
		if ((mask & 1) != 0)
			++count;
	return count;
}

HRESULT WINAPI enumtextures(struct _DDPIXELFORMAT* fmt, LPVOID ctx)
{
	const DWORD mask0 = (DDPF_PALETTEINDEXED2 | DDPF_PALETTEINDEXED1);
	const DWORD mask1 = (DDPF_PALETTEINDEXEDTO8 | DDPF_PALETTEINDEXED8 | DDPF_PALETTEINDEXED4);
	const DWORD mask2 = (DDPF_RGB);

	if ((fmt->dwFlags & mask0) == 0 && (fmt->dwFlags & mask1) == 0 && (fmt->dwFlags & mask2) != 0)
	{
		auto t = &Texturetypelist[D3D_NumTextureTypes];
		memcpy(&t->pfmt, fmt, sizeof(*fmt));

		if (fmt->dwFlags & DDPF_ALPHAPIXELS)
			t->bits_alpha = count_bits(fmt->dwRGBAlphaBitMask);
		else t->bits_alpha = 0;

		t->bits_red = count_bits(fmt->dwRBitMask);
		t->bits_green = count_bits(fmt->dwGBitMask);
		t->bits_blue = count_bits(fmt->dwBBitMask);

		D3D_NumTextureTypes++;
	}

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
