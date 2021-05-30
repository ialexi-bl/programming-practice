#include <stdio.h>

#include "lib/graph/Graph.h"
#include "lib/graph/topological-sort.h"

int main() {
  Graph *graph = fscanAdjMatrix("./graph.txt");
  int *result = getTopologicalSort(graph);

  if (!result) {
    printf("Topological sort doesn't exist\n");
  } else {
    for (int i = 0; i < graph->n; i++) {
      printf("%d", result[i]);
      if (i != graph->n - 1) printf(" -> ");
    }
    printf("\n");
  }
}