#ifndef MCARDX_H
#define MCARDX_H

#include "CORE.H"

#include "MCARD/MEMCARD.H"

extern void MCARDX_initialize(mcmenu_t* mcmenu, memcard_t* memcard, int nblocks);
extern int MCARDX_set_buffer(mcmenu_t* mcmenu, void* buffer, int nbytes);
extern void MCARDX_begin();
extern int MCARDX_main(mcmenu_t* mcmenu, int index);

#endif