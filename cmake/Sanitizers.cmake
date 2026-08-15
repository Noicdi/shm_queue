include_guard(GLOBAL)

include(CheckCXXCompilerFlag)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

if (ENABLE_ASAN AND ENABLE_TSAN)
    message(FATAL_ERROR "ENABLE_ASAN and ENABLE_TSAN cannot be enabled simultaneously.")
endif ()

#if (ENABLE_ASAN)
#    check_cxx_compiler_flag("-fsanitize=address" SANITIZER_ADDRESS_SUPPORTED)
#    check_cxx_compiler_flag("-fsanitize=undefined" SANITIZER_UNDEFINED_SUPPORTED)
#
#    if (NOT SANITIZER_ADDRESS_SUPPORTED)
#        message(FATAL_ERROR "Current compiler does not support AddressSanitizer.")
#    endif ()
#    if (NOT SANITIZER_UNDEFINED_SUPPORTED)
#        message(FATAL_ERROR "Current compiler does not support UndefinedSanitizer.")
#    endif ()
#endif ()
#
#if (ENABLE_TSAN)
#    check_cxx_compiler_flag("-fsanitize=thread" SANITIZER_TSAN_SUPPORTED)
#
#    if (NOT SANITIZER_TSAN_SUPPORTED)
#        message(FATAL_ERROR "Current compiler does not support ThreadSanitizer.")
#    endif ()
#endif ()

function(target_enable_sanitizers target)
    if (NOT TARGET ${target})
        message(FATAL_ERROR "target_enable_sanitizers: target '${target}' does not exist")
    endif ()

    #
    # 仅 Debug 生效
    #

    if (ENABLE_ASAN)
        target_compile_options(${target} PRIVATE
                $<$<CONFIG:Debug>:-fsanitize=address>
                $<$<CONFIG:Debug>:-fsanitize=undefined>
                $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )

        target_link_options(${target} PRIVATE
                $<$<CONFIG:Debug>:-fsanitize=address>
                $<$<CONFIG:Debug>:-fsanitize=undefined>
        )
    endif ()

    if (ENABLE_TSAN)
        target_compile_options(${target} PRIVATE
                $<$<CONFIG:Debug>:-fsanitize=thread>
                $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )

        target_link_options(${target} PRIVATE
                $<$<CONFIG:Debug>:-fsanitize=thread>
        )
    endif ()
endfunction()
