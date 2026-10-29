cmake_minimum_required(VERSION 3.16)
project(ColorSpaces)

# Set CXX standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fügen Sie hier alle Ihre Quelldateien (.cpp) hinzu
add_executable(ColorSpaces
        ColorSpaces.cpp
        ImageConverter.cpp
        ImageIO.cpp
        ImageBase.cpp
        ImageManipulation.cpp
)

# Regression tests (CTest): Netpbm round-trips and malformed-input handling.
enable_testing()
add_executable(ImageIOTests tests/test_image_io.cpp ImageIO.cpp ImageBase.cpp)
target_include_directories(ImageIOTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME ImageIOTests COMMAND ImageIOTests)
set_tests_properties(ImageIOTests PROPERTIES TIMEOUT 30)