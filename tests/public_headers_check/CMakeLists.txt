add_library(public_headers_c90 OBJECT
        ${CMAKE_CURRENT_LIST_DIR}/spsc_queue_c90.c
        ${CMAKE_CURRENT_LIST_DIR}/spmc_queue_c90.c
        ${CMAKE_CURRENT_LIST_DIR}/mpmc_queue_c90.c
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
        ${CMAKE_CURRENT_LIST_DIR}/spsc_queue_cxx11.cpp
        ${CMAKE_CURRENT_LIST_DIR}/spmc_queue_cxx11.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mpmc_queue_cxx11.cpp
)

target_include_directories(public_headers_cxx11
        PRIVATE "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(public_headers_cxx11 PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)
