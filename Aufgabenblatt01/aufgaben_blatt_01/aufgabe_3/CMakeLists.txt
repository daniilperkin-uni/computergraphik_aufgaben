cmake_minimum_required(VERSION 3.16)

project(aufgabe_3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(aufgabe_3
        code/01_raii.cpp
        code/MatrixInt.cpp
)

target_include_directories(aufgabe_3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/code)