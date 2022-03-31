#include "MAINVM.H"
#include "../../PSX/MAIN.H"

struct _G2AppDataVM_Type _appDataVM;

#if defined(PSXPC_VERSION)
int main(int argc, char* argv[])
#else
int main()
#endif
{ 
	return MainG2(&_appDataVM);
}

#if defined(PC_VERSION)
int MainG2_UpdateLoop()
{
}
#endif