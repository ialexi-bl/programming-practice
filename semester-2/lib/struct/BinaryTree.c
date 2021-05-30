#include "BinaryTree.h"

#include <stdio.h>
#include <stdlib.h>

BinaryNode *createBinaryNode() {
  printf("BinaryTree.c: %ld\n", sizeof(BinaryNode));
  BinaryNode *node = malloc(sizeof(BinaryNode));
  node->left = node->right = NULL;
  return node;
}

BinaryNode *assignLeft(BinaryNode *root) {
  BinaryNode *newNode = createBinaryNode();
  root->left = newNode;
  return newNode;
}

BinaryNode *assignRight(BinaryNode *root) {
  BinaryNode *newNode = createBinaryNode();
  root->right = newNode;
  return newNode;
}

void freeBinaryTree(BinaryNode *root) {
  if (root->left) freeBinaryTree(root->left);
  if (root->right) freeBinaryTree(root->right);
  free(root);
}

void printBinaryTree(BinaryNode *root, void (*printNode)(BinaryNode *)) {
  if (root->left) printNode(root->left);
  printNode(root);
  if (root->right) printNode(root->right);
}
