#include "Math.h"

int gcd(int x, int y) {
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
  }

  if (x != 0) {
    return x << result;
  } else {
    return y << result;
  }
}
int lcm(int x, int y) { return x * y / gcd(x, y); }