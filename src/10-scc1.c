#include <stdio.h>

#include "lib/graph/Graph.h"
#include "lib/graph/graph-scc-1.h"

int main() {
  Graph *graph = fscanAdjMatrix("./graph.txt");

  int *list = getSccListSlow(graph);
  for (int i = 0; i < graph->n; i++) {
    printf("%d: component #%d\n", i, list[i]);
  }
}