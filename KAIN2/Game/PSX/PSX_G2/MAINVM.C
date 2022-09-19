#include "MAINVM.H"
#include "PSX/MAIN.H"

#if defined(PSXPC_VERSION)
#include "CORE.H"
#if defined(SDL2) && defined(UWP_SDL2) || defined(_WIN32) && !defined(SN_TARGET_PSP2)  && !defined(PLATFORM_NX)  && !defined(PLATFORM_NX_ARM)
#undef R13
#undef R12
#undef R11
#include <SDL_main.h>
#endif
#endif

struct _G2AppDataVM_Type _appDataVM;

EMULATOR_THREAD_DEF

#if defined(PSXPC_VERSION) && !defined(UWP) && !defined(PLATFORM_NX) || defined(UWP_SDL2) && !defined(__EMSCRIPTEN__)
int main(int argc, char *argv[])
#elif defined(UWP)
int main(Platform::Array<Platform::String^>^ args)
#elif defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
extern "C" void nnMain()
#else
int main()
#endif
{ 
#if defined(PLATFORM_NX) || defined(PLATFORM_NX_ARM)
    MainG2(&_appDataVM);
#else
    return MainG2(&_appDataVM);
#endif
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