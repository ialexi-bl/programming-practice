#include <chrono>
#include <stdexcept>

#include "lib/SparseMatrix.hpp"
#include "lib/benchmark.hpp"

static void testMatrices(const std::string &filename1,
                         const std::string &filename2,
                         const std::string &filenameExpected)
{
    std::cout << "> Reading " << filename1 << "..." << std::endl;
    SparseMatrix m1(filename1);
    std::cout << "> Reading " << filename2 << "..." << std::endl;
    SparseMatrix m2(filename2);
    std::cout << "> Reading " << filenameExpected << "..." << std::endl;
    SparseMatrix expected(filenameExpected);

    std::cout << "> Multiplying..." << std::endl;

    Timer timer;
    std::unique_ptr<SparseMatrix> product = m1.multiply(m2);
    int ms = timer.stop();

    if (expected != *product.get()) {
        throw std::runtime_error("Product is not equal to expected matrix");
    }

    std::cout << "Test passed, multiplied in " << ms << " ms" << std::endl;
}

int main()
{
    std::cout << "Test #1" << std::endl;
    testMatrices("./test/matrices/sparse-1.txt", "./test/matrices/sparse-2.txt",
                 "./test/matrices/sparse-12.txt");
    std::cout << "Test #2" << std::endl;
    testMatrices("./test/matrices/sparse-2.txt", "./test/matrices/sparse-3.txt",
                 "./test/matrices/sparse-23.txt");
}