cd ../../../
cmake . -B"%cd%/Build_NX_Arm/" -DCMAKE_BUILD_TYPE=Release -DPLATFORM_NX_ARM=1 -G "Visual Studio 17 2022" -A Win32
cd Build_NX_Arm