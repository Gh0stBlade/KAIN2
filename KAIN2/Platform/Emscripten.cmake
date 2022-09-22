if(EMSCRIPTEN)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -sASYNCIFY -g3 --std=c++11 -s FETCH_DEBUG -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_PTHREADS=1 -s WASM=1 -s GLOBAL_BASE=1024 -s ALLOW_MEMORY_GROWTH=1 -lopenal -s USE_SDL=2 -s TOTAL_MEMORY=1024MB -sPTHREAD_POOL_SIZE=32")
set(SDL2_LIBRARIES "-s USE_SDL=2")

#Emscripten bug, this is now moved to commandline invocation
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s EXPORTED_FUNCTIONS='["GAMELOOP_RequestLevelChange",\"Emulator_SetFocus\"]'")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]'")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")

link_directories(${ROOT_DIR}/Build_EMSDK/Emulator/)

set(PSX_LIB ${ROOT_DIR}/Build_EMSDK/Emulator/libValkyrie.a)

set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()