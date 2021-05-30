#pragma once
#define LINKED_LIST_FIELDS int nodeId;

#include "../graph/Graph.h"
#include "../struct/LinkedList.h"

typedef struct BipartiteGraph {
  LinkedList first, second;
} BipartiteGraph;

BipartiteGraph *createBipartiteGraph(int n, int colors[]);
/* If printNode is NULL, just prints node id */
void printBipartiteGraph(BipartiteGraph *graph, void (*printNode)(int));