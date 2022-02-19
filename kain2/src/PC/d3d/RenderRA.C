#include "../../core.h"

int RenderOptions,
	LastRenderOptions,
	RenderResolution,
	LastRenderResolution;

//0001 : 00073fa0       _RenderG2_Init             00474fa0 f   RenderRA.obj
//0001 : 00073fb0       _RenderG2_ReInit           00474fb0 f   RenderRA.obj
//0001 : 00073fc0       _RenderG2_ShutDown         00474fc0 f   RenderRA.obj
//0001 : 00073fd0       _RenderG2_Pause            00474fd0 f   RenderRA.obj
//0001 : 00073fe0       _RenderG2_GetRenderOptions 00474fe0 f   RenderRA.obj
//0001 : 00074040       _RenderG2_SetRenderOptions 00475040 f   RenderRA.obj
//0001 : 000742a0       _RenderG2_SetResolution    004752a0 f   RenderRA.obj
//0001 : 00074340       _RenderG2_GetResolution    00475340 f   RenderRA.obj
//0001 : 00074360       _RenderG2_Clear            00475360 f   RenderRA.obj
//0001 : 00074370       _RenderG2_SetBGColor       00475370 f   RenderRA.obj
//0001 : 00074390       _RenderG2_SetFog           00475390 f   RenderRA.obj
//0001 : 000743e0       _RenderG2_GetScreenShot    004753e0 f   RenderRA.obj
//0001 : 000743f0       _RenderG2_Swap             004753f0 f   RenderRA.obj
//0001 : 00074400       _RenderG2_FlushDraw        00475400 f   RenderRA.obj