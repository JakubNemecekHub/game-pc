cmake_minimum_required(VERSION 3.0.0)

project(
    components
    VERSION 0.1.0
    DESCRIPTION "Various game engine compponents."
)

add_library(
    ${PROJECT_NAME}
    src/Animation.cpp
    src/RenderableObject.cpp
    src/Texture.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
