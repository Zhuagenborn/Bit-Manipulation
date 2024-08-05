set(TEST_NAME ${CMAKE_PROJECT_NAME}-test)

add_executable(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE
        bit_manip_test.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        bit-manip
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        ${GTEST_LIBS}
        ${GMOCK_LIBS}
)

gtest_discover_tests(${TEST_NAME})