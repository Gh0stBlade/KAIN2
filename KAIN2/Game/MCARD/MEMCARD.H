#ifndef MEMCARD_H
#define MEMCARD_H

#include "MENU/MENU.H"

typedef enum mcpsx_err_t // Hashcode: 0xB64074E4 (dec: -1237289756)
{
	mcpsx_err_none = 0,
	mcpsx_err_card_not_exist = 1,
	mcpsx_err_card_invalid = 2,
	mcpsx_err_new_card = 3,
	mcpsx_err_not_format = 4,
	mcpsx_err_file_not_exist = 5,
	mcpsx_err_already_exists = 6,
	mcpsx_err_block_full = 7,
	mcpsx_err_busy = 8,
	mcpsx_error = 9,
	mcpsx_err_num_items = 10,
} mcpsx_err_t;

typedef enum mcpsx_func_t // Hashcode: 0xA495DFC7 (dec: -1533681721)
{
	func_none = 0,
	func_exist = 1,
	func_accept = 2,
	func_read_file = 3,
	func_write_file = 4,
	func_create_file = 5,
	func_delete_file = 6,
	func_format = 7,
	func_unformat = 8,
	func_directory = 9,
	func_num_items = 10,
}mcpsx_func_t;

typedef enum mcpsx_sync_t // Hashcode: 0xC0D85FD5 (dec: -1059561515)
{
	sync_idle = -1,
	sync_busy = 0,
	sync_done = 1,
	sync_func = 2,
	sync_num_items = 3,
}mcpsx_sync_t;

typedef enum mcpsx_mode_t // Hashcode: 0x8A03A4E4 (dec: -1979472668)
{
	mode_bogus = 0,
	mode_initialized = 1,
	mode_running = 2,
	mode_terminated = 3,
	mode_num_items = 4,
}mcpsx_mode_t;

typedef enum state_t // Hashcode: 0x67CC6ED5 (dec: 1741450965)
{
	fsmcard_card_not_exist = 0,
	fsmcard_card_invalid = 1,
	fsmcard_not_format = 2,
	fsmcard_new_card = 3,
	fsmcard_directory = 4,
	fsmcard_ready = 5,
	fsmcard_format = 6,
	fsmcard_unformat = 7,
	fsmcard_delete = 8,
	fsmcard_create = 9,
	fsmcard_write = 10,
	fsmcard_read = 11,
	fsmcard_error = 12,
	fsmcard_num_items = 13,
}state_t;

typedef enum fsm_t // Hashcode: 0x64C4BA3C (dec: 1690614332)
{
	fsm_card_not_exist = 0,
	fsm_card_invalid = 1,
	fsm_not_format = 2,
	fsm_new_card = 3,
	fsm_ready = 4,
	fsm_format = 5,
	fsm_start = 6,
	fsm_loading = 7,
	fsm_query_overwrite = 8,
	fsm_overwrite = 9,
	fsm_saving = 10,
	fsm_create = 11,
	fsm_save_complete = 12,
	fsm_error = 13,
	fsm_num_items = 14,
}fsm_t;

typedef enum mcard_status_t // Hashcode: 0xD1B8064B (dec: -776468917)
{
	mcard_status_card_not_exist = 0,
	mcard_status_card_invalid = 1,
	mcard_status_not_format = 2,
	mcard_status_new_card = 3,
	mcard_status_ready = 4,
	mcard_status_accessing = 5,
	mcard_status_error = 6,
	mcard_status_num_items = 7,
}mcard_status_t;

struct mcpsx_directory_t // hashcode: 0xED9CBF92 (dec: -308494446)
{
	char name[20]; // size=20, offset=0
	int nbytes; // size=0, offset=20
};

union MEMCARD_79fake // Hashcode: 0xCBE8E87E (dec: -873928578)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void* fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: memcard.o, memcard.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: memcard.o, memcard.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: memcard.o, memcard.o
};

