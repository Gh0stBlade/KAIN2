#include <windows.h>
#include <d3d9.h>
#include <d3d9types.h>

LPDIRECT3D9 d3d9;
LPDIRECT3DDEVICE9 d3d9dev;

void __cdecl D3D9_SetGammaNormalized(int level)
{
	D3DGAMMARAMP ramp;
	d3d9dev->SetGammaRamp(0, 0, &ramp);
}

void __cdecl D3D9_ActivateFogUnit(int index)
{
	D3D_FogColor = d3d_fogtbl[index].col;
	D3D_FogFar = d3d_fogtbl[index].far;
	D3D_FogNear = d3d_fogtbl[index].near;
	if (D3D_UseVertexFog)
	{
		d3d9dev->SetRenderState(D3DRS_FOGCOLOR, D3D_FogColor);
		D3D_FogZScale = 254.f / (D3D_FogFar - D3D_FogNear);
	}
}