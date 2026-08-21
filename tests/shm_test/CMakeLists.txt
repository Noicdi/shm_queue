add_executable(shm_test
        shm_test.cpp
        "${PROJECT_SOURCE_DIR}/src/shm.cpp"
)

target_include_directories(shm_test
        PRIVATE "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(shm_test
        PRIVATE
        GTest::gtest_main
)

set_target_properties(shm_test PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)


include(GoogleTest)
gtest_discover_tests(shm_test)
