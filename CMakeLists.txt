cmake_minimum_required(VERSION 3.17)
project(make-alternative-versions VERSION 0.0.0.4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# specify compilers and tools
set(CMAKE_C_COMPILER gcc-10)
set(CMAKE_CXX_COMPILER g++-10)
set(CMAKE_ASM_COMPILER gcc-10)
set(CMAKE_AR gcc-ar-10)
set(CMAKE_OBJCOPY gcc-objcopy-10)
set(CMAKE_OBJDUMP gcc-objdump-10)
set(SIZE gcc-size-10)

# set directories
set(BASE_DIR ${PROJECT_SOURCE_DIR})


include_directories(
        src
        third-party
)
add_executable(make-alternative-versions
        src/main.cpp

        cmakeConfig.hpp src/cmakeConfig.hpp.in src/myExceptions.hpp)


# Compiler/Linker Settings
if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    message(" ")
    message("CMAKE IN DEBUG MODE")
    message(" ")
    set(DEBUG 1)
    set(RELEASE 0)
elseif (${CMAKE_BUILD_TYPE} MATCHES Release)
    message(" ")
    message("CMAKE IN RELEASE MODE")
    message(" ")
    set(DEBUG 0)
    set(RELEASE 1)
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
set(CMAKE_C_FLAGS_DEBUG "-g -Og")

set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -flto -Wl,-flto")
set(CMAKE_C_FLAGS_RELEASE "-g -O3 -flto -Wl,-flto")


# Pass info to cmakeConfig.hpp
configure_file(${BASE_DIR}/src/cmakeConfig.hpp.in ${PROJECT_BINARY_DIR}/cmakeConfig.hpp)
target_include_directories(make-alternative-versions PUBLIC "${PROJECT_BINARY_DIR}")
