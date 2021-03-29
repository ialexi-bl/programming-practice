#pragma once

extern char complexOperators[][7];

/* Enums */

typedef enum TokenType {
  TT_START,
  TT_EMPTY,
  TT_NUMBER,
  TT_PUNCTUATION,
  TT_SIMPLE_OPERATOR,
  TT_COMPLEX_OPERATOR,
} TokenType;

typedef enum ComplexOperatorType {
  COP_GCD = 0,
  COP_LCM = 1,
} ComplexOperatorType;

/* Structs */

typedef struct NumberToken {
  int whole, decimal, isFraction;
} NumberToken;

typedef struct ComplexOperatorToken {
  int options[10], current;
} ComplexOperatorToken;

typedef struct MathToken {
  struct MathToken *next;
  enum TokenType type;
  int position;

  union {
    char symbol;
    void *data;
  };
} MathToken;

MathToken *tokenizeMath(char string[], char *error);

MathToken *getStartToken();
MathToken *getEmptyToken(int position);
MathToken *getPunctuationToken(int position, char symbol);
MathToken *getSimpleOperatorToken(int position, char operator);
MathToken *getComplexOperatortoken(int position, ComplexOperatorToken *cotoken);
MathToken *getNumberToken(int position, char symbol);

void filterOptions(int *options, char c, int position);
void freeTokenList(MathToken *start);