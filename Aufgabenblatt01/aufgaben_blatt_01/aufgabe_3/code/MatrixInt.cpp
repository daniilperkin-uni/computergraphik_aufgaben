#include "MatrixInt.hpp"

#include <algorithm>
#include <iostream>
#include <cstring>


MatrixInt::MatrixInt() : m_row_cnt(0), m_column_cnt(0), m_raw_data(nullptr) {
}

MatrixInt::MatrixInt(unsigned int row_cnt, unsigned int column_cnt)
    : m_row_cnt(row_cnt), m_column_cnt(column_cnt), m_raw_data(nullptr)
{
    if (m_row_cnt == 0 || m_column_cnt == 0) {
        return;
    }

    // Allocate memory for the matrix data.
    m_raw_data = new int[m_row_cnt * m_column_cnt];

    // Initialize the allocated memory with zeros.
    std::memset(m_raw_data, 0, m_row_cnt * m_column_cnt * sizeof(int));
}

MatrixInt::~MatrixInt() {

    delete[] m_raw_data;

    m_raw_data = nullptr;
}

MatrixInt::MatrixInt(MatrixInt const& other)
    : m_row_cnt(other.m_row_cnt), m_column_cnt(other.m_column_cnt), m_raw_data(nullptr)
{
    if (other.m_raw_data != nullptr) {
        m_raw_data = new int[m_row_cnt*m_column_cnt];
        std::memcpy(m_raw_data, other.m_raw_data, m_row_cnt*m_column_cnt * sizeof(int));
    }
}

MatrixInt::MatrixInt(MatrixInt&& other) : MatrixInt() {
    std::swap(m_raw_data, other.m_raw_data);
    std::swap(m_row_cnt, other.m_row_cnt);
    std::swap(m_column_cnt, other.m_column_cnt);
}

MatrixInt& MatrixInt::operator=(MatrixInt const& rhs) {
    if (this != &rhs) {
        MatrixInt tmp(rhs);
        std::swap(m_raw_data, tmp.m_raw_data);
        std::swap(m_row_cnt, tmp.m_row_cnt);
        std::swap(m_column_cnt, tmp.m_column_cnt);
    }

    return *this;
}

MatrixInt& MatrixInt::operator=(MatrixInt&& rhs) {
    if (this != &rhs) {
        std::swap(m_raw_data, rhs.m_raw_data);
        std::swap(m_row_cnt, rhs.m_row_cnt);
        std::swap(m_column_cnt, rhs.m_column_cnt);
    }

    return *this;
}

MatrixInt MatrixInt::operator*(MatrixInt const& rhs)
{

    unsigned int rows1 = this->getRowCount();
    unsigned int columns1 = this->getColumnCount();


    unsigned int rows2 = rhs.getRowCount();
    unsigned int columns2 = rhs.getColumnCount();

    if (columns1 != rows2) {
        std::cerr << "Matrix multiplication size mismatch." << std::endl;
        return MatrixInt();
    }

    MatrixInt resultMatrix(rows1, columns2);

    for (unsigned int i = 0; i < rows1; ++i) {
        for (unsigned int j = 0; j < columns2; ++j) {
            for (unsigned int k = 0; k < columns1; ++k) {

                int a_val = this->m_raw_data[i * this->m_column_cnt + k];
                int b_val = rhs.m_raw_data[k * rhs.m_column_cnt + j];

                resultMatrix.m_raw_data[i * resultMatrix.m_column_cnt + j] += a_val * b_val;
            }
        }
    }

    return resultMatrix;
}