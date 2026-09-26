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
# Explicit listings: file(GLOB) does not see files added or removed after the
# configure step, so a new source file would silently miss the build.
set(source_files
  Math1.cpp
  Rasterizer.cpp
  main.cpp)
set(header_files
  Math1.h
  Rasterizer.h)
set(scene_source_files
  scene/Camera.cpp
  scene/Lights/AmbientLight.cpp
  scene/Lights/LightObject.cpp
  scene/Lights/PointLight.cpp
  scene/Object.cpp
  scene/Objects/Container.cpp
  scene/Objects/Cube.cpp
  scene/Objects/SceneObject.cpp
  scene/Objects/Sphere.cpp
  scene/Objects/Triangle.cpp
  scene/Scene.cpp)
set(scene_header_files
  scene/Camera.h
  scene/Lights/AmbientLight.h
  scene/Lights/LightObject.h
  scene/Lights/PointLight.h
  scene/Object.h
  scene/Objects/Container.h
  scene/Objects/Cube.h
  scene/Objects/SceneObject.h
  scene/Objects/Sphere.h
  scene/Objects/Triangle.h
  scene/Scene.h)
set(image_source_files
  image/ImageBase.cpp
  image/ImageIO.cpp
  image/ImageViewer.cpp)
set(image_header_files
  image/Image.h
  image/ImageBase.h
  image/ImageIO.h
  image/ImageTraits.h
  image/ImageViewer.h)
set(shader_files
  shader/display_texture_f.glsl
  shader/display_texture_v.glsl
  shader/display_texture_v_downgrade.glsl)

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
