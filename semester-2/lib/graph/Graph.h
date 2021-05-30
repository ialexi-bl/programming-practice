#pragma once

typedef struct GraphAdjNode {
  struct GraphAdjNode *next;
  int id;
} GraphAdjNode;

typedef struct GraphAdjChain {
  GraphAdjNode *start, *end;
} GraphAdjChain;

typedef struct Graph {
  int n;
  GraphAdjChain list[];
} Graph;

Graph *createGraph(int n);
GraphAdjNode *createAdjNode(int id);
Graph *invertGraph(Graph *graph);

Graph *fscanAdjMatrix(const char *path);
int *getAdjMatrixRow(Graph *graph, int row);
void fprintAdjMatrix(Graph *graph, const char *path);

void linkDirNodes(Graph *graph, int from, int to);
void linkNodes(Graph *graph, int id1, int id2);

void unlinkDirNodes(Graph *graph, int id1, int id2);
void unlinkNodes(Graph *graph, int id1, int id2);

void freeGraph(Graph *graph);
void printGraph(Graph *graph);

int *getSccList(Graph *graph, int *count);
