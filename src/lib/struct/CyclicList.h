#pragma once

#ifndef CYCLIC_LIST_FIELDS
#define CYCLIC_LIST_FIELDS
#endif

#define cyclicListStart(cyclicList) cyclicList->start
#define cyclicListEnd(cyclicList) cyclicList->end

typedef struct CyclicNode {
  struct CyclicNode *next;
  CYCLIC_LIST_FIELDS
} CyclicNode;

typedef struct CyclicList {
  CyclicNode *start, *end;
} CyclicList;

CyclicNode *createCyclicNode();
CyclicList *createCyclicList();
void cyclicListInsert(CyclicList *list, CyclicNode *node);
void cyclicListInsertAfter(CyclicNode *node, CyclicNode *newNode);
void removeCyclicNodeAfter(CyclicList *list, CyclicNode *node);
int isCyclicListEmpty(CyclicList *list);
void cyclicListRemove(CyclicList *list, CyclicNode *node);
void freeCyclicList(CyclicList *list);

void printCyclicList(CyclicList *list, void (*printNode)(CyclicNode *));