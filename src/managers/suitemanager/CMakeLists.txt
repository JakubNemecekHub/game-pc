cmake_minimum_required(VERSION 3.0.0)

project(
    suitemanager
    VERSION 0.1.0
    DESCRIPTION "Manager that handles Rooms. Room has its background texture and ambient animations. Also a collection of game object
    the player can interact with. Suite manager mediates this interaction"
)

add_library(
    ${PROJECT_NAME}
    src/RoomManager.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
