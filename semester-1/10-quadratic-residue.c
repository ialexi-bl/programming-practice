#include <stdio.h>

int powMod(int x, int y, int m) {
  int result = 1;
  while (y) {
    if (y & 1) {
      result = (result * x) % m;
    }
    x = (x * x) % m;
    y >>= 1;
  }
  return result;
}

void removeTwos(int* result, int* a, int p) {
  if (!(*a & 1)) {
    int twosCount = 0;
    while (!(*a & 1)) {
      twosCount++;
      *a >>= 1;
    }

    // if twosCount and (p*p - 1)/8 are odd
    if (twosCount & 1 && (p * p - 1) & 0b1000) {
      *result *= -1;
    }
  }
}
int legendreSymbol(int a, int p) {
  if (a % p == 0) return 0;
  a %= p;

  int result = 1;
  removeTwos(&result, &a, p);

  while (a > 1) {
    // if (a - 1)/2 and (p - 1)/2 are odd
    if ((a - 1) & 0b10 && (p - 1) & 0b10) {
      result *= -1;
    }
    int t = p;
    p = a;
    a = t % p;
    removeTwos(&result, &a, p);
  }

  return result;
}

int quadResidue(int a, int p) {
  int legendre = legendreSymbol(a, p);
  if (legendre < 1) {
    return legendre;
  }

  int s = 0;
  int q = p - 1;
  while (!(q & 1)) {
    q >>= 1;
    s++;
  }

  int nonResidue;
  for (int i = 0; i <= ){} }

int main() {
  int a, p;
  printf("Input a and p (prime) in x²≡a(mod p):\n");
  scanf("%d %d", &a, &p);

  int result = quadResidue(a, p);

  if (result < 0) {
    printf("No solutions\n");
  } else {
    printf("Solution: (±%d)²≡%d(mod %d)\n", result, a, p);
  }
}