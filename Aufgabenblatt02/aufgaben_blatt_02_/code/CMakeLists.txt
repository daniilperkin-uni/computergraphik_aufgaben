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