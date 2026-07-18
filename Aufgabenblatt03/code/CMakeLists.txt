cmake_minimum_required(VERSION 3.5)

project(Raytracer)

file(GLOB SOURCES "./*.cpp" "./*.h")

# Configure the compiler.
if (${CMAKE_COMPILER_IS_GNUCXX})
  add_definitions(-pedantic -fPIC -DUNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DDEBUG -D_DEBUG -ggdb")
endif()

if (${MSVC})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -EHsc")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)