#pragma once
#include <stdint.h>

typedef struct BigInt {
  unsigned int length;
  uint32_t *number;
} BigInt;

BigInt *getBigInt(const char *number);
BigInt *getEmptyBigInt(int length);
void assign(BigInt *n1, const BigInt *n2);

BigInt *addBigInts(const BigInt *n1, const BigInt *n2);
BigInt *shiftRightBigInt(const BigInt *n, int amount);
BigInt *multiplyBigInts(const BigInt *n1, const BigInt *n2);
BigInt *subtractBigInts(const BigInt *subtrahend, const BigInt *minuend);

char *bigIntToString(const BigInt *number);
void printBigIntBinary(const BigInt *number);
void printBigInt(const BigInt *number);
