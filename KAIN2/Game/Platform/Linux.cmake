if(UNIX AND NOT(ANDROID) AND NOT(EMSCRIPTEN) AND NOT (PLATFORM_NX_ARM))

#Find libs
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})
#We want to link GLEW statically
set(GLEW_USE_STATIC_LIBS ON)
find_package(GLEW REQUIRED)

#We want to link GLEW statically
set(GLEW_USE_STATIC_LIBS ON)

#Setup project include directories
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})

set(PSX_LIB libValkyrie.a)

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build_Linux/Emulator/$(Configuration))

set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -fpermissive -Wno-narrowing")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -fpermissive -Wno-narrowing")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -fpermissive -Wno-narrowing1")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fpermissive -Wno-narrowing")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -fpermissive -Wno-narrowing")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fpermissive -Wno-narrowing")

endif()
