#ifndef DEBUG_H
#define DEBUG_H

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

enum DEBUG_LINE_TYPE // Hashcode: 0x03F31324 (dec: 66261796)
{
	DEBUG_LINE_TYPE_BIT = 0,
	DEBUG_LINE_TYPE_LONG = 1,
	DEBUG_LINE_TYPE_ACTION = 2,
	DEBUG_LINE_TYPE_ACTION_WITH_LINE = 3,
	DEBUG_LINE_TYPE_MENU = 4,
	DEBUG_LINE_TYPE_ENDLIST = 5,
	DEBUG_LINE_TYPE_FORMAT = 6,
	DEBUG_LINE_TYPE_INFO = 7,
};

struct GameCheat // hashcode: 0x9BFF36C0 (dec: -1677773120)
{
	unsigned char cheatKeys[12]; // size=12, offset=0
	short cheatLen; // size=0, offset=12
	short cheatStage; // size=0, offset=14
};

struct debug_format_t // hashcode: 0xCC4C6103 (dec: -867409661)
{
	int xpos; // size=0, offset=0
	int ypos; // size=0, offset=4
	int is_centered; // size=0, offset=8
};

union DEBUG_260fake // Hashcode: 0x8C9106F7 (dec: -1936652553)
{
	struct Intro* intro; // size=76, offset=0
	struct _SFXMkr* sfxMarker; // size=36, offset=0
};

struct DebugMenuLine // hashcode: 0x1BD18E81 (dec: 466718337)
{
	enum DEBUG_LINE_TYPE type; // size=4, offset=0
	long lower; // size=0, offset=4
	long upper; // size=0, offset=8
	char* text; // size=0, offset=12
	long* var_address; // size=0, offset=16
	long bit_mask; // size=0, offset=20
};

struct debug_dispatch_t // hashcode: 0x928EA8E4 (dec: -1836144412)
{
	enum DEBUG_LINE_TYPE type; // size=4, offset=0
	void (*fn)(); // size=0, offset=4
};

extern struct DebugMenuLine AlukaMenu[10];
extern struct DebugMenuLine AshVillageMenu[7];
extern struct DebugMenuLine OracleMenu[10];
extern struct DebugMenuLine PillarsMenu[6];
extern struct DebugMenuLine SilencedCathedralMenu[11];
extern struct DebugMenuLine SkinnerMenu[7];
extern struct DebugMenuLine StoneMenu[5];
extern struct DebugMenuLine SunLightMenu[7];
extern struct DebugMenuLine TombMenu[6];
extern struct DebugMenuLine WaterMenu[4];
extern struct DebugMenuLine CityMenu[5];
extern struct DebugMenuLine CliffMenu[3];
extern struct DebugMenuLine UnderMenu[3];
extern struct DebugMenuLine MorlockMenu[3];
extern struct DebugMenuLine HubAMenu[5];
extern struct DebugMenuLine HubBMenu[3];
extern struct DebugMenuLine TrainingMenu[5];
extern struct DebugMenuLine DarkEdenMenu[3];
extern struct DebugMenuLine BossAreasMenu[9];
extern struct DebugMenuLine AnteRoomsMenu[8];

extern struct DebugMenuLine debugRazielMenu[8];
extern struct DebugMenuLine debugSpecialAbilitiesMenu[10];
extern struct DebugMenuLine debugForgedAbilitiesMenu[7];
extern struct DebugMenuLine debugGlyphAbilitiesMenu[11];
extern struct DebugMenuLine debugImbueSoulReaverMenu[7];
extern struct DebugMenuLine levelSelectMenu[14];
extern struct DebugMenuLine level2SelectMenu[11];

extern struct DebugMenuLine standardMenu[12]; // offset 0x800CFCB0

extern struct DebugMenuLine* currentMenu; // offset 0x800CDB3C
extern struct DebugMenuLine pauseMenu[];

extern unsigned long debugRazielFlags1;
extern unsigned long debugRazielFlags2;
extern unsigned long debugRazielFlags3;

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
//extern int isdigit(char c); // 0x8001399C
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
extern void DEBUG_FatalError(const char *fmt, ...); // 0x800144F0
extern void DEBUG_ProcessSecondController(struct GameTracker *gameTracker); // 0x80014544
extern void DEBUG_ProcessCheat(struct GameTracker *gameTracker); // 0x8001454C
extern void DEBUG_DoAreaProtection(); // 0x800146C0

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
