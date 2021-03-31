#include "parse.h"

#include <stdlib.h>
#include <string.h>

#include "../errorio.h"
#define LITERALS_COUNT 26 * 2

static char *tokenize(char *string, char *error) {
  char *tokens = malloc(sizeof(char) * strlen(string));
}

static int parseDisjunction(Graph *graph, int *i, char *string, char **error) {
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
          sprintfError(error, string, *i, "Expected '/' after '\\'");
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
      default: {
        if (!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))) {
          sprintfError(error, string, *i, "Unexpected token '%c'", c);
          return 0;
        }
        if (disj ? (lit2 != -1) : (lit1 != -1)) {
          sprintfError(error, string, *i, "Unexpected literal '%c'", c);
          return 0;
        }

        int lit = c >= 'a' ? c - 'a' : c - 'A';
        if (disj) {
          lit2 = lit;
        } else {
          lit1 = lit;
        }
      }
    }
  }
interrupt:;

  linkDirNodes(graph, lit1 + LITERALS_COUNT * !neg1,
               lit2 + LITERALS_COUNT * neg2);
  linkDirNodes(graph, lit2 + LITERALS_COUNT * !neg2,
               lit1 + LITERALS_COUNT * neg1);
  return 1;
}
// (x\/y)/\(x\/y)

Graph *parse2SatExpression(char *string, char *error) {
  // There are LITERALS_COUNT possible literals, each can be negated
  // First half of vertices are normal literals, second - negated ones
  Graph *graph = createGraph(LITERALS_COUNT * 2);

  int lastType = 1;
  int i = -1;
  char c;
  while ((c = string[++i])) {
    if (c == '(' && lastType == 1) {
      int success = parseDisjunction(graph, &i, string, error);
      if (!success) {
        freeGraph(graph);
        return NULL;
      }
      lastType = 0;
    } else if (c == '/' && lastType == 0) {
      if (string[i + 1] != '/') {
        sprintfError(error, string, i, "Expected '\\' after '/'");
      }
      i++;
      lastType = 1;
    } else {
      sprintfError(error, string, i, "Unexpected token '%c'", c);
      freeGraph(graph);
      return NULL;
    }
  }

  if (lastType == 1) {
    sprintfError(error, string, i - 1, "Unexpected end of input");
    freeGraph(graph);
    return NULL;
  }

  return graph;
}