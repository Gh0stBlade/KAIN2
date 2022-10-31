cd ../../../
cmake . -B"%cd%/Build/Build_NX_Arm/" -DCMAKE_BUILD_TYPE=NX_Release -DPLATFORM_NX_ARM=1 -G "Visual Studio 14 2015" -A Win32
cd Build/Build_NX_Arm