include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_CXX_COMPILER "C:/Program Files (x86)/SCE/PSP2 SDKs/0.996/host_tools/build/bin/psp2snc.exe" CACHE FILEPATH "PSP2 C++ compiler")
set(CMAKE_C_COMPILER "C:/Program Files (x86)/SCE/PSP2 SDKs/0.996/host_tools/build/bin/psp2snc.exe" CACHE FILEPATH "PSP2 C compiler")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(PLATFORM_PSP2 1)