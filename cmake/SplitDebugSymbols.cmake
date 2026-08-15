cmake_minimum_required(VERSION 3.21)

option(ENABLE_SPLIT_DEBUG_SYMBOLS "Split debug symbols for RelWithDebInfo builds" ON)

function(target_enable_split_debug_symbols target_name)
    if (NOT TARGET ${target_name})
        message(FATAL_ERROR "target_enable_split_debug_symbols: target '${target_name}' does not exist")
    endif ()

    if (NOT ENABLE_SPLIT_DEBUG_SYMBOLS)
        message(STATUS "SplitDebugSymbols: disabled by option for target '${target_name}'")
        return()
    endif ()

    if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
        return()
    endif ()

    if (NOT CMAKE_OBJCOPY)
        message(FATAL_ERROR "SplitDebugSymbols: CMAKE_OBJCOPY is not set")
    endif ()
    if (NOT CMAKE_STRIP)
        message(FATAL_ERROR "SplitDebugSymbols: CMAKE_STRIP is not set")
    endif ()

    set(debug_file "$<TARGET_FILE:${target_name}>.debug")

    add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND "${CMAKE_OBJCOPY}" --only-keep-debug
            "$<TARGET_FILE:${target_name}>" "${debug_file}"
            COMMAND "${CMAKE_STRIP}" --strip-unneeded
            "$<TARGET_FILE:${target_name}>"
            COMMAND "${CMAKE_OBJCOPY}" "--add-gnu-debuglink=${debug_file}"
            "$<TARGET_FILE:${target_name}>"
            COMMENT "SplitDebugSymbols: splitting debug symbols for target '${target_name}'"
            VERBATIM
    )

    message(STATUS "SplitDebugSymbols: enabled for target '${target_name}' in ${CMAKE_BUILD_TYPE}")
endfunction()
