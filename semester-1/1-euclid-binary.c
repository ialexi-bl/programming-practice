#include <limits.h>
#include <stdio.h>
#define COUNT 2

int euclidBinary(int x, int y) {
  if (x == 0) return y;
  if (y == 0) return x;

  int result = 0;

  while (x != 0 && y != 0) {
    if (!(x & 1)) {
      if (!(y & 1)) {
        result++;
        x >>= 1;
        y >>= 1;
      } else {
        while (!(x & 1)) x >>= 1;
      }
    } else if (!(y & 1)) {
      while (!(y & 1)) y >>= 1;
    } else if (y > x) {
      y = y - x;
    } else {
      x = x - y;
    }

    // a  = q1b + r1
    // b  = r1q1 + r2
    // q1 = r2q2 + r3
  }

  if (x != 0) {
    return x << result;
  } else {
    return y << result;
  }

  /*
    if (x & 1) {
      if (y & 1) {
        if (x > y) {
          return euclidBinary((x - y) >> 1, y);
        }
        return euclidBinary((y - x) >> 1, x);
      }
      return euclidBinary(x, y >> 1);
    }
    if (y & 1) {
      return euclidBinary(y, x >> 1);
    }
    return euclidBinary(y >> 1, x >> 1) << 1; */
}
// int euclid(int x, int y) {
//   // Одно Евклидово деление
//   if (x > y) return euclidBinary(x - y, y);
//   return euclidBinary(y - x, x);
// }

int main() {
  // int a[COUNT];
  int x, y;
  scanf("%d %d", &x, &y);
  // for (int i = 0; i < COUNT; i++) {
  //   printf("%d\n", i);
  //   printf("Input number #%d:\n", i);
  //   scanf("%d", &a[i]);
  // }

  // printf("abc");

  int gcd = euclidBinary(x, y);
  // for (int i = 1; i < COUNT; i++) {
  //   if (gcd == 0 && a[i] == 0) {
  //     gcd = INT_MAX;
  //     break;
  //   }
  //   gcd = euclidBinary(gcd, a[i]);
  // }

  printf("%d\n", gcd);
}