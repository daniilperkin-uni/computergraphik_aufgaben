cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(SceneShading)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif ()

# Dependencies
include(libs/libs.cmake)

# Project files
# Sources are listed explicitly: file(GLOB) does not see files added after the
# configure step, so a new source file would silently miss the build.
set(source_files
  src/core.cpp
  src/main.cpp)
set(header_files
  include/core.hpp
  include/scene.hpp)
file(GLOB shader_files "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*")

# Target
add_executable(${PROJECT_NAME} ${source_files} ${header_files} ${shader_files})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm tinygltf)

# VS
source_group("Header Files" FILES ${header_files})
source_group("Source Files" FILES ${source_files})
source_group("shaders" FILES ${shader_files})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if(MSVC)
  set_property(DIRECTORY . PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
  set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD TRUE)
endif()

# Install
include(GNUInstallDirs)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "" FORCE)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_INSTALL_PREFIX}")
# Note: this sheet ships no resources/ directory (the scene asset lives outside
# the repository, see README.md), so there is nothing else to install.
