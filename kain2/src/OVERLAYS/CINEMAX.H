#ifndef CINEMAX_H
#define CINEMAX_H

#define MAX_BUFFER_COUNT (3*2)

struct BufferInfo
{
	char* buffer[MAX_BUFFER_COUNT];

	unsigned short unk_18;
	unsigned short unk_1A;
	unsigned short unk_1C;
	unsigned short unk_1E;

	unsigned short unk_20;
	unsigned short unk_22;
	unsigned short unk_24;
	unsigned short unk_26;

	unsigned short unk_28;
	unsigned short unk_2A;
	unsigned short unk_2C;
	unsigned short unk_2E;

	unsigned short unk_30;
	unsigned short unk_32;
	unsigned short unk_34;
	unsigned short unk_36;
	unsigned short unk_38;

	int unk_3A;
	int unk_3C;
};

extern int CINEMAX_Play(char* strfile, unsigned short mask, int buffers);
extern int CINEMAX_ActuallyPlay(char* strfile, unsigned short mask, int buffers);
extern void CINEMAX_ClearBuffers();
extern void CINEMAX_VSync();
extern void CINEMAX_InitBufferDetails(char* b1, char* b2, char* b3, char* b4, BufferInfo* bd);
extern void CINEMAX_InitStream(char* buffer, CdlFILE* fp, void (*func)());
extern void CINEMAX_Unknown();
extern void CINEMAX_E1EB0(unsigned int* func, int unk2);
extern void CINEMAX_E1FCC();
extern int CINEMAX_E1658(BufferInfo* buffer_info);
extern int CINEMAX_E1708(BufferInfo* buffer_info);
extern void CINEMAX_E190C(CdlFILE* fp);

#endif