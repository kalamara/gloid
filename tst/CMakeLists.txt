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
    mock_sdl_opengl.cpp
    model_test.cpp)

add_executable(functions_test
    ${SUT}/GLoid.h
    ${SUT}/Text.h
    ${SUT}/Point.h
    ${SUT}/Point.cpp
    functions_test.cpp)

add_executable(engine_test
    ${SUT}/GLoid.h
    ${SUT}/Text.h
    ${SUT}/Game.h
    ${SUT}/Game.cpp
    ${SUT}/Engine.h
    ${SUT}/Engine.cpp
    mock_sdl_opengl.cpp
    engine_test.cpp)

add_executable(game_test
    ${SUT}/GLoid.h
    ${SUT}/Point.h
    ${SUT}/Point.cpp
    ${SUT}/Text.h
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
    ${SUT}/Game.h
    ${SUT}/Game.cpp
    ${SUT}/Engine.h
    ${SUT}/Engine.cpp
    ${SUT}/game/Loading.h
    ${SUT}/game/Loading.cpp
    ${SUT}/World.h
    mock_sdl_opengl.cpp
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

target_link_libraries(engine_test
    ${CPPUTEST}
    ${CPPUTEST_EXT})

