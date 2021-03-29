#include "Queue.h"

#include <stdlib.h>

Queue *createQueue() {
  Queue *queue = malloc(sizeof(Queue));
  queue->start = queue->end = NULL;
  return queue;
}

QueueNode *createQueueNode() {
  QueueNode *node = malloc(sizeof(QueueNode));
  node->next = NULL;
  return node;
}

QueueNode *queuePop(Queue *queue) {
  if (!queue->start) return NULL;

  QueueNode *first = queue->start;
  if (first == queue->end) {
    queue->start = queue->end = NULL;
  } else {
    queue->start = first->next;
  }
  return first;
}

void queuePush(Queue *queue, QueueNode *newNode) {
  if (queue->end) {
    queue->end = queue->end->next = newNode;
  } else {
    queue->end = queue->start = newNode;
  }
}
int isQueueEmpty(Queue *queue) { return queue->start == NULL; }

void freeQueue(Queue *queue) {
  QueueNode *previous, *current = queue->start;
  if (current) {
    while (current->next) {
      previous = current;
      current = current->next;
      free(previous);
    }
  }
  free(current);
  free(queue);
}
