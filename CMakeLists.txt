cmake_minimum_required (VERSION 3.9)

project(foo LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)

add_executable(foo
               ${PROJECT_SOURCE_DIR}/src/foo.cpp
               ${PROJECT_SOURCE_DIR}/src/features.h)
