#include <stdio.h>

#include "lib/2sat/parse.h"
#include "lib/errorio.h"
#include "lib/graph/Graph.h"
#include "lib/graph/graph-scc.h"

int main() {
  char string[256] = "(x\\/y)/\\(y\\/z)", error[128];
  // scanf("%[^\\n]255s", string);

  Graph *graph = parse2SatExpression(string, error);
  if (!graph) {
    printf("%s\n", error);
    return 1;
  }

  int *scc = getSccList(graph);
  
}