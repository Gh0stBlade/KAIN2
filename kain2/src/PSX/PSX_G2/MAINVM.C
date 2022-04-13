#include "MAINVM.H"
#include "PSX/MAIN.H"

struct _G2AppDataVM_Type _appDataVM;

int main()
{ 
	return MainG2(&_appDataVM);
}

#if defined(PC_VERSION)
int MainG2_UpdateLoop()
{
}
#endif