cmake_minimum_required(VERSION 3.0.0)

project(
    Game
    VERSION 0.1.0
    DESCRIPTION "Very simple point and click adventure game written in c++ using SDL2."
)

# External lbraries such as SDL2
INCLUDE_DIRECTORIES(D:/Prog/cpp/vcpkg/installed/x64-windows/include/)
LINK_DIRECTORIES(D:/Prog/cpp/vcpkg/installed/x64-windows/lib/)

# My internal libraries
add_subdirectory(src/components)
add_subdirectory(src/managers)
add_subdirectory(src/math)
add_subdirectory(src/math_objects)

# Don't know what this is
# include(CTest)
# enable_testing()

add_executable(${PROJECT_NAME} main.cpp)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  SDL2::SDL2 SDL2::SDL2main
  SDL2::SDL2_image SDL2::SDL2_ttf
  nlohmann_json::nlohmann_json
  components managers math math_objects
)

# Don't know what this is
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

add_compile_options( -g -Wall -std=c++17)
