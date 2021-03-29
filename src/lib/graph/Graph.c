#include "Graph.h"

#include <stdio.h>
#include <stdlib.h>

static void pushAdjNode(GraphAdjChain *chain, GraphAdjNode *node);
static void linkDirNodesUnsafe(Graph *graph, int from, int to);

Graph *createGraph(int n) {
  Graph *graph = malloc(sizeof(Graph) + n * sizeof(GraphAdjChain));
  if (!graph) return NULL;

  for (int i = 0; i < n; i++) {
    graph->list[i].start = graph->list[i].end = NULL;
  }
  graph->n = n;
  return graph;
}

GraphAdjNode *createAdjNode(int id) {
  GraphAdjNode *adjNode = malloc(sizeof(GraphAdjNode));
  if (!adjNode) return NULL;
  adjNode->next = NULL;
  adjNode->id = id;
  return adjNode;
}

Graph *invertGraph(Graph *graph) {
  Graph *inverted = createGraph(graph->n);
  for (int i = 0; i < graph->n; i++) {
    GraphAdjNode *current = graph->list[i].start;
    while (current) {
      linkDirNodesUnsafe(inverted, current->id, i);
      current = current->next;
    }
  }
  return inverted;
}

int *getAdjMatrixRow(Graph *graph, int row) {
  int *matrix = malloc(graph->n * sizeof(int));

  for (int i = 0; i < graph->n; i++) {
    matrix[i] = 0;
  }

  GraphAdjNode *current = graph->list[row].start;
  while (current) {
    matrix[current->id] = 1;
    current = current->next;
  }

  return matrix;
}

void fprintAdjMatrix(Graph *graph, const char *path) {
  FILE *file = fopen(path, "w");
  int *matrix;

  fprintf(file, "%d\n", graph->n);
  for (int i = 0; i < graph->n; i++) {
    matrix = getAdjMatrixRow(graph, i);
    for (int j = 0; j < graph->n; j++) {
      fprintf(file, "%d ", matrix[j]);
    }
    fprintf(file, "\n");
    free(matrix);
  }
  fclose(file);
}

Graph *fscanAdjMatrix(const char *path) {
  FILE *file = fopen(path, "r");
  if (!file) return NULL;

  int n;
  fscanf(file, "%d", &n);

  Graph *graph = createGraph(n);
  for (int i = 0; i < graph->n; i++) {
    for (int j = 0; j < graph->n; j++) {
      fscanf(file, "%d", &n);
      if (n) {
        linkDirNodesUnsafe(graph, i, j);
      }
    }
  }

  fclose(file);
  return graph;
}

static void pushAdjNode(GraphAdjChain *chain, GraphAdjNode *node) {
  if (chain->end) {
    chain->end->next = node;
  } else {
    chain->start = node;
  }
  chain->end = node;
}

static void linkDirNodesUnsafe(Graph *graph, int from, int to) {
  pushAdjNode(&graph->list[from], createAdjNode(to));
}

void linkDirNodes(Graph *graph, int from, int to) {
  if (from == to || from < 0 || to < 0 || from >= graph->n || to >= graph->n) {
    printf("Error - invalid indexes to link: %d and %d\n", from, to);
    return;
  }

  GraphAdjNode *current = graph->list[from].start;
  while (current) {
    if (current->id == to) return;
    current = current->next;
  }
  linkDirNodesUnsafe(graph, from, to);
}

void linkNodes(Graph *graph, int id1, int id2) {
  linkDirNodes(graph, id1, id2);
  linkDirNodes(graph, id2, id1);
}

void removeFromChain(GraphAdjChain *chain, int id) {
  GraphAdjNode *deleting, *current = chain->start;
  while (current->next) {
    if (current->next->id == id) {
      deleting = current->next;
      current->next = deleting->next;
      free(deleting);
      break;
    }
    current = current->next;
  }
}

void unlinkDirNodes(Graph *graph, int id1, int id2) {
  removeFromChain(&graph->list[id1], id2);
}

void unlinkNodes(Graph *graph, int id1, int id2) {
  unlinkDirNodes(graph, id1, id2);
  unlinkDirNodes(graph, id2, id1);
}

void freeAdjChain(GraphAdjChain *chain) {
  GraphAdjNode *previous, *current = chain->start;
  while (current) {
    previous = current;
    current = current->next;
    free(previous);
  }
}

void freeGraph(Graph *graph) {
  for (int i = 0; i < graph->n; i++) {
    freeAdjChain(&graph->list[i]);
  }
  free(graph);
}

void printGraph(Graph *graph) {
  for (int i = 0; i < graph->n; i++) {
    printf("%d is linked to", i);

    GraphAdjNode *current = graph->list[i].start;
    if (!current)
      printf(" nothing\n");
    else {
      printf("\n  ");
      while (current) {
        printf("%d", current->id);
        current = current->next;
        if (current) printf(", ");
      }
      printf("\n");
    }
  }
}
