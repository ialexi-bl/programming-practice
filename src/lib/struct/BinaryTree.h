#pragma once

#ifndef BINARY_NODE_FIELDS
#define BINARY_NODE_FIELDS
#endif

typedef struct BinaryNode {
  struct BinaryNode *left, *right;
  BINARY_NODE_FIELDS
} BinaryNode;

BinaryNode *createBinaryNode();
BinaryNode *assignLeft(BinaryNode *root);
BinaryNode *assignRight(BinaryNode *root);
void freeBinaryTree(BinaryNode *root);
void printBinaryTree(BinaryNode *root, void (*printNode)(BinaryNode *));
