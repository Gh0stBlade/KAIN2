#ifndef MEMCARD_H
#define MEMCARD_H


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

extern void memcard_item(void *opaque, TDRFuncPtr_memcard_item1fn fn, long parameter, long flags, char *text); // 0x800B9D78


typedef int (*TDRFuncPtr_memcard_item1fn)();




#endif
