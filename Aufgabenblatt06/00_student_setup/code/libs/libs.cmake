# Dependencies

# Setup FetchContent
include(FetchContent)

# Suppress deprecation warning for FetchContent_Populate.
# We intentionally use it for ImGui to perform a custom step (file copy).
cmake_policy(SET CMP0169 OLD)

# By default, we want to see the population status
set(FETCHCONTENT_QUIET OFF)

# GLFW 3.4 (shared version across all OpenGL sheets)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
# X11 only: avoids requiring wayland-scanner/xkbcommon on Linux
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
if (WIN32)
  set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)
endif ()
FetchContent_Declare(glfw
  URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip
  URL_HASH SHA256=a133ddc3d3c66143eba9035621db8e0bcf34dba1ee9514a9e23e96afd39fd57a
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(glfw)
set_target_properties(glfw PROPERTIES FOLDER libs)

# glm 1.0.1 (header-only)
FetchContent_Declare(glm
  URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
  URL_HASH SHA256=09c5716296787e1f7fcb87b1cbdbf26814ec1288ed6259ccd30d5d9795809fa5
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(glm)
# imgui
# We use the older FetchContent_Populate pattern here because we need to
# copy a custom CMakeLists.txt file into the downloaded source directory.
FetchContent_Declare(imgui
        URL https://github.com/ocornut/imgui/archive/refs/tags/v1.86.zip
        URL_HASH MD5=bccb1c173296979e6a61b4f9da572429
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  message(STATUS "Fetch imgui ...")
  FetchContent_Populate(imgui)
  # This custom CMakeLists.txt is still needed for ImGui
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/CMakeLists.txt DESTINATION ${imgui_SOURCE_DIR})
  add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(imgui PROPERTIES FOLDER libs)
endif()


# local libraries

# glad
if (NOT TARGET glad)
set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad")
add_library(glad
        ${GLAD_DIR}/include/glad/gl.h
        ${GLAD_DIR}/include/KHR/khrplatform.h
        ${GLAD_DIR}/src/gl.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
target_link_libraries(glad ${CMAKE_DL_LIBS})
set_target_properties(glad PROPERTIES FOLDER libs)
endif ()

# glowl
set(GLOWL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glowl")
add_library(glowl
        ${GLOWL_DIR}/GLSLProgram.cpp
        ${GLOWL_DIR}/GLSLProgram.hpp
        ${GLOWL_DIR}/Texture.hpp
        ${GLOWL_DIR}/Texture2D.cpp
        ${GLOWL_DIR}/Texture2D.hpp)
target_include_directories(glowl PUBLIC ${GLOWL_DIR})
target_link_libraries(glowl glad)
set_target_properties(glowl PROPERTIES FOLDER libs)