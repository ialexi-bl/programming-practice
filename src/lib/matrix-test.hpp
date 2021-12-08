#pragma once
#include "Matrix.hpp"
#include "benchmark.hpp"
#include <iostream>
#include <stdexcept>
#include <type_traits>

template <class M>
concept IsMatrix = std::is_base_of<Matrix, M>::value;

template <class M1, class M2, class MExpected, bool MultiThread = false>
requires IsMatrix<M1> && IsMatrix<M2> && IsMatrix<MExpected>
void testMultiplication(const std::string &filename1,
                        const std::string &filename2,
                        const std::string &filenameExpected)
{
    std::cout << "> Reading " << filename1 << "..." << std::endl;
    M1 m1(filename1);
    std::cout << "> Reading " << filename2 << "..." << std::endl;
    M2 m2(filename2);
    std::cout << "> Reading " << filenameExpected << "..." << std::endl;
    MExpected expected(filenameExpected);

    std::cout << "> Multiplying..." << std::endl;

    Timer timer;
    std::unique_ptr<Matrix> product =
        MultiThread ? m1.dmultiply(m2) : m1.multiply(m2);
    int ms = timer.stop();

    if (expected != *product.get()) {
        throw std::runtime_error("Product is not equal to expected matrix");
    }

    std::cout << "Test passed, multiplied in " << ms << " ms" << std::endl;
}