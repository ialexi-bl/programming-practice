#define STACK_FIELDS int nodeId;

#include "graph-scc-2.h"

#include "../struct/Stack.h"
#include "Graph.h"

#define createGraphStackNode(id)         \
  ({                                     \
    StackNode *node = createStackNode(); \
    node->nodeId = id;                   \
    node;                                \
  })

static void dfs(Graph *graph, int startingId, int a[], int b[], int visited[],
                int *counter) {
  Stack *stack = createStack();
  stackPush(stack, createGraphStackNode(startingId));

  int min = *counter;
  while (!isStackEmpty(stack)) {
    StackNode *node = stackTop(stack);

    if (visited[node->nodeId]) {
      b[node->nodeId] = min;
      visited[node->nodeId] = 2;
      free(stackPop(stack));
    } else {
      visited[node->nodeId] = 1;
      b[node->nodeId] = (*counter)++;

      GraphAdjNode *current = graph->list[node->nodeId].start;
      while (current) {
        if (visited[current->id] == 0) {
          stackPush(stack, createGraphStackNode(current->id));
        } else if (visited[current->id] == 1) {
          if (b[current->id] < min) {
            min = b[current->id];
          }
        }

        current = current->next;
      }
    }
  }
}

int *getSccList(Graph *graph) {
  int a[graph->n];
  int b[graph->n];
  int visited[graph->n];

  for (int i = 0; i < graph->n; i++) {
    visited[i] = a[i] = b[i] = 0;
  }

  int counter = 0;
  for (int i = 0; i < graph->n; i++) {
    if (!visited[i]) {
      dfs(graph, i, a, b, visited, &counter);
    }
  }
}