#define STACK_FIELDS int nodeId;

#include "graph-fast-scc.h"

#include <limits.h>
#include <stdlib.h>

#include "../struct/Stack.h"
#include "Graph.h"

#define createGraphStackNode(id)         \
  ({                                     \
    StackNode *node = createStackNode(); \
    node->nodeId = id;                   \
    node;                                \
  })

static int dfs(Graph *graph, int id, int a[], int b[], int visited[],
               int *counter) {
  int min = b[id] = (*counter)++;
  visited[id] = 1;

  GraphAdjNode *current = graph->list[id].start;
  while (current) {
    if (visited[current->id] == 1) {
      if (b[current->id] < min) min = b[current->id];
    } else if (visited[current->id] == 0) {
      int new = dfs(graph, current->id, a, b, visited, counter);
      if (new < min) min = new;
    }
    current = current->next;
  }

  visited[id] = 2;
  return (b[id] = min);
}

int *getSccListFast(Graph *graph) {
  int *b = malloc(graph->n * sizeof(int));
  int a[graph->n];
  int visited[graph->n];

  for (int i = 0; i < graph->n; i++) {
    visited[i] = 0;
    b[i] = INT_MAX;
  }

  int counter = 0;
  for (int i = 0; i < graph->n; i++) {
    if (!visited[i]) {
      dfs(graph, i, a, b, visited, &counter);
    }
  }

  return b;
}