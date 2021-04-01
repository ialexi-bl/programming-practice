#include <math.h>
#include <stdio.h>

#include "lib/Math.h"

/* Macros */

#define FRACTION -1
#define concatDigit(variable, digit) variable = variable * 10 + (digit - '0')
#define lastNodeValue(linkedList) linkedList->end->value
#define hasTwoNumbers(stack) (stack->top && stack->top->next)
#define lastNodeIs(linkedList, _type) \
  (linkedList->end && linkedList->end->type == _type)

/* Enums */

typedef enum NodeType { TT_NUMBER, OPERATOR, INCOMPLETE } NodeType;
typedef enum OperatorType {
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE,
  POWER,
  COP_GCD,
  COP_LCM
} OperatorType;

/* Data structures  */

#define LINKED_LIST_FIELDS \
  NodeType type;           \
  void *value;
#define STACK_FIELDS double number;
#include "lib/struct/LinkedList.h"
#include "lib/struct/Stack.h"

/* Structs */

typedef union NumberNode {
  double value;
  struct {
    int whole, decimal, isFraction, complete;
  };
} NumberNode;
typedef struct OperatorNode {
  OperatorType operator;
  char *sequence;
  int length, expected;
} OperatorNode;  // gcd

/* Incomplete node sequences */

char *gcdSequence = "gcd";
int gcdLength = 3;
char *lcmSequence = "lcm";
int lcmLength = 3;

/* Error messages */

char *notEnoughNumbers = "Not enough numbers in stack to perform operation %c";
char *invalidCharacter = "Invalid character %c";
char *expectedCharacter = "Expected character %c";
char *notEnoughOperators =
    "Not enough operators: more than one number left in stack";
char *tooManyOperators = "Invalid operations: not enough numbers in stack";

/* Functions */

// 5 3 + 9 -

/*
tokens: [
  NumberNode { value: 5 },
  NumberNode { value: 3 },
  OperatorNode { operator: + },
  NumberNode { value: 9 },
  OperatorNode { opreator: - },
]
*/

LinkedNode *wrapNode(void *value, NodeType type) {
  LinkedNode *node = createLinkedNode();
  node->type = type;
  node->value = value;
  return node;
}
LinkedNode *createNumberNode(char initialValue) {
  NumberNode *number = malloc(sizeof(NumberNode));
  number->whole = initialValue == FRACTION ? 0 : initialValue - '0';
  number->isFraction = initialValue == FRACTION;
  number->decimal = number->complete = 0;
  return wrapNode(number, TT_NUMBER);
}
LinkedNode *createOperatorNode(char symbol) {
  OperatorNode *operator= malloc(sizeof(OperatorNode));
  switch (symbol) {
    case '+':
      operator->operator= PLUS;
      break;
    case '-':
      operator->operator= MINUS;
      break;
    case '*':
      operator->operator= MULTIPLY;
      break;
    case '/':
      operator->operator= DIVIDE;
      break;
    case '^':
      operator->operator= POWER;
      break;
  }
  return wrapNode(operator, OPERATOR);
}
LinkedNode *createIncompleteNode(OperatorType target, char *sequence,
                                 int length) {
  OperatorNode *node = malloc(sizeof(OperatorNode));
  node->operator= target;
  node->sequence = sequence;
  node->length = length;
  node->expected = 1;
  return wrapNode(node, INCOMPLETE);
}
void normalizeNumberNode(NumberNode *node) {
  double value = node->whole, decimal = node->decimal;
  while (decimal >= 1) {
    decimal /= 10;
  }
  node->value = value + decimal;
}
void freeTokensList(LinkedList *tokens) {
  LinkedNode *current = tokens->start, *previous;
  while (current) {
    previous = current;
    current = current->next;
    free(previous->value);
    free(previous);
  }
}

