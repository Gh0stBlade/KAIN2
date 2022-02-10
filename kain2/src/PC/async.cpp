#include <windows.h>
#include <stdio.h>

class ASLD_File
{
public:
	ASLD_File *next;
	int index;
	DWORD type;
	char name[256];
	HANDLE handle;
	int read_size;
	int pos;
	void(__cdecl* fn_read)(HGLOBAL, DWORD, int);
	int read;
};

HANDLE asld_evt, asld_obj, bigfileid;
ASLD_File asld_ftbl[256],
	*asld_fcur;
int asld_actv, ASLD_Debug, SuperSlowLoads;

//0001:00076460       _SynchronousCallback       00477460 f   asyncld.obj
void SynchronousCallback(HGLOBAL a1, DWORD a2, int a3)
{
	*(DWORD*)a3 = (DWORD)a1;
}
//0001:000765f0       _FileLoadThread            004775f0 f   asyncld.obj
DWORD __stdcall FileLoadThread(void* lpThreadParameter)
{
	HANDLE CurrentThread; // eax
	ASLD_File* next; // esi
	ASLD_File* v3; // eax
	ASLD_File* v4; // edi
	ASLD_File* f; // ecx
	void* type; // esi
	HGLOBAL buffer; // ebx
	HANDLE handle; // eax
	HANDLE FileA; // edi
	void(__cdecl * fn_read)(HGLOBAL, DWORD, int); // eax
	ASLD_File* v11; // eax
	ASLD_File* v12; // ecx
	void* v13; // esi
	DWORD result; // eax
	DWORD NumberOfBytesRead; // [esp+10h] [ebp-8h] BYREF
	int v16; // [esp+14h] [ebp-4h]

	CurrentThread = GetCurrentThread();
	SetThreadPriority(CurrentThread, THREAD_PRIORITY_LOWEST);
	v16 = 0;
	do
	{
		WaitForSingleObject(asld_evt, INFINITE);
		next = asld_ftbl[0].next;
		SetEvent(asld_evt);
		if (!next)
			goto LABEL_38;
		if (next->index)
		{
			WaitForSingleObject(asld_evt, INFINITE);
			v3 = asld_ftbl[0].next;
			v4 = 0;
			f = asld_ftbl;
			if (asld_ftbl[0].next)
			{
				while (v3 != next)
				{
					f = v3;
					v3 = v3->next;
					if (!v3)
						goto LABEL_10;
				}
				v4 = next;
				f->next = next->next;
				type = (void*)next->type;
				if (type != (void*)0xDEADBEEF)
					SetEvent(type);
			}
		LABEL_10:
			SetEvent(asld_evt);
			if (v4)
				goto LABEL_37;
		}
		else
		{
			if (SuperSlowLoads && next->handle == bigfileid)
				Sleep(0x1F40u);
			buffer = GlobalAlloc(4096u, next->read_size);
			handle = next->handle;
			if (handle)
			{
				SetFilePointer(handle, next->pos, 0, 0);
				if (!ReadFile(next->handle, buffer, next->read_size, &NumberOfBytesRead, 0))
					NumberOfBytesRead = 0;
			}
			else
			{
				FileA = CreateFileA(next->name, 0x80000000, 1u, 0, 3u, 0x80u, 0);
				SetFilePointer(FileA, next->pos, 0, 0);
				if (!ReadFile(FileA, buffer, next->read_size, &NumberOfBytesRead, 0))
					NumberOfBytesRead = 0;
				CloseHandle(FileA);
			}
			if (NumberOfBytesRead != next->read_size)
			{
				if (next->handle)
					DBG_Print("Data read error on file: %x\n", next->handle);
				else
					DBG_Print("Data read error on file: %s\n", next->name);
				DBG_Print("Offset: %i, Reqsize: %i, Readsize: %i\n", next->pos, next->read_size, NumberOfBytesRead);
				if (++v16 <= 8)
				{
					GlobalFree(buffer);
					goto LABEL_38;
				}
			}
			fn_read = next->fn_read;
			v4 = 0;
			v16 = 0;
			if (fn_read)
			{
				fn_read(buffer, NumberOfBytesRead, next->read);
			}
			else
			{
				DBG_Print("No callback in ASLD system!\n");
				GlobalFree(buffer);
			}
			WaitForSingleObject(asld_evt, 0xFFFFFFFF);
			v11 = asld_ftbl[0].next;
			v12 = asld_ftbl;
			if (asld_ftbl[0].next)
			{
				while (v11 != next)
				{
					v12 = v11;
					v11 = v11->next;
					if (!v11)
						goto LABEL_36;
				}
				v4 = next;
				v12->next = next->next;
				v13 = (void*)next->type;
				if (v13 != (void*)0xDEADBEEF)
					SetEvent(v13);
			}
		LABEL_36:
			SetEvent(asld_evt);
			if (v4)
			{
			LABEL_37:
				WaitForSingleObject(asld_evt, 0xFFFFFFFF);
				v4->next = asld_fcur;
				asld_fcur = v4;
				SetEvent(asld_evt);
			}
		}
	LABEL_38:
		result = asld_actv;
	} while (!asld_actv);
	return result;
}

