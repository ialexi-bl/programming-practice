#include <stdio.h>

#include "lib/graph/Graph.h"
#include "lib/graph/graph-scc.h"

int main() {
  Graph *graph = fscanAdjMatrix("./graph.txt");
  int n;
  // scanf("%d", &n);
  printf("%d\n", n);

  int *list = getSccList(graph, NULL);
  for (int i = 0; i < graph->n; i++) {
    printf("%d: component #%d\n", i, list[i]);
  }
}