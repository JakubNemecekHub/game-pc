cmake_minimum_required(VERSION 3.0.0)

project(
    managers
    VERSION 0.1.0
    DESCRIPTION "Various game engine managers."
)


add_library(
    ${PROJECT_NAME}
    src/LogManager.cpp
    src/RenderManager.cpp
    src/TextManager.cpp
    src/WindowManager.cpp
    src/ItemManager.cpp
    src/RoomManager.cpp
    src/AssetManager.cpp
    src/PlayerManager.cpp
    src/ControlManager.cpp
    src/StateManager.cpp
    src/SerializationManager.cpp
    src/ScriptManager.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
