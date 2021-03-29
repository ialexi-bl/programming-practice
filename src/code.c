typedef enum colour { NONCOL, FIRST, SECOND } colour;
#ifndef LINKEDNODE_FIELDS
#define LINKEDNODE_FIELDS colour node_col;
#endif

#include "lib/graph.h"

#ifndef STACKNODE_TYPE
#define STACKNODE_TYPE List*
#endif

// define the queue element type
#ifndef QUEUENODE_TYPE
#define QUEUENODE_TYPE List*
#endif

#include "lib/queue.h"
#include "lib/stack.h"

// functions to make all NONCOL
void def_fields(graph* g) {
  for (int i = 0; i < g->n; i++) g->list[i].node_col = NONCOL;
}
// function  to work with colour
colour switch_colour(colour current) {
  if (current == FIRST)
    return SECOND;
  else
    return FIRST;
}

// dfs algorithm
int dfs(List* start, graph* g) {
  if (start->node_col == NONCOL) start->node_col = FIRST;
  stack* stack_s = get_stack();
  if (!stack_s) {
    printf("memory wasn't allocated for stack\n");
    return 0;
  }
  graph_print(g);
  if (!push_stack(stack_s, start)) {
    printf("Push_stack was failed\n");
    return 0;
  }
  while (stack_s->head != NULL) {
    List* del_el;
    /*if (!del_el){
        printf("We haven't declare del_el in line : 'List * del_el;\n'");
        return 0;
        break;
    }*/
    pop_stack(stack_s, &del_el);
    if (!del_el) {
      printf("Del_el wasn't exising after pop_stack. line 58\n'");
      return 0;
      break;
    }
    LinkedNode* j = del_el->head;
    colour current = del_el->node_col;
    while (j != NULL) {
      if (g->list[j->number_of_node].node_col == NONCOL) {
        g->list[j->number_of_node].node_col = switch_colour(current);
        if (!push_stack(stack_s, &g->list[j->number_of_node])) {
          printf("Push_stack was failed\n");
          return 0;
        }
      } else if (g->list[j->number_of_node].node_col == current) {
        printf("Error. Bipartite graph isn't existing\n");
        return 0;
      }
      j = j->next;
    }
  }
  free(stack_s);
  return 1;
}
void get_Bipartite_graph_dfs(graph* g) {
  def_fields(g);
  for (int i = 0; i < g->n; i++) {
    if (g->list[i].node_col == NONCOL)
      if (!dfs(&g->list[i], g)) {
        printf("We have a problem\n");
        return;
      }
  }
}

// bfs algorithm
int bfs(List* start, graph* g) {
  if (start->node_col == NONCOL) start->node_col = FIRST;
  queue* queue_q = get_queue();
  if (!queue_q) {
    return 0;
  }
  push_queue(queue_q, start);
  while (queue_q->head != NULL) {
    List* del_el;
    if (!pop_queue(queue_q, &del_el)) {
      printf("Pop_queue was failed\n");
      return 0;
    }
    LinkedNode* j = del_el->head;
    colour current = del_el->node_col;
    while (j != NULL) {
      if (g->list[j->number_of_node].node_col == NONCOL) {
        g->list[j->number_of_node].node_col = switch_colour(current);
        if (!push_queue(queue_q, &g->list[j->number_of_node])) {
          printf("Push_queue was failed\n");
          return 0;
        }
      } else if (g->list[j->number_of_node].node_col == current) {
        printf("Error. Bipartite graph isn't existing\n");
        return 0;
      }
      j = j->next;
    }
  }
  free(queue_q);
  return 1;
}
void get_Bipartite_graph_bfs(graph* g) {
  for (int i = 0; i < g->n; i++) {
    if (g->list[i].node_col == NONCOL)
      if (!bfs(&g->list[i], g)) return;
  }
}

// function to make the example from lesson
graph* make_example_graph() {
  graph* b = graph_init(9);
  add_edge(b, 1, 3);
  add_edge(b, 0, 1);
  add_edge(b, 0, 2);
  add_edge(b, 2, 3);
  add_edge(b, 3, 5);
  add_edge(b, 3, 8);
  add_edge(b, 8, 6);
  add_edge(b, 7, 8);
  def_fields(b);
  return b;
}
// functions to print parts
void print_colour(graph* g, colour colour_t) {
  for (int i = 0; i < g->n; i++)
    if (g->list[i].node_col == colour_t) printf("%d ", i);
}
void print_graph_parts(graph* g) {
  printf("FIRST ");
  print_colour(g, FIRST);
  printf("\nSECOND ");
  print_colour(g, SECOND);
  printf("\n");
}
graph* into_graph(FILE* fl) {
  printf("code.c: %d\n", sizeof(List));
  int n = 0;
  char k;
  while ((k = fgetc(fl)) != '\n') {
    n *= 10;
    n += k - '0';
  }
  graph* g = graph_init(n);
  for (int i = 0; i < n; i++) {
    int t = 0;
    while (((k = fgetc(fl)) != '\n') && (t < n)) {
      if (k == '1') add_arc(g, i, t);
      t++;
    }
  }
  return g;
}

int main() {
  // printf("%ld", (long int)sizeof(LinkedNode));
  printf("Example from the lesson\n");
  // FILE* fl1;
  // if ((fl1 = fopen("input.txt", "r")) == NULL) {
  //   printf("file  wasn't opened\n");
  //   return 0;
  // }
  // graph* b = into_graph(fl1);
  // graph_print(b);
  // fclose(fl1);
  graph* b = make_example_graph();
  if (!b) {
    printf("Making graph was failed!\n");
    return 0;
  }
  graph_print(b);
  get_Bipartite_graph_dfs(b);
  printf("Result of dfs :\n");
  print_graph_parts(b);
  /*
   graph * a = make_example_graph();
   get_Bipartite_graph_bfs(a);
   printf("Result of bfs :\n");
   print_graph_parts(a);*/
  return 0;
}
// gcc ./5_bfs_dfs.c ./graph.c ./stack.c ./queue.c -o bin_vs_code/5_dfs_bfs &&
// ./bin_vs_code/5_dfs_bfs gcc -g ./5_bfs_dfs.c ./graph.c ./stack.c ./queue.c -o
// bin_vs_code/main