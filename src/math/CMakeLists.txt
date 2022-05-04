cmake_minimum_required(VERSION 3.0.0)

project(
    math
    VERSION 0.1.0
    DESCRIPTION "Math anstract objects."
)

add_library(
    ${PROJECT_NAME}
    src/Polygon.cpp
    src/Vector2D.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
