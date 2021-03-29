#include "BigInt.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INT_32 4294967295
#define MOD_INT_32 4294967296

static int estimateConvertedLength(int length, int from, int to) {
  // Estimating size: if number base "from" has length n, then
  // it is less than from^(n+1) = to^(log(to)(from^(n+1))) =
  // to^((n+1)*log(to)(from)), so number base "to" has less than
  // (n+1)*ln(from)/ln(to) digits
  return ceil(((double)length + 1) * log(from) / log(to));
}
BigInt* getBigInt(const char* _number) {
  /* Converting number to binary */
  // Copying number to writable memory
  int decLength = strlen(_number);
  char* number = malloc(sizeof(char) * decLength);
  for (int i = 0; i < decLength; i++) {
    number[i] = _number[i] - '0';
  }

  // Creating array with binary representation
  int binLength = estimateConvertedLength(decLength, 10, 2);
  char* binary = calloc(binLength, sizeof(char));

  int pointer = 0;
  int nonZeroDigits;

  // If some digits are not zero
  do {
    nonZeroDigits = 0;
    binary[pointer++] = number[decLength - 1] & 1;

    // Dividing number by two
    int carry = 0;
    for (int i = 0; i < decLength; i++) {
      int n = number[i] + carry;
      number[i] = n / 2;
      carry = n & 1 ? 10 : 0;

      if (number[i] > 0) nonZeroDigits++;
    }
  } while (nonZeroDigits);

  free(number);
  // Ignoring unsignificant zeros
  for (int i = binLength - 1; i >= 0; i--) {
    if (binary[i]) {
      break;
    } else {
      binLength--;
    }
  }

  /* Creating BigInt */
  BigInt* result = malloc(sizeof(BigInt));

  int intSize = sizeof(uint32_t) * 8;
  unsigned int length = ceil((float)binLength / (float)intSize);
  result->length = length;
  result->number = malloc(length * sizeof(uint32_t));

  for (int i = 0; i < length; i++) {
    int offset = i * intSize;
    int block = 0;
    for (int j = 0; j < intSize && offset + j < binLength; j++) {
      block += (binary[offset + j] << j);
    }
    result->number[i] = block;
  }

  free(binary);
  return result;
}
BigInt* getEmptyBigInt(int length) {
  BigInt* result = malloc(sizeof(BigInt));
  result->length = length;
  result->number = calloc(length, sizeof(uint32_t));
  return result;
}
char* bigIntTostring(const BigInt* bigInt) {
  if (bigInt->length == 1 && !bigInt->number[0]) {
    char* str = malloc(2 * sizeof(char));
    str[0] = '0';
    str[1] = 0;
    return str;
  }

  // Converting from binary back to decimal
  int binLength = bigInt->length * sizeof(uint32_t) * 8;
  int decLength = estimateConvertedLength(binLength, 2, 10);
  int digits[decLength];
  for (int i = 0; i < decLength - 1; i++) {
    digits[i] = 0;
  }
  digits[decLength - 1] = 1;

  int p1, p2;
  for (int i = bigInt->length - 1; i >= 0; i--) {
    for (int j = 31; j >= 0; j--) {
      if (bigInt->number[i] & (1 << j)) {
        p1 = j > 0 ? i : i - 1;
        p2 = j > 0 ? j - 1 : 31;

        goto foundStart;
      }
    }
  }
foundStart:

  for (int i = p1; i >= 0; i--) {
    for (int j = p2; j >= 0; j--) {
      // Multiplying by two
      int carry = 0;
      for (int k = decLength - 1; k >= 0; k--) {
        int newDigit = digits[k] * 2 + carry;
        digits[k] = newDigit % 10;
        carry = newDigit >= 10 ? 1 : 0;
      }

      if (bigInt->number[i] & (1 << j)) {
        digits[decLength - 1] += 1;
      }
    }
    p2 = 31;
  }

  int offset = 0;
  while (!digits[offset]) offset++;

  char* result = malloc((decLength - offset + 1) * sizeof(char));
  for (int i = 0; i < decLength - offset; i++) {
    result[i] = digits[offset + i] + '0';
  }
  result[decLength - offset] = '\0';

  return result;
}

