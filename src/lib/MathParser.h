#pragma once
#include "MathTokenizer.h"

/* Enums */

typedef enum OperationType {
  OP_GCD,
  OP_LCM,
  OP_PLUS,
  OP_MINUS,
  OP_START,
  OP_POWER,
  OP_DIVIDE,
  OP_UNKNOWN,
  OP_MULTIPLY,
} OperationType;

typedef enum OperandType {
  ET_EMPTY,
  ET_NUMBER,
  ET_EXPRESSION,
} OperandType;

typedef enum ParserMode {
  PM_DEFAULT,
  PM_ARGUMENT,
  PM_NESTED,
} ParserMode;

/* Structs */

typedef struct MathOperand {
  enum OperandType type;

  union {
    double number;
    struct MathOperatorNode *expression;
  };
} MathOperand;

typedef struct MathOperatorNode {
  int isComplex;

  struct MathOperatorNode *parent;
  int position;

  enum OperationType operation;
  int operandsAmount;

  MathOperand operands[];
} MathOperatorNode;

/* Functions */

MathOperatorNode *parseMath(char string[], char *error);
MathOperatorNode *getOperatorNode(int operandsAmount, int isComplex);
MathOperatorNode *getStartNode();
OperationType getOperationType(MathToken *token);
OperationType getComplexOperationType(ComplexOperatorToken *token);
void printOperation(OperationType type);
int sprintOperation(char *dest, OperationType type);
int getPriority(MathOperatorNode *node);
int getLastEmptyOperand(MathOperatorNode *node, MathOperand **operand);
void freeMathTree(MathOperatorNode *start);
void printTokensList(MathToken *start);
void printMathTree(MathOperatorNode *start);
void printMathWithParentheses(MathOperatorNode *start);