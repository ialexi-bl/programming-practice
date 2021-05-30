#include "LinkedList.h"

#include <stdio.h>
#include <stdlib.h>

LinkedNode *createLinkedNode() {
  LinkedNode *node = malloc(sizeof(LinkedNode));
  node->next = NULL;
  return node;
}

LinkedList *createLinkedList() {
  LinkedList *list = malloc(sizeof(LinkedList));
  list->start = list->end = NULL;
  return list;
}

void linkedListInsert(LinkedList *list, LinkedNode *node, LinkedNode *after) {
  if (!after) after = list->end;

  if (after) {
    after->next = node;
    if (after == list->end) list->end = node;
  } else {
    list->start = list->end = node;
  }
}

int isLinkedListEmpty(LinkedList *list) { return list->start == NULL; }

void linkedListRemove(LinkedList *list, LinkedNode *node) {}

void freeLinkedList(LinkedList *list) {
  LinkedNode *previous, *current = list->start;
  while (current) {
    previous = current;
    free(previous);
    current = current->next;
  }
  free(list);
}

void printLinkedList(LinkedList *list, void (*printNode)(LinkedNode *)) {
  LinkedNode *current = list->start;
  while (current) {
    printNode(current);
    current = current->next;
    if (current) printf(", ");
  };
}
