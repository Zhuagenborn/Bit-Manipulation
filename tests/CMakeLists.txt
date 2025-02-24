set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

add_executable(${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE
        ${TEST_NAME}.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        ${CMAKE_PROJECT_NAME}
        ${GTEST_LIBS}
)

gtest_discover_tests(${TEST_NAME})