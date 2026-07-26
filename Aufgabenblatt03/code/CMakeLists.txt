cmake_minimum_required(VERSION 3.5)

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
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
