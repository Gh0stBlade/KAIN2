if(ANDROID)

Message("Compiling for Android!")

include_directories(${ROOT_DIR}/EXTERNAL/SDL/include)

set(SDL2MAIN_LIBRARY SDL2main)
set(SDL2_LIBRARY SDL2)
set(GLES3_LIBRARY GLESv3)
set(EGL_LIBRARY EGL)

target_link_libraries(${PROJECT_NAME}
  ${SDL2MAIN_LIBRARY}
  ${SDL2_LIBRARY}
  ${GLES3_LIBRARY}
  ${EGL_LIBRARY}
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions -Wno-c++11-narrowing -fno-exceptions -Wnonportable-include-path")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-exceptions -Wno-c++11-narrowing -fno-exceptions -Wnonportable-include-path")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-exceptions -Wno-c++11-narrowing -fno-exceptions -Wnonportable-include-path")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -Wno-c++11-narrowing -fno-exceptions -Wnonportable-include-path")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions -Wno-c++11-narrowing -fno-exceptions -Wnonportable-include-path")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions -Wno-c++11-narrowing -fno-exceptions -Wnonportable-include-path")


endif()