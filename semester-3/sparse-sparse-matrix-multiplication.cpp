#include <chrono>
#include <stdexcept>

#include "lib/SparseMatrix.hpp"
#include "lib/matrix-test.hpp"

int main()
{
    std::cout << "Test #1" << std::endl;
    testMultiplication<SparseMatrix, SparseMatrix, SparseMatrix>(
        "./test/matrices/sparse-1.txt",
        "./test/matrices/sparse-2.txt",
        "./test/matrices/sparse-12.txt"
    );

    std::cout << "Test #2" << std::endl;
    testMultiplication<SparseMatrix, SparseMatrix, SparseMatrix>(
        "./test/matrices/sparse-2.txt",
        "./test/matrices/sparse-3.txt",
        "./test/matrices/sparse-23.txt"
    );
}