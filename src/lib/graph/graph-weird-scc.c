#define STACK_FIELDS int nodeId;

#include "graph-weird-scc.h"

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
  b[id] = (*counter)++;
  visited[id] = 1;

  GraphAdjNode *current = graph->list[id].start;
  while (current) {
    if (b[current->id] < b[id]) b[id] = b[current->id];
    current = current->next;
  }

  current = graph->list[id].start;
  while (current) {
    if (!visited[current->id] == 0) {
      int new = dfs(graph, current->id, a, b, visited, counter);
      if (new < b[id]) {
        b[id] = new;
      }
    }
    current = current->next;
  }

  visited[id] = 2;
  return b[id];
}

int *getSccListWeird(Graph *graph) {
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