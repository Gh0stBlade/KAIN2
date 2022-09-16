if(EMSCRIPTEN)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(USE_FLAGS "-s USE_SDL=2 --std=c++11 -sASYNCIFY -s USE_WEBGL2=1 -s FETCH_DEBUG -s FULL_ES3=1 -s WASM=1 -s GLOBAL_BASE=1024 -s ALLOW_MEMORY_GROWTH=1 -s USE_PTHREADS=1 -lopenal")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS} -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS} -Wno-error=format-security -Wno-narrowing -Wno-nonportable-include-path -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]'")

endif()