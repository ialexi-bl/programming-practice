#define STACK_FIELDS int nodeId;
#include "graph-fast-scc.h"

#include <limits.h>
#include <stdlib.h>

#include "../struct/Stack.h"
#include "Graph.h"
#define createSccStackNode(_id)            \
  ({                                       \
    StackNode *__node = createStackNode(); \
    __node->nodeId = _id;                  \
    __node;                                \
  })

static int dfs(Graph *graph, int id, int a[], int b[], int visited[], int *time,
               Stack *stack) {
  a[id] = b[id] = (*time)++;
  visited[id] = 1;
  stackPush(stack, createSccStackNode(id));

  GraphAdjNode *current = graph->list[id].start;
  while (current) {
    if (visited[current->id] != 2) {
      if (visited[current->id]) {
        if (a[current->id] < b[id]) {
          b[id] = a[current->id];
        }
      } else {
        int new = dfs(graph, current->id, a, b, visited, time, stack);
        if (new < b[id]) b[id] = new;
      }
    }
    current = current->next;
  }

  if (a[id] == b[id]) {
    StackNode *st;
    while ((st = stackPop(stack)) && st->nodeId != id) {
      b[st->nodeId] = b[id];
    }
  }
  visited[id] = 2;
  return b[id];
}

int *getSccListFast(Graph *graph) {
  int a[graph->n], *b = malloc(graph->n * sizeof(int));
  int visited[graph->n];

  for (int i = 0; i < graph->n; i++) {
    visited[i] = 0;
    b[i] = a[i] = INT_MAX;
  }

  Stack *stack = createStack();

  int counter = 0;
  for (int i = 0; i < graph->n; i++) {
    if (!visited[i]) {
      dfs(graph, i, a, b, visited, &counter, stack);
    }
  }

  return b;
}