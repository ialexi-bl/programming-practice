#include "../graph/Graph.h"
#define LITERALS_COUNT 26 * 2

Graph *parse2SatExpression(char *string, int literals[LITERALS_COUNT],
                           char *error);