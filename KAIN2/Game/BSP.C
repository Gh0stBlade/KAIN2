#include "CORE.H"

void SBSP_IntroduceInstances(struct _Terrain* terrain, int unitID)
{
	int i;
	struct Intro* intro;

	i = terrain->numIntros;
	intro = terrain->introList;

	if (i-- != 0)
	{
		if (!(intro[i].flags & 0x4008))
		{
			INSTANCE_IntroduceInstance(&intro[0], unitID);
		}
	}
}

void SBSP_IntroduceInstancesAndLights(struct _Terrain* terrain, struct _CameraCore_Type* cameraCore, struct LightInfo* lightInfo, int unitID)  // Matching - 100%
{
	SBSP_IntroduceInstances(terrain, unitID);
}