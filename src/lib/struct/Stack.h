#pragma once

#ifndef STACK_FIELDS
#define STACK_FIELDS
#endif

#define stackTop(stack) (stack->top)
#define stackNext(node) node->next

typedef struct StackNode {
  struct StackNode *next;
  void *value;
  STACK_FIELDS
} StackNode;

typedef struct Stack {
  StackNode *top;
} Stack;

Stack *createStack();
StackNode *createStackNode(void *value);
StackNode *stackPop(Stack *stack);
int isStackEmpty(Stack *stack);
void stackPush(Stack *stack, StackNode *newNode);
void freeStack(Stack *stack);
