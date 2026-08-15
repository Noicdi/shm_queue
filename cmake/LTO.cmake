cmake_minimum_required(VERSION 3.24)

include(CheckCXXCompilerFlag)

option(ENABLE_LTO "Enable LTO for release builds" ON)

check_cxx_compiler_flag("-flto" RELEASE_LTO_SUPPORTED)

function(target_enable_lto target_name)
    if (NOT TARGET ${target_name})
        message(FATAL_ERROR "target_enable_lto: target '${target_name}' does not exist")
    endif ()

    if (NOT ENABLE_LTO)
        message(STATUS "ReleaseLTO: disabled by option for target '${target_name}'")
        return()
    endif ()

    if (NOT RELEASE_LTO_SUPPORTED)
        message(STATUS "ReleaseLTO: compiler does not support -flto, skip target '${target_name}'")
        return()
    endif ()

    target_compile_options(${target_name} PRIVATE
            $<$<CONFIG:Release>:-flto>
            $<$<CONFIG:RelWithDebInfo>:-flto>
    )

    target_link_options(${target_name} PRIVATE
            $<$<CONFIG:Release>:-flto>
            $<$<CONFIG:RelWithDebInfo>:-flto>
    )

    if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        message(STATUS "ReleaseLTO: enabled for target '${target_name}' in ${CMAKE_BUILD_TYPE}")
    endif ()
endfunction()
