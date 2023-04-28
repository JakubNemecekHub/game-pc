cmake_minimum_required(VERSION 3.0.0)

project(
    Game
    VERSION 0.1.0
    DESCRIPTION "Very simple point and click adventure game written in c++ using SDL2."
)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(/wd4251 /wd4275) # Ignore warnings raised by yaml-cpp

# External lbraries such as SDL2
INCLUDE_DIRECTORIES(D:/Prog/cpp/vcpkg/installed/x64-windows/include/)
LINK_DIRECTORIES(D:/Prog/cpp/vcpkg/installed/x64-windows/lib/)

# My internal libraries
add_subdirectory(src/components)
add_subdirectory(src/managers)
add_subdirectory(src/math)
add_subdirectory(src/logic)
add_subdirectory(src/gui)

add_executable(${PROJECT_NAME} main.cpp Game.cpp)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  SDL2::SDL2 SDL2::SDL2main
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
  yaml-cpp
  math components managers logic gui
)
