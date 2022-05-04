cmake_minimum_required(VERSION 3.0.0)

project(
    math_objects
    VERSION 0.1.0
    DESCRIPTION "Presentable math objects."
)

add_library(
    ${PROJECT_NAME}
    src/PolygonObject.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
