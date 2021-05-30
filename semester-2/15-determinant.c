#include <stdio.h>

#include "lib/struct/IntegralMatrix.h"

int main() {
  Matrix *matrix = readMatrix("matrix.txt");
  long long determinant = computeDeterminant(matrix);
  printf("\n- Determinant is equal to %lld\n\n", determinant);
}