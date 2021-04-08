#include "graph-from-scc.h"

#include <stdlib.h>

#include "Graph.h"
#include "graph-scc.h"

Graph *getSccGraphFromList(Graph *graph, int *scc, int sccCount) {
  Graph *sccGraph = createGraph(sccCount);

  for (int i = 0; i < graph->n; i++) {
    GraphAdjNode *current = graph->list[i].start;

    while (current) {
      if (scc[i] != scc[current->id]) {
        linkDirNodes(sccGraph, scc[i], scc[current->id]);
      }

      current = current->next;
    }
  }

  return sccGraph;
}

Graph *getSccGraph(Graph *graph) {
  int count;
  int *scc = getSccList(graph, &count);
  Graph *sccGraph = getSccGraphFromList(graph, scc, count);
  free(scc);
  return sccGraph;
}