//0001:00075e90       _ASLD_Free                 00476e90 f   asyncld.obj
void __cdecl ASLD_Free(HGLOBAL hMem)
{
	GlobalFree(hMem);
}
//0001:00075ea0       _ASLD_OpenFile             00476ea0 f   asyncld.obj
HANDLE __cdecl ASLD_OpenFile(LPCSTR lpFileName)
{
	return CreateFileA(lpFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}
//0001:00075ec0       _ASLD_ReadFile             00476ec0 f   asyncld.obj
void __cdecl ASLD_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead)
{
	ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, &nNumberOfBytesToRead, 0);
}
//0001:00075ee0       _ASLD_CloseFile            00476ee0 f   asyncld.obj
void __cdecl ASLD_CloseFile(HANDLE hObject)
{
	ASLD_File* i; // eax

	WaitForSingleObject(asld_evt, INFINITE);
	for (i = asld_ftbl[0].next; i; i = i->next)
	{
		if (i->handle == hObject)
			i->index = 1;
	}
	SetEvent(asld_evt);
	CloseHandle(hObject);
}

//0001:00075f30       _ASLD_RequestFileData      00476f30 f   asyncld.obj
void __cdecl ASLD_RequestFileData(const char* name, int pos, int size, void(__cdecl* fn)(HGLOBAL, DWORD, int), int read, int a6)
{
	ASLD_File* find; // ebp
	ASLD_File* f; // ebp
	ASLD_File* next; // eax
	ASLD_File* i; // ecx

	WaitForSingleObject(asld_evt, INFINITE);
	for (find = asld_ftbl[0].next; find; find = find->next)
	{
		if (!find->index && !strcmp(find->name, name) && find->pos == pos && find->read_size == size && find->fn_read == fn && find->read == read)
			break;
	}
	SetEvent(asld_evt);

	if (!find)
	{
		WaitForSingleObject(asld_evt, INFINITE);
		f = asld_fcur;
		if (asld_fcur)
			asld_fcur = asld_fcur->next;
		else
			f = 0;
		SetEvent(asld_evt);
		while (!f)
		{
			WaitForSingleObject(asld_evt, INFINITE);
			f = asld_fcur;
			if (asld_fcur)
				asld_fcur = asld_fcur->next;
			else
				f = 0;
			SetEvent(asld_evt);
		}
		f->index = 0;
		f->type = 0xDEADBEEF;
		strcpy_s(f->name, sizeof(f->name), name);
		f->fn_read = fn;
		f->read_size = size;
		f->handle = 0;
		f->pos = pos;
		f->read = read;
		if (a6)
		{
			WaitForSingleObject(asld_evt, INFINITE);
			next = asld_ftbl[0].next;
			for (i = asld_ftbl; next; next = next->next)
				i = next;
			i->next = f;
			f->next = 0;
			SetEvent(asld_evt);
		}
		else
		{
			WaitForSingleObject(asld_evt, INFINITE);
			f->next = asld_ftbl[0].next;
			asld_ftbl[0].next = f;
			SetEvent(asld_evt);
		}
	}
}

