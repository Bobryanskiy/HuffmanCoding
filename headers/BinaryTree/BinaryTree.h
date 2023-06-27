#pragma once

typedef struct Node {
    int weight;
    char ch;
    int isLeaf;
    struct Node* left, *right;
}NODE;

NODE* createBinaryTree(int val, char ch);
NODE* addToTree(NODE* root, int value, char ch);
NODE* deleteFromTree(NODE* root, int value);
int* arrayFromTree(NODE* root, int* size);
NODE* treeFromArray(int* array, int size, char* chars);
void print_on_left_side(const NODE* root, int level);
NODE* uniteTwoTrees(NODE* tree1, NODE* tree2);