LinkedList *tokenize(char input[], char **error, char *detail) {
  LinkedList *tokens = createLinkedList();

  int i = -1;
  char c;
  while (++i, c = input[i]) {
    switch (c) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        if (lastNodeIs(tokens, TT_NUMBER)) {
          NumberNode *number = lastNodeValue(tokens);
          if (!number->complete) {
            if (number->isFraction) {
              concatDigit(number->decimal, c);
            } else {
              concatDigit(number->whole, c);
            }
            break;
          }
        }

        LinkedNode *node = createNumberNode(c);
        linkedListInsert(tokens, node, NULL);
        break;
      }
      case ' ':
      case ',':
      case ';': {
        if (lastNodeIs(tokens, TT_NUMBER)) {
          NumberNode *number = lastNodeValue(tokens);
          number->complete = 1;
        }
        break;
      }
      case '.': {
        if (lastNodeIs(tokens, TT_NUMBER)) {
          NumberNode *number = lastNodeValue(tokens);
          if (!number->complete) {
            number->isFraction = 1;
            break;
          }
        }

        LinkedNode *node = createNumberNode(FRACTION);
        linkedListInsert(tokens, node, NULL);
        break;
      }
      case '+':
      case '-':
      case '*':
      case '/':
      case '^': {
        LinkedNode *node = createOperatorNode(c);
        linkedListInsert(tokens, node, NULL);
        break;
      }
      case 'g': {  // gcd
        LinkedNode *node =
            createIncompleteNode(COP_GCD, gcdSequence, gcdLength);
        linkedListInsert(tokens, node, NULL);
        break;
      }
      case 'l': {  // lcm
        LinkedNode *node =
            createIncompleteNode(COP_LCM, lcmSequence, lcmLength);
        linkedListInsert(tokens, node, NULL);
        break;
      }
      default: {
        if (lastNodeIs(tokens, INCOMPLETE)) {
          OperatorNode *node = lastNodeValue(tokens);
          if (node->sequence[node->expected] == c) {
            node->expected++;
            if (node->expected >= node->length) {
              linkedListEnd(tokens)->type = OPERATOR;
            }
            break;
          }
        }
        freeTokensList(tokens);
        *error = invalidCharacter;
        *detail = c;
        return NULL;
      }
    }
  }

  LinkedNode *current = linkedListStart(tokens);
  while (current) {
    if (current->type == TT_NUMBER) {
      normalizeNumberNode(current->value);
    } else if (current->type == INCOMPLETE) {
      OperatorNode *node = current->value;
      *error = expectedCharacter;
      *detail = node->sequence[node->expected];
      freeLinkedList(tokens);
    }
    current = current->next;
  }
  return tokens;
}

int main() {
  char input[256];
  scanf("%[^\n]255s", input);

  char *error, detail;
  LinkedList *tokens = tokenize(input, &error, &detail);

  if (!tokens) {
    printf(error, detail);
    printf("\n");
    return 1;
  }

  Stack *stack = createStack();
  LinkedNode *current = linkedListStart(tokens);

  while (current) {
    if (current->type == TT_NUMBER) {
      NumberNode *number = current->value;
      StackNode *node = createStackNode();
      node->number = number->value;
      stackPush(stack, node);
    } else {
      OperatorNode *operator= current->value;
      if (!hasTwoNumbers(stack)) {
        printf("%s\n", tooManyOperators);
        return 1;
      }
      StackNode *top = stackPop(stack);
      StackNode *target = stackTop(stack);

      switch (operator->operator) {
        case PLUS:
          target->number += top->number;
          break;
        case MINUS:
          target->number -= top->number;
          break;
        case MULTIPLY:
          target->number *= top->number;
          break;
        case DIVIDE:
          target->number /= top->number;
          break;
        case POWER:
          target->number = pow(target->number, top->number);
          break;
        case COP_GCD:
          if (target->number != (int)target->number ||
              top->number != (int)top->number) {
            printf("Unable to calculate GCD for fractional numbers %f and %f\n",
                   top->number, target->number);
            return 1;
          }
          target->number = gcd(target->number, top->number);
          break;
        case COP_LCM:
          if (target->number != (int)top->number ||
              top->number != (int)top->number) {
            printf("Unable to calculate LCM for fractional numbers %f and %f\n",
                   top->number, target->number);
            return 1;
          }
          target->number = lcm(target->number, top->number);
          break;
      }
    }
    current = current->next;
  }

  if (hasTwoNumbers(stack)) {
    printf("%s\n", notEnoughOperators);
    return 1;
  }
  if (isStackEmpty(stack)) {
    printf("No answer\n");
    return 1;
  } else {
    printf("%f\n", stackTop(stack)->number);
    return 0;
  }
}