cmake_minimum_required(VERSION 3.8)
project(ColorSpaces)

# Set CXX standard to 11 for compatibility
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fügen Sie hier alle Ihre Quelldateien (.cpp) hinzu
add_executable(ColorSpaces
        ColorSpaces.cpp
        ImageConverter.cpp
        ImageIO.cpp
        ImageBase.cpp
        ImageManipulation.cpp
)