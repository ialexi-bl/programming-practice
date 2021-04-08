#include "Graph.h"
#include "stdlib.h"

#define STACK_FIELDS int id;
#include "../struct/Stack.h"

#define createSccStackNode(_id)            \
  ({                                       \
    StackNode *__node = createStackNode(); \
    __node->id = _id;                      \
    __node;                                \
  })

static void getOrder(Graph *graph, int node, int visited[], Stack *stack) {
  GraphAdjNode *current = graph->list[node].start;
  while (current) {
    int id = current->id;
    if (!visited[id]) {
      visited[id] = 1;
      getOrder(graph, id, visited, stack);
    }
    current = current->next;
  }

  StackNode *ndoe = createSccStackNode(node);
  stackPush(stack, ndoe);
}

static void sccDfs(Graph *graph, int node, int visited[], int result[],
                   int counter) {
  Stack *stack = createStack();
  stackPush(stack, createSccStackNode(node));

  while (!isStackEmpty(stack)) {
    StackNode *stackNode = stackPop(stack);
    int id = stackNode->id;
    free(stackNode);

    result[id] = counter;

    GraphAdjNode *current = graph->list[id].start;
    while (current) {
      if (visited[current->id] != 2) {
        visited[current->id] = 2;
        stackPush(stack, createSccStackNode(current->id));
      }
      current = current->next;
    }
  }
  free(stack);
}

int *getSccList(Graph *graph, int *count) {
  Graph *inverted = invertGraph(graph);
  Stack *stack = createStack();

  int visited[graph->n];
  int *result = malloc(graph->n * sizeof(int));
  for (int i = 0; i < graph->n; i++) visited[i] = 0;

  for (int i = 0; i < graph->n; i++) {
    if (!visited[i]) {
      visited[i] = 1;
      getOrder(inverted, i, visited, stack);
    }
  }
  freeGraph(inverted);

  int counter = 0;
  while (!isStackEmpty(stack)) {
    StackNode *node = stackPop(stack);
    int id = node->id;
    free(node);

    if (visited[id] == 2) continue;
    visited[id] = 2;

    sccDfs(graph, id, visited, result, counter++);
  }
  free(stack);

  if (count) *count = counter;
  return result;
}
