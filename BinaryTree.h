#pragma once

typedef struct Node {
    int weight;
    char ch;
    struct Node* left, *right;
}NODE;

NODE* createBinaryTree(int val, char ch);
NODE* addToTree(NODE* root, int value, char ch);
NODE* deleteFromTree(NODE* root, int value);
int* arrayFromTree(NODE* root, int* size);
NODE* treeFromArray(int* array, int size, char* chars);
