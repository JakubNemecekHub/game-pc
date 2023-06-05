cmake_minimum_required(VERSION 3.0.0)

project(
    logic
    VERSION 0.1.0
    DESCRIPTION "Scenes that define game logic."
)


add_library(
    ${PROJECT_NAME}
    src/IntroState.cpp
    src/MenuState.cpp
    src/ExitState.cpp
    src/Gameplay.NormalState.cpp
    src/Gameplay.InventoryState.cpp
    editor/src/EditorState.cpp
    editor/src/EditorState.Item.cpp
    editor/src/EditorState.HotSpot.cpp
    editor/src/EditorState.WalkArea.cpp
    editor/src/EditorState.Bitmap.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
