#include "CORE.H"

void SBSP_IntroduceInstances(struct _Terrain* terrain, int unitID)
{
#if defined(PSX_VERSION)

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

#elif defined(PC_VERSION)
	struct Intro* introList; // esi
	int numIntros; // edi
	int flags; // eax

	introList = terrain->introList;
	if (terrain->numIntros)
	{
		numIntros = terrain->numIntros;
		do
		{
			flags = introList->flags;
			if ((flags & 8) == 0 && (flags & 0x4000) == 0)
				INSTANCE_IntroduceInstance(introList, unitID);
			++introList;
			--numIntros;
		} while (numIntros);
	}
#endif
}

void SBSP_IntroduceInstancesAndLights(struct _Terrain* terrain, struct _CameraCore_Type* cameraCore, struct LightInfo* lightInfo, int unitID)
{
#if defined(PSX_VERSION)
	SBSP_IntroduceInstances(terrain, unitID);
#elif defined(PC_VERSION)
	struct Intro* introList; // esi
	int numIntros; // edi
	int flags; // eax

	introList = terrain->introList;
	if (terrain->numIntros)
	{
		numIntros = terrain->numIntros;
		do
		{
			flags = introList->flags;
			if ((flags & 8) == 0 && (flags & 0x4000) == 0)
				INSTANCE_IntroduceInstance(introList, unitID);
			++introList;
			--numIntros;
		} while (numIntros);
	}
#endif
}




