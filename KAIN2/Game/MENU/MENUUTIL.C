#include "CORE.H"
#include "MENUUTIL.H"
#include "GAMELOOP.H"
#include "MENU.H"
#include "FONT.H"
#include "SOUND.H"

struct menu_sound_entry_t the_menu_sounds[6] = { menu_sound_none, 5, menu_sound_default, 5, menu_sound_select, 5, menu_sound_adjust, 5, menu_sound_engage, 5, menu_sound_pop, 5, };

enum menu_ctrl_t menu_get_ctrl(void *gt)
{
	enum menu_ctrl_t ctrl;
	
	if ((((struct GameTracker*)gt)->controlCommand[0][1] & 0x1))
	{
		ctrl = menu_ctrl_up;
	}
	else if ((((struct GameTracker*)gt)->controlCommand[0][1] & 0x2))
	{
		ctrl = menu_ctrl_down;
	}
	else if ((((struct GameTracker*)gt)->controlCommand[0][1] & 0x4))
	{
		ctrl = menu_ctrl_left;
	}
	else if ((((struct GameTracker*)gt)->controlCommand[0][1] & 0x8))
	{
		ctrl = menu_ctrl_right;
	}
	else if ((((struct GameTracker*)gt)->controlCommand[0][1] & 0x80) && !(((struct GameTracker*)gt)->controlCommand[0][0] & 0x300))
	{
		ctrl = menu_ctrl_engage;
	}
	else
	{
		if ((((struct GameTracker*)gt)->controlCommand[0][1] & 0x10))
		{
			ctrl = menu_ctrl_cancel;
		}
		else
		{
			if ((((struct GameTracker*)gt)->controlCommand[0][1] & 0x4000))
			{
				ctrl = menu_ctrl_start;

			}
			else
			{
				ctrl = menu_ctrl_none;
			}
		}
	}
	
	return ctrl;
}

void menu_print(int xpos, int ypos, char *text, int color)
{ 
	FONT_SetCursor(xpos, ypos);

	if (color != 0)
	{
		FONT_SetColorIndex(color);
	}

	FONT_Print2(text);

	if (color != 0)
	{
		FONT_SetColorIndex(0);
	}
}

int menu_text_width(char *text)
{
	return FONT_GetStringWidth(text);
}

void menu_sound(enum menu_sound_t sound)
{
	struct menu_sound_entry_t *entry;

	entry = &the_menu_sounds[sound];

	SndPlay(entry->sfx);
}




