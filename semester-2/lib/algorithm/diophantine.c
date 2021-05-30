#include "diophantine.h"

#define abs(a) (a < 0 ? -a : a)

ll solveDiophantineSimple(ll a, ll b, ll c, ll* x, ll* y) {
  ll m[4] = {1, 0, 0, 1};
  ll q, r, k = 0;

  while (b != 0) {
    k++;
    q = a / b;
    r = a % b;

    ll t = m[0];
    m[0] = m[0] * q + m[1];
    m[1] = t;
    t = m[2];
    m[2] = m[2] * q + m[3];
    m[3] = t;

    a = b;
    b = r;
  }

  if (c % a != 0) {
    return 0;
  }
  ll coef = c / a;

  if (x) *x = m[3] * (k & 1 ? -1 : 1) * coef;
  if (y) *y = m[1] * (k & 1 ? 1 : -1) * coef;
  return abs(a);
}

static ll extendedEuclid(ll a, ll b, ll* x, ll* y) {
  ll m[4] = {1, 0, 0, 1};
  ll q, r, k = 0;

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

ll solveDiophantine(int n, const ll* in, ll* out, ll c) {
  ll gcd, x, y;

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
    return 0;
  }

  ll k = c / gcd;
  for (int i = 0; i < n; i++) {
    out[i] *= k;
  }
  return 1;
}