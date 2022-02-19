#include "MAINVM.H"
#include "PSX/MAIN.H"

static struct _G2AppDataVM_Type _appDataVM;

int main()
{ 
	return MainG2(&_appDataVM);
}

#if defined(PC_VERSION)
int MainG2_UpdateLoop()
{
    MSG Msg;
	InputG2_Update();

    if (!PeekMessageA(&Msg, 0, 0, 0, 0))
        return 1;
    while (1)
    {
        auto MessageA = GetMessageW(&Msg, 0, 0, 0);
        TranslateMessage(&Msg);
        DispatchMessageW(&Msg);
        if (!MessageA)
            break;
        if (!dword_C65188)
            WaitMessage();
        if (!PeekMessageW(&Msg, 0, 0, 0, 0))
            return 1;
    }
    return 0;
}
#endif