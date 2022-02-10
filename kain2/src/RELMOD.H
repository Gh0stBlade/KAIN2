#ifndef RELMOD_H
#define RELMOD_H


extern void RELMOD_InitModulePointers(int baseaddr, int *relocs); // 0x8007C974

extern void RELMOD_RelocModulePointers(int baseaddr, int offset, int *relocs); // 0x8007CA58


#endif
