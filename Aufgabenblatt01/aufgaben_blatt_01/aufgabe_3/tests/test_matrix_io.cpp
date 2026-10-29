// Regression test for loadMatrix() (QA-004).
//
// A ragged matrix file used to leave columnCount at the width of the last row
// and then indexed every row with it, reading past the end of the shorter rows.
// Such a file has to be rejected instead.
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "MatrixInt.hpp"

bool loadMatrix(std::string const& filename, MatrixInt& matrix);
bool writeMatrix(std::string const& filename, MatrixInt const& matrix);

namespace
{
    int failures = 0;

    void check(const bool condition, const std::string& what)
    {
        if (!condition)
        {
            std::cerr << "FAIL: " << what << '\n';
            ++failures;
        }
    }

    void write_file(const std::string& path, const std::string& content)
    {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        file << content;
    }
}

int main()
{
    {
        const std::string path = "matrix_square.txt";
        write_file(path, "2 3\n4 5\n");

        MatrixInt matrix;
        check(loadMatrix(path, matrix), "a rectangular matrix loads");
        check(matrix.getRowCount() == 2 && matrix.getColumnCount() == 2, "rectangular matrix size");
        check(matrix.data()[0] == 2 && matrix.data()[1] == 3 && matrix.data()[2] == 4 && matrix.data()[3] == 5,
              "rectangular matrix values");

        std::remove(path.c_str());
    }

    {
        const std::string path = "matrix_ragged_short_first.txt";
        write_file(path, "1 2\n3 4 5\n");

        MatrixInt matrix;
        check(!loadMatrix(path, matrix), "a ragged matrix (longer second row) is rejected");

        std::remove(path.c_str());
    }

    {
        const std::string path = "matrix_ragged_short_last.txt";
        write_file(path, "1 2 3\n4 5\n");

        MatrixInt matrix;
        check(!loadMatrix(path, matrix), "a ragged matrix (shorter second row) is rejected");

        std::remove(path.c_str());
    }

    if (failures != 0)
    {
        std::cerr << failures << " check(s) failed\n";
        return 1;
    }

    std::cout << "All matrix I/O checks passed\n";
    return 0;
}
