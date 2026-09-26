cmake_minimum_required(VERSION 3.16)
project(ImageViewer07 LANGUAGES C CXX)

# User options
option(DOWNGRADE_GL "Downgrade OpenGL to version 3.3 (default behavior for Apple)." OFF)

# Dependencies: GLFW 3.4 via FetchContent (same pin as sheets 06/08/09)
include(FetchContent)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
  URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip
  URL_HASH SHA256=a133ddc3d3c66143eba9035621db8e0bcf34dba1ee9514a9e23e96afd39fd57a
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(glfw)

# Application sources (glad/glowl/imgui are compiled in directly).
# The sheet's own sources are listed explicitly: file(GLOB) does not see files
# added after the configure step, so a new source file would be left out.
set(source_files
  ImageBase.cpp
  ImageConverter.cpp
  ImageFilter.cpp
  ImageIO.cpp
  ImageViewer.cpp
  main.cpp)
set(header_files
  Image.h
  ImageBase.h
  ImageConverter.h
  ImageFilter.h
  ImageIO.h
  ImageTraits.h
  ImageViewer.h)
file(GLOB glowl_files "${CMAKE_CURRENT_SOURCE_DIR}/glowl/*pp")
file(GLOB imgui_files "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*")
file(GLOB_RECURSE glad_files "${CMAKE_CURRENT_SOURCE_DIR}/glad/*")

add_executable(${PROJECT_NAME} ${source_files} ${header_files} ${glowl_files} ${imgui_files} ${glad_files})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/glowl
  ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${CMAKE_DL_LIBS})

if (${DOWNGRADE_GL})
  target_compile_definitions(${PROJECT_NAME} PRIVATE DOWNGRADE_GL)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

source_group("Header Files" FILES ${header_files})
source_group("Source Files" FILES ${source_files})
source_group("glowl" FILES ${glowl_files})
source_group("imgui" FILES ${imgui_files})