union MEMCARD_80fake // Hashcode: 0xCA90C9EC (dec: -896480788)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MEMCARD_117fake // hashcode: 0x86F561E0 (dec: -2030738976)
{
	struct {
		struct MultiSpline* posSpline; // size=0, offset=0
		struct MultiSpline* targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: memcard.o, memcard.o
	struct {
		_Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: memcard.o, memcard.o
	struct {
		struct _TFace* tface; // size=12, offset=0
		struct _TFace* oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: memcard.o, memcard.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace* tface; // size=12, offset=8
		struct _TFace* oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: memcard.o, memcard.o
};

struct mcard_t // hashcode: 0x55AF6CB2 (dec: 1437559986)
{
	struct {
		enum mcpsx_err_t err; // size=4, offset=0
		enum state_t fsm; // size=4, offset=4
		enum mcard_status_t status; // size=4, offset=8
		int not_exists; // size=0, offset=12
	} state; // size=16, offset=0, found in object files: memcard.o, memcard.o
	struct {
		long parameter; // size=0, offset=0
		long* nfilesptr; // size=0, offset=4
		struct mcpsx_directory_t* directory; // size=24, offset=8
		char* filename; // size=0, offset=12
		void* buffer; // size=0, offset=16
		int nbytes; // size=0, offset=20
		int nblocks; // size=0, offset=24
	} params; // size=28, offset=16, found in object files: memcard.o, memcard.o
	struct mcpsx_t* mcpsx; // size=56, offset=44
	void* opaque; // size=0, offset=48
};

struct mcmenu_t // hashcode: 0xCDD1805D (dec: -841908131)
{
	struct {
		enum fsm_t fsm; // size=4, offset=0
		enum fsm_t fsm_prev; // size=4, offset=4
		enum mcard_status_t status; // size=4, offset=8
		int running; // size=0, offset=12
		int slot; // size=0, offset=16
	} state; // size=20, offset=0, found in object files: memcard.o, memcard.o
	struct {
		int nblocks; // size=0, offset=0
		int nbytes; // size=0, offset=4
		void* buffer; // size=0, offset=8
		struct mcpsx_directory_t directory[15]; // size=360, offset=12
		long nfiles; // size=0, offset=372
	} params; // size=376, offset=20, found in object files: memcard.o, memcard.o
	struct mcard_t* mcard; // size=52, offset=396
	void* opaque; // size=0, offset=400
};

struct memcard_t // hashcode: 0xAEC9A344 (dec: -1362517180)
{
	struct Object* object; // size=76, offset=0
	struct mcmenu_table_t* table; // size=36, offset=4
	struct mcmenu_t* mcmenu; // size=404, offset=8
	short running; // size=0, offset=12
	short wrongVerison; // size=0, offset=14
};

union MEMCARD_196fake // Hashcode: 0xF7F7C377 (dec: -134757513)
{
	struct _Instance* target; // size=616, offset=0
	_Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	_Position throwVector; // size=6, offset=0
};

struct mcmenu_table_t // hashcode: 0xB78C7C17 (dec: -1215529961)
{
	int  (*data_size)(); // size=0, offset=0
	void (*initialize)(struct mcmenu_t*, struct memcard_t*, int); // size=0, offset=4
	void (*terminate)(); // size=0, offset=8
	void (*begin)(struct mcmenu_t*); // size=0, offset=12
	void (*end)(struct mcmenu_t*); // size=0, offset=16
	int (*set_buffer)(struct mcmenu_t*, void*, int); // size=0, offset=20
	int (*main)(struct mcmenu_t*, int index); // size=0, offset=24
	int (*pause)(struct mcmenu_t*, int index); // size=0, offset=28
	char* versionID; // size=0, offset=32
};

struct mcpsx_t // hashcode: 0x79B44761 (dec: 2041857889)
{
	struct {
		enum mcpsx_mode_t mode; // size=4, offset=0
		enum mcpsx_func_t func; // size=4, offset=4
		enum mcpsx_sync_t sync; // size=4, offset=8
		enum mcpsx_err_t err; // size=4, offset=12
		int observed; // size=0, offset=16
	} state; // size=20, offset=0, found in object files: memcard.o, memcard.o
	struct {
		long channel; // size=0, offset=0
		char* filename; // size=0, offset=4
		void* buffer; // size=0, offset=8
		long offset; // size=0, offset=12
		long nbytes; // size=0, offset=16
		long nblocks; // size=0, offset=20
		struct mcpsx_directory_t* directory; // size=24, offset=24
		long* nfilesptr; // size=0, offset=28
	} params; // size=32, offset=20, found in object files: memcard.o, memcard.o
	void* opaque; // size=0, offset=52
};

struct MEMCARD_216fake // hashcode: 0x1C10213E (dec: 470819134)
{
	struct mcmenu_t dummy1; // size=404, offset=0
	struct mcard_t dummy2; // size=52, offset=404
	struct mcpsx_t dummy3; // size=56, offset=456
};

struct SavedInfoTracker // hashcode: 0x03231C9A (dec: 52632730)
{
	char* MemoryCardBuffer; // size=0, offset=0
	char* InfoStart; // size=0, offset=4
	char* InfoEnd; // size=0, offset=8
	char* EndOfMemory; // size=0, offset=12
};

struct mcmenu 
{
	struct mcmenu_t dummy1; // size=404, offset=0
	struct mcard_t dummy2; // size=52, offset=404
	struct mcpsx_t dummy3; // size=56, offset=456
};

extern struct mcmenu gMcmenu;

typedef int (*TDRFuncPtr_memcard_item1fn)();

extern struct memcard_t gMemcard;
extern int the_header_size;
extern struct SavedInfoTracker savedInfoTracker;

extern int MEMCARD_IsWrongVersion(struct memcard_t *memcard); // 0x800B9924
extern void load(struct memcard_t *memcard); // 0x800B9938
extern void unload(struct memcard_t *memcard); // 0x800B9A04
extern int memcard_data_size(); // 0x800B9A58
extern int memcard_initialize(struct memcard_t *memcard, void *gt, int nblocks, void *buffer, int nbytes); // 0x800B9A60
extern void memcard_end(struct memcard_t *memcard); // 0x800B9B20
extern int maybe_start(struct memcard_t *memcard); // 0x800B9B64
extern int memcard_main_menu(void *gt, int index); // 0x800B9BD0
extern int memcard_pause_menu(void *gt, int index); // 0x800B9C24
extern void * gt2mcmenu(void *gt); // 0x800B9C78
extern void memcard_pop(void *opaque); // 0x800B9C8C
extern void memcard_start(void *opaque); // 0x800B9CC8
extern void memcard_load(void *opaque); // 0x800B9D0C
extern void memcard_save(void *opaque); // 0x800B9D58
extern void memcard_item(void* opaque, int (*fn)(void*, long, enum menu_ctrl_t), long parameter, long flags, char* text);
#endif
