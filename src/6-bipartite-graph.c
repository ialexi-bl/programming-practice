#include <stdio.h>

#include "lib/graph/BipartiteGraph.h"
#include "lib/graph/Graph.h"

#define STACK_FIELDS int nodeId;
#define QUEUE_FIELDS int nodeId;

#include "lib/struct/Queue.h"
#include "lib/struct/Stack.h"

int bipartiteDfs(Graph *graph, int id, int colors[], Stack *stack) {
  StackNode *sn = createStackNode((void *)id);
  stackPush(stack, sn);

  colors[id] = 0;

  StackNode *current;
  GraphAdjNode *currentChild;
  int childId;

  while ((current = stackPop(stack))) {
    id = (int)current->value;

    currentChild = graph->list[id].start;
    while (currentChild) {
      childId = currentChild->id;

      if (colors[childId] < 0) {
        colors[childId] = !colors[id];

        StackNode *sn = createStackNode((void *)childId);
        stackPush(stack, sn);
      } else if (colors[childId] == colors[id]) {
        return 0;
      }
      currentChild = currentChild->next;
    }
  }
  return 1;
}

BipartiteGraph *bipartite(Graph *graph) {
  Stack *stack = createStack();

  int colors[graph->n];
  for (int i = 0; i < graph->n; i++) colors[i] = -1;

  for (int i = 0; i < graph->n; i++) {
    if (colors[i] < 0) {
      int success = bipartiteDfs(graph, i, colors, stack);
      if (!success) {
        freeStack(stack);
        return NULL;
      }
    }
  }

  freeStack(stack);
  return createBipartiteGraph(graph->n, colors);
}

int main() {
  Graph *graph = createGraph(9);
  linkNodes(graph, 0, 1);
  linkNodes(graph, 0, 2);
  linkNodes(graph, 1, 3);
  linkNodes(graph, 2, 3);
  linkNodes(graph, 3, 5);
  linkNodes(graph, 3, 8);
  linkNodes(graph, 8, 6);
  linkNodes(graph, 8, 7);

  BipartiteGraph *bg = bipartite(graph);
  if (!bg) {
    printf("This graph is not bipartite\n");
  } else {
    printBipartiteGraph(bg, NULL);
  }
}