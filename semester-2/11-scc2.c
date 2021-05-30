#include <stdio.h>

#include "lib/graph/Graph.h"
#include "lib/graph/graph-fast-scc.h"

int main() {
  Graph *graph = fscanAdjMatrix("./graph.txt");

  int *list = getSccListFast(graph);
  for (int i = 0; i < graph->n; i++) {
    printf("%d: component #%d\n", i, list[i]);
  }
}
