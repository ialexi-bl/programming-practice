#include <limits.h>
#include <stdio.h>

#define COUNT 2

int euclid(int x, int y) {
  if (x == 0 && y == 0) return INT_MAX;

  if (x < 0) x = -x;
  if (y < 0) y = -y;

  while (x != y) {
    if (x == 0) return y;
    if (y == 0) return x;

    if (x > y) {
      x = x % y;
    } else {
      y = y % x;
    }
  }

  return x;
}

int main() {
  int a[COUNT];
  for (int i = 0; i < COUNT; i++) {
    printf("Input number #%d:\n", i);
    scanf("%d", &a[i]);
  }

  int gcd = a[0];
  for (int i = 1; i < COUNT; i++) {
    gcd = euclid(gcd, a[i]);
  }

  printf("%d\n", gcd);
}