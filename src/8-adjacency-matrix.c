#include <stdio.h>

#include "lib/Graph.h"

int main() {
  Graph *graph = createGraph(5);
  linkNodes(graph, 0, 1);
  linkNodes(graph, 0, 2);
  linkNodes(graph, 0, 3);
  linkNodes(graph, 0, 4);
  linkDirNodes(graph, 1, 3);
  linkDirNodes(graph, 1, 2);
  linkDirNodes(graph, 2, 4);

  fprintAdjMatrix(graph, "./graph.txt");
  freeGraph(graph);

  Graph *newGraph = fscanAdjMatrix("./graph.txt");
  printGraph(newGraph);
  freeGraph(graph);
}