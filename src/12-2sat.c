#include <stdio.h>

#include "lib/Graph.h"
#include "lib/errorio.h"
#include "lib/graph-scc.h"

#define true 1
#define false 0

Graph *parse2Sat(const char *string, char *error) {
  Graph *graph = createGraph(26 * 2 * 2);
  int i = -1;
  char c;

  char lit1 = false, lit2 = false;
  int lparen = false, rparen = false;
  int neg1 = false, neg2 = false;
  int disj = false;

  while (1) {
    c = string[++i];
    switch (c) {
      case '(': {
        if (lparen) {
          sprintfError(error, string, i, "Unexpected parenthesis");
          freeGraph(graph);
          return NULL;
        }
        lparen = true;
        break;
      }
      case ')': {
        if (!lit2 || rparen) {
          sprintfError(error, string, i, "Unexpected parenthesis");
          freeGraph(graph);
          return NULL;
        }
        rparen = true;
        break;
      }
      case '|': {
        if (!lit1 || disj) {
          sprintfError(error, string, i, "Unexpected disjunction operator");
          freeGraph(graph);
          return NULL;
        }
        disj = true;
        break;
      }
      case '!': {
        if (!(lparen && !neg1 && !lit1 || disj && !neg2 && !lit2)) {
          sprintfError(error, string, i, "Unexpected negation operator");
          freeGraph(graph);
          return NULL;
        }
        if (disj) {
          neg2 = true;
        } else {
          neg1 = true;
        }
        break;
      }
      default: {
        if (c < 'A' || c > 'z') {
          sprintfError(error, string, i, "Unexpected character \"%c\"", c);
          freeGraph(graph);
          return NULL;
        }
        if (!((lparen && !lit1) || (disj && !lit2))) {
          sprintfError(error, string, i, "Unexpected literal \"%c\"", c);
          freeGraph(graph);
          return NULL;
        }

        if (disj) {
          lit2 = c;
        } else {
          lit1 = c;
        }
        break;
      }
      case '&':
      case '\0': {
        if (!rparen) {
          sprintfError(error, string, i,
                       c == '&' ? "Unexpected conjunction operator"
                                : "Unexpected end of input");
          freeGraph(graph);
          return NULL;
        }

        int i1 = (lit1 - 'A') + (26 * 2 * !neg1);
        int i2 = (lit2 - 'A') + (26 * 2 * neg2);
        linkDirNodes(graph, i1, i2);

        lit1 = lit2 = '\0';
        neg1 = neg2 = lparen = rparen = disj = false;

        if (c == '\0') {
          return graph;
        }
        break;
      }
    }
  }
}

int main() {
  char string[256], error[128];
  scanf("%[^\\n]255s", string);

  Graph *graph = parse2sat(string, error);
  int *scc = getSccList(graph);

  int result[26 * 2];
  for (int i = 0; i < 26 * 2; i++) {
    int neg = i + 26 * 2;
    if (!graph->list[i].start && !graph->list[neg].start) {
      result[i] = -1;
      continue;
    }

    if (scc[i] == scc[neg]) {
      printf("No solution\n");
      return 0;
    }
  }
}