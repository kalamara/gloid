cmake_minimum_required(VERSION 2.8)

project(gloid)

find_library(SDL sdl)
find_library(SDL_TTF SDL_ttf)
find_library(GL GL)
find_library(GLU GLU)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    "Engine.h"
    "Engine.cpp"
    "Game.h"
    "Game.cpp"
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
