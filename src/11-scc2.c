#include <stdio.h>

#include "lib/Graph.h"
#include "lib/graph-scc-2.h"

int main() {
  Graph *graph = fscanAdjMatrix("./graph.txt");

  int *list = getSccList(graph);
  for (int i = 0; i < graph->n; i++) {
    printf("%d: component #%d\n", i, list[i]);
  }
}