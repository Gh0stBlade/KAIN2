#include "../core.H"
#include "MAING2.H"

#ifdef PC_VERSION
#include "../PC/snd.h"
#endif

extern int RenderOptions,
	LastRenderOptions,
	RenderResolution,
	LastRenderResolution;

enum _G2Bool_Enum MainG2_InitEngine(void *appData, unsigned int screenWidth, unsigned int screenHeight, char *filesystemName)
{
#if defined(PSX_VERSION)
	return (_G2Bool_Enum)1;
#elif defined(PC_VERSION)
	if (!ViewportG2_Init(appData)) return 0;

	SoundG2_Init(appData);
	if (!InputG2_Init(appData))
		return 0;

	if (!RenderG2_Init(appData))
		return 0;

	int opt = RenderG2_GetRenderOptions();
	RenderOptions = opt;
	LastRenderOptions = opt;
	RenderResolution = RenderG2_GetResolution();
	LastRenderResolution = RenderResolution;

	return 0;
#endif
}

void MainG2_ShutDownEngine(void* appData)
{
}