cmake_minimum_required(VERSION 3.16)

project(aufgabe_4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(aufgabe_4
        code/01_functions.cpp
        code/source1.cpp
        code/source2.cpp
        code/Definition.cpp
)

target_include_directories(aufgabe_4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/code)