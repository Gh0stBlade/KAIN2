EXTERN_C_START

void SynchronousCallback(HGLOBAL a1, DWORD a2, int a3);
DWORD __stdcall FileLoadThread(void* lpThreadParameter);
void ASLD_Free(HGLOBAL hMem);
HANDLE ASLD_OpenFile(LPCSTR lpFileName);
void ASLD_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead);
void ASLD_CloseFile(HANDLE hObject);
void ASLD_RequestFileData(const char* name, int pos, int size, void(__cdecl* fn)(HGLOBAL, DWORD, int), int read, int a6);
void ASLD_RequestFileDataFP(HANDLE handle, int pos, int read_size, void(__cdecl* fn_read)(HGLOBAL, DWORD, int), int read, int mode);
void ASLD_CancelRequestFP(HANDLE a1, int a2, int a3, void(__cdecl* a4)(HGLOBAL, DWORD, int), int a5);
int ASLD_GetFileData(const char* a1, int a2, int a3);
int ASLD_GetFileDataFP(void* a1, int a2, int a3);
void ASLD_Init(void);
int ASLD_IsBusy();
void ASLD_FinishLoading();
void ASLD_ReportStatus();
void ASLD_Shutdown();
FILE* ASLD_FetchFile(char* FileName, BYTE memType);

EXTERN_C_END
