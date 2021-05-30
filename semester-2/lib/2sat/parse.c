#include "parse.h"

#include <stdlib.h>
#include <string.h>

#include "../errorio.h"

static int parseDisjunction(Graph *graph, int literals[LITERALS_COUNT], int *i,
                            char *string, char *error) {
  int lit1 = -1, lit2 = -1;
  int neg1 = 0, neg2 = 0, disj = 0;

  char c;
  while ((c = string[++(*i)])) {
    switch (c) {
      case '~': {
        if (disj ? (lit2 != -1 || neg2) : (lit1 != -1 || neg1)) {
          sprintfError(error, string, *i, "Unexpected token ~");
          return 0;
        }
        if (disj) {
          neg2 = 1;
        } else {
          neg1 = 1;
        }
        break;
      }
      case '\\': {
        if (lit1 == -1 || disj) {
          sprintfError(error, string, *i, "Unexpected token \\");
          return 0;
        }
        if (string[*i + 1] != '/') {
          sprintfError(error, string, *i + 1, "Expected '/' after '\\'");
          return 0;
        }
        disj = 1;
        (*i)++;
        break;
      }
      case ')': {
        if (lit2 == -1) {
          sprintfError(error, string, *i,
                       "Expected two literals and disjunction operator before "
                       "closing parenthesis");
          return 0;
        }
        goto interrupt;
      }
      case ' ':
        break;
      default: {
        if (!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))) {
          sprintfError(error, string, *i, "Unexpected token '%c'", c);
          return 0;
        }
        if (disj ? (lit2 != -1) : (lit1 != -1)) {
          sprintfError(error, string, *i, "Unexpected literal '%c'", c);
          return 0;
        }

        int lit = c >= 'a' ? c - 'a' + 26 : c - 'A';
        if (disj) {
          lit2 = lit;
        } else {
          lit1 = lit;
        }
      }
    }
  }
interrupt:;

  literals[lit1] = literals[lit2] = 1;
  linkDirNodes(graph, lit1 + LITERALS_COUNT * !neg1,
               lit2 + LITERALS_COUNT * neg2);
  linkDirNodes(graph, lit2 + LITERALS_COUNT * !neg2,
               lit1 + LITERALS_COUNT * neg1);
  return 1;
}
// (x\/y)/\(x\/y)

Graph *parse2SatExpression(char *string, int literals[LITERALS_COUNT],
                           char *error) {
  // There are LITERALS_COUNT possible literals, each can be negated
  // First half of vertices are normal literals, second - negated ones
  Graph *graph = createGraph(LITERALS_COUNT * 2);
  for (int i = 0; i < LITERALS_COUNT; i++) literals[i] = 0;

  int lastType = 1;
  int i = -1;
  char c;
  while ((c = string[++i])) {
    if (c == '(' && lastType == 1) {
      int success = parseDisjunction(graph, literals, &i, string, error);
      if (!success) {
        freeGraph(graph);
        return NULL;
      }
      lastType = 0;
    } else if (c == '/' && lastType == 0) {
      if (string[i + 1] != '\\') {
        sprintfError(error, string, i + 1, "Expected '\\' after '/'");
        freeGraph(graph);
        return NULL;
      }
      i++;
      lastType = 1;
    } else if (c != ' ') {
      sprintfError(error, string, i, "Unexpected token '%c'", c);
      freeGraph(graph);
      return NULL;
    }
  }

  if (lastType == 1) {
    sprintfError(error, string, i, "Unexpected end of input");
    freeGraph(graph);
    return NULL;
  }

  return graph;
}