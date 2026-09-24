cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(tinygltf)

add_library(tinygltf STATIC src/tiny_gltf.cpp)
target_include_directories(tinygltf PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_definitions(tinygltf PRIVATE TINYGLTF_USE_CPP14)
