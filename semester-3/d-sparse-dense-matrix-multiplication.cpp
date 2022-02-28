#include <stdexcept>

#include "lib/DenseMatrix.hpp"
#include "lib/SparseMatrix.hpp"
#include "lib/matrix-test.hpp"

int main()
{
    std::cout << "Test #1: Sparse x Dense" << std::endl;
    testMultiplication<SparseMatrix, DenseMatrix, DenseMatrix>(
        "./test/matrices/sd-sparse.txt", "./test/matrices/sd-dense.txt",
        "./test/matrices/sd-result.txt");

    std::cout << "Test #2: Dense x Sparse" << std::endl;
    testMultiplication<DenseMatrix, SparseMatrix, DenseMatrix, true>(
        "./test/matrices/ds-dense.txt", "./test/matrices/ds-sparse.txt",
        "./test/matrices/ds-result.txt");
}