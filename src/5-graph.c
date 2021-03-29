#include <stdio.h>

#include "lib/graph/Graph.h"

int main() {
  Graph *graph = createGraph(5);
  linkNodes(graph, 0, 1);
  linkNodes(graph, 0, 2);
  linkNodes(graph, 0, 3);
  linkNodes(graph, 0, 4);
  linkNodes(graph, 1, 3);
  linkNodes(graph, 1, 2);
  linkNodes(graph, 2, 4);

  printGraph(graph);
  printf("--------------\n");

  unlinkNodes(graph, 0, 2);
  unlinkNodes(graph, 1, 3);

  printGraph(graph);

  freeGraph(graph);
}