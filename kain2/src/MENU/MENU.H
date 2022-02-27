#ifndef MENU_H
#define MENU_H

struct Extents2d 
{
	int xmin; // size=0, offset=0
	int xmax; // size=0, offset=4
	int ymin; // size=0, offset=8
	int ymax; // size=0, offset=12
};

struct menu_format_t // hashcode: 0xD91CEBF0 (dec: -652416016)
{
	int xpos; // size=0, offset=0
	int ypos; // size=0, offset=4
	int lineskip; // size=0, offset=8
	int itemskip; // size=0, offset=12
	int width; // size=0, offset=16
	int center; // size=0, offset=20
	int border; // size=0, offset=24
};

struct menu_stack_t // hashcode: 0x04101191 (dec: 68161937)
{
	int (*fn)(void*, int); // size=0, offset=0
	int index; // size=0, offset=4
	struct menu_format_t format; // size=28, offset=8
};

struct menu_item_t // hashcode: 0xF260D8A8 (dec: -228534104)
{
	char* text; // size=0, offset=0
	int (*fn)(); // size=0, offset=4
	long parameter; // size=0, offset=8
	long flags; // size=0, offset=12
};

enum menu_ctrl_t // Hashcode: 0xE0BD1E93 (dec: -524476781)
{
	menu_ctrl_none = 0,
	menu_ctrl_up = 1,
	menu_ctrl_down = 2,
	menu_ctrl_left = 3,
	menu_ctrl_right = 4,
	menu_ctrl_engage = 5,
	menu_ctrl_cancel = 6,
	menu_ctrl_start = 7,
	menu_ctrl_num_items = 8,
};

enum menu_sound_t // Hashcode: 0x0248C7C9 (dec: 38324169)
{
	menu_sound_none = 0,
	menu_sound_default = 1,
	menu_sound_select = 2,
	menu_sound_adjust = 3,
	menu_sound_engage = 4,
	menu_sound_pop = 5,
};

struct menu_t // hashcode: 0x8AB0D2F2 (dec: -1968123150)
{
	int nmenus; // size=0, offset=0
	int nitems; // size=0, offset=4
	int nbytes; // size=0, offset=8
	struct menu_stack_t stack[5]; // size=180, offset=12
	struct menu_item_t items[12]; // size=192, offset=192
	char bytes[512]; // size=512, offset=384
	void (*drawfn)(void*); // size=0, offset=896
	void* opaque; // size=0, offset=900
};

union MENU_71fake // Hashcode: 0x9417F09E (dec: -1810370402)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: menu.o, menu.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: menu.o, menu.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: menu.o, menu.o
};

union MENU_72fake // Hashcode: 0xE575C374 (dec: -445267084)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MENU_113fake // hashcode: 0x792A6BAF (dec: 2032823215)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: menu.o, menu.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: menu.o, menu.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: menu.o, menu.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: menu.o, menu.o
};

struct menu_sound_entry_t // hashcode: 0xFFB1DFB2 (dec: -5120078)
{
	enum menu_sound_t sound; // size=4, offset=0
	int sfx; // size=0, offset=4
};

typedef struct menu_sound_entry_t menu_sound_entry_t;

typedef int (*TDRFuncPtr_menu_item_flags1fn)();

typedef int (*TDRFuncPtr_menu_item1fn)();

typedef int (*TDRFuncPtr_menu_push1fn)();

typedef int (*TDRFuncPtr_menu_set1fn)();

extern int menu_data_size(); // 0x800B7A40
extern void menu_initialize(struct menu_t *menu, void *opaque); // 0x800B7A48
extern void menu_format(struct menu_t *menu, int center, int xpos, int ypos, int width, int lineskip, int itemskip, int border); // 0x800B7A8C
extern void menu_set(struct menu_t *menu, int fn(void*, int)); // 0x800B7ADC
extern void menu_push(struct menu_t *menu, int fn(void*, int)); // 0x800B7B00
extern void menu_pop(struct menu_t *menu); // 0x800B7B70
extern void menu_item_flags(struct menu_t *menu, TDRFuncPtr_menu_item_flags1fn fn, long parameter, long flags, char *format); // 0x800B7B84
extern void menu_item(struct menu_t *menu, int (*fn)(void*, long, enum menu_ctrl_t), long parameter, char *format); // 0x800B7C14
extern void menu_build(struct menu_t *menu); // 0x800B7CB8
extern void DisplayHintBox(int len, int y); // 0x800B7D10
extern void DisplayMenuBox(int x0, int x1, int y0, int y1); // 0x800B7E3C
extern int menu_draw_item(struct menu_t *menu, int ypos, int xadj, int yadj, char *text, int color, long flags, struct Extents2d *e); // 0x800B8074
extern void menu_draw(struct menu_t *menu); // 0x800B8478
extern void menu_run(struct menu_t *menu); // 0x800B85D8
extern void menu_process(struct menu_t *menu); // 0x800B87C8


#endif
