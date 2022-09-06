cd ../../../
cmake cmake . -B%cd%"/Build_PSP2/" -DPLATFORM_PSP2=1 -DCMAKE_TOOLCHAIN_FILE=%cd%/Emulator/Platform/PSP2/Toolchain/PSP2.cmake -G "Visual Studio 9 2008" -A Win32
cd Build_PSP2