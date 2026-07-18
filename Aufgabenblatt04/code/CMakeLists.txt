cmake_minimum_required(VERSION 3.5)

project(Raytracer)

file(GLOB SOURCES "./*.cpp" "./*.h")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Configure the compiler.
if (${CMAKE_COMPILER_IS_GNUCXX})
        add_definitions(-pedantic -fPIC -DUNIX )
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DDEBUG -D_DEBUG -ggdb")
endif (${CMAKE_COMPILER_IS_GNUCXX})
if (${MSVC})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -EHsc")
endif (${MSVC})

add_executable(${PROJECT_NAME} ${SOURCES})
