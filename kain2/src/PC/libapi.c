#include "../core.H"

void *input_buffer0, *input_buffer1;

//0001:000288e0       _CloseEvent                004298e0 f   libapi.obj
//0001:000288f0       _DeliverEvent              004298f0 f   libapi.obj
//0001:00028900       _DisableEvent              00429900 f   libapi.obj
//0001:00028910       _EnableEvent               00429910 f   libapi.obj
//0001:00028920       _EnterCriticalSection      00429920 f   libapi.obj
//0001:00028930       _ExitCriticalSection       00429930 f   libapi.obj
//0001:00028940       _GetRCnt                   00429940 f   libapi.obj
long GetRCnt(unsigned long t)
{
	return 0;
}
//0001:00028950       _CdControlF                00429950 f   libapi.obj
//0001:00028960       _CdGetSector               00429960 f   libapi.obj
//0001:00028970       _CdControl                 00429970 f   libapi.obj
//0001:00028980       _CdPosToInt                00429980 f   libapi.obj
//0001:00028990       _CdIntToPos                00429990 f   libapi.obj
//0001:000289a0       _CdSearchFile              004299a0 f   libapi.obj
//0001:000289b0       _CdDataCallback            004299b0 f   libapi.obj
//0001:000289c0       _CdSyncCallback            004299c0 f   libapi.obj
//0001:000289d0       _CdReadyCallback           004299d0 f   libapi.obj
//0001:000289e0       _CdSetDebug                004299e0 f   libapi.obj
//0001 : 000289f0       _ResetCallback             004299f0 f   libapi.obj
//0001:00028a00       _CdInit                    00429a00 f   libapi.obj
//0001:00028a10       _FlushCache                00429a10 f   libapi.obj
//0001:00028a20       _InitPAD                   00429a20 f   libapi.obj
long InitPAD(char* buf0, long size0, char* buf1, long size1)
{
	input_buffer0 = buf0;
	input_buffer1 = buf1;

	return 1;
}
//0001:00028a40       _OpenEvent                 00429a40 f   libapi.obj
//0001:00028a50       _ResetRCnt                 00429a50 f   libapi.obj
//0001:00028a60       _SetRCnt                   00429a60 f   libapi.obj
//0001:00028a70       _StartPAD                  00429a70 f   libapi.obj
//0001:00028a80       _StartRCnt                 00429a80 f   libapi.obj
//0001:00028a90       _StopPAD                   00429a90 f   libapi.obj
//0001:00028aa0       _StopRCnt                  00429aa0 f   libapi.obj
//0001:00028ab0       _TestEvent                 00429ab0 f   libapi.obj
//0001:00028ac0       _UnDeliverEvent            00429ac0 f   libapi.obj
//0001:00028ad0       _WaitEvent                 00429ad0 f   libapi.obj