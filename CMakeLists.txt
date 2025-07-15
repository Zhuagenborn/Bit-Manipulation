cmake_minimum_required(VERSION 3.20.0)

project(bit_manip LANGUAGES CXX)

set(LIB_NAME ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(src)

option(BIT_MANIP_BUILD_TESTS "Build unit tests for the bit manipulation library" OFF)
if(BIT_MANIP_BUILD_TESTS)
    find_package(GTest)
    if(GTest_FOUND)
        enable_testing()
        set(GTEST_LIB GTest::gtest GTest::gtest_main)
        set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
        add_subdirectory(tests)
    endif()
endif()