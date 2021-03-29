#include <stdio.h>

int extendedEuclid(int a, int b, int* x, int* y) {
  int m[4] = {1, 0, 0, 1};
  int q, r, k = 0;

  while (b != 0) {
    k++;
    q = a / b;
    r = a % b;

    //              (q,             1   )
    //              (1,             0   )
    // (m[0], m[1]) (m[0]*q + m[1], m[0])
    // (m[2], m[3]) (m[2]*q + m[3], m[2])

    int t = m[0];
    m[0] = m[0] * q + m[1];
    m[1] = t;
    t = m[2];
    m[2] = m[2] * q + m[3];
    m[3] = t;

    a = b;
    b = r;
  }

  *x = m[3] * (k & 1 ? -1 : 1);
  *y = m[1] * (k & 1 ? 1 : -1);
  return a;
}

// int extendedEuclidRecursive(int a, int b, int* x, int* y) {
//   if (a == 0) {
//     *x = 0;
//     *y = 1;
//   } else if (b == 0) {
//     *y = 0;
//     *x = 1;
//   }

//   int q = a / b;
//   int r = a % b;
//   int x1, y1;
//   extendedEuclidRecursive();
// }

int main() {
  int a, b;
  printf("Input two numbers:\n");
  scanf("%d %d", &a, &b);

  int x, y;
  int gcd = extendedEuclid(a, b, &x, &y);
  printf("GCD(a, b) = %d\nGCD(a, b) = a*(%d) + b*(%d)\n", gcd, x, y);
}