#include <stdio.h>

#include "lib/CyclicList.h"

#define RANGE 25

definedCyclicList(int n);
definePrintCyclicList();
void printCyclicNode(CyclicNode *node) { printf("%3d ", node->n + 1); }

CyclicList *initializeCyclicList(int count) {
  CyclicList *list = createCyclicList();
  for (int i = 0; i < count; i++) {
    CyclicNode *node = createCyclicNode();
    node->n = i;
    cyclicListInsert(list, node);
  }
  return list;
}

CyclicList *josephus(int n, int s, int t) {
  CyclicList *list = initializeCyclicList(n);
  CyclicNode *current = cyclicListEnd(list);

  for (int i = 0; i < t && !isCyclicListEmpty(list); i++) {
    for (int j = 1; j < s; j++) {
      current = current->next;
    }
    removeCyclicNodeAfter(list, current);
  }

  return list;
}

int main() {
  int results[RANGE];
  printf(" n ");
  for (int i = 0; i < RANGE; i++) {
    CyclicList *list = josephus(i + 1, 2, i);
    results[i] = cyclicListStart(list)->n;
    freeCyclicList(list);

    printf("│ %2d ", i + 1);
  }
  printf("\n───");
  for (int i = 0; i < RANGE; i++) {
    printf("┼────");
  }
  printf("\n   ");
  for (int i = 0; i < RANGE; i++) {
    printf("│ %2d ", results[i] + 1);
  }
  printf("\n");
}