cmake_minimum_required(VERSION 2.8)

project(gloid_test)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(CPPUTEST CppUTest)
find_library(CPPUTEST_EXT CppUTestExt)
set(SUT ../src/)
include_directories(
    ${SUT}
    .)

add_executable(model_test
    ${SUT}/GLoid.h
    ${SUT}/model/WhatUC.h
    ${SUT}/model/Particle.h
    ${SUT}/model/Particle.cpp
    ${SUT}/model/Ball.h
    ${SUT}/model/Ball.cpp
    ${SUT}/model/Vaus.h
    ${SUT}/model/Vaus.cpp
    ${SUT}/model/Brick.h
    ${SUT}/model/Brick.cpp

    GameMock.h
    model_test.cpp)

add_executable(functions_test
    ${SUT}/GLoid.h
    ${SUT}/Functions.h
    ${SUT}/Functions.cpp
    functions_test.cpp)

add_executable(game_test
    ${SUT}/GLoid.h
    ${SUT}/Game.h
    ${SUT}/Game.cpp
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


