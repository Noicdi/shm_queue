cmake_minimum_required(VERSION 3.24)

project(ShmQueue
        DESCRIPTION "A message queue implemented based on shared memory ringbuffer"
        VERSION 0.0.1
)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(FATAL_ERROR "Please specify a build type using -DCMAKE_BUILD_TYPE=Debug/Release/RelWithDebInfo/MinSizeRel")
endif ()

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    message(STATUS "Enable ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif (CCACHE_PROGRAM)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(LTO)
include(Sanitizers)
include(SplitDebugSymbols)
include(Version)

include(GNUInstallDirs)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)

set(CPACK_GENERATOR "TGZ")
include(CPack)
