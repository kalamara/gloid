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
    ${SUT}/Game.h
    ${SUT}/Text.h
    ${SUT}/Point.h
    ${SUT}/Engine.h
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
    GameMock.h
    model_test.cpp)

add_executable(functions_test
    ${SUT}/GLoid.h
    ${SUT}/Text.h
    ${SUT}/Point.h
    ${SUT}/Point.cpp
    functions_test.cpp)

add_executable(game_test
    ${SUT}/GLoid.h
    ${SUT}/Game.h
    ${SUT}/Game.cpp
    ${SUT}/Text.h
    ${SUT}/Engine.h
    ${SUT}/Engine.cpp
    ${SUT}/World.h
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


