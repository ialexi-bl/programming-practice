#include <stdbool.h>
#include <stdio.h>

int extendedEuclid(int a, int b, int* x, int* y) {
  int m[4] = {1, 0, 0, 1};
  int q, r, k = 0;

  while (b != 0) {
    k++;
    q = a / b;
    r = a % b;

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

bool solveDiophantine(int n, int* in, int* out, int c) {
  int gcd, x, y;

  gcd = extendedEuclid(in[0], in[1], &x, &y);
  out[0] = x;
  out[1] = y;

  for (int i = 2; i < n; i++) {
    gcd = extendedEuclid(gcd, in[i], &x, &y);

    out[i] = y;
    for (int j = 0; j < i; j++) {
      out[j] *= x;
    }
  }

  if (c % gcd != 0) {
    return false;
  }

  int k = c / gcd;
  for (int i = 0; i < n; i++) {
    out[i] *= k;
  }
  return true;
}

int main() {
  int n, c;
  printf("Input amount of variables:\n");
  scanf("%d", &n);

  int in[n], out[n];
  for (int i = 0; i < n; i++) {
    printf("Input coefficient #%d\n", i);
    scanf("%d", &in[i]);
  }

  printf("Input c:\n");
  scanf("%d", &c);

  bool a = solveDiophantine(n, in, out, c);

  if (a) {
    printf("\nSolution:\n");
    for (int i = 0; i < n; i++) {
      printf("x#%d = %d\n", i + 1, out[i]);
    }
  } else {
    printf("\nNo solution exists");
  }
}