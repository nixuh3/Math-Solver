cmake_minimum_required(VERSION 3.20)
project(math_solver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/main.cpp
    src/solver.cpp
    src/scanner.cpp
    src/parser.cpp
    src/engine.cpp
    src/token.cpp
    src/error.cpp
    src/arena.cpp
    src/utils.cpp
)

add_executable(math_solver ${SOURCES})
