cmake_minimum_required(VERSION 2.8)

project(gloid_test)


find_library(CPPUTEST CppUTest)
find_library(CPPUTEST_EXT CppUTestExt)
set(SUT ../src/)
include_directories(
    ${SUT}
    .)

add_executable(model_test
    ${SUT}/GLoid.h
    ${SUT}/Text.h
    ${SUT}/Point.h
    ${SUT}/Point.cpp
    ${SUT}/Engine.h
    ${SUT}/Game.h
    ${SUT}/World.h
    ${SUT}/model/WhatUC.h
    ${SUT}/model/Particle.h
    ${SUT}/model/Particle.cpp
    ${SUT}/model/Ball.h
    ${SUT}/model/Ball.cpp
    ${SUT}/model/Vaus.h
    ${SUT}/model/Vaus.cpp
    ${SUT}/model/Brick.h
    ${SUT}/model/Brick.cpp
    ${SUT}/model/Pill.h
    ${SUT}/model/Pill.cpp
    ${SUT}/model/Alien.h
    ${SUT}/model/Alien.cpp
    ${SUT}/model/Shot.h
    ${SUT}/model/Shot.cpp
    ${SUT}/model/Crosshair.h
    ${SUT}/model/Crosshair.cpp
    mock_sdl_opengl.h
    mock_sdl_opengl.cpp
    model_test.cpp)

add_executable(functions_test
    ${SUT}/GLoid.h
    ${SUT}/Text.h
    ${SUT}/Point.h
    ${SUT}/Point.cpp
    functions_test.cpp)

add_executable(game_test
    ${SUT}/GLoid.h
    ${SUT}/Point.h
    ${SUT}/Point.cpp
    ${SUT}/model/WhatUC.h
    ${SUT}/model/Particle.h
    ${SUT}/model/Ball.h
    ${SUT}/model/Vaus.h
    ${SUT}/model/Brick.h
    ${SUT}/model/Pill.h
    ${SUT}/model/Alien.h
    ${SUT}/model/Shot.h
    ${SUT}/Text.h
    ${SUT}/World.h
    ${SUT}/Game.h
    ${SUT}/Game.cpp
    ${SUT}/Engine.h
    ${SUT}/Engine.cpp
    ${SUT}/Step.h
    ${SUT}/Hud.h
    ${SUT}/Hud.cpp
    ${SUT}/game/Loading.h
    ${SUT}/game/Loading.cpp
    ${SUT}/game/Play.h
    ${SUT}/game/Play.cpp
    ${SUT}/game/Waiting.h
    ${SUT}/game/Waiting.cpp
    ${SUT}/game/Intro.h
    ${SUT}/game/Intro.cpp
    ${SUT}/World.h
    mock_sdl_opengl.h
    mock_sdl_opengl.cpp
    engine_test.cpp
    game_test.cpp)

target_link_libraries(model_test
    ${CPPUTEST}
    ${CPPUTEST_EXT})

target_link_libraries(functions_test
    ${CPPUTEST}
    ${CPPUTEST_EXT})

target_link_libraries(game_test
    ${CPPUTEST}
    ${CPPUTEST_EXT})

