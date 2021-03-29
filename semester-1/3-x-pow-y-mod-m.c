#include <stdio.h>

int powMod(int x, int y, int m) {
  int result = x % m;
  while (y > 1) {
    printf("result: %d, y: %d\n", result, y);

    if (y & 1) {
      result = (result * result * x) % m;
    } else {
      result = (result * result) % m;
    }
    y >>= 1;
  }

  return result;
}

int main() {
  int x, y, m;
  printf("Input x, y and m in \"x^y mod m\":\n");
  scanf("%d %d %d", &x, &y, &m);

  printf("%d\n", powMod(x, y, m));
}