static void freeBigInt(BigInt* n) {
  free(n->number);
  free(n);
}
static void normalizeBigInt(BigInt* n) {
  int p = n->length - 1;
  while (!n->number[p] && p > 1) p--;
  if (p < n->length - 1) {
    n->length = p;
    n->number = realloc(n->number, n->length * sizeof(uint32_t));
  }
}
void assign(BigInt* x, const BigInt* y) {
  free(x->number);
  if (y->length != x->length) {
    x->length = y->length;
    x->number = realloc(x->number, x->length * sizeof(uint32_t));
  }
  memcpy(x->number, y->number, x->length * sizeof(uint32_t));
}

BigInt* addBigInts(const BigInt* n1, const BigInt* n2) {
  BigInt* result = getEmptyBigInt(fmax(n1->length, n2->length) + 1);

  char carry = 0;
  for (int i = 0; i < result->length; i++) {
    uint32_t val1 = i < n1->length ? n1->number[i] : 0;
    uint32_t val2 = i < n2->length ? n2->number[i] : 0;
    long sum = (long)val1 + (long)val2 + carry;

    if (sum >> 32) {
      result->number[i] = (int)sum;
      carry = 1;
    } else {
      result->number[i] = (int)sum;
      carry = 0;
    }
  }

  if (carry) {
    result->number[result->length - 1] = carry;
  } else if (!result->number[result->length - 1]) {
    result->length--;
    result->number = realloc(result->number, result->length * sizeof(uint32_t));
  }

  return result;
}
BigInt* shiftRightBigInt(const BigInt* n, int c) {
  int insignificant = 32;
  for (int i = 31; i >= 0; i--) {
    if (n->number[n->length - 1] & (1 << i)) {
      insignificant = 31 - i;
      break;
    }
  }

  BigInt* result = malloc(sizeof(BigInt));
  result->length = n->length;
  if (insignificant < c) {
    result->length += ceil((c - insignificant) / 32.0);
  }
  result->number = calloc(result->length, sizeof(uint32_t));

  int move = c / 32;
  c %= 32;
  for (int i = n->length - 1; i >= 0; i--) {
    result->number[i + move] = n->number[i];
  }

  long carry = 0;
  for (int i = 0; i < n->length; i++) {
    long res = ((long)result->number[i + move] << (long)c) + carry;
    result->number[i + move] = res % MOD_INT_32;
    carry = res / MOD_INT_32;
  }
  if (carry) {
    result->number[result->length - 1] = carry;
  }

  return result;
}
BigInt* multiplyBigInts(const BigInt* n1, const BigInt* n2) {
  BigInt* result = malloc(sizeof(BigInt));
  result->length = n1->length + n2->length;
  result->number = malloc(result->length * sizeof(uint32_t));

  for (int i = 0; i < n2->length; i++) {
    for (int j = 0; j < 32; j++) {
      if (n2->number[i] & (1 << j)) {
        BigInt* shifted = shiftRightBigInt(n1, i * 32 + j);
        BigInt* sum = addBigInts(result, shifted);

        free(result->number);
        freeBigInt(shifted);

        result->length = sum->length;
        result->number = sum->number;
      }
    }
  }
  normalizeBigInt(result);
  return result;
}
BigInt* subtractBigInts(const BigInt* n1, const BigInt* n2) {
  // Checking that first number is greater than second
  if (n1->length < n2->length) return getEmptyBigInt(1);
  if (n1->length == n2->length) {
    for (int i = n1->length - 1; i >= 0; i--) {
      if (n1->number[i] > n2->number[i]) {
        break;
      }
      if (n1->number[i] < n2->number[i]) {
        return getEmptyBigInt(1);
      }
    }
  }

  BigInt* result = getEmptyBigInt(n1->length);
  for (int i = 0; i < result->length; i++) {
    long subtrahend = n1->number[i];
    int minuend = i < n2->length ? n2->number[i] : 0;

    if (subtrahend < minuend) {
      subtrahend += 1l >> 32;
      n1->number[i + 1]--;
    }
    result->number[i] = subtrahend - minuend;
  }

  normalizeBigInt(result);
  return result;
}

void printBigIntBinary(const BigInt* n) {
  for (int i = n->length - 1; i >= 0; i--) {
    for (int j = 31; j >= 0; j--) {
      printf("%d", n->number[i] & (1 << j) ? 1 : 0);
    }
    printf("\n");
  }
}
void printBigInt(const BigInt* n) {
  char* string = bigIntTostring(n);
  printf("%s", string);
  free(string);
}