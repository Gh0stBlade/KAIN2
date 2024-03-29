#include "Game/CORE.H"
#include "MENUDEFS.H"
#include "Game/MENU/MENU.H"
#include "Game/GAMELOOP.H"
#include "Game/PSX/MAIN.H"
#include "Game/LOCAL/LOCALSTR.H"
#include "Game/FONT.H"
#include "Game/MEMPACK.H"
#include "Game/MCARD/MEMCARD.H"
#include "Game/GAMEPAD.H"
#include "Game/SOUND.H"
#include "Game/DEBUG.H"

int StartGameFading;
int MAIN_YPOS = 30;
int MAIN_XPOS = 135;
int MAIN_WIDTH = 172;
int LINESKIP = 14; // offset 0x800d1fa8
int ITEMSKIP = 2; // offset 0x800d1fac
int PAUSE_XPOS = 256; // offset 0x800d1fa0
int PAUSE_WIDTH = 256; // offset 0x800d1fa4
int PAUSE_YPOS = 60; // offset 0x800d1f94
int hack_reset_attract;
int hack_attract;
int hack_attract_movie;
char* the_attract_movies[4] = { "\\KAINDEM1.STR;1", "\\KAINDEM2.STR;1", "\\KAINDEM3.STR;1", "\\KAINDEM4.STR;1" };

void do_check_controller(void *gt)
{
	if (((struct GameTracker*)gt)->gameMode == 6)
	{
		GAMEPAD_DisplayControllerStatus(170);
	}
	else
	{
		GAMEPAD_DisplayControllerStatus(200);
	}
}

int do_push_menu(void *gt, intptr_t menuparam, enum menu_ctrl_t ctrl)
{
	if (ctrl != menu_ctrl_engage)
	{
		return 0;
	}
	
	typedef int (*ret)(void*, int);
	ret returnFunction = (ret)menuparam;

	menu_push(((struct GameTracker*)gt)->menu, returnFunction);

	return 1;
}

int do_pop_menu(void *gt, intptr_t param, enum menu_ctrl_t ctrl)
{
	if (ctrl != menu_ctrl_engage)
	{
		return 0;
	}

	menu_pop(((struct GameTracker*)gt)->menu);

	return 1;
}

int do_function(void* gt, intptr_t fnparam, enum menu_ctrl_t ctrl)
{ 
	if (ctrl == menu_ctrl_engage)
	{
		typedef void (*retFunction)();
		retFunction ret = (retFunction)fnparam;
		ret();
		return 1;
	}

	return 0;
}

int do_start_game(void *gt, intptr_t parameter, enum menu_ctrl_t ctrl)
{
	if (ctrl == menu_ctrl_engage)
	{
#if defined(__EMSCRIPTEN__)
		if (MEMCARD_IsWrongVersion(((struct GameTracker*)gt)->memcard) == 0 && 0)//Skip for now
#else
		if (MEMCARD_IsWrongVersion(((struct GameTracker*)gt)->memcard) == 0)
#endif
		{
			menu_push(((struct GameTracker*)gt)->menu, memcard_main_menu);
			return 1;
		}
		else
		{
#if defined(DEMO)
			MAIN_StartDemo();
#else
			MAIN_StartGame();
#endif
			return 1;
		}
	}
	
	return 0;
}

int do_save_menu(void *gt, intptr_t parameter, enum menu_ctrl_t ctrl)
{ 
	if (ctrl == menu_ctrl_engage)
	{
		menu_push(((struct GameTracker*)gt)->menu, memcard_pause_menu);
		return 1;
	}

	return 0;
}

void womp_background(char *tim_path)
{ 
	MEMPACK_Free((char*)mainMenuScreen);
	mainMenuScreen = (int*)MAIN_LoadTim(tim_path);
}


// autogenerated function stub: 
// void /*$ra*/ play_movie(char *name /*$s0*/)
#if !defined(PSX_VERSION)
void play_movie(char *name)
{ // line 291, offset 0x800b8ae8
	/* begin block 1 */
		// Start line: 292
		// Start offset: 0x800B8AE8
	/* end block 1 */
	// End offset: 0x800B8B20
	// End Line: 311

	/* begin block 2 */
		// Start line: 608
	/* end block 2 */
	// End Line: 609

}
#endif

void menudefs_reset_hack_attract_mode()
{ 
	if (hack_attract > 0)
	{
		hack_attract = gameTrackerX.vblCount;
	}
}

void check_hack_attract()
{
	if (hack_attract > 0)
	{
		if (hack_attract + 2000 < (int)gameTrackerX.vblCount)
		{
			hack_attract = 1;
			play_movie(the_attract_movies[hack_attract_movie += 1 & 3]);
			hack_attract = gameTrackerX.vblCount;
		}
	}
}

