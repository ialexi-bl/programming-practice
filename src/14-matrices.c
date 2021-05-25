#include <stdio.h>

#include "lib/struct/IntegralMatrix.h"

int main() {
  Matrix *matrix = readMatrix("matrix.txt");
  printMatrix(matrix);
}