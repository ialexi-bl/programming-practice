#define STACK_FIELDS int nodeId;
#include <stdlib.h>

#include "../struct/Stack.h"
#include "Graph.h"

#define createGraphStackNode(id)         \
  ({                                     \
    StackNode *node = createStackNode(); \
    node->nodeId = id;                   \
    node;                                \
  })

static int topologicalDfs(Graph *graph, int startingNode, int visited[],
                          int *ptr, int result[]) {
  Stack *stack = createStack();
  stackPush(stack, createGraphStackNode(startingNode));

  while (!isStackEmpty(stack)) {
    StackNode *node = stackTop(stack);

    if (visited[node->nodeId]) {
      visited[node->nodeId] = 2;
      result[(*ptr)--] = node->nodeId;
      free(stackPop(stack));
    } else {
      visited[node->nodeId] = 1;

      GraphAdjNode *current = graph->list[node->nodeId].start;
      while (current) {
        if (visited[current->id] == 0) {
          stackPush(stack, createGraphStackNode(current->id));
        } else if (visited[current->id] == 1) {
          freeStack(stack);
          return 0;
        }
        current = current->next;
      }
    }
  }

  freeStack(stack);
  return 1;
}

int *getTopologicalSort(Graph *graph) {
  int visited[graph->n];
  int *result = malloc(graph->n * sizeof(int));
  for (int i = 0; i < graph->n; i++) visited[i] = 0;

  int ptr = graph->n - 1;
  for (int i = 0; i < graph->n; i++) {
    if (visited[i]) continue;

    int success = topologicalDfs(graph, i, visited, &ptr, result);
    if (!success) {
      free(result);
      return NULL;
    }
  }

  return result;
}