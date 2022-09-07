cd ../../../
cmake . -B"%cd%/Build_PSP2/" -DCMAKE_BUILD_TYPE=Release -DPLATFORM_PSP2=1 -G "Visual Studio 9 2008" -A Win32
cd Build_PSP2