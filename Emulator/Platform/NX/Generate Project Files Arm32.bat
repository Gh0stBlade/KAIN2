cd ../../../

cmake . -B"%cd%/Build_NX_Arm/" -DCMAKE_VERBOSE_MAKEFILE=1 -DCMAKE_TOOLCHAIN_FILE=Emulator/Platform/NX/Toolchain/clang-arm.cmake -DCMAKE_BUILD_TYPE=Release -DPLATFORM_NX_ARM=1 -G "MinGW Makefiles"
cd Build_NX_Arm