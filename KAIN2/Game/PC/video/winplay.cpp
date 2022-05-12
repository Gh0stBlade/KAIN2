#include <windows.h>

//0001:000da90a       Player_ShutDownSoundSystem 004db90a f   Winplay:winplay.dll
void _Player_ShutDownSoundSystem();

//0001:000da910       Player_ShutDownVideoSystem 004db910 f   Winplay:winplay.dll
void Player_ShutDownVideoSystem();

//0001:000da916       Player_InitSoundSystem     004db916 f   Winplay:winplay.dll
int Player_InitSoundSystem(HWND hWnd) { return 0; }
//0001:000da91c       Player_InitVideoSystem     004db91c f   Winplay:winplay.dll
int Player_InitVideoSystem(HWND hWnd, int a2) { return 0; }
//0001:000da922       Player_ShutDownMovie       004db922 f   Winplay:winplay.dll
int Player_ShutDownMovie() { return 0; }
//0001:000da928       Player_ShutDownVideo       004db928 f   Winplay:winplay.dll
int Player_ShutDownVideo() { return 0; }
//0001:000da92e       Player_ShutDownSound       004db92e f   Winplay:winplay.dll
int Player_ShutDownSound() { return 0; }
//0001:000da934       Player_ReturnPlaybackMode  004db934 f   Winplay:winplay.dll
int Player_ReturnPlaybackMode() { return 0; }
//0001:000da93a       Player_StopTimer           004db93a f   Winplay:winplay.dll
int Player_StopTimer(DWORD* pMovie);
//0001:000da940       Player_PlayFrame           004db940 f   Winplay:winplay.dll
int Player_PlayFrame(DWORD* pMovie, DWORD* pVideo, DWORD* pSound, int unk1/*0*/, DWORD** pFrame, int unk2/*0*/, int unk3/*0*/, int unk4/*0*/);
//0001:000da946       Player_BlankScreen         004db946 f   Winplay:winplay.dll
int Player_BlankScreen(int x0, int y0, int x1, int y1);
//0001:000da94c       Player_StartTimer          004db94c f   Winplay:winplay.dll
int Player_StartTimer(DWORD* pMovie);
//0001:000da952       Player_InitMoviePlayback   004db952 f   Winplay:winplay.dll
int Player_InitMoviePlayback(DWORD* pMovie, DWORD* pVideo, DWORD* pSound);
//0001:000da958       Player_MapVideo            004db958 f   Winplay:winplay.dll
int Player_MapVideo(DWORD* pVideo, int unk1/*0*/);
//0001:000da95e       Player_InitSound           004db95e f   Winplay:winplay.dll
int Player_InitSound(DWORD** pSound, int unk1/*0x4000*/, int unk2/*4*/, int unk3/*0*/, int unk4/*0x1000*/, int unk5/*2*/, int unk6/*0xAC44*/, int unk7/*4*/, int unk8/*2*/);
//0001:000da964       Movie_GetSoundChannels     004db964 f   Winplay:winplay.dll
int Movie_GetSoundChannels(DWORD* pMovie, int SoundRate);
//0001:000da96a       Movie_GetSoundRate         004db96a f   Winplay:winplay.dll
int Movie_GetSoundRate(DWORD* pMovie, int SoundPrecision);;
//0001:000da970       Movie_GetSoundPrecision    004db970 f   Winplay:winplay.dll
int Movie_GetSoundPrecision(DWORD* pMovie, int unk1);
//0001:000da976       Player_InitPlaybackMode    004db976 f   Winplay:winplay.dll
int Player_InitPlaybackMode(HWND hWnd, DWORD* pVideo, int unk1/*1*/, int unk2/*0*/);
//0001:000da97c       Player_InitVideo           004db97c f   Winplay:winplay.dll
int Player_InitVideo(DWORD** pVideo, DWORD* pMovie, int XSize, int YSize, int unk1/*0*/, int unk2/*0*/, int unk3/*0*/, int unk4/*0*/, int XSize2/*640*/, int YSize2/*480*/, int unk5/*0*/, int unk6/*1*/, int unk7/*136*/)
{
	return 0;
}

//0001:000da982       Movie_GetXSize             004db982 f   Winplay:winplay.dll
int Movie_GetXSize(DWORD *pMovie);
//0001:000da988       Movie_GetYSize             004db988 f   Winplay:winplay.dll
int Movie_GetYSize(DWORD * pMovie);
//0001:000da98e       Player_InitMovie           004db98e f   Winplay:winplay.dll
int Player_InitMovie(DWORD** pMovie, int unk1, int unk2, const char* pFileName, HANDLE handle);
