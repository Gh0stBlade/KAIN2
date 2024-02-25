#ifndef MENUUTIL_H
#define MENUUTIL_H

#include "MENU.H"

extern enum menu_ctrl_t menu_get_ctrl(void *gt); // 0x800B87FC
extern void menu_print(int xpos, int ypos, char *text, int color); // 0x800B887C
extern int menu_text_width(char *text); // 0x800B88E4
extern void menu_sound(enum menu_sound_t sound); // 0x800B8904

#endif
