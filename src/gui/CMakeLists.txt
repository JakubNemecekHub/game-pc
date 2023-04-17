cmake_minimum_required(VERSION 3.0.0)

project(
    gui
    VERSION 0.1.0
    DESCRIPTION "Elements of the Graphical User Interface."
)


add_library(
    ${PROJECT_NAME}
    src/Button.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
