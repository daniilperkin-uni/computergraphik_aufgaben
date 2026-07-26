cmake_minimum_required(VERSION 3.5)

project(Raytracer04)

# Explicit source listing (avoid file(GLOB) which does not track added/removed files).
set(SOURCES
    main.cpp
    sceneobject.cpp
)

set(HEADERS
    pointlight.h
    scene.h
    sceneobject.h
    util.h
    vec3.h
)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

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
