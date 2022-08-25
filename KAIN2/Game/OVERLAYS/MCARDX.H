#ifndef MCARDX_H
#define MCARDX_H

#include "CORE.H"

#include "MCARD/MEMCARD.H"

extern void MCARDX_initialize(struct mcmenu_t* mcmenu, struct memcard_t* memcard, int nblocks);
extern int MCARDX_set_buffer(struct mcmenu_t* mcmenu, void* buffer, int nbytes);
extern void MCARDX_begin(struct mcmenu_t* mcmenu);
extern int MCARDX_main(struct mcmenu_t* mcmenu, int index);
extern void sub_80163C8C(struct mcpsx_t* mcpsx);
extern void sub_80164380(struct mcard_t* mcard);

#endif