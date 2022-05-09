cd ../../../External/emsdk
call emsdk_env.bat
cd ../../
emcmake cmake . -B%cd%"/Build_EMSDK/"
cd Build_EMSDK
cmake --build .