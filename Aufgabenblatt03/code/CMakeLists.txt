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

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Automated verification: the raytracer renders one image and compares it with
# the matching reference PPM in this directory (see comparePPM in util.h). It
# exits non-zero on a mismatch, so the check can run in CI via ctest.
# The working directory is the build directory, which is what the relative
# reference paths in main.cpp expect.
enable_testing()
add_test(NAME ${PROJECT_NAME}_reference
         COMMAND ${PROJECT_NAME}
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
