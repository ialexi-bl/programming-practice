#include "graph-from-scc.h"

#include "Graph.h"
#include "graph-scc.h"

Graph *getSccGraph(Graph *graph) {
  int count;
  int *scc = getSccList(graph, &count);

  Graph *sccGraph = create(count);
}