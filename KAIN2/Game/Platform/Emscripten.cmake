if(EMSCRIPTEN)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -sALLOW_MEMORY_GROWTH -sPTHREAD_POOL_SIZE_STRICT=0 -sASYNCIFY --std=c++11 -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_PTHREADS=1 -s WASM=1 -s USE_SDL=2")

set(SDL2_LIBRARIES "-s USE_SDL=2")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O3 -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3 -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")

link_directories(${ROOT_DIR}/Build_EMSDK/libValkyrie/)

set(PSX_LIB ${ROOT_DIR}/Build/Build_EMSDK/External/libValkyrie/libValkyrie.a)

set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()