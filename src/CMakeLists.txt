add_library(${CMAKE_PROJECT_NAME} INTERFACE)

set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME}
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

target_sources(${CMAKE_PROJECT_NAME}
    INTERFACE
        ${HEADER_PATH}/${CMAKE_PROJECT_NAME}.h
)