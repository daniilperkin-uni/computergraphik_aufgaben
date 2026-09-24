cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

if (POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif ()

project(Geometrie)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif ()

# Dependencies
include(libs/libs.cmake)

# Project files
file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE header_files "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*")
file(GLOB shader_files "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*")

# Target
add_executable(${PROJECT_NAME} ${source_files} ${header_files} ${shader_files})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm lodepng)

# VS
source_group("Header Files" FILES ${header_files})
source_group("Source Files" FILES ${source_files})
source_group("shaders" FILES ${shader_files})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if (MSVC)
  set_property(DIRECTORY . PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
  set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD TRUE)
endif ()

# Install
include(GNUInstallDirs)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "" FORCE)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_INSTALL_PREFIX}")
