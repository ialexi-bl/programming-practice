#include <stdbool.h>
#include <stdio.h>
#define ll long long

ll solveDiophantine(ll a, ll b, ll c, ll* x, ll* y) {
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

  *x = m[3] * (k & 1 ? -1 : 1) * coef;
  *y = m[1] * (k & 1 ? 1 : -1) * coef;
  return a;
}

void chineseRemainder(int n, ll* rem, ll* mod, ll* resMod, ll* resRem) {
  ll m = mod[0], r = rem[0];

  for (int i = 1; i < n; i++) {
    ll x, y;
    ll gcd = solveDiophantine(m, -mod[i], rem[i] - r, &x, &y);

    if (gcd == 0) {
      *resMod = *resRem = -1;
      return;
    }

    // x = a1 (mod m1)
    // x = a2 (mod m2)
    // a1 +m1*p = a2*m2*q
    // p =x + m2*k
    // x = a1 + m1*(x + m2*k)
    // x = a1 + m1*x + m1*m2*k

    r = r + m * x;
    m *= mod[i];

    if (r < 0) r += m * (-r / m + 1);
    if (r > m) r %= m;
  }

  *resMod = m;
  *resRem = r;
}

int main() {
  int n;
  printf("Insert amount of equations:\n");
  scanf("%d", &n);

  ll rem[n], mod[n];
  for (int i = 0; i < n; i++) {
    printf("Insert remainder and mod #%d:\n", i + 1);
    scanf("%lld %lld", &rem[i], &mod[i]);
  }

  ll m, r;
  chineseRemainder(n, rem, mod, &m, &r);
  printf("Result: x = %lld (mod %lld)\n", r, m);
}