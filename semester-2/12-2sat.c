#include <stdio.h>

#include "lib/2sat/parse.h"
#include "lib/errorio.h"
#include "lib/graph/Graph.h"
#include "lib/graph/graph-from-scc.h"
#include "lib/graph/graph-scc.h"

static void satDfs(Graph *sccGraph, int id, int *scc, int result[]) {
  GraphAdjNode *current = sccGraph->list[id].start;
  while (current) {
    if (result[current->id] == -1) {
      satDfs(sccGraph, current->id, scc, result);
    }
    current = current->next;
  }

  if (result[id] == -1) {
    result[id] = 1;

    for (int i = 0; i < LITERALS_COUNT * 2; i++) {
      // If literal i is in currently processed SCC
      if (scc[i] == id) {
        // Calculate its negated literal
        int inverse = i + LITERALS_COUNT * (i < LITERALS_COUNT ? 1 : -1);
        // And assign 0 to it
        result[scc[inverse]] = 0;
      }
    }
  }
}

// (x\/~y)/\(z\/y)/\(~z\/y)/\(~x\/~z) - x=1, y=1, z=0
// (k\/l)/\(l\/~k)/\(~k\/~l) - k=0, l=1
// (x\/y)/\(x\/~y)/\(~x\/~y)/\(~x\/y)
// (A\/~B) /\ (~A\/~C) /\ (A\/B) /\ (D\/~C) /\ (D\/~A) - A=1, B=0, C=0, D=1

int main() {
  char string[256], error[128];
  printf("Input a 2SAT problem:\n  ");
  scanf("%[^\n]255s", string);

  int literals[LITERALS_COUNT];
  Graph *graph = parse2SatExpression(string, literals, error);
  if (!graph) {
    printf("%s\n", error);
    return 1;
  }

  int sccCount, *scc = getSccList(graph, &sccCount);
  for (int i = 0; i < LITERALS_COUNT; i++) {
    // If literal and it's negation are in the same SCC, stop
    if (scc[i] == scc[i + LITERALS_COUNT]) {
      printf("No solution exists\n");
      return 0;
    }
  }

  Graph *sccGraph = getSccGraphFromList(graph, scc, sccCount);
  int result[sccCount];
  for (int sccIndex = 0; sccIndex < sccCount; sccIndex++) result[sccIndex] = -1;
  for (int sccIndex = 0; sccIndex < sccCount; sccIndex++) {
    if (result[sccIndex] == -1) {
      satDfs(sccGraph, sccIndex, scc, result);
    }
  }

  printf("Solution:\n");
  for (int literal = 0; literal < LITERALS_COUNT; literal++) {
    if (literals[literal]) {
      // ABC -=% abc
      printf("  %c = %d\n",
             literal < 26 ? (literal + 'A') : (literal - 26 + 'a'),
             result[scc[literal]]);
    }
  }
}