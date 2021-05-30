#include <stdio.h>
#include <stdlib.h>

#define BINARY_NODE_FIELDS int value;
#include "lib/struct/BinaryTree.h"

void printNode(BinaryNode *node) { printf("%d ", node->value); }

int main() {
  printf("1-tree-sort.c: %d\n", sizeof(BinaryNode));
  int n, value;
  scanf("%d", &n);

  scanf("%d\n", &value);
  BinaryNode *root = createBinaryNode(), *current;
  root->value = value;

  for (int i = 0; i < n - 1; i++) {
    scanf("%d", &value);
    BinaryNode *new = createBinaryNode();
    new->value = value;

    current = root;
    while (1) {
      if (value < current->value) {
        if (current->left == NULL) {
          current->left = new;
          break;
        } else {
          current = current->left;
        }
      } else {
        if (current->right == NULL) {
          current->right = new;
          break;
        } else {
          current = current->right;
        }
      }
    }
  }

  printBinaryTree(root, printNode);
  printf("\n");
  freeBinaryTree(root);
}