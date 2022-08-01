#ifndef RESOLVE_H
#define RESOLVE_H

#include "CORE.H"

struct RedirectList // hashcode: 0x4C185B3B (dec: 1276664635)
{
	long numPointers; // size=0, offset=0
	long* data; // size=0, offset=4
};

extern void RESOLVE_Pointers(struct RedirectList *redirectList, long *data, long *baseAddr); // 0x8003C678

#endif
