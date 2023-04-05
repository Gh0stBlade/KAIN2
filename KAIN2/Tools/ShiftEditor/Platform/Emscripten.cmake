if(EMSCRIPTEN)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -sASYNCIFY --std=c++11 -s FETCH_DEBUG -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_PTHREADS=1 -s WASM=1 -s GLOBAL_BASE=0 -s ALLOW_MEMORY_GROWTH=1 -lopenal -s USE_SDL=2 -s TOTAL_MEMORY=512MB")

set(SDL2_LIBRARIES "-s USE_SDL=2")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -s TOTAL_MEMORY=1024MB -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O2 -s TOTAL_MEMORY=1024MB -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -s TOTAL_MEMORY=1024MB -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s TOTAL_MEMORY=1024MB -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2 -s TOTAL_MEMORY=1024MB -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s TOTAL_MEMORY=1024MB -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path")

link_directories(${ROOT_DIR}/Build_EMSDK/External/libValkyrie/)

set(PSX_LIB ${ROOT_DIR}/Build_EMSDK/External/libValkyrie/libValkyrie.a)

set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()