int get_volume(void *gt, enum sfx_t sfx)
{ 
	int raw;

	if (sfx == sfx_sound)
	{
		raw = ((struct GameTracker*)gt)->sound.gSfxVol;
	}
	else if (sfx == sfx_music)
	{
		raw = ((struct GameTracker*)gt)->sound.gMusicVol;
	}
	else if (sfx == sfx_voice)
	{
		raw = ((struct GameTracker*)gt)->sound.gVoiceVol;
	}
	else
	{
		raw = 3;
	}

	return (raw * 10) / 127;
}

void set_volume(enum sfx_t sfx, int cooked)
{
	int raw;

	raw = (cooked * 127 + 9) / 10;

	if (sfx == sfx_music)
	{
		SOUND_SetMusicVolume(raw);
	}
	else if (sfx == sfx_sound)
	{
		SOUND_SetSfxVolume(raw);
	}
	else if (sfx == sfx_voice)
	{
		SOUND_SetVoiceVolume(raw);
	}
}

int do_sound_adjust(void *gt, intptr_t sfxparam, enum menu_ctrl_t ctrl)
{
	int volume;
	
	volume = get_volume(gt, (sfx_t)sfxparam);

	if (ctrl == menu_ctrl_left)
	{
		if (volume > 0)
		{
			set_volume((sfx_t)sfxparam, volume - 1);
		}
		else
		{
			return 1;
		}
	}
	else if (ctrl == menu_ctrl_right)
	{
		if (volume < 10)
		{
			set_volume((sfx_t)sfxparam, volume + 1);
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

void sound_item(void *gt, char *text, enum sfx_t sfx)
{
	menu_item(((struct GameTracker*)gt)->menu, do_sound_adjust, sfx, "%s %d", text, get_volume(gt, sfx));
}

int menudefs_toggle_dualshock(void* gt, intptr_t param, enum menu_ctrl_t ctrl)
{ 
	if ((unsigned int)ctrl - 3 < 2)
	{
		if (GAMEPAD_DualShockEnabled() != 0)
		{
			GAMEPAD_DisableDualShock();
			return 1;
		}

		GAMEPAD_EnableDualShock();
		GAMEPAD_Shock1(128, 524288);
		return 1;
	}

	return 0;
}

int options_menu(void *gt, int index)
{
	static int wasDualShock;
	int dualShock;

	hack_attract = 0;

	MENUFACE_ChangeStateRandomly(index);

	do_check_controller(gt);
	
	menu_item_flags(((struct GameTracker*)gt)->menu, NULL, 0, 4, localstr_get(LOCALSTR_options));
	
	sound_item(gt, localstr_get(LOCALSTR_sound), sfx_sound);
	sound_item(gt, localstr_get(LOCALSTR_music), sfx_music);
	sound_item(gt, localstr_get(LOCALSTR_voice), sfx_voice);

	dualShock = GAMEPAD_ControllerIsDualShock();

	if (dualShock != 0)
	{
		if (GAMEPAD_DualShockEnabled() == 0)
		{
			menu_item(((struct GameTracker*)gt)->menu, menudefs_toggle_dualshock, 0, localstr_get(LOCALSTR_vibration_off));
		}
		else
		{
			menu_item(((struct GameTracker*)gt)->menu, menudefs_toggle_dualshock, 0, localstr_get(LOCALSTR_vibration_on));
		}
	}
	
	menu_item(((struct GameTracker*)gt)->menu, do_pop_menu, 0, localstr_get(LOCALSTR_done));

	if (dualShock != wasDualShock && index >= 4)
	{
		index = dualShock + 4;
	}

	wasDualShock = dualShock;

	if (index < 0)
	{
		return 1;
	}

	return index;
}

int main_menu(void *gt, int index)
{
	menu_format(((struct GameTracker*)gt)->menu, 1, MAIN_XPOS, MAIN_YPOS, MAIN_WIDTH, LINESKIP, ITEMSKIP, 0);
	
	MENUFACE_ChangeStateRandomly(index);
	
	do_check_controller(gt);
	
	menu_item(((struct GameTracker*)gt)->menu, do_start_game, 0, localstr_get(LOCALSTR_start_game));
	menu_item(((struct GameTracker*)gt)->menu, do_push_menu, (intptr_t)&options_menu, localstr_get(LOCALSTR_options));

	if (index < 0)
	{
		return 0;
	}
	
	return index;
}

int do_main_menu(void* gt, intptr_t param, enum menu_ctrl_t ctrl)
{ 
	if (StartGameFading == 0 && (ctrl == menu_ctrl_start || ctrl == menu_ctrl_engage))
	{
		((struct GameTracker*)gt)->wipeType = 10;
		((struct GameTracker*)gt)->wipeTime = -20 * FRAMERATE_MULT;
		((struct GameTracker*)gt)->maxWipeTime = 20 * FRAMERATE_MULT;
		StartGameFading = 1;
		return 1;
	}
	
	return 0;
}

char* flashStart()
{ 
	static int counter = -1;
	int intpl;
	int fcols[2][3] = { 0xC0, 0xD2, 0xD2, 0xC0, 0xC0, 0x40 };
	int r;
	int g;
	int b;

	gameTrackerX.gameFramePassed = 1;

	if (StartGameFading == 1)
	{
		hack_reset_attract = 1;

		if (gameTrackerX.wipeTime == -1)
		{
			womp_background("\\kain2\\game\\psx\\bkgdmenu.tim");
			
			gameTrackerX.wipeType = 10;
			gameTrackerX.wipeTime = 20 * FRAMERATE_MULT;
			gameTrackerX.maxWipeTime = 20 * FRAMERATE_MULT;
			
			StartGameFading = 0;

			menu_pop(gameTrackerX.menu);
			menu_push(gameTrackerX.menu, main_menu);

			return NULL;
		}
	}
	else
	{
		counter = (counter + 1) % (60 * FRAMERATE_MULT);
		
		if (counter < (10 * FRAMERATE_MULT))
		{
			intpl = 0;
		}
		else if (counter < (30 * FRAMERATE_MULT))
		{
			intpl = ((counter - (10 * FRAMERATE_MULT)) << 12) / (20 * FRAMERATE_MULT);
		}
		else
		{
			if (counter < (40 * FRAMERATE_MULT))
			{
				intpl = 4096;
			}
			else
			{
				intpl = (((60 * FRAMERATE_MULT) - counter) << 12) / (20 * FRAMERATE_MULT);
			}
		}

		r = ((fcols[0][0] * (4096 - intpl)) + (fcols[1][0] * (intpl))) >> 12;
		g = ((fcols[0][1] * (4096 - intpl)) + (fcols[1][1] * (intpl))) >> 12;
		b = ((fcols[0][2] * (4096 - intpl)) + (fcols[1][2] * (intpl))) >> 12;

		FONT_SetColorIndex(4);
		FONT_SetColorIndexCol(4, r, g, b);
	}

	return localstr_get(LOCALSTR_press_start);
}

int menudefs_main_menu(void *gt, int index)
{ 
	if (hack_reset_attract != 0)
	{
		hack_reset_attract = 0;
		hack_attract = gameTrackerX.vblCount;
	}

	check_hack_attract();
	menu_format(((struct GameTracker*)gt)->menu, 1, 366, 144, 100, LINESKIP, ITEMSKIP, 0);
	menu_item(((struct GameTracker*)gt)->menu, do_main_menu, 0, flashStart());

	if (index >= 0)
	{
		return index;
	}

	return 0;
}

int menudefs_confirmexit_menu(void *gt, int index)
{
	hack_attract = 0;

	do_check_controller(gt);
	
	menu_item_flags(((struct GameTracker*)gt)->menu, NULL, 0, 4, localstr_get(LOCALSTR_query_quit));
	
	menu_item(((struct GameTracker*)gt)->menu, do_pop_menu, 0, localstr_get(LOCALSTR_no));
	menu_item(((struct GameTracker*)gt)->menu, do_function, (long)DEBUG_ExitGame, localstr_get(LOCALSTR_yes));

	if (index < 0)
	{
		return 1;
	}

	return index;
}

int menudefs_pause_menu(void *gt, int index)
{
	do_check_controller(gt);
	
	hack_attract = 0;

	menu_format(((struct GameTracker*)gt)->menu, 1, PAUSE_XPOS, PAUSE_YPOS, PAUSE_WIDTH, LINESKIP, ITEMSKIP, 1);
	
	menu_item_flags(((struct GameTracker*)gt)->menu, NULL, 0, 4, localstr_get(LOCALSTR_paused));
	
	menu_item(((struct GameTracker*)gt)->menu, do_function, (long)DEBUG_ContinueGame, localstr_get(LOCALSTR_resume_game));

	if (!(gameTrackerX.streamFlags & 0x4))
	{
		menu_item(((struct GameTracker*)gt)->menu, do_save_menu, 0, localstr_get(LOCALSTR_save_game));
		
		menu_item(((struct GameTracker*)gt)->menu, do_push_menu, (long)options_menu, localstr_get(LOCALSTR_options));
		
		menu_item(((struct GameTracker*)gt)->menu, do_push_menu, (long)menudefs_confirmexit_menu, localstr_get(LOCALSTR_quit_game));
	}

	if (index < 0)
	{
		return 1;
	}

	return index;
}
