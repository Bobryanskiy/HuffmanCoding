#pragma once

typedef struct Node {
    int number;
    struct Node* left, *right;
}NODE;

NODE* createBinaryTree(NODE* root, int val);
NODE* addToTree(NODE* root, int value);
NODE* deleteFromTree(NODE* root, int value);
int* arrayFromTree(NODE* root, int* size);
NODE* treeFromArray(int* array, int size);
