#ifndef LOAD3D_H
#define LOAD3D_H


extern void LOAD_InitCd(); // 0x80036D44

extern void LOAD_CdSeekCallback(unsigned char intr, unsigned char *result); // 0x80036D74

extern void LOAD_CdDataReady(); // 0x80036D90

extern void LOAD_CdReadReady(unsigned char intr, unsigned char *result); // 0x80036EB0

extern void LOAD_UpdateCheckSum(long bytes); // 0x80036F9C

extern void LOAD_DoCDReading(); // 0x80036FE0

extern void LOAD_DoCDBufferedReading(); // 0x8003708C

extern void LOAD_SetupFileToDoCDReading(); // 0x800370A8

extern void LOAD_SetupFileToDoBufferedCDReading(); // 0x80037124

extern void LOAD_ProcessReadQueue(); // 0x80037198

extern char * LOAD_ReadFileFromCD(char *filename, int memType); // 0x80037314

extern void LOAD_CdReadFromBigFile(long fileOffset, unsigned long *loadAddr, long bytes, long chksumLevel, long checksum); // 0x800373F4

extern struct _BigFileDir * LOAD_ReadDirectory(struct _BigFileDirEntry *dirEntry); // 0x80037420

extern void LOAD_InitCdLoader(char *bigFileName, char *voiceFileName); // 0x80037488

extern int LOAD_SetupFileInfo(struct _NonBlockLoadEntry *loadEntry); // 0x800375DC

extern void LOAD_CD_ReadPartOfFile(struct _NonBlockLoadEntry *loadEntry); // 0x80037664

extern long LOAD_HashName(char *string); // 0x80037704

extern long LOAD_HashUnit(char *name); // 0x80037858

extern struct _BigFileEntry * LOAD_GetBigFileEntryByHash(long hash); // 0x800378FC

extern struct _BigFileEntry * LOAD_GetBigFileEntry(char *fileName); // 0x80037984

extern long LOAD_DoesFileExist(char *fileName); // 0x800379AC

extern void LOAD_NonBlockingReadFile(struct _NonBlockLoadEntry *loadEntry); // 0x800379E0

extern void LOAD_LoadTIM(long *addr, long x_pos, long y_pos, long clut_x, long clut_y); // 0x80037A9C

extern void LOAD_LoadTIM2(long *addr, long x_pos, long y_pos, long width, long height); // 0x80037B3C

extern long LOAD_RelocBinaryData(long *data, long fileSize); // 0x80037B84

extern void LOAD_CleanUpBuffers(); // 0x80037C3C

extern void * LOAD_InitBuffers(); // 0x80037C84

extern void LOAD_InitCdStreamMode(); // 0x80037CD4

extern void LOAD_DumpCurrentDir(); // 0x80037D1C

extern int LOAD_ChangeDirectoryByID(int id); // 0x80037D6C

extern void LOAD_SetSearchDirectory(long id); // 0x80037E74

extern long LOAD_GetSearchDirectory(); // 0x80037E80

extern int LOAD_ChangeDirectoryFlag(); // 0x80037E8C

extern void LOAD_UpdateBigFilePointers(struct _BigFileDir *oldDir, struct _BigFileDir *newDir); // 0x80037E98

extern int LOAD_IsFileLoading(); // 0x80037EC8

extern void LOAD_StopLoad(); // 0x80037ED4


#endif
