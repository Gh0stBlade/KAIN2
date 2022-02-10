#ifndef BSP_H
#define BSP_H


extern void SBSP_IntroduceInstances(struct _Terrain *terrain, int unitID); // 0x8003D928

extern void SBSP_IntroduceInstancesAndLights(struct _Terrain *terrain, struct _CameraCore_Type *cameraCore, struct LightInfo *lightInfo, int unitID); // 0x8003D98C


#endif
