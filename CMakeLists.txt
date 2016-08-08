cmake_minimum_required(VERSION 3.5)
project(swift_for_db2_cli)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/connect.h
    include/disconnect.h
    setup/env.sh
    setup/install.sh
    src/connect.c
    src/disconnect.c
    tests/include/test_connect.hpp
    tests/include/test_main.hpp
    tests/src/test_connect.cpp
    tests/src/test_main.cpp
    connect.o
    CONTRIBUTING.md
    LICENSE.txt
    makefile
    README.md)

add_executable(swift_for_db2_cli ${SOURCE_FILES})