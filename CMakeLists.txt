cmake_minimum_required(VERSION 3.16)

project(computergraphik_aufgaben LANGUAGES CXX)

# Root build: configure every assignment sheet that ships its own CMakeLists.txt
# from a single place. Each sheet is added via add_subdirectory() and is guarded
# by an EXISTS check so the root configuration still succeeds if a sheet (or its
# CMakeLists.txt) is absent.
#
# Per-sheet builds remain fully supported - you can still cd into
# a sheet: cmake -S <sheet> -B <sheet>/build
#
# NOTE on target names:
#   - Aufgabenblatt03 and Aufgabenblatt04 originally both defined a `Raytracer`
#     target. They have been renamed to `Raytracer03` and `Raytracer04` so both
#     can coexist in a single root build.
#   - Aufgabenblatt07 defines `ImageViewer07` so it can coexist with the
#     `ImageViewer` target of Aufgabenblatt06.
# NOTE on dependencies:
#   Aufgabenblatt06-09 fetch GLFW 3.4 / GLM 1.0.1 (and imgui, tinygltf) via
#   FetchContent; the shared `glad` target is defined once. Only OpenGL and
#   X11 development headers must be installed on the system.

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt01/aufgaben_blatt_01/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt01/aufgaben_blatt_01)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt02/aufgaben_blatt_02_/code/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt02/aufgaben_blatt_02_/code)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt03/code/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt03/code)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt04/code/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt04/code)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt06/00_student_setup/code/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt06/00_student_setup/code)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt07/00_student_setup/code/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt07/00_student_setup/code)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt08/00_student_setup/code/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt08/00_student_setup/code)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt09/code/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt09/code)
endif()
