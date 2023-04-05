if((APPLE))

#Find libs
find_package(OpenGL REQUIRED)

set(PSX_LIB libValkyrie.a)

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build/Build_Mac/External/libValkyrie/$(Configuration))

include_directories(${EXTERN_LIB_PATH}/SDL/include)

endif()