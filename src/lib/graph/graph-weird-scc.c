#define STACK_FIELDS int nodeId;

#include "graph-weird-scc.h"

#include <limits.h>
#include <stdlib.h>

#include "../struct/Stack.h"
#include "Graph.h"

#define createGraphStackNode(id)           \
  ({                                       \
    StackNode *__node = createStackNode(); \
    __node->nodeId = id;                   \
    __node;                                \
  })

static int dfs(Graph *graph, int id, int b[], int visited[], int *counter) {
  b[id] = (*counter)++;
  visited[id] = 1;

  // Get minimum
  GraphAdjNode *child = graph->list[id].start;
  while (child) {
    if (b[child->id] < b[id]) {
      b[id] = b[child->id];
    }

    child = child->next;
  }

  // DFS
  child = graph->list[id].start;
  while (child) {
    if (!visited[child->id] == 0) {
      int new = dfs(graph, child->id, b, visited, counter);
      if (new < b[id]) {
        b[id] = new;
      }
    }
    child = child->next;
  }

  visited[id] = 2;
  return b[id];
}

int *getSccListWeird(Graph *graph) {
  int *b = malloc(graph->n * sizeof(int));
  int visited[graph->n];

  for (int i = 0; i < graph->n; i++) {
    visited[i] = 0;
    b[i] = INT_MAX;
  }

  int counter = 0;
  for (int i = 0; i < graph->n; i++) {
    if (!visited[i]) {
      dfs(graph, i, b, visited, &counter);
    }
  }

  return b;
}