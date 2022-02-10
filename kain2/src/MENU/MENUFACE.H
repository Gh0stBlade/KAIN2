#ifndef MENUFACE_H
#define MENUFACE_H


extern char * NextTimAddr(char *addr, int w, int h, enum bdepth bpp); // 0x800B9538

extern void menuface_initialize(); // 0x800B9580

extern void menuface_terminate(); // 0x800B96AC

extern void MENUFACE_ChangeStateRandomly(int index); // 0x800B9758

extern void MENUFACE_RefreshFaces(); // 0x800B987C


#endif
