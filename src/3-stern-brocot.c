#include <math.h>
#include <stdio.h>

#define BINARY_NODE_FIELDS \
  int numerator;           \
  int denominator;
#define QUEUE_FIELDS   \
  BinaryNode *sbtNode; \
  int depth;
#include "lib/struct/BinaryTree.h"
#include "lib/Queue.h"

#define M_PHI 1.618033988749894848204

BinaryNode *buildSbtNode(int n1, int d1, int n2, int d2, int currentDepth,
                         int maxDepth) {
  BinaryNode *node = createBinaryNode();
  int n = n1 + n2, d = d1 + d2;
  node->numerator = n;
  node->denominator = d;

  if (currentDepth < maxDepth) {
    node->left = buildSbtNode(n1, d1, n, d, currentDepth + 1, maxDepth);
    node->right = buildSbtNode(n, d, n2, d2, currentDepth + 1, maxDepth);
  }

  return node;
}

BinaryNode *buildSbt(int depth) { return buildSbtNode(0, 1, 1, 0, 0, depth); }

void printSbt(BinaryNode *root) {
  Queue *queue = createQueue();
  QueueNode *current = createQueueNode();
  current->sbtNode = root;
  current->depth = 0;

  int lastDepth = 0;

  while (current) {
    if (current->depth != lastDepth) {
      printf("\n");
      lastDepth = current->depth;
    }
    printf("%d/%d; ", current->sbtNode->numerator,
           current->sbtNode->denominator);

    if (current->sbtNode->left) {
      QueueNode *left = createQueueNode(), *right = createQueueNode();
      left->sbtNode = current->sbtNode->left;
      right->sbtNode = current->sbtNode->right;
      left->depth = right->depth = lastDepth + 1;
      queuePush(queue, left);
      queuePush(queue, right);
    }
    current = queuePop(queue);
  }

  printf("\n");
  freeQueue(queue);
}

void approximate(BinaryNode *root, double number) {
  while (root->left) {
    // printf("%f, %f\n", number, root->numerator / (double)root->denominator);
    if (number < root->numerator / (double)root->denominator) {
      printf("L");
      root = root->left;
    } else {
      printf("R");
      root = root->right;
    }
  }
  printf("\n");
}

int main() {
  int depth;
  scanf("%d", &depth);

  BinaryNode *sbt = buildSbt(depth - 1);
  // printSbt(sbt);

  printf("Pi approximation:  ");
  approximate(sbt, M_PI);
  printf("E approximation:   ");
  approximate(sbt, M_E);
  printf("Phi approximation: ");
  approximate(sbt, M_PHI);
}