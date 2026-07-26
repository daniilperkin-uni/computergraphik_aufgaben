cmake_minimum_required(VERSION 3.5)

project(computergraphik_aufgaben LANGUAGES CXX)

# Root build: configure every assignment sheet that ships its own CMakeLists.txt
# from a single place. Each sheet is added via add_subdirectory() and is guarded
# by an EXISTS check so the root configuration still succeeds if a sheet (or its
# CMakeLists.txt) is absent.
#
# Per-sheet builds remain fully supported - you can still cd into
# AufgabenblattXX/.../code and run `cmake .` there directly.
#
# NOTE on target names:
#   - Aufgabenblatt03 and Aufgabenblatt04 originally both defined a `Raytracer`
#     target. They have been renamed to `Raytracer03` and `Raytracer04` so both
#     can coexist in a single root build.
#   - Aufgabenblatt06 and Aufgabenblatt07 both define an `ImageViewer` target.
#     Only Aufgabenblatt06 is added here; build Aufgabenblatt07 standalone to
#     avoid a duplicate-target error.
# NOTE on dependencies:
#   Aufgabenblatt06-09 require OpenGL 4.4+/4.5+ plus GLFW, GLAD, GLM and (for 09)
#   glowl/tinygltf. If those are not installed, configure their add_subdirectory
#   line out (or build them per-sheet in an environment that has the deps).

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

# Aufgabenblatt07 reuses the `ImageViewer` target name (same as Blatt 06) and
# therefore cannot be added alongside Blatt 06 in a single build tree. Build it
# per-sheet instead:
#   cd Aufgabenblatt07/00_student_setup/code && cmake -B build && cmake --build build
# if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt07/00_student_setup/code/CMakeLists.txt")
#     add_subdirectory(Aufgabenblatt07/00_student_setup/code)
# endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt08/00_student_setup/code/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt08/00_student_setup/code)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Aufgabenblatt09/code/CMakeLists.txt")
    add_subdirectory(Aufgabenblatt09/code)
endif()
