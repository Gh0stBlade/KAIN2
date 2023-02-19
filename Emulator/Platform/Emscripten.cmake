if(EMSCRIPTEN)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

set(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -g3 -s TOTAL_MEMORY=1024MB -s USE_SDL=2 --std=c++11 -sASYNCIFY -s USE_WEBGL2=1 -s FETCH_DEBUG -s FULL_ES3=1 -s WASM=1 -s GLOBAL_BASE=0 -s ALLOW_MEMORY_GROWTH=1 -s USE_PTHREADS=1 -lopenal")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O2 -s USE_SDL=2 -s TOTAL_MEMORY=1024MB -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -O2 -s USE_SDL=2 -s TOTAL_MEMORY=1024MB -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path -pthread")

endif()