#include "Stack.h"

#include <stdlib.h>

Stack *createStack() {
  Stack *stack = malloc(sizeof(Stack));
  stack->top = NULL;
  return stack;
}

StackNode *createStackNode(void *value) {
  StackNode *node = malloc(sizeof(StackNode));
  node->next = NULL;
  node->value = value;
  return node;
}

int isStackEmpty(Stack *stack) { return stack->top == NULL; }

void stackPush(Stack *stack, StackNode *newNode) {
  newNode->next = stack->top;
  stack->top = newNode;
}

StackNode *stackPop(Stack *stack) {
  if (!stack->top) return NULL;

  StackNode *top = stack->top;
  stack->top = top->next;
  return top;
}

void freeStack(Stack *stack) {
  StackNode *previous, *current = stack->top;
  if (current) {
    while (current->next) {
      previous = current;
      current = current->next;
      free(previous);
    }
  }
  free(current);
  free(stack);
}
