#ifndef MENUDEFS_H
#define MENUDEFS_H

enum sfx_t : int 
{
	sfx_sound = 0,
	sfx_music = 1,
	sfx_voice = 2,
	sfx_error = 3,
};

union MENUDEFS_71fake // Hashcode: 0x2BC919B9 (dec: 734599609)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: menudefs.o, menudefs.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: menudefs.o, menudefs.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: menudefs.o, menudefs.o
};

union MENUDEFS_72fake // Hashcode: 0x8E6F3A2D (dec: -1905313235)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MENUDEFS_109fake // hashcode: 0x99AA84CD (dec: -1716878131)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: menudefs.o, menudefs.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: menudefs.o, menudefs.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: menudefs.o, menudefs.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: menudefs.o, menudefs.o
};

extern void do_check_controller(void *gt); // 0x800B8938
extern int do_push_menu(void *gt, long menuparam, enum menu_ctrl_t ctrl); // 0x800B896C
extern int do_pop_menu(void *gt, long param, enum menu_ctrl_t ctrl); // 0x800B89A4
extern int do_function(void *gt, long fnparam, enum menu_ctrl_t ctrl); // 0x800B89DC
extern int do_start_game(void *gt, long parameter, enum menu_ctrl_t ctrl); // 0x800B8A10
extern int do_save_menu(void *gt, long parameter, enum menu_ctrl_t ctrl); // 0x800B8A74
extern void womp_background(char *tim_path); // 0x800B8AB0
#if !defined(PSX_VERSION)
extern void play_movie(char *name); // 0x800B8AE8
#endif
extern void menudefs_reset_hack_attract_mode(); // 0x800B8B30
extern void check_hack_attract(); // 0x800B8B54
extern int get_volume(void *gt, enum sfx_t sfx); // 0x800B8BC4
extern void set_volume(enum sfx_t sfx, int cooked); // 0x800B8C30
extern int do_sound_adjust(void *gt, long sfxparam, enum menu_ctrl_t ctrl); // 0x800B8CBC
extern void sound_item(void *gt, char *text, enum sfx_t sfx); // 0x800B8D38
extern int menudefs_toggle_dualshock(void *gt, long param, enum menu_ctrl_t ctrl); // 0x800B8D9C
extern int options_menu(void *gt, int index); // 0x800B8E00
extern int main_menu(void *gt, int index); // 0x800B8F48
extern int do_main_menu(void *gt, long param, enum menu_ctrl_t ctrl); // 0x800B904C
extern char * flashStart(); // 0x800B90A0
extern int menudefs_main_menu(void *gt, int index); // 0x800B92A4
extern int menudefs_confirmexit_menu(void *gt, int index); // 0x800B934C
extern int menudefs_pause_menu(void *gt, int index); // 0x800B93F4

#endif
