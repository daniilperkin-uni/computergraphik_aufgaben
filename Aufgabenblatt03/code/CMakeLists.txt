cmake_minimum_required(VERSION 3.16)

project(Raytracer03)

# Explicit source listing (avoid file(GLOB) which does not track added/removed files).
set(SOURCES
    main.cpp
    sceneobject.cpp
)

set(HEADERS
    scene.h
    sceneobject.h
    util.h
    vec3.h
)

# Configure the compiler.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_definitions(-pedantic -fPIC -DUNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DDEBUG -D_DEBUG -ggdb")
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -EHsc")
endif()

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Absolute path to the reference images: the comparison must work both in a
# standalone build (code/build) and in the root build tree (build/Aufgabenblatt03/code).
target_compile_definitions(${PROJECT_NAME} PRIVATE REFERENCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Automated verification: the raytracer renders one image and compares it with
# the matching reference PPM in this directory (see comparePPM in util.h). It
# exits non-zero on a mismatch, so the check can run in CI via ctest.
# The reference directory is passed as REFERENCE_DIR, so the check works in a
# standalone build tree as well as in the root build tree.
enable_testing()

# Regression test for the comparePPM() mismatch diagnostic (header-only, no render).
add_executable(ComparePPMTests tests/test_compare_ppm.cpp)
target_compile_features(ComparePPMTests PRIVATE cxx_std_17)
target_include_directories(ComparePPMTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME ComparePPMTests COMMAND ComparePPMTests)
set_tests_properties(ComparePPMTests PROPERTIES TIMEOUT 30)

# Regression test for Vec3::normalize() (header-only).
add_executable(Vec3Tests tests/test_vec3.cpp)
target_compile_features(Vec3Tests PRIVATE cxx_std_17)
target_include_directories(Vec3Tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME Vec3Tests COMMAND Vec3Tests)

add_test(NAME ${PROJECT_NAME}_reference
         COMMAND ${PROJECT_NAME}
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