//0001:00076120       _ASLD_RequestFileDataFP    00477120 f   asyncld.obj
void __cdecl ASLD_RequestFileDataFP(HANDLE handle, int pos, int read_size, void(__cdecl* fn_read)(HGLOBAL, DWORD, int), int read, int mode)
{
	ASLD_File* find; // esi
	ASLD_File* f; // esi
	ASLD_File* next; // eax
	ASLD_File* j; // ecx

	WaitForSingleObject(asld_evt, INFINITE);
	for (find = asld_ftbl[0].next; find; find = find->next)
	{
		if (!find->index && find->handle == handle && find->pos == pos && find->read_size == read_size && find->fn_read == fn_read && find->read == read)
			break;
	}
	SetEvent(asld_evt);

	if (!find)
	{
		WaitForSingleObject(asld_evt, INFINITE);
		f = asld_fcur;
		if (asld_fcur)
			asld_fcur = asld_fcur->next;
		else
			f = 0;
		SetEvent(asld_evt);
		while (!f)
		{
			WaitForSingleObject(asld_evt, INFINITE);
			f = asld_fcur;
			if (asld_fcur)
				asld_fcur = asld_fcur->next;
			else
				f = 0;
			SetEvent(asld_evt);
		}
		f->pos = pos;
		f->handle = handle;
		f->read_size = read_size;
		f->index = 0;
		f->type = 0xDEADBEEF;
		f->name[0] = 0;
		f->fn_read = fn_read;
		f->read = read;
		if (mode)
		{
			WaitForSingleObject(asld_evt, INFINITE);
			next = asld_ftbl[0].next;
			for (j = asld_ftbl; next; next = next->next)
				j = next;
			j->next = f;
			f->next = 0;
			SetEvent(asld_evt);
		}
		else
		{
			WaitForSingleObject(asld_evt, INFINITE);
			f->next = asld_ftbl[0].next;
			asld_ftbl[0].next = f;
			SetEvent(asld_evt);
		}
	}
}
//0001:000762b0       _ASLD_CancelRequestFP      004772b0 f   asyncld.obj
void __cdecl ASLD_CancelRequestFP(HANDLE a1, int a2, int a3, void(__cdecl* a4)(HGLOBAL, DWORD, int), int a5)
{
	ASLD_File* i; // eax

	WaitForSingleObject(asld_evt, INFINITE);
	for (i = asld_ftbl[0].next; i; i = i->next)
	{
		if (i->handle == a1 && i->pos == a2 && i->read_size == a3 && i->fn_read == a4 && i->read == a5)
			i->index = 1;
	}
	SetEvent(asld_evt);
}
//0001:00076330       _ASLD_GetFileData          00477330 f   asyncld.obj
int __cdecl ASLD_GetFileData(const char* a1, int a2, int a3)
{
	HANDLE EventA; // edi
	ASLD_File* v4; // ebx
	int v6; // [esp+14h] [ebp-4h] BYREF

	EventA = CreateEventA(0, 0, 0, 0);
	WaitForSingleObject(asld_evt, INFINITE);
	v4 = asld_fcur;
	if (asld_fcur)
		asld_fcur = asld_fcur->next;
	else
		v4 = 0;
	SetEvent(asld_evt);
	while (!v4)
	{
		WaitForSingleObject(asld_evt, INFINITE);
		v4 = asld_fcur;
		if (asld_fcur)
			asld_fcur = asld_fcur->next;
		else
			v4 = 0;
		SetEvent(asld_evt);
	}
	v4->type = (int)EventA;
	v4->index = 0;
	strcpy(v4->name, a1);
	v4->handle = 0;
	v4->read_size = a3;
	v4->pos = a2;
	v4->fn_read = SynchronousCallback;
	v4->read = (int)&v6;
	WaitForSingleObject(asld_evt, INFINITE);
	v4->next = asld_ftbl[0].next;
	asld_ftbl[0].next = v4;
	SetEvent(asld_evt);
	WaitForSingleObject(EventA, INFINITE);
	CloseHandle(EventA);
	return v6;
}
//0001:00076470       _ASLD_GetFileDataFP        00477470 f   asyncld.obj
int __cdecl ASLD_GetFileDataFP(void* a1, int a2, int a3)
{
	ASLD_File* f; // esi
	HANDLE hHandle; // [esp+Ch] [ebp-8h]
	int v6; // [esp+10h] [ebp-4h] BYREF

	hHandle = CreateEventA(0, 0, 0, 0);
	WaitForSingleObject(asld_evt, INFINITE);
	f = asld_fcur;
	if (asld_fcur)
		asld_fcur = asld_fcur->next;
	else
		f = 0;
	SetEvent(asld_evt);
	while (!f)
	{
		WaitForSingleObject(asld_evt, INFINITE);
		f = asld_fcur;
		if (asld_fcur)
			asld_fcur = asld_fcur->next;
		else
			f = 0;
		SetEvent(asld_evt);
	}
	f->type = (int)hHandle;
	f->handle = a1;
	f->index = 0;
	f->name[0] = 0;
	f->read_size = a3;
	f->pos = a2;
	f->fn_read = SynchronousCallback;
	f->read = (int)&v6;
	WaitForSingleObject(asld_evt, INFINITE);
	f->next = asld_ftbl[0].next;
	asld_ftbl[0].next = f;
	SetEvent(asld_evt);
	WaitForSingleObject(hHandle, INFINITE);
	CloseHandle(hHandle);
	return v6;
}
//0001:00076580       _ASLD_Init                 00477580 f   asyncld.obj
void ASLD_Init(void)
{
	ASLD_File* f; // ecx
	ASLD_File* p_index; // eax
	DWORD ThreadId; // [esp+0h] [ebp-4h] BYREF

	asld_ftbl[0].next = 0;
	f = 0;
	p_index = &asld_ftbl[255];
	for(int i = 0; i<256; i++)
	{
		p_index->index = (int)f;
		f = p_index--;
	}
	asld_fcur = f;
	asld_actv = 0;
	asld_evt = CreateEventA(0, 0, 1, 0);
	asld_obj = CreateThread(0, 0, FileLoadThread, 0, 0, &ThreadId);
}
//0001:000768b0       _ASLD_IsBusy               004778b0 f   asyncld.obj
int ASLD_IsBusy()
{
	int busy; // esi
	ASLD_File* f; // eax

	busy = 0;
	WaitForSingleObject(asld_evt, INFINITE);
	for (f = asld_ftbl[0].next; f; ++busy)
		f = f->next;
	SetEvent(asld_evt);
	return busy;
}
//0001:000768f0       _ASLD_FinishLoading        004778f0 f   asyncld.obj
void __cdecl ASLD_FinishLoading()
{
	int v0; // ebx
	int loading; // esi
	ASLD_File* i; // eax

	v0 = 0;
	DBG_Print("\n");
	while (1)
	{
		loading = 0;
		WaitForSingleObject(asld_evt, 0xFFFFFFFF);
		for (i = asld_ftbl[0].next; i; ++loading)
			i = i->next;
		SetEvent(asld_evt);
		if (!loading)
			break;
		if (loading != v0)
			DBG_Print("\n%d files loading", loading);
		DBG_Print(".");
		v0 = loading;
	}
}
//0001:00076960       _ASLD_ReportStatus         00477960 f   asyncld.obj
void __cdecl ASLD_ReportStatus()
{
	ASLD_File* i; // esi
	HANDLE handle; // eax

	if (ASLD_Debug)
	{
		FONT_Print("Files in queue:\n");
		WaitForSingleObject(asld_evt, 0xFFFFFFFF);
		for (i = asld_ftbl[0].next; i; i = i->next)
		{
			handle = i->handle;
			if (handle)
				FONT_Print("%x  %i  %i\n", handle, i->pos, i->read_size);
			else
				FONT_Print("%s  %i  %i\n", i->name, i->pos, i->read_size);
		}
		SetEvent(asld_evt);
	}
}

//0001:000769f0       _ASLD_Shutdown             004779f0 f   asyncld.obj
void __cdecl ASLD_Shutdown()
{
	asld_actv = 1;
	CloseHandle(asld_obj);
	CloseHandle(asld_evt);
}

//0001:00076a20       _ASLD_FetchFile            00477a20 f   asyncld.obj
FILE* __cdecl ASLD_FetchFile(char* FileName, BYTE memType)
{
	FILE* fp; // esi
	int size; // edi
	char* buf; // ebx

	fp = fopen(FileName, "rb");
	if (fp)
	{
		fseek(fp, 0, 2);
		size = ftell(fp);
		fseek(fp, 0, 0);
		buf = MEMPACK_Malloc(size, memType);
		fread(buf, 1, size, fp);
		fclose(fp);
		return (FILE*)buf;
	}
	return fp;
}