#pragma once
#define max(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b;      \
  })
#define min(a, b)           \
  ({                        \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b;      \
  })
#define fit(a, min, max)               \
  ({                                   \
    __typeof__(a) _a = (a);            \
    __typeof__(min) _min;              \
    __typeof__(max) _max;              \
    _a<_min ? _min : a> _max ? max : a \
  })

int gcd(int x, int y);
int lcm(int x, int y);