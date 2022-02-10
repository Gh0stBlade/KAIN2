#ifndef DEBUG_H
#define DEBUG_H


extern void DEBUG_UpdateHealth(long *var); // 0x80012EC0

extern void DEBUG_UpdateMana(long *var); // 0x80012EE4

extern void DEBUG_FillUpHealth(long *var); // 0x80012F08

extern void process_cheat_codes(struct GameTracker *gt, long *ctrl); // 0x80012F30

extern void DEBUG_Process(struct GameTracker *gameTracker); // 0x800131F8

extern void DEBUG_Draw(struct GameTracker *gameTracker, unsigned long **ot); // 0x800133A4

extern long DEBUG_MenuCountLength(struct DebugMenuLine *menu); // 0x800133EC

extern void DEBUG_ExitMenus(); // 0x80013418

extern struct DebugMenuLine * get_last_menu_line(struct DebugMenuLine *line); // 0x800134B0

extern int num_menu_items(struct DebugMenuLine *menu); // 0x800134E0

extern void maybe_change_menu_choice(struct GameTracker *gt, struct DebugMenuLine *menu); // 0x80013510

extern void handle_line_type_long(struct GameTracker *gt, struct DebugMenuLine *line); // 0x800135D8

extern void handle_line_type_bit(struct GameTracker *gt, struct DebugMenuLine *line); // 0x800136C4

extern void handle_line_type_action(struct GameTracker *gt, struct DebugMenuLine *line); // 0x8001371C

extern void handle_line_type_action_with_line(struct GameTracker *gt, struct DebugMenuLine *line); // 0x80013758

extern void handle_line_type_menu(struct GameTracker *gt, struct DebugMenuLine *line); // 0x800137BC

extern void process_menu_line(struct GameTracker *gt, struct DebugMenuLine *menu); // 0x8001383C

extern int pre_process_functions(struct GameTracker *gt, struct DebugMenuLine *menu); // 0x800138A0

extern void post_process_functions(struct GameTracker *gt, struct DebugMenuLine *menu); // 0x800138C8

extern void set_debug_leading(); // 0x8001396C

extern void set_user_leading(); // 0x80013984

extern int isdigit(char c); // 0x8001399C

extern void adjust_format(char *ctrl, struct debug_format_t *fmt); // 0x800139AC

extern char * find_eol(char *text); // 0x80013B54

extern void draw_menu_item(struct GameTracker *gt, struct debug_format_t *fmt, char *text); // 0x80013B8C

extern void draw_menu(struct GameTracker *gt, struct DebugMenuLine *menu); // 0x80013C8C

extern void DEBUG_Menu(struct GameTracker *gt); // 0x80013E98

extern void DEBUG_DisplayStatus(struct GameTracker *gameTracker); // 0x80013FAC

extern void DEBUG_DrawShrinkCels(unsigned long **polyAddr); // 0x80014214

extern void DEBUG_ContinueGame(); // 0x8001421C

extern void DEBUG_ExitGame(); // 0x80014234

extern void DEBUG_EndViewVram(struct GameTracker *gameTracker); // 0x80014270

extern void DEBUG_ViewVram(struct GameTracker *gameTracker); // 0x800142C0

extern void DEBUG_CaptureScreen(struct GameTracker *gameTracker); // 0x800143E0

extern void DEBUG_PageFlip(); // 0x800143E8

extern void DEBUG_FatalError(char *fmt); // 0x800144F0

extern void DEBUG_ProcessSecondController(struct GameTracker *gameTracker); // 0x80014544

extern void DEBUG_ProcessCheat(struct GameTracker *gameTracker); // 0x8001454C

extern void DEBUG_DoAreaProtection(); // 0x800146C0


#endif
