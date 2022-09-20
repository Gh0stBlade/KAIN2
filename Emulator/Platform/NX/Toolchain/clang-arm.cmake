set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(GCC_ARM_TOOLCHAIN $ENV{NINTENDO_SDK_ROOT}/Compilers/NX/bin)

LIST(APPEND CMAKE_PROGRAM_PATH ${GCC_ARM_TOOLCHAIN})

# Specify the cross compiler
# The target triple needs to match the prefix of the binutils exactly
# (e.g. CMake looks for arm-none-eabi-ar)
set(CLANG_TARGET_TRIPLE )
set(GCC_ARM_TOOLCHAIN-_PREFIX ${CLANG_CLANG_TARGET_TRIPLE})
set(CMAKE_C_COMPILER nx-clang)
set(CMAKE_C_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})
set(CMAKE_CXX_COMPILER nx-clang++)
set(CMAKE_CXX_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})
set(CMAKE_ASM_COMPILER nx-clang)
set(CMAKE_ASM_COMPILER_TARGET ${CLANG_TARGET_TRIPLE})

# Don't run the linker on compiler check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Specify compiler flags
set(ARCH_FLAGS "--target=armv7l-nintendo-nx-eabihf -mabi=aapcs-linux -mcpu=cortex-a57 -mfpu=crypto-neon-fp-armv8 -mfloat-abi=hard")
set(CMAKE_C_FLAGS "-fno-common -fno-short-enums -ffunction-sections -fdata-sections -fPIC ${ARCH_FLAGS}" CACHE STRING "Common flags for C compiler")
set(CMAKE_CXX_FLAGS "-x c++ -std=gnu++14 -fno-common -fno-short-enums -ffunction-sections -fdata-sections -fPIC ${ARCH_FLAGS}" CACHE STRING "Common flags for C++ compiler")
set(CMAKE_ASM_FLAGS "-fno-common -fno-short-enums -ffunction-sections -fdata-sections -fPIC ${ARCH_FLAGS} -x assembler-with-cpp" CACHE STRING "Common flags for assembler")

# C/C++ toolchain
set(GCC_ARM_SYSROOT "${GCC_ARM_TOOLCHAIN}/${GCC_ARM_TOOLCHAIN_PREFIX}")
# set(CMAKE_SYSROOT ${GCC_ARM_SYSROOT})
set(CMAKE_FIND_ROOT_PATH ${GCC_ARM_SYSROOT})

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_C_OUTPUT_EXTENSION ".o")
set(CMAKE_CXX_OUTPUT_EXTENSION ".o")
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)
set(CMAKE_ASM_OUTPUT_EXTENSION_REPLACE 1)
