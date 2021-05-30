#pragma once
#include <stdlib.h>

struct Matrix {
  int width, height;
  size_t cellSize;

  void *content;
};

typedef struct Matrix Matrix;

#define createGenericSquareMatrix(size, cellSize) \
  createMatrix(size, size, cellSize)

Matrix *createGenericMatrix(int width, int height, size_t cellSize);
Matrix *readGenericMatrix(const char *filename, size_t cellSize,
                          const char *cellFormat);
Matrix *copyMatrix(const Matrix *matrix);
void printGenericMatrix(Matrix *matrix, void (*printCell)(void *));
void freeMatrix(Matrix *matrix);