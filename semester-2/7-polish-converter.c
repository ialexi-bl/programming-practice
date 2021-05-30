#include <stdio.h>

#include "lib/MathParser.h"

int sprintPolishNotation(char *dest, MathOperatorNode *node) {
  char *initial = dest;

  for (int i = 0; i < node->operandsAmount; i++) {
    if (node->operands[i].type == ET_NUMBER) {
      dest += sprintf(dest, "%g ", node->operands[i].number);
    } else {
      dest += sprintPolishNotation(dest, node->operands[i].expression);
    }
  }
  if (node->operation != OP_START) {
    dest += sprintOperation(dest, node->operation) + 1;
    dest[-1] = ' ';
    dest[0] = '\0';
  }
  return dest - initial;
}

char *string = "gcd(7 - 2 * lcm(8^(9 + 1.5), 2-gcd(7, 8^2)), 123) + 9 - 8^7*8";
int main() {
  char error[128];
  MathOperatorNode *start = parseMath(string, error);

  if (!start) {
    printf("%s", error);
    return 1;
  }

  // printf("Parsing result:\n");
  // printMathTree(start);
  // printf("\n");
  printf("Expression with all parentheses: ");
  printMathWithParentheses(start);

  char polish[128];
  printf("        Reverse polish notation: ");
  sprintPolishNotation(polish, start);
  printf("%s\n", polish);
}