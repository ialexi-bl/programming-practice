#include "BipartiteGraph.h"

#include <stdio.h>
#include <stdlib.h>

BipartiteGraph *createBipartiteGraph(int n, int colors[]) {
  BipartiteGraph *bgraph = malloc(sizeof(BipartiteGraph));
  if (!bgraph) return NULL;

  for (int i = 0; i < n; i++) {
    LinkedNode *node = createLinkedNode();
    node->nodeId = i;
    if (colors[i]) {
      linkedListInsert(&bgraph->second, node, NULL);
    } else {
      linkedListInsert(&bgraph->first, node, NULL);
    }
  }

  return bgraph;
}

void printNodeId(int id) { printf("%d", id); }
void printBipartiteList(LinkedList *list, void (*printNode)(int)) {
  LinkedNode *current = list->start;
  while (current) {
    printNode(current->nodeId);
    current = current->next;
    if (current) printf(", ");
  }
}
void printBipartiteGraph(BipartiteGraph *graph, void (*printNode)(int)) {
  if (!printNode) printNode = printNodeId;

  printf("First part:\n  ");
  printBipartiteList(&graph->first, printNode);
  printf("\nSecond part:\n  ");
  printBipartiteList(&graph->second, printNode);
  printf("\n");
}