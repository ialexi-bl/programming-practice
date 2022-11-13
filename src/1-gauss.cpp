#include "lib/matrix.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    math::Matrix matrix(3, 3, {{1, 0, 3}, {4, 6, 1}, {2, 3, 5}});

    // auto inverse = matrix.invert();
    // std::cout << inverse << std::endl;

    // auto product = matrix * inverse;
    // std::cout << product << std::endl;

    // auto lu = matrix.decomposeLU();
    // std::cout << lu.first << std::endl;
    // std::cout << lu.second << std::endl;
    // std::cout << (lu.first * lu.second) << std::endl;

    std::vector<math::matrix_value> result2 = math::solveLinearSystemUsingLU(matrix, {17, 24, 28});

    // std::cout << result2[0] << std::endl;
    // std::cout << "Solution: ";
    // for (int i = 0; i < result2.size(); i++) {
    //     std::cout << (i > 0 ? ", " : "") << result2[i];
    // }
    // std::cout << std::endl << std::endl;

    // math::Matrix v(3, 1, {0.833333, 1.83333, 4.16667});
    // std::cout << matrix * v << std::endl;

    return 0;
}

// std::cout << matrix << std::endl;

// auto result = math::solveLinearSystem(matrix, {8, 9, 20});

// std::cout << "Solution: ";
// for (int i = 0; i < result.size(); i++) {
//     std::cout << (i > 0 ? ", " : "") << result[i];
// }
// std::cout << std::endl << std::endl;

// math::Matrix inverse = matrix.fromColumns(0, 3).invert();
// std::cout << inverse << std::endl;

// auto lu = matrix.decomposeLU();
// std::cout << "LU:" << std::endl;
// std::cout << lu.first << std::endl;
// std::cout << lu.second << std::endl;

// std::cout << result2[0] << std::endl;
// std::cout << "Solution: ";
// for (int i = 0; i < result2.size(); i++) {
//     std::cout << (i > 0 ? ", " : "") << result2[i];
// }
// std::cout << std::endl << std::endl