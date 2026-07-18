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
FetchContent_GetProperties(glfw)
if (NOT glfw_POPULATED)
  message(STATUS "Fetch glfw ...")
  FetchContent_Populate(glfw)
  option(GLFW_BUILD_DOCS "" OFF)
  option(GLFW_BUILD_EXAMPLES "" OFF)
  option(GLFW_BUILD_TESTS "" OFF)
  option(GLFW_INSTALL "" OFF)
  if (WIN32)
    option(GLFW_USE_HYBRID_HPG "" ON)
  endif ()
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
  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(glfw PROPERTIES
    FOLDER libs)
endif ()

# glm
FetchContent_Declare(glm
  URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
  message(STATUS "Fetch glm ...")
  FetchContent_Populate(glm)
  mark_as_advanced(FORCE
    FETCHCONTENT_SOURCE_DIR_GLM
    FETCHCONTENT_UPDATES_DISCONNECTED_GLM)
  add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

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
  file(COPY ${CMAKE_SOURCE_DIR}/libs/tinygltf/CMakeLists.txt DESTINATION ${tinygltf_BINARY_DIR})
  file(COPY ${CMAKE_SOURCE_DIR}/libs/tinygltf/tiny_gltf.cpp DESTINATION ${tinygltf_BINARY_DIR}/src)
  add_subdirectory(${tinygltf_BINARY_DIR} ${tinygltf_BINARY_DIR} EXCLUDE_FROM_ALL)
  set_target_properties(tinygltf PROPERTIES FOLDER libs)
endif ()

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
