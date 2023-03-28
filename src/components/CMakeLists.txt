cmake_minimum_required(VERSION 3.0.0)

project(
    components
    VERSION 0.1.0
    DESCRIPTION "Various game engine compponents."
)

add_library(
    ${PROJECT_NAME}
    src/Door.cpp
    src/HotSpot.cpp
    src/Item.cpp
    src/Room.cpp
    src/Inventory.cpp
    src/Sprite.cpp
    src/Visitor.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
