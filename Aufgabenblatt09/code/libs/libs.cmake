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
# tinygltf
FetchContent_Declare(tinygltf
  URL https://github.com/syoyo/tinygltf/archive/e7f1ff5c59d3ca2489923beb239bdf93d863498f.zip
  URL_HASH MD5=c86fafd4bb8ae09805e5e709565559a7
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_GetProperties(tinygltf)
if (NOT tinygltf_POPULATED)
  message(STATUS "Fetch tinygltf ...")
  FetchContent_Populate(tinygltf)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_TINYGLTF
    FETCHCONTENT_UPDATES_DISCONNECTED_TINYGLTF)
  # TODO tinygltf CMake now better supports add_subdirectory, but not as static library yet.
  file(COPY ${tinygltf_SOURCE_DIR}/json.hpp DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image.h DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${tinygltf_SOURCE_DIR}/stb_image_write.h DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${tinygltf_SOURCE_DIR}/tiny_gltf.h DESTINATION ${tinygltf_BINARY_DIR}/include)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinygltf/CMakeLists.txt DESTINATION ${tinygltf_BINARY_DIR})
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinygltf/tiny_gltf.cpp DESTINATION ${tinygltf_BINARY_DIR}/src)
  add_subdirectory(${tinygltf_BINARY_DIR} ${tinygltf_BINARY_DIR} EXCLUDE_FROM_ALL)
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
