#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "MatrixInt.hpp"


/** Splits a string into substrings seperated by whitespaces.*/
void tokenize(const std::string& line, std::vector<std::string>& tokens)
{
    std::stringstream iss(line);
    std::string token;
    while (iss >> token)
        tokens.push_back(token);
}

/** Loads the values of a matrix from a file and stores them in the given matrix object.*/
bool loadMatrix(std::string const& filename, MatrixInt& matrix)
{
    unsigned int rowCount = 0;
    unsigned int columnCount = 0;

    std::ifstream inFile;
    inFile.open(filename, std::ifstream::in);

    if (!inFile.good())
        return false;

    std::vector<std::vector<std::string>> lines; // stores contents of file

    // Read file and query number of rows and columns
    while (inFile.good()) {

        std::string line;
        getline(inFile, line);

        if (line == "")
            break;

        std::vector<std::string> tokens;
        tokenize(line, tokens);

        columnCount = static_cast<unsigned int>(tokens.size());
        rowCount++;

        lines.push_back(tokens);
    }

    // Intialization of the Matrix
    matrix = MatrixInt(rowCount, columnCount);

    // Copy values to matrix
    for (size_t currentRow = 0; currentRow<rowCount; currentRow++)
    {
        for (size_t currentColumn = 0; currentColumn<columnCount; currentColumn++)
        {
            int value = stoi(lines[currentRow][currentColumn]);

            // Lösung:
            matrix.data()[currentRow * columnCount + currentColumn] = value;
        }
    }

    return true;
}

bool writeMatrix(std::string const& filename, MatrixInt const& matrix)
{
    unsigned int rowCount = matrix.getRowCount();
    unsigned int columnCount = matrix.getColumnCount();

    std::ofstream outFile;
    outFile.open(filename, std::ofstream::out);

    if (!outFile.good()) {
        return false;
    }
    else {

        for (size_t i = 0; i < rowCount; ++i)
        {
            for (size_t j = 0; j < columnCount; ++j)
            {
                outFile << matrix.data()[i*columnCount + j] << " ";
            }
            outFile << std::endl;
        }
    }

    return true;
}
int main()
{
    std::string matrix1path;
    std::string matrix2path;

    std::cout << "Geben Sie den Pfad der ersten Matrixdatei ein:" << std::endl;
    std::cin >> matrix1path;
    if (std::cin.fail()) {
        std::cerr << "Invalid value" << std::endl;
        return -1;
    }
    std::cout << "Geben Sie den Pfad der zweiten Matrixdatei ein:" << std::endl;
    std::cin >> matrix2path;
    if (std::cin.fail()) {
        std::cerr << "Invalid value" << std::endl;
        return -1;
    }

    MatrixInt matrix1;
    MatrixInt matrix2;


    if (loadMatrix(matrix1path, matrix1) &&
        loadMatrix(matrix2path, matrix2))
    {
        std::cout << "Matrizen erfolgreich geladen." << std::endl;


        std::cout << "Matrix 1 hat " << matrix1.getRowCount() << " Zeilen und " << matrix1.getColumnCount() << " Spalten." << std::endl;
        std::cout << "Matrix 2 hat " << matrix2.getRowCount() << " Zeilen und " << matrix2.getColumnCount() << " Spalten." << std::endl;

        MatrixInt resultMatrix = matrix1 * matrix2;


        std::cout << "Multiplikation durchgeführt. Ergebnis-Matrix hat " << resultMatrix.getRowCount() << " Zeilen und " << resultMatrix.getColumnCount() << " Spalten." << std::endl;

        if (writeMatrix("matrix3.txt", resultMatrix)) {
            std::cout << "Ergebnis wurde erfolgreich in 'matrix3.txt' geschrieben und befindet sich in ...cmake-build-debug" << std::endl;
        } else {
            std::cerr << "FEHLER: Ergebnis konnte nicht in 'matrix3.txt' geschrieben werden!" << std::endl;
        }
    }
    else
    {
        std::cout << "Matrixdateien nicht gefunden oder konnten nicht gelesen werden." << std::endl;
    }

    return 0;
}
