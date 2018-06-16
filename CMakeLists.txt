cmake_minimum_required(VERSION 2.8)

project(GLoid)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories( ${CMAKE_SOURCE_DIR},
    ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(tst)
