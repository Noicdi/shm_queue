add_library(public_headers_c90 OBJECT
        spsc_queue_c90.c
        spmc_queue_c90.c
        mpmc_queue_c90.c
)

target_include_directories(public_headers_c90
        PRIVATE "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(public_headers_c90 PROPERTIES
        C_STANDARD 90
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)

add_library(public_headers_cxx11 OBJECT
        spsc_queue_cxx11.cpp
        spmc_queue_cxx11.cpp
        mpmc_queue_cxx11.cpp
)

target_include_directories(public_headers_cxx11
        PRIVATE "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(public_headers_cxx11 PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

add_test(
        NAME public_headers_compile
        COMMAND "${CMAKE_COMMAND}"
        --build "${CMAKE_BINARY_DIR}"
        --config "$<CONFIG>"
        --target public_headers_c90 public_headers_cxx11
)

set_tests_properties(public_headers_compile PROPERTIES
        LABELS "public_headers"
        RUN_SERIAL TRUE
)
