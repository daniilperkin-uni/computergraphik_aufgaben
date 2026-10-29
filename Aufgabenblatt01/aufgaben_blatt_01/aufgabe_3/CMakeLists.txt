cmake_minimum_required(VERSION 3.16)

project(aufgabe_3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(aufgabe_3
        code/01_raii.cpp
        code/MatrixInt.cpp
)

target_include_directories(aufgabe_3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/code)

# Regression test (CTest): a ragged matrix file must be rejected instead of
# indexing past the end of a short row.
enable_testing()
add_executable(MatrixIOTests tests/test_matrix_io.cpp code/01_raii.cpp code/MatrixInt.cpp)
target_compile_definitions(MatrixIOTests PRIVATE MATRIX_IO_TEST)
target_include_directories(MatrixIOTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/code)
add_test(NAME MatrixIOTests COMMAND MatrixIOTests)
set_tests_properties(MatrixIOTests PROPERTIES TIMEOUT 30)