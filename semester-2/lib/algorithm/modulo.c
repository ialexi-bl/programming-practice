#include "modulo.h"

#include <stdlib.h>

#include "diophantine.h"

ll getModuloInverse(ll a, ll p) {
  ll x;
  solveDiophantineSimple(a, p, 1, &x, NULL);
  return x;
}

void chineseRemainder(int n, ll* rem, ll* mod, ll* resMod, ll* resRem) {
  ll m = mod[0], r = rem[0];

  for (int i = 1; i < n; i++) {
    ll x, y;
    ll gcd = solveDiophantineSimple(m, -mod[i], rem[i] - r, &x, &y);

    if (gcd == 0) {
      *resMod = *resRem = -1;
      return;
    }

    r = r + m * x;
    m *= mod[i] / gcd;

    if (r < 0) r += m * (-r / m + 1);
    if (r > m) r %= m;
  }

  if (resMod) *resMod = m;
  if (resRem) *resRem = r;
}