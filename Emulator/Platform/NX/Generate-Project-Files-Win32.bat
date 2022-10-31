cd ../../../
cmake . -B"%cd%/Build/Build_NX_Win/" -DCMAKE_BUILD_TYPE=Release -DPLATFORM_NX=1 -G "Visual Studio 17 2022" -A Win32
cd Build/Build_NX_Win