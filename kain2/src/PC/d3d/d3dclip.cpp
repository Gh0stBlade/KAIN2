#include "d3d.h"

DWORD D3D_ClipPlaneMask, D3D_InverseClipPlanes;
float D3D_LeftClip, D3D_BottomClip, D3D_TopClip, D3D_RightClip,
	D3D_FarClip, D3D_NearClip;

//0001:00068490       _D3D_SetClipRect           00469490 f   d3dclip.obj
void __cdecl D3D_SetClipRect(int left, int top, int right, int bottom)
{
	if (left < 0) left = 0;
	if (left > D3D_XRes) left = D3D_XRes;
	if (top < 0) top = 0;
	if (top > D3D_YRes) top = D3D_YRes;
	if (right < 0) left = 0;
	if (right > D3D_XRes) right = D3D_XRes;
	if (bottom < 0) top = 0;
	if (bottom > D3D_YRes) bottom = D3D_YRes;

	D3D_LeftClip = (float)left;
	D3D_TopClip = (float)top;
	D3D_RightClip = (float)right;
	D3D_BottomClip = (float)bottom;
}
//0001:00068530       _D3D_GetClipRect           00469530 f   d3dclip.obj
void __cdecl D3D_GetClipRect(int* left, int* top, int* right, int* bottom)
{
	*left = (int)D3D_LeftClip;
	*top = (int)D3D_TopClip;
	*right = (int)D3D_RightClip;
	*bottom = (int)D3D_BottomClip;
}
//0001:00068580 ?D3D_GetClipRect_float@@YAXPAM000@Z 00469580 f   d3dclip.obj
void __cdecl D3D_GetClipRect_float(float* left, float* top, float* right, float* bottom)
{
	*left = 0.f;
	*top = 0.f;
	*right = (float)D3D_XRes;
	*bottom = (float)D3D_YRes;
}
//0001:000685b0 _D3D_CalcOutCode           004695b0 f   d3dclip.obj
void __cdecl D3D_CalcOutCode(MYTRI* p)
{
	p->col = 0;
	if (p->z < D3D_NearClip)
		p->col = 0x10;
	if (p->z < D3D_FarClip)
		p->col |= 0x20;
	if (p->x < D3D_LeftClip)
		p->col |= 1;
	if (p->x > D3D_RightClip)
		p->col |= 2;
	if (p->y < D3D_TopClip)
		p->col |= 4;
	if (p->y > D3D_BottomClip)
		p->col |= 8;
}
//0001:00068620 ?AddClippedTri@@YAXPAUMATBUCKET@@PAUMYD3DVERTEX@@@Z 00469620 f   d3dclip.obj
//0001:00069280 ?AddPlaneClippedTri@@YAXPAUMATBUCKET@@PAUMYD3DVERTEX@@@Z 0046a280 f   d3dclip.obj