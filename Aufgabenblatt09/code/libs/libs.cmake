# Dependencies

# Setup FetchContent
include(FetchContent)
mark_as_advanced(FORCE
  FETCHCONTENT_BASE_DIR
  FETCHCONTENT_FULLY_DISCONNECTED
  FETCHCONTENT_QUIET
  FETCHCONTENT_UPDATES_DISCONNECTED)

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
# tinygltf (pinned commit). SOURCE_SUBDIR points at a directory that does not
# exist, so FetchContent_MakeAvailable only downloads the sources and does not
# add tinygltf's own CMake project (loader_example, install rules, ...). The
# implementation is compiled once as a local static library instead.
set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE BOOL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(tinygltf
  URL https://github.com/syoyo/tinygltf/archive/e7f1ff5c59d3ca2489923beb239bdf93d863498f.zip
  URL_HASH SHA256=4a91b7debca7aa9c0b49b1d84479a61b87cb2188365824fdc8c4bfcac9b98911
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  SOURCE_SUBDIR cmake-no-subproject)
FetchContent_MakeAvailable(tinygltf)
mark_as_advanced(FORCE
  FETCHCONTENT_SOURCE_DIR_TINYGLTF
  FETCHCONTENT_UPDATES_DISCONNECTED_TINYGLTF)
if (NOT TARGET tinygltf)
  add_library(tinygltf STATIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinygltf/tiny_gltf.cpp)
  target_include_directories(tinygltf PUBLIC ${tinygltf_SOURCE_DIR})
  target_compile_definitions(tinygltf PRIVATE TINYGLTF_USE_CPP14)
  target_compile_features(tinygltf PUBLIC cxx_std_14)
  set_target_properties(tinygltf PROPERTIES FOLDER libs)
endif ()

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
