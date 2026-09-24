cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(ImageViewer)

# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif ()

# User options
option(DOWNGRADE_GL "Downgrade OpenGL to version 3.3 (default behavior for Apple)." OFF)

# Dependencies
find_package(OpenMP)
include(libs/libs.cmake)

# Project files
file(GLOB source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")
file(GLOB_RECURSE scene_header_files "${CMAKE_CURRENT_SOURCE_DIR}/scene/*.h")
file(GLOB_RECURSE scene_source_files "${CMAKE_CURRENT_SOURCE_DIR}/scene/*.cpp")
file(GLOB image_header_files "${CMAKE_CURRENT_SOURCE_DIR}/image/*.h")
file(GLOB image_source_files "${CMAKE_CURRENT_SOURCE_DIR}/image/*.cpp")
file(GLOB shader_files "${CMAKE_CURRENT_SOURCE_DIR}/shader/*")

# Target
add_executable(${PROJECT_NAME}
  ${source_files}
  ${header_files}
  ${scene_header_files}
  ${scene_source_files}
  ${image_header_files}
  ${image_source_files}
  ${shader_files})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm imgui glowl)

if(OpenMP_CXX_FOUND)
  target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

if (${DOWNGRADE_GL})
  target_compile_definitions(${PROJECT_NAME} PRIVATE DOWNGRADE_GL)
endif()

# VS
source_group("Header Files" FILES ${header_files})
source_group("Source Files" FILES ${source_files})
source_group("scene" FILES ${scene_header_files} ${scene_source_files})
source_group("image" FILES ${image_header_files} ${image_source_files})
source_group("shaders" FILES ${shader_files})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if(MSVC)
  set_property(DIRECTORY . PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
  set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD TRUE)
endif()

# Install
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shader" DESTINATION .)
