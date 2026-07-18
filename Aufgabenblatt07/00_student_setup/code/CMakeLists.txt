cmake_minimum_required(VERSION 3.8)
project(ImageViewer)

# User options
option(DOWNGRADE_GL "Downgrade OpenGL to version 3.3 (default behavior for Apple)." OFF)

# Set CXX standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH "" FORCE)
endif()

# Configure GLFW
include(ExternalProject)

ExternalProject_Add(glfw3_ext
    PREFIX ${CMAKE_BINARY_DIR}/glfw-log
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/glfw
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    INSTALL_DIR ${CMAKE_BINARY_DIR}/glfw
    CMAKE_ARGS
        -DCMAKE_BUILD_TYPE:String=${CMAKE_BUILD_TYPE}
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/glfw)

include_directories(${CMAKE_SOURCE_DIR}/glowl/ ${CMAKE_SOURCE_DIR}/glad/include/ ${CMAKE_SOURCE_DIR}/glfw/include/)
link_directories(${CMAKE_BINARY_DIR}/glfw/lib)

# Your application target
file(GLOB source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")
file(GLOB glowl_files "${CMAKE_CURRENT_SOURCE_DIR}/glowl/*pp")
file(GLOB imgui_files "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*")
file(GLOB_RECURSE glad_files "${CMAKE_CURRENT_SOURCE_DIR}/glad/*")

add_executable(ImageViewer ${source_files} ${header_files} ${glowl_files} ${imgui_files} ${glad_files})
add_dependencies(ImageViewer glfw3_ext)
target_link_libraries(ImageViewer glfw3)
target_include_directories(ImageViewer
    INTERFACE $<BUILD_INTERFACE:${IMGUI_INCLUDE_ONLY_DIR}>
    PRIVATE $<BUILD_INTERFACE:${IMGUI_INCLUDE_ONLY_DIR}/blubb>)

if (${DOWNGRADE_GL})
  target_compile_definitions(ImageViewer PRIVATE DOWNGRADE_GL)
endif()

# Install executable to the bin directory
install(TARGETS ImageViewer RUNTIME DESTINATION bin)

source_group("Header Files" FILES ${header_files})
source_group("Source Files" FILES ${source_files})
source_group("glowl" FILES ${glowl_files})
source_group("imgui" FILES ${imgui_files})

# Include additional libraries depending on the OS
if (UNIX)
    target_link_libraries(ImageViewer dl X11 Xrandr Xxf86vm Xext Xi GL Xinerama Xcursor pthread)
endif()
if (APPLE)
    target_link_libraries(ImageViewer dl objc "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()
