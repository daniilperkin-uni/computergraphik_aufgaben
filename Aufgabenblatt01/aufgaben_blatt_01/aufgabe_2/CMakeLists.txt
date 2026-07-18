cmake_minimum_required(VERSION 3.23)

project(aufgabe_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(aufgabe_2)

target_sources(aufgabe_2
        PRIVATE
        code/01_stl.cpp
        code/LineStrip3D.cpp
)

target_include_directories(aufgabe_2
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/code
)