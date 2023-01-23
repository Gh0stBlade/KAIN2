#include "Game/CORE.H"
#include "Game/CINEMA/CINEPSX.H"
#include "Game/STREAM.H"
#include "Game/STRMLOAD.H"
#include "Game/PSX/MAIN.H"
#include "Game/OVERLAYS/CINEMAX.H"
#include "Game/GAMEPAD.H"

#include <stddef.h>

static struct cinema_fn_table_t* the_cine_table; // offset 0x800CAD90
static struct _ObjectTracker* the_cine_tracker; // offset 0x800CAD94
int StCdIntrFlag;

int CINE_CDIntrQuery()
{
	if (StCdIntrFlag != 0)
	{
		StCdIntrFlag = 0;
		return 1;
	}

	return 0;
}

unsigned short CINE_Pad(int pad)
{
	if (pad != 0)
	{
		return readGPBuffer2.data.pad;
	}
	else
	{
		return readGPBuffer1.data.pad;
	}
}

void CINE_Play(char *strfile, unsigned short mask, int buffers)
{ 
	if (the_cine_table != NULL)
	{
#if defined(PSXPC_VERSION)
		if (!strcmp(the_cine_table->versionID, __DATE__))
#else
		if (the_cine_table->versionID == __DATE__)
#endif
		{
			the_cine_table->play(strfile, mask, buffers);
		}
		else
		{
			printf("CINEMA : Version number is wrong. Not playing the cinematics.\n");
		}
	}
}

#if defined(PSXPC_VERSION)
void CINE_PatchTable()
{
	the_cine_table->versionID = __DATE__;
	the_cine_table->play = CINEMAX_Play;
}

#endif

int CINE_Load()
{
	struct _ObjectTracker* tracker;
	int attempts;

	attempts = 0;
	tracker = STREAM_GetObjectTracker("cinemax");

	do
	{
		if (tracker->objectStatus == 2)
		{
			break;
		}
		
		attempts++;
		STREAM_PollLoadQueue();
		VSync(0);
	} while (attempts < 400);
	
	if (attempts < 400)
	{
		the_cine_tracker = tracker;
		the_cine_table = (struct cinema_fn_table_t*)tracker->object->relocModule;

#if defined(PSXPC_VERSION)
		CINE_PatchTable();
#endif

		return 1;
	}
	else
	{
		printf("cinema timeout\n");
	}

	return 0;
}

int CINE_Loaded()
{
	return the_cine_tracker != NULL;
}

void CINE_Unload()
{
	VSyncCallback(VblTick);

	the_cine_table = NULL;

	if (the_cine_tracker != NULL)
	{
		STREAM_DumpObject(the_cine_tracker);

		the_cine_tracker = NULL;
	}
}

void CINE_PlayIngame(int number)
{
	char movie_name[24];

	sprintf(movie_name, "\\CHRONO%d.STR;1", number);

	if (CINE_Load() != 0)
	{
		CINE_Play(movie_name, 0x1, 2);

		CINE_Unload();
	}
}