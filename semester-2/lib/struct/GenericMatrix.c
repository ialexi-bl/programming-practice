#include "GenericMatrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Matrix *createGenericMatrix(int height, int width, size_t cellSize) {
  Matrix *matrix = malloc(sizeof(Matrix));
  matrix->width = width;
  matrix->height = height;
  matrix->cellSize = cellSize;
  matrix->content = calloc(width * height, cellSize);
  return matrix;
}

Matrix *readGenericMatrix(const char *filename, size_t cellSize,
                          const char *cellFormat) {
  FILE *file = fopen(filename, "r");

  int height, width;
  int read = fscanf(file, "%d %d", &height, &width);

  if (read < 2) {
    return NULL;
  }

  Matrix *matrix = createGenericMatrix(height, width, cellSize);
  void *value = malloc(cellSize);
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      read = fscanf(file, cellFormat, value);
      if (!read) {
        freeMatrix(matrix);
        return NULL;
      }

      memcpy(matrix->content + (row * matrix->width + col) * matrix->cellSize,
             value, matrix->cellSize);
    }
  }

  return matrix;
}

Matrix *copyMatrix(const Matrix *matrix) {
  Matrix *newMatrix =
      createGenericMatrix(matrix->width, matrix->height, matrix->cellSize);
  memcpy(newMatrix->content, matrix->content,
         matrix->width * matrix->height * matrix->cellSize);
  return newMatrix;
}

void printGenericMatrix(Matrix *matrix, void (*printCell)(void *)) {
  for (int row = 0; row < matrix->height; row++) {
    for (int col = 0; col < matrix->width; col++) {
      printCell(matrix->content +
                (row * matrix->width + col) * matrix->cellSize);
      printf(" ");
    }
    printf("\n");
  }
}

void freeMatrix(Matrix *matrix) {
  free(matrix->content);
  free(matrix);
}