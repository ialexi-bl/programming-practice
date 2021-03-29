#include <stdio.h>
#include <string.h>
#include <time.h>

// Exponentiation solution
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
int legendreSymbolExp(int a, int p) {
  if (a % p == 0) return 0;

  int result = powMod(a % p, (p - 1) >> 1, p);
  return result > 1 ? result - p : result;
}

// Quadratic reciprocity law solution
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

void printLegendre(int a, int p) {
  printf("Legendre symbol is %d\n", legendreSymbol(a, p));
}
void benchmarkLegendre(int a, int p) {
  float start, end;

  start = (float)clock() / CLOCKS_PER_SEC;
  for (int i = 0; i < 3e7; i++) {
    legendreSymbolExp(a, p);
  }
  end = (float)clock() / CLOCKS_PER_SEC;
  printf("legendreExp took %f seconds\n", end - start);

  start = (float)clock() / CLOCKS_PER_SEC;
  for (int i = 0; i < 3e7; i++) {
    legendreSymbol(a, p);
  }
  end = (float)clock() / CLOCKS_PER_SEC;
  printf("legendreQuad took %f seconds\n", end - start);
}

int main(int argc, char* argv[]) {
  int a, p;
  printf("Input any number and a prime number: ");
  scanf("%d %d", &a, &p);

  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "--benchmark") == 0) {
      benchmarkLegendre(a, p);
      return 0;
    }
  }

  printLegendre(a, p);
}