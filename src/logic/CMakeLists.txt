cmake_minimum_required(VERSION 3.5.0)

project(
    logic
    VERSION 0.1.0
    DESCRIPTION "Scenes that define game logic."
)


add_library(
    ${PROJECT_NAME}
    src/State.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
