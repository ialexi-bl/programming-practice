#include "lib/matrix.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    math::Matrix matrix(3, 4, {{1, 4, 7, 8}, {2, 5, 10, 9}, {3, 6, 15, 20}});
    // math::Matrix matrix(2, 3, {5, 7, -2, 6, 16, -4});
    std::cout << matrix << std::endl;

    std::vector<math::matrix_value> result = matrix.solve();

    for (int i = 0; i < result.size(); i++) {
        std::cout << "Solution: " << (i > 0 ? ", " : "") << result[i];
    }
    std::cout << std::endl << std::endl;

    math::Matrix inverse = matrix.fromColumns(0, 3).invert();
    std::cout << inverse << std::endl;

    return 0;
}
