#include "Game/CORE.H"
#include "MEMCARD.H"
#include "Game/GAMELOOP.H"
#include "Game/MEMPACK.H"
#include "Game/STRMLOAD.H"
#include "Game/RELMOD.H"
#include "Game/MENU/MENU.H"
#include "Game/PSX/MAIN.H"
#include "Game/SAVEINFO.H"

#if defined(PSXPC_VERSION)
#include "Game/OVERLAYS/MCARDX.H"
#endif

struct memcard_t gMemcard;
int the_header_size;
struct SavedInfoTracker savedInfoTracker;
struct mcmenu gMcmenu;

int MEMCARD_IsWrongVersion(struct memcard_t *memcard)
{
	int result = 1;
	if (memcard != NULL)
	{
		result = memcard->wrongVerison;
	}

	return result;
}

void load(struct memcard_t* memcard)
{
	struct Object* object;
	
	if ((gameTrackerX.gameFlags & 0x8000000))
	{
		object = (struct Object*)&gameTrackerX.primPool->prim[0];
	}
	else
	{
		object = (struct Object*)MEMPACK_Malloc(40000, 0x2B);
	}

	LOAD_LoadToAddress("\\kain2\\object\\mcardx\\mcardx.drm", object, 1);
	memcard->table = (struct mcmenu_table_t*)object->relocModule;

	RELMOD_InitModulePointers((uintptr_t)object->relocModule, (int*)object->relocList);
	
	memcard->object = object;

#if !defined(PSXPC_VERSION)
	if (memcard->table->versionID != "Jun 30 1999")
	{
		if (!(gameTrackerX.gameFlags & 0x8000000))
		{
			MEMPACK_Free((char*)object);
		}

		memcard->table = NULL;
	}
#elif defined(PSXPC_VERSION)
	//PSXPC version we have to override this stuff now.
	memcard->table->data_size = NULL;
	memcard->table->initialize = MCARDX_initialize;
	memcard->table->terminate = NULL;
	memcard->table->begin = MCARDX_begin;
	memcard->table->end = MCARDX_end;
	memcard->table->set_buffer = MCARDX_set_buffer;
	memcard->table->main = MCARDX_main;
	memcard->table->pause = MCARDX_pause;
#endif
}

void unload(struct memcard_t* memcard)
{
	if (memcard->object != NULL)
	{
		if ((char*)memcard != (char*)&gameTrackerX.primPool->prim)
		{
			MEMPACK_Free((char*)memcard->object);
		}

		memcard->object = NULL;
	}

	memcard->table = NULL;
}

int memcard_data_size()
{
	return sizeof(memcard_t);
}

int memcard_initialize(struct memcard_t *memcard, void *gt, int nblocks, void *buffer, int nbytes)
{
	int header_size;
	
	memset(memcard, 0, sizeof(struct memcard_t));

#if !defined(__EMSCRIPTEN__)//Crashes on Emscripten!
	load(memcard);
#endif
	
	header_size = 0;
	memcard->wrongVerison = 0;

	if (memcard->table != NULL)
	{
		memcard->mcmenu = &gMcmenu.dummy1;
		memcard->table->initialize(memcard->mcmenu, memcard, nblocks);
		header_size = memcard->table->set_buffer(memcard->mcmenu, buffer, nbytes);
		unload(memcard);
	}
	else
	{
		memcard->wrongVerison = 1;
	}

	return header_size;
}

void memcard_end(struct memcard_t* memcard)
{ 
	memcard->table->end(memcard->mcmenu);

	unload(memcard);

	memcard->running = 0;
}

int maybe_start(struct memcard_t *memcard)
{
	if (memcard->running == 0)
	{
		if (memcard->object == NULL)
		{
			load(memcard);
		}
		memcard->running = 1;
		memcard->table->begin(memcard->mcmenu);
	}
	
	return 1;
}

int memcard_main_menu(void *gt, int index)
{
	struct memcard_t *memcard;
	
	memcard = ((struct GameTracker*)gt)->memcard;
	
	if (maybe_start(memcard) != 0)
	{
		memcard->table->main(memcard->mcmenu, index);
	}
	
	return -1;
}

int memcard_pause_menu(void *gt, int index)
{ 
	struct memcard_t *memcard;

	memcard = ((struct GameTracker*)gt)->memcard;

	if (maybe_start(memcard))
	{
		return memcard->table->pause(memcard->mcmenu, index);
	}

	return -1;
}

void* gt2mcmenu(void* gt)
{
	struct memcard_t* memcard;
	
	memcard = ((struct GameTracker*)gt)->memcard;

	return memcard->mcmenu;
}

void memcard_pop(void* opaque)
{
	menu_pop(gameTrackerX.menu);

	memcard_end(gameTrackerX.memcard);
}

void memcard_start(void* opaque)
{
	gameTrackerX.streamFlags |= 0x1000000;

#if defined(DEMO)
	MAIN_StartDemo();
#else
	MAIN_StartGame();
#endif

	memcard_end(gameTrackerX.memcard);
}

void memcard_load(void* opaque)
{
	gameTrackerX.streamFlags |= 0x200000;

	SAVE_RestoreGame();
	
#if defined(DEMO)
	MAIN_StartDemo();
#else
	MAIN_StartGame();
#endif

	memcard_end(gameTrackerX.memcard);
}

void memcard_save(void *opaque)
{
	SAVE_SaveGame();
}

void memcard_item(void* opaque, int (*fn)(void*, intptr_t, enum menu_ctrl_t), intptr_t parameter, long flags, char* text)
{
	if (flags != 0)
	{
		menu_item_flags(gameTrackerX.menu, fn, parameter, flags, text);
	}
	else
	{
		menu_item(gameTrackerX.menu, fn, parameter, text);
	}
}