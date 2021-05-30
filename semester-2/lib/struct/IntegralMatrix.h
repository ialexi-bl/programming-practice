#pragma once
#include <stdio.h>

#include "GenericMatrix.h"

#define matrixElement(matrix, row, col) \
  ((int *)matrix->content)[row * matrix->width + col]
#define createMatrix(height, width) \
  createGenericMatrix(height, width, sizeof(int))
#define readMatrix(filename) readGenericMatrix(filename, sizeof(int), "%d")
#define printMatrix(matrix) printGenericMatrix(matrix, printIntegralMatrixCell)

void printIntegralMatrixCell(void *value);
long long computeDeterminant(const Matrix *matrix);