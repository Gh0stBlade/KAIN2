#ifndef MENUDEFS_H
#define MENUDEFS_H

#include "MENU.H"

typedef enum sfx_t
{
	sfx_sound = 0,
	sfx_music = 1,
	sfx_voice = 2,
	sfx_error = 3,
}sfx_t;

extern int PAUSE_XPOS; // offset 0x800d1fa0
extern int PAUSE_WIDTH; // offset 0x800d1fa4
extern int PAUSE_YPOS; // offset 0x800d1f94

extern void do_check_controller(void *gt); // 0x800B8938
extern int do_push_menu(void* gt, intptr_t menuparam, enum menu_ctrl_t ctrl); // 0x800B896C
extern int do_pop_menu(void *gt, intptr_t param, enum menu_ctrl_t ctrl); // 0x800B89A4
extern int do_function(void* gt, intptr_t fnparam, enum menu_ctrl_t ctrl); // 0x800B89DC
extern int do_start_game(void *gt, intptr_t parameter, enum menu_ctrl_t ctrl); // 0x800B8A10
extern int do_save_menu(void* gt, intptr_t parameter, enum menu_ctrl_t ctrl); // 0x800B8A74
extern void womp_background(char *tim_path); // 0x800B8AB0
#if !defined(PSX_VERSION)
extern void play_movie(char *name); // 0x800B8AE8
#endif
extern void menudefs_reset_hack_attract_mode(); // 0x800B8B30
extern void check_hack_attract(); // 0x800B8B54
extern int get_volume(void *gt, enum sfx_t sfx); // 0x800B8BC4
extern void set_volume(enum sfx_t sfx, int cooked); // 0x800B8C30
extern int do_sound_adjust(void *gt, intptr_t sfxparam, enum menu_ctrl_t ctrl); // 0x800B8CBC
extern void sound_item(void *gt, char *text, enum sfx_t sfx); // 0x800B8D38
extern int menudefs_toggle_dualshock(void *gt, intptr_t param, enum menu_ctrl_t ctrl); // 0x800B8D9C
extern int options_menu(void *gt, int index); // 0x800B8E00
extern int main_menu(void *gt, int index); // 0x800B8F48
extern int do_main_menu(void *gt, intptr_t param, enum menu_ctrl_t ctrl); // 0x800B904C
extern char * flashStart(); // 0x800B90A0
extern int menudefs_main_menu(void *gt, int index); // 0x800B92A4
extern int menudefs_confirmexit_menu(void *gt, int index); // 0x800B934C
extern int menudefs_pause_menu(void *gt, int index); // 0x800B93F4

#endif
