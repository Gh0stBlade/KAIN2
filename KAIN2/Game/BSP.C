#include "CORE.H"

#ifdef 0
// @fixme in spite of matching better than the version below, this function somehow renders all areas from the debug menu inaccessible.  
void SBSP_IntroduceInstances(struct _Terrain* terrain, int unitID) // Matching - 100%
{
	int i;
	struct Intro* intro;

	for (intro = terrain->introList, i = terrain->numIntros; i != 0; i--, intro++)
	{
		if (!(intro->flags & 0x4008))
		{
			INSTANCE_IntroduceInstance(intro, unitID);
		}
	}
}
#else
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
#endif

void SBSP_IntroduceInstancesAndLights(struct _Terrain* terrain, struct _CameraCore_Type* cameraCore, struct LightInfo* lightInfo, int unitID)  // Matching - 100%
{
	SBSP_IntroduceInstances(terrain, unitID);
}