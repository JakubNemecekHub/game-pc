cmake_minimum_required(VERSION 3.0.0)

project(
    managers
    VERSION 0.1.0
    DESCRIPTION "Various game engine managers."
)


add_library(
    ${PROJECT_NAME}
    src/ControlManager.cpp
    src/LogManager.cpp
    src/RenderManager.cpp
    src/TextManager.cpp
    src/WindowManager.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
