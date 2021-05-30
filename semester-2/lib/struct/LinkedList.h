#pragma once

#ifndef LINKED_LIST_FIELDS
#define LINKED_LIST_FIELDS
#endif

#define linkedListEnd(linkedList) linkedList->end
#define linkedListStart(linkedList) linkedList->start

typedef struct LinkedNode {
  struct LinkedNode *next;
  LINKED_LIST_FIELDS
} LinkedNode;

typedef struct LinkedList {
  LinkedNode *start, *end;
} LinkedList;

LinkedNode *createLinkedNode();
LinkedList *createLinkedList();
int isLinkedListEmpty(LinkedList *list);
/** If "after" is null, inserts node in the end of the list */
void linkedListInsert(LinkedList *list, LinkedNode *node, LinkedNode *after);
void linkedListRemove(LinkedList *list, LinkedNode *node);
void freeLinkedList(LinkedList *list);

void printLinkedList(LinkedList *list, void (*printNode)(LinkedNode *));
