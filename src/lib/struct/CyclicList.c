#include "CyclicList.h"

#include <stdio.h>
#include <stdlib.h>

CyclicNode *createCyclicNode() {
  CyclicNode *node = malloc(sizeof(CyclicNode));
  node->next = NULL;
  return node;
}

CyclicList *createCyclicList() {
  CyclicList *list = malloc(sizeof(CyclicList));
  list->start = list->end = NULL;
  return list;
}

void cyclicListInsert(CyclicList *list, CyclicNode *node) {
  if (list->start) {
    node->next = list->start;
    list->end = list->end->next = node;
  } else {
    list->start = list->end = node;
    node->next = node;
  }
}

void cyclicListInsertAfter(CyclicNode *node, CyclicNode *newNode) {
  newNode->next = node->next;
  node->next = newNode;
}

void removeCyclicNodeAfter(CyclicList *list, CyclicNode *node) {
  if (node->next == node) {
    free(node);
    list->start = list->end = NULL;
  } else {
    CyclicNode *next = node->next->next;
    if (node->next == list->start) list->start = next;
    if (node->next == list->end) list->end = next;
    free(node->next);
    node->next = next;
  }
}

int isCyclicListEmpty(CyclicList *list) { return list->start == NULL; }

void cyclicListRemove(CyclicList *list, CyclicNode *node) {}

void freeCyclicList(CyclicList *list) {
  if (list->end) list->end->next = NULL;
  CyclicNode *previous, *current = list->start;
  while (current) {
    previous = current;
    current = current->next;
    free(previous);
  }
  free(list);
}

void printCyclicList(CyclicList *list, void (*printNode)(CyclicNode *)) {
  if (!list->start) return;

  CyclicNode *current = list->start;
  do {
    if (current != list->start) printf(", ");
    printNode(current);
    current = current->next;
  } while (current != list->start);
  printf("\n");
}
