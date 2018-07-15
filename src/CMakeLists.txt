cmake_minimum_required(VERSION 2.8)

project(gloid)

find_library(SDL sdl)
find_library(SDL_TTF SDL_ttf)
find_library(GL GL)
find_library(GLU GLU)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    "Step.h"
    "game/Loading.h"
    "game/Loading.cpp"
    "game/Play.h"
    "game/Play.cpp"
    "Game.h"
    "Game.cpp"
    "Engine.h"
    "Engine.cpp"
    "Text.h"
    "World.h"
    "Point.h"
    "GLoid.h"
    "GLoid.cpp")

target_link_libraries(gloid
    ${SDL_TTF}
    ${SDL}
    ${GL}
    ${GLU})

file(COPY "${CMAKE_SOURCE_DIR}/DejaVuSans.ttf"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY "${CMAKE_SOURCE_DIR}/sounds"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
