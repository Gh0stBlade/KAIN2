cd ../../../
cmake . -B"%cd%/Build_PSP2/" -DCMAKE_BUILD_TYPE=Release -DPLATFORM_PSP2=1 -G "Visual Studio 10 2010" -A PSVita -T SNC
cd Build_PSP2