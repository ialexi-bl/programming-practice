#include "MathParser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errorio.h"

#define COMPLEX_ARGUMENTS_COUNT 2
#define assignNonNull(variable, value) \
  if (variable) *variable = value

static MathOperatorNode *_parseMath(MathToken *current, ParserMode mode,
                                    MathToken **expressionEnd, char string[],
                                    char *error) {
  MathOperatorNode *root = getStartNode(), *currNode = root;

  int used = 1;
  MathOperand lastOperand;

  do {
    switch (current->type) {
      case TT_START:
      case TT_EMPTY:
        break;
      case TT_NUMBER: {
        NumberToken *ntoken = current->data;

        if (!used) {
          sprintfError(error, string, current->position,
                       "ParseError: unexpected number");
          freeMathTree(root);
          return NULL;
        }

        double value = ntoken->decimal;
        while (value >= 1) value /= 10;
        value += ntoken->whole;

        lastOperand.type = ET_NUMBER;
        lastOperand.number = value;
        used = 0;
        break;
      }
      case TT_SIMPLE_OPERATOR: {
        // If no first operand exists
        if (used) {
          sprintfError(error, string, current->position,
                       "ParseError: unexpected operator \"%c\"",
                       current->symbol);
          freeMathTree(root);
          return NULL;
        }

        MathOperatorNode *opnode = getOperatorNode(2, 0);
        opnode->operation = getOperationType(current);

        int priority = getPriority(opnode);

        if (priority > getPriority(currNode)) {
          MathOperand *wrapperOperand = NULL;
          int position = getLastEmptyOperand(currNode, &wrapperOperand);

          if (!wrapperOperand) {
            sprintfError(error, string, current->position,
                         "ParseError: unexpected operator \"%c\"",
                         current->symbol);
            freeMathTree(root);
            free(opnode);
            return NULL;
          }

          wrapperOperand->type = ET_EXPRESSION;
          wrapperOperand->expression = opnode;
          opnode->parent = currNode;
          opnode->position = position;

          opnode->operands[0] = lastOperand;
        } else {
          MathOperand *operand;

          // Completing operands of currNode
          getLastEmptyOperand(currNode, &operand);
          *operand = lastOperand;

          MathOperatorNode *parent = currNode->parent;
          while (getPriority(parent) >= priority) {
            currNode = parent;
            parent = parent->parent;
          }

          // Substituting opnode for currNode in parent's operands
          operand = &parent->operands[currNode->position];
          operand->expression = opnode;

          opnode->operands[0].type = ET_EXPRESSION;
          opnode->operands[0].expression = currNode;

          opnode->parent = parent;
          currNode->parent = opnode;
        }
        currNode = opnode;
        used = 1;
        break;
      }
      case TT_COMPLEX_OPERATOR: {
        if (!used) {
          sprintfError(error, string, current->position,
                       "ParseError: unexpected operator");
          freeMathTree(root);
          return NULL;
        }

        ComplexOperatorToken *cotoken = current->data;
        MathOperatorNode *opnode = getOperatorNode(COMPLEX_ARGUMENTS_COUNT, 1);
        opnode->operation = getComplexOperationType(cotoken);

        if (!current->next || current->next->symbol != '(') {
          sprintfError(
              error, string, current->position,
              "ParseError: expected argument list after operator \"%s\"",
              complexOperators[cotoken->options[0]]);
          freeMathTree(root);
          free(opnode);
          return NULL;
        }
        current = current->next;

        for (int i = 0; i < COMPLEX_ARGUMENTS_COUNT; i++) {
          MathOperatorNode *argument =
              _parseMath(current->next, PM_ARGUMENT, &current, string, error);
          if (!argument) {
            freeMathTree(root);
            return NULL;
          }

          if (i < COMPLEX_ARGUMENTS_COUNT - 1 && current->symbol == ')') {
            sprintfError(error, string, current->position,
                         "ParseError: not enough arguments for operator \"%s\" "
                         "(expected %d, got %d)",
                         complexOperators[cotoken->options[0]],
                         COMPLEX_ARGUMENTS_COUNT, i);
            freeMathTree(root);
            return NULL;
          }
          if (i == COMPLEX_ARGUMENTS_COUNT - 1 && current->symbol != ')') {
            sprintfError(error, string, current->position,
                         "ParseError: too many arguments for operator \"%s\" "
                         "(expected %d",
                         complexOperators[cotoken->options[0]],
                         COMPLEX_ARGUMENTS_COUNT);
            freeMathTree(root);
            return NULL;
          }

          opnode->operands[i].type = ET_EXPRESSION;
          opnode->operands[i].expression = argument;
        }

        used = 0;
        lastOperand.type = ET_EXPRESSION;
        lastOperand.expression = opnode;
        break;
      }
      case TT_PUNCTUATION: {
        switch (current->symbol) {
          case '(': {
            if (!used) {
              sprintfError(error, string, current->position,
                           "ParseError: unexpected nested expression");
              freeMathTree(root);
              return NULL;
            }

            MathToken *nestedEnd;
            MathOperatorNode *nestedRoot =
                _parseMath(current->next, PM_NESTED, &nestedEnd, string, error);
            if (!nestedRoot) {
              freeMathTree(root);
              return NULL;
            }

            current = nestedEnd;
            lastOperand.type = ET_EXPRESSION;
            lastOperand.expression = nestedRoot;
            used = 0;
            break;
          }
          case ')': {
            if (mode == PM_DEFAULT) {
              sprintfError(error, string, current->position,
                           "ParseError: unmatched parenthesis");
              freeMathTree(root);
              return NULL;
            }

            assignNonNull(expressionEnd, current);
            // Break from do...while loop
            goto stop;
          }
          case ',': {
            if (mode != PM_ARGUMENT) {
              sprintfError(error, string, current->position,
                           "ParseError: unexpected separator");
              freeMathTree(root);
              return NULL;
            }

            assignNonNull(expressionEnd, current);
            // Break from do...while loop
            goto stop;
          }
        }
        break;
      }
    }
  } while ((current = current->next));
stop:;

  MathOperand *operand;
  while (getLastEmptyOperand(currNode, &operand), operand) {
    if (!used) {
      *operand = lastOperand;
    } else {
      sprintfError(error, string, strlen(string) - 1,
                   "ParseError: missing operand");
      freeMathTree(root);
      return NULL;
    }
  }
  return root;
}

MathOperatorNode *parseMath(char string[], char *error) {
  MathToken *start = tokenizeMath(string, error);
  if (!start) return NULL;

  MathOperatorNode *result = _parseMath(start, PM_DEFAULT, NULL, string, error);
  freeTokenList(start);
  return result;
}

MathOperatorNode *getOperatorNode(int operandsAmount, int isComplex) {
  MathOperatorNode *node =
      malloc(sizeof(MathOperatorNode) + operandsAmount * sizeof(MathOperand));
  node->operandsAmount = operandsAmount;
  node->isComplex = isComplex;

  for (int i = 0; i < operandsAmount; i++) {
    node->operands[i].type = ET_EMPTY;
  }

  return node;
}

MathOperatorNode *getStartNode() {
  MathOperatorNode *node = getOperatorNode(1, 0);
  node->operation = OP_START;
  node->position = 0;
  return node;
}

void freeMathTree(MathOperatorNode *start) {
  for (int i = 0; i < start->operandsAmount; i++) {
    if (start->operands[i].type == ET_EXPRESSION) {
      freeMathTree(start->operands[i].expression);
    }
  }
  free(start);
}

OperationType getOperationType(MathToken *token) {
  switch (token->symbol) {
    case '+':
      return OP_PLUS;
    case '-':
      return OP_MINUS;
    case '/':
      return OP_DIVIDE;
    case '*':
      return OP_MULTIPLY;
    case '^':
      return OP_POWER;
    default:
      assert(0);
  }
}
OperationType getComplexOperationType(ComplexOperatorToken *token) {
  switch (token->options[0]) {
    case COP_GCD:
      return OP_GCD;
    case COP_LCM:
      return OP_LCM;
    default:
      assert(0);
  }
}

int sprintOperation(char *dest, OperationType type) {
  switch (type) {
    case OP_PLUS:
      return sprintf(dest, "%c", '+');
    case OP_MINUS:
      return sprintf(dest, "%c", '-');
    case OP_DIVIDE:
      return sprintf(dest, "%c", '/');
    case OP_MULTIPLY:
      return sprintf(dest, "%c", '*');
    case OP_POWER:
      return sprintf(dest, "%c", '^');
    case OP_GCD:
      return sprintf(dest, "gcd");
    case OP_LCM:
      return sprintf(dest, "lcm");

    default:
      assert(0);
  }
}
void printOperation(OperationType type) {
  char symbol[8];
  sprintOperation(symbol, type);
  printf("%s", symbol);
}
int getPriority(MathOperatorNode *node) {
  switch (node->operation) {
    case OP_PLUS:
    case OP_MINUS:
      return 5;
    case OP_DIVIDE:
    case OP_MULTIPLY:
      return 10;
    case OP_POWER:
      return 15;
    case OP_START:
      return -1;
    default:
      assert(0);
  }
}

int getLastEmptyOperand(MathOperatorNode *node, MathOperand **operand) {
  int i;
  for (i = 0; i < node->operandsAmount; i++) {
    if (node->operands[i].type == ET_EMPTY) {
      *operand = &node->operands[i];
      return i;
    }
  }
  *operand = NULL;
  return -1;
}

static void _printMathTree(MathOperatorNode *node, int indent[32], int curr) {
  indent[curr] = 1;
  switch (node->operation) {
    case OP_START:
      printf("[START]\n");
      break;
    case OP_PLUS:
      printf("[PLUS]\n");
      break;
    case OP_MINUS:
      printf("[MINUS]\n");
      break;
    case OP_MULTIPLY:
      printf("[MULTIPLY]\n");
      break;
    case OP_DIVIDE:
      printf("[DIVIDE]\n");
      break;
    case OP_POWER:
      printf("[POWER]\n");
      break;
    case OP_GCD:
      printf("[GCD]\n");
      break;
    case OP_LCM:
      printf("[LCM]\n");
      break;
    case OP_UNKNOWN:
      printf("[UNKNOWN]");
      break;
  }
  for (int i = 0; i < node->operandsAmount; i++) {
    for (int i = 0; i < curr; i++) {
      printf(indent[i] ? "┃  " : "   ");
    }
    if (i == node->operandsAmount - 1) {
      indent[curr] = 0;
      printf("┕ ");
    } else {
      printf("┝ ");
    }
    switch (node->operands[i].type) {
      case ET_EMPTY:
        printf(" Empty\n");
        break;
      case ET_NUMBER:
        printf(" %g\n", node->operands[i].number);
        break;
      case ET_EXPRESSION:
        _printMathTree(node->operands[i].expression, indent, curr + 1);
        break;
    }
  }
}
void printMathTree(MathOperatorNode *start) {
  int indent[32];
  for (int i = 0; i < 32; i++) indent[i] = 0;
  _printMathTree(start, indent, 0);
}

static void _printMathWithParentheses(MathOperatorNode *node) {
  if (node->operation == OP_START) {
    if (node->operands[0].type == ET_NUMBER) {
      printf("%g", node->operands[0].number);
    } else {
      _printMathWithParentheses(node->operands[0].expression);
    }
    return;
  }

  if (node->isComplex) {
    printOperation(node->operation);
    printf("(");
    for (int i = 0; i < COMPLEX_ARGUMENTS_COUNT; i++) {
      if (i) printf(", ");
      _printMathWithParentheses(node->operands[i].expression);
    }
    printf(")");
  } else {
    for (int i = 0; i < 2; i++) {
      switch (node->operands[i].type) {
        case ET_EMPTY:
          printf("?");
          break;
        case ET_NUMBER:
          printf("%g", node->operands[i].number);
          break;
        case ET_EXPRESSION:
          printf("(");
          _printMathWithParentheses(node->operands[i].expression);
          printf(")");
          break;
      }
      if (!i) {
        printOperation(node->operation);
      }
    }
  }
}
void printMathWithParentheses(MathOperatorNode *start) {
  _printMathWithParentheses(start);
  printf("\n");
}