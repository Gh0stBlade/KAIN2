if(DEFINED PLATFORM_NX_ARM)

target_compile_definitions(${PROJECT_NAME} PRIVATE NN_NINTENDO_SDK)

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/include")

target_include_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Common/Configs/Targets/NX-NXFP2-a32/Include")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Externals/glew/lib/Release/${CMAKE_GENERATOR_PLATFORM}")

target_link_directories(${PROJECT_NAME} PRIVATE "$ENV{NINTENDO_SDK_ROOT}/Libraries/NX-NXFP2-a32/Release")

target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_DIR}/Build_NX_Arm/Emulator)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_USER_PROPS "$(NintendoSdkSamplePropertySheetDirectory)NintendoSdkSampleSpec_NX.props $(NintendoSdkSamplePropertySheetDirectory)NintendoSdkSampleBuildType_Debug.props $(NintendoSdkSamplePropertySheetDirectory)NintendoSdkSampleVcProjectSettings.props")

target_compile_definitions(${PROJECT_NAME} PRIVATE "PLATFORM_NX_ARM")

target_compile_definitions(${PROJECT_NAME} PRIVATE "NN_SDK_BUILD_RELEASE")

target_compile_definitions(${PROJECT_NAME} PRIVATE "NN_NINTENDO_SDK")

#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-nostartfiles -Wl,--gc-sections -Wl,--build-id=uuid -Wl,-init=_init,-fini=_fini -Wl,-pie -Wl,--export-dynamic,-z,combreloc,-z,relro,--enable-new-dtags -Wl,-u,malloc -Wl,-u,calloc -Wl,-u,realloc -Wl,-u,aligned_alloc -Wl,-u,free -Wl,-u,calloc -Wl,-T ")

set(PSX_LIB Valkyrie.a)

set(CMAKE_C_STANDARD_LIBRARIES "")
set(CMAKE_CXX_STANDARD_LIBRARIES "")

endif()