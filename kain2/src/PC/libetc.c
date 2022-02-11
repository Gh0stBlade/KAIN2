//0001:00028ae0       _StopCallback              00429ae0 f   libetc.obj
int StopCallback(void) {}
//0001:00028af0       _VSync                     00429af0 f   libetc.obj
int VSync(int mode) { return 0; }
//0001:00028b00       _VSyncCallback             00429b00 f   libetc.obj

void (*cb_vsync)(void);

int VSyncCallback(void (*f)(void))
{
	cb_vsync = f;
	return 1;
}