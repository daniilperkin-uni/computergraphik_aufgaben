#ifndef MatrixInt_hpp
#define MatrixInt_hpp

/**
* Simple implementation of an integer-valued matrix.
*/
class MatrixInt
{
private:
    int* m_raw_data;           ///< Pointer to raw data storage of the matrix
    unsigned int m_row_cnt;    ///< Number of rows of the matrix
    unsigned int m_column_cnt; ///< Number of columns of the matrix

public:
    /**
     * Default Constructor.
     * Create matrix of size 0 with sets raw data storage pointer to nullptr.
     */
    MatrixInt();
    /**
     * Constructor that creates a matrix of given size, intially filled with zeros.
     * If row or coloumn count is zero, raw data storage pointer is set to nullptr.
     */
    MatrixInt(unsigned int row_cnt, unsigned int column_cnt);
    ~MatrixInt();
    /** Copy constructor */
    MatrixInt(MatrixInt const& other);
    /** Move constructor */
    MatrixInt(MatrixInt&& other);
    /** Copy-assignment operator */
    MatrixInt& operator=(MatrixInt const& rhs);
    /** Move-assigment operator */
    MatrixInt& operator=(MatrixInt&& other);
    
    /**
    * Multiplication operator. Multiplies this matrix with another matrix and returns the result.
    * If the multiplication is not possible, a matrix of size zero is returned.
    */
    MatrixInt operator*(MatrixInt const& other);

    /** Read/write access to the internal data storage of the matrix. */
    int* data() noexcept {
        return m_raw_data;
    }

    /** Read only access to the internal data storage of the matrix. */
    int const* data() const noexcept {
        return m_raw_data;
    }

    /** Returns the number of rows of the matrix. */
    unsigned int getRowCount() const noexcept {
        return m_row_cnt;
    }

    /** Returns the number of columns of the matrix. */
    unsigned int getColumnCount() const noexcept {
        return m_column_cnt;
    }

};

#endif //!MatrixInt_hpp
