cd ../../../External/emsdk
call emsdk_env.bat
cd ../../
emcmake cmake -DCMAKE_SHARED_LINKER_FLAGS="-sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE=Emulator_SetFocus -sEXPORTED_FUNCTIONS=Emulator_SetFocus"  -DCMAKE_EXE_LINKER_FLAGS="-sEXPORTED_RUNTIME_METHODS=ccall,cwrap -sEXPORTED_FUNCTIONS=_GAMELOOP_RequestLevelChange" . -B%cd%"/Build_EMSDK/"
cd Build_EMSDK
cmake --build .