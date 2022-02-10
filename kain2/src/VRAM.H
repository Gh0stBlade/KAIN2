#ifndef VRAM_H
#define VRAM_H


extern void VRAM_InitVramBlockCache(); // 0x800728D0

extern void VRAM_EnableTerrainArea(); // 0x80072930

extern void VRAM_DisableTerrainArea(); // 0x80072960

extern int VRAM_ConcatanateMemory(struct _BlockVramEntry *curBlock); // 0x8007298C

extern void VRAM_GarbageCollect(); // 0x80072B74

extern int VRAM_InsertFreeBlock(struct _BlockVramEntry *block); // 0x80072BA8

extern void VRAM_DeleteFreeBlock(struct _BlockVramEntry *block); // 0x80072C38

extern void VRAM_InsertUsedBlock(struct _BlockVramEntry *block); // 0x80072CA0

extern void VRAM_DeleteUsedBlock(struct _BlockVramEntry *block); // 0x80072CD4

extern struct _BlockVramEntry * VRAM_GetOpenBlock(); // 0x80072D38

extern int VRAM_DeleteFreeVram(short x, short y, short w, short h); // 0x80072D6C

extern int VRAM_InsertFreeVram(short x, short y, short w, short h, int flags); // 0x80072E9C

extern struct _BlockVramEntry * VRAM_CheckVramSlot(short *x, short *y, short w, short h, int type, int startY); // 0x80073030

extern void VRAM_ClearVramBlock(struct _BlockVramEntry *block); // 0x8007342C

extern void AdjustVramCoordsObject(int oldx, int oldy, int newx, int newy, struct Object *object); // 0x80073460

extern struct _BlockVramEntry * VRAM_InsertionSort(struct _BlockVramEntry *rootNode, struct _BlockVramEntry *newBlock, int pack_type); // 0x8007364C

extern void VRAM_ClearVram(); // 0x80073704

extern struct _BlockVramEntry * VRAM_RearrangeVramsLayer(long whichLayer, short *AddX, short *AddY, short AddW, int AddH, int AddType, int AddStartY); // 0x80073740

extern void VRAM_TransferBufferToVram(void *dataPtr, long dataSize, short status, void *data1, void *data2); // 0x80073DC8

extern void VRAM_LoadReturn(void *dataPtr, void *data1, void *data2); // 0x80074040

extern long VRAM_GetObjectVramSpace(struct VramSize *vramSize, struct _ObjectTracker *objectTracker); // 0x80074060

extern void VRAM_InitMorphPalettes(); // 0x80074158

extern void VRAM_UpdateMorphPalettes(); // 0x80074160

extern void MORPH_ChangeAreaPalettes(long time); // 0x80074168


#endif
