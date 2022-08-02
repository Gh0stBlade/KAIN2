#include "MAINVM.H"
#include "PSX/MAIN.H"

#if defined(PSXPC_VERSION)
#include "CORE.H"
#if defined(SDL2)
#undef R13
#undef R12
#undef R11
#include <SDL_main.h>
#endif
#endif

struct _G2AppDataVM_Type _appDataVM;

#if defined(PSXPC_VERSION) && !defined(UWP) || defined(UWP_SDL2) && !defined(__EMSCRIPTEN__)
int main(int argc, char *argv[])
#elif defined(UWP)
int main(Platform::Array<Platform::String^>^ args)
#else
int main()
#endif
{ 
	return MainG2(&_appDataVM);
}

#if defined(UWP_SDL2)
#include <wrl.h>

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    if (FAILED(Windows::Foundation::Initialize(RO_INIT_MULTITHREADED)))
    {
        return 1;
    }

    SDL_WinRTRunApp(SDL_main, NULL);
    return 0;
}
#endif

#if defined(PC_VERSION)
int MainG2_UpdateLoop()
{
}
#endif