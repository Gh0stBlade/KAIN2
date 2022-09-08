#ifndef VM_H
#define VM_H

#include "STREAM.H"

extern void VM_Tick(long time); // 0x8004F638

extern void VM_UpdateMorph(struct Level *level, int initFlg); // 0x8004F658

extern void VM_VMObjectSetTable(struct Level *level, struct _VMObject *vmobject, int table); // 0x8004FA30


#endif
