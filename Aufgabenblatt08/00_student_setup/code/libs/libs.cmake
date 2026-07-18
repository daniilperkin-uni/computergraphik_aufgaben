# Dependencies

# Setup FetchContent
include(FetchContent)
mark_as_advanced(FORCE
  FETCHCONTENT_BASE_DIR
  FETCHCONTENT_FULLY_DISCONNECTED
  FETCHCONTENT_QUIET
  FETCHCONTENT_UPDATES_DISCONNECTED)

# GLFW
FetchContent_Declare(glfw
  URL https://github.com/glfw/glfw/archive/refs/tags/3.3.6.zip
  URL_HASH MD5=ae8fafce42bf94b658101aa921a74c13
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
if (WIN32)
  option(GLFW_USE_HYBRID_HPG "" ON)
endif ()

FetchContent_MakeAvailable(glfw)

mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_GLFW
  FETCHCONTENT_UPDATES_DISCONNECTED_GLFW
  BUILD_SHARED_LIBS
  GLFW_BUILD_DOCS
  GLFW_BUILD_EXAMPLES
  GLFW_BUILD_TESTS
  GLFW_INSTALL
  GLFW_USE_HYBRID_HPG
  GLFW_USE_OSMESA
  GLFW_USE_WAYLAND
  GLFW_VULKAN_STATIC
  USE_MSVC_RUNTIME_LIBRARY_DLL
  X11_xcb_icccm_INCLUDE_PATH
  X11_xcb_icccm_LIB)

set_target_properties(glfw PROPERTIES FOLDER libs)

# glm
FetchContent_Declare(glm
  URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.zip
  URL_HASH MD5=a421a37d1896ab74765c3f94431ca0f6
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

FetchContent_MakeAvailable(glm)

mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_GLM
  FETCHCONTENT_UPDATES_DISCONNECTED_GLM)

# local libraries

# glad
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/libs/glad")
add_library(glad
  ${GLAD_DIR}/include/glad/gl.h
  ${GLAD_DIR}/include/KHR/khrplatform.h
  ${GLAD_DIR}/src/gl.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
target_link_libraries(glad ${CMAKE_DL_LIBS})
set_target_properties(glad PROPERTIES FOLDER libs)

# lodepng
set(LODEPNG_DIR "${CMAKE_SOURCE_DIR}/libs/lodepng")
add_library(lodepng
  ${LODEPNG_DIR}/include/lodepng/lodepng.h
  ${LODEPNG_DIR}/src/lodepng.cpp)
target_include_directories(lodepng PUBLIC ${LODEPNG_DIR}/include)
set_target_properties(lodepng PROPERTIES FOLDER libs)
