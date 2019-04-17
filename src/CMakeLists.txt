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
    "Hud.h"
    "Hud.cpp"
    "World.h"
    "Game.h"
    "Game.cpp"
    "Engine.h"
    "Engine.cpp"
    "Text.h"
    "World.h"
    "Point.h"
    "Point.cpp"
    "GLoid.h"
    "game/Intro.h"
    "game/Intro.cpp"
    "game/Loading.h"
    "game/Loading.cpp"
    "game/Play.h"
    "game/Play.cpp"
    "game/Waiting.h"
    "game/Waiting.cpp"
    "game/Win.h"
    "game/Win.cpp"
    "model/WhatUC.h"
    "model/Particle.h"
    "model/Particle.cpp"
    "model/Ball.h"
    "model/Ball.cpp"
    "model/Vaus.h"
    "model/Vaus.cpp"
    "model/Brick.h"
    "model/Brick.cpp"
    "model/Pill.h"
    "model/Pill.cpp"
    "model/Alien.h"
    "model/Alien.cpp"
    "model/Shot.h"
    "model/Shot.cpp"
    "model/Crosshair.h"
    "model/Crosshair.cpp"
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

file(COPY "${CMAKE_SOURCE_DIR}/textures"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(COPY "${CMAKE_SOURCE_DIR}/levels"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(COPY "${CMAKE_SOURCE_DIR}/hallofame.txt"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
