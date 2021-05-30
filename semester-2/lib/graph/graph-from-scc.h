#pragma once
#include "Graph.h"

Graph *getSccGraph(Graph *graph);
Graph *getSccGraphFromList(Graph *graph, int *scc, int count);
