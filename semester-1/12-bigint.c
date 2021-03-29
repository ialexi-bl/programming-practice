#include <stdio.h>

#include "lib/BigInt.h"

int main() {
  BigInt *num1 = getBigInt("104923874102938471203847102938478102734");
  BigInt *num2 = getBigInt("120938412984712693874126938126938162349");
  BigInt *prod = multiplyBigInts(num1, num2);

  printBigInt(num1);
  printf(" *\n");
  printBigInt(num2);
  printf(" =\n");
  printBigInt(prod);
  printf("\n");
}