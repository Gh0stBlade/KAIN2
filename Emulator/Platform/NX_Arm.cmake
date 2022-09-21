if(DEFINED PLATFORM_NX_ARM)

target_compile_definitions(${PROJECT_NAME} PRIVATE NN_NINTENDO_SDK)

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/include")

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Common/Configs/Targets/NX-NXFP2-a32/Include")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Externals/glew/lib/Release/${CMAKE_GENERATOR_PLATFORM}")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Libraries/NX-NXFP2-a32/$<CONFIG>")

target_compile_definitions(${PROJECT_NAME} PRIVATE "NN_SDK_BUILD_RELEASE")

target_compile_definitions(${PROJECT_NAME} PRIVATE "PLATFORM_NX_ARM")

target_compile_definitions(${PROJECT_NAME} PRIVATE "NN_NINTENDO_SDK")

set(NX_PROPS ${CMAKE_CURRENT_SOURCE_DIR}/Platform/NX/Props/NX32.user.props CACHE INTERNAL "")

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_USER_PROPS "${NX_PROPS}")

endif()