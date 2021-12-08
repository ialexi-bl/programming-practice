#include <stdexcept>

#include "lib/DenseMatrix.hpp"
#include "lib/benchmark.hpp"
#include "lib/matrix-test.hpp"

int main()
{
    std::cout << "Test #1" << std::endl;
    testMultiplication<DenseMatrix, DenseMatrix, DenseMatrix>(
        "./test/matrices/dense-1.txt", "./test/matrices/dense-2.txt",
        "./test/matrices/dense-12.txt");

    std::cout << "Test #2" << std::endl;
    testMultiplication<DenseMatrix, DenseMatrix, DenseMatrix>(
        "./test/matrices/dense-2.txt", "./test/matrices/dense-3.txt",
        "./test/matrices/dense-23.txt");
}