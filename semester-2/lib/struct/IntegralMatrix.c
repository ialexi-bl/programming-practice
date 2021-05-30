#include "IntegralMatrix.h"

#include <math.h>

#include "../algorithm/modulo.h"

#define ll long long
#define PRIMES_COUNT 10
#define PRIMES \
  { 101, 863, 1747, 2833, 4297, 6217, 6577, 7919, 37199, 71993 }
#define mod(x, p) ((x % p) + p) % p

void printIntegralMatrixCell(void* value) { printf("%3d", *(int*)value); }

static long estimateDeterminant(const Matrix* matrix) {
  long estimate = 1, value;

  for (int col = 0; col < matrix->width; col++) {
    long length = 0;
    for (int row = 0; row < matrix->height; row++) {
      value = matrixElement(matrix, row, col);
      length += value * value;
    }
    estimate *= ceil(sqrt((double)length));
  }

  return estimate;
}

static int computeModuloDeterminant(const Matrix* _matrix, int p) {
  Matrix* matrix = copyMatrix(_matrix);

  ll size = matrix->width;
  for (int col = 0; col < size; col++) {
    for (int row = col + 1; row < size; row++) {
      ll coef = (ll)matrixElement(matrix, row, col) *
                getModuloInverse(matrixElement(matrix, col, col), p);

      for (int currCol = col; currCol < size; currCol++) {
        matrixElement(matrix, row, currCol) =
            mod((ll)matrixElement(matrix, row, currCol) -
                    (ll)matrixElement(matrix, col, currCol) * coef,
                (ll)p);
      }
    }
  }

  ll determinant = 1;
  for (int col = 0; col < size; col++) {
    determinant = (determinant * matrixElement(matrix, col, col)) % p;
  }
  return determinant;
}

ll computeDeterminant(const Matrix* matrix) {
  if (matrix->width != matrix->height) {
    return 0;
  }

  ll estimate = 2 * estimateDeterminant(matrix);
  ll currentEstimate = 1;
  ll primes[PRIMES_COUNT] = PRIMES;

  int i;
  ll det[PRIMES_COUNT];
  for (i = 0; currentEstimate < estimate; i++) {
    det[i] = computeModuloDeterminant(matrix, primes[i]);
    currentEstimate *= primes[i];
  }

  ll modulo, determinant;
  chineseRemainder(i, det, primes, &modulo, &determinant);
  return determinant <= modulo / 2 ? determinant : determinant - modulo;
}