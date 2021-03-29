#pragma once
#include <stdlib.h>

#ifndef QUEUE_FIELDS
#define QUEUE_FIELDS
#endif

typedef struct QueueNode {
  struct QueueNode *next;
  QUEUE_FIELDS;
} QueueNode;

typedef struct Queue {
  QueueNode *start, *end;
} Queue;

Queue *createQueue();
QueueNode *createQueueNode();
QueueNode *queuePop(Queue *queue);
void queuePush(Queue *queue, QueueNode *newNode);
int isQueueEmpty(Queue *queue);
void freeQueue(Queue *queue);