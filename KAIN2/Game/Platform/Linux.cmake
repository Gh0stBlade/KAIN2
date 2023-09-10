if(UNIX AND NOT(ANDROID) AND NOT(EMSCRIPTEN) AND NOT (PLATFORM_NX_ARM) AND NOT (APPLE))

#Find libs
find_package(OpenGL REQUIRED)

set(PSX_LIB libValkyrie.a)

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build/Build_Linux/External/libValkyrie/)

include_directories(${EXTERN_LIB_PATH}/SDL/include)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -fpermissive -Wno-narrowing -DDEBUG -D_DEBUG")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -fpermissive -Wno-narrowing -DDEBUG -D_DEBUG")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -fpermissive -Wno-narrowing -DDEBUG -D_DEBUG")
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fpermissive -Wno-narrowing -DDEBUG -D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -fpermissive -Wno-narrowing -DDEBUG -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fpermissive -Wno-narrowing -DDEBUG -D_DEBUG")

endif()
