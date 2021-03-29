#include <stdio.h>
#define ull unsigned long long

void multiplyMatrix(int* m1, int* m2, int m) {
  int a11 = m1[0] * m2[0] + m1[1] * m2[2];
  int a12 = m1[0] * m2[1] + m1[1] * m2[3];
  int a21 = m1[0] * m2[2] + m1[2] * m2[3];
  int a22 = m1[1] * m2[2] + m1[3] * m2[3];

  m1[0] = a11 % m;
  m1[1] = a12 % m;
  m1[2] = a21 % m;
  m1[3] = a22 % m;
}

int fibonacci(ull n, int m) {
  int mtrx[4] = {1, 1, 1, 0};
  int res[4] = {1, 0, 0, 1};
  n -= 1;

  while (n) {
    if (n & 1) {
      multiplyMatrix(res, mtrx, m);
    }

    multiplyMatrix(mtrx, mtrx, m);
    n >>= 1;
  }

  return res[0] % m;
}

void main() {
  ull n;
  int m;
  printf("Input n and m:\n");
  scanf("%lld %d", &n, &m);

  int result = fibonacci(n, m);
  printf("%d\n", result);
}
