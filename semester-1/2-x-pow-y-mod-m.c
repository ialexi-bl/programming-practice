#include <stdio.h>

int powMod(int x, int y, int m) {
  if (y > 1) {
    int halfPower = powMod(x, y >> 1, m);

    if (y & 1) {
      return (x * halfPower * halfPower) % m;
    } else {
      return (halfPower * halfPower) % m;
    }
  } else {
    return x % m;
  }
}

int main() {
  int x, y, m;
  printf("Input x, y and m in \"x^y mod m\":");
  scanf("%d %d %d", &x, &y, &m);

  printf("%d\n", powMod(x, y, m));
}
