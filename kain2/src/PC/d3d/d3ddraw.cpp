#include "../../core.h"

//0001 : 0006a3e0       _DRAW_LightVertex          0046b3e0 f   d3ddraw.obj
//0001 : 0006b4a0       _CalculateVertexLight      0046c4a0 f   d3ddraw.obj
//0001 : 0006b690       _DrawFadedModel            0046c690 f   d3ddraw.obj
//0001 : 0006bd00       _DrawModel                 0046cd00 f   d3ddraw.obj
//0001 : 0006c440       _SetClipPlane              0046d440 f   d3ddraw.obj
//0001 : 0006f5d0       _GetPortalNormal           004705d0 f   d3ddraw.obj
void __cdecl GetPortalNormal(struct StreamUnitPortal* portal, struct _Vector* pos)
{
	int xdiff0 = portal->t1[0].x - portal->t1[1].x,
		ydiff0 = portal->t1[0].y - portal->t1[1].y,
		zdiff0 = portal->t1[0].z - portal->t1[1].z,
		xdiff1 = portal->t1[0].x - portal->t1[2].x,
		ydiff1 = portal->t1[0].y - portal->t1[2].y,
		zdiff1 = portal->t1[0].z - portal->t1[2].z;

	pos->x = ydiff0 * zdiff1 - zdiff0 * ydiff1;
	pos->y = zdiff0 * xdiff1 - xdiff0 * zdiff1;
	pos->z = xdiff0 * ydiff1 - ydiff0 * xdiff1;
}

//0001  :00069d50       _DRAW_DisplayPolytopeList_C 0046ad50 f   d3ddraw.obj
//0001 : 0006a4b0       _D3DDRAW_DrawSegmentShadow 0046b4b0 f   d3ddraw.obj
//0001 : 0006a760       _D3DDRAW_DrawShadow        0046b760 f   d3ddraw.obj
//0001 : 0006a860       _D3DDRAW_DrawRing          0046b860 f   d3ddraw.obj
//0001 : 0006aac0       _FX_DrawModel              0046bac0 f   d3ddraw.obj
//0001 : 0006ae70       _PIPE3D_InstanceTransformAndDraw 0046be70 f   d3ddraw.obj
//0001 : 0006c3c0       _PIPE3D_HalvePlaneInstanceTransformAndDraw 0046d3c0 f   d3ddraw.obj
//0001 : 0006c990       _D3D_FXDrawList            0046d990 f   d3ddraw.obj
//0001 : 0006de20       _DrawGlow                  0046ee20 f   d3ddraw.obj
//0001 : 0006e000       _PIPE3D_DoGlow             0046f000 f   d3ddraw.obj
//0001 : 0006e450       _D3DDRAW_RingLine          0046f450 f   d3ddraw.obj
//0001 : 0006e610       _DRAW_RingPoint            0046f610 f   d3ddraw.obj
//0001 : 0006e6b0       _DRAW_DrawRingPoints       0046f6b0 f   d3ddraw.obj
//0001 : 0006ece0       _DRAW_TranslucentQuad      0046fce0 f   d3ddraw.obj
//0001 : 0006eef0       _DRAW_FlatQuad             0046fef0 f   d3ddraw.obj
//0001 : 0006f0f0       _D3D_DrawFogRectangle      004700f0 f   d3ddraw.obj
//0001 : 0006f3b0       _D3DDRAW_DrawPortal        004703b0 f   d3ddraw.obj
//0001 : 0006f660       _D3D_PortalSetClipRectangle 00470660 f   d3ddraw.obj [unused]
//0001 : 0006fd70       _D3D_FacingPortal          00470d70 f   d3ddraw.obj
//0001 : 0006fe20       _D3D_PortalSetClipPlanes   00470e20 f   d3ddraw.obj
//0001 : 00070290       _D3D_PortalSetInverseClipPlanes 00471290 f   d3ddraw.obj
//0001 : 000702c0       _D3D_PortalDisableClipPlanes 004712c0 f   d3ddraw.obj
//0001 : 000702e0       _D3D_SmallCalcOutCode      004712e0 f   d3ddraw.obj
//0001 : 00070330       _D3D_RenderLetter          00471330 f   d3ddraw.obj
//0001 : 000706e0       _D3D_DeathSwirl            004716e0 f   d3ddraw.obj