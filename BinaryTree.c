#include "BinaryTree.h"
#include <stdlib.h>
#include <stdio.h>

NODE **findLast(NODE *root, int* size) {
    *size += 1;
    if (root->left == NULL) {    
        return &root->left;
    }
    if (root->right == NULL) {
        return &root->right;
    }
    int temp = *size;
    NODE** node1 = findLast(root->left, size);
    int size2 = *size;
    *size = temp;
    NODE** node2 = findLast(root->right, size);
    if (size2 <= *size) {
        *size = size2;
        return node1;
    } else {
        return node2;
    }
}

int nodeCount(NODE *node) {
    if (node->left == NULL && node->right == NULL)
        return 1;
    int left, right;
    if (node->left != NULL)
        left = nodeCount(node->left);
    else
        left = 0;
    if (node->right != NULL)
        right = nodeCount(node->right); 
    else 
        right = 0;
    return left + right + 1;
}

NODE *createNode(int value) {
    NODE* node = malloc(sizeof(NODE));
    node->left = node->right = NULL;
    node->number = value;
    return node;
}

NODE *createBinaryTree(NODE *root, int val) {
    root = createNode(val);
    return root;
}

NODE *addToTree(NODE *root, int value) {
    int size = 0;
    *findLast(root, &size) = createNode(value);
    return 0; 
}

NODE *deleteFromTree(NODE *root, int value) {
    return 0;
}

NODE *findByIndex(NODE *root, int** arr, int index) {
    if (root->right) {
        (*arr)[(index + 1) * 2] = root->right->number;
        findByIndex(root->right, arr, (index + 1) * 2);
    }
    if (root->left) {
        (*arr)[index * 2 + 1] = root->left->number;
        findByIndex(root->left, arr, index * 2 + 1);
    }
}

int *arrayFromTree(NODE *root, int *size) {
    int temp = 0;
    int treeSize = nodeCount(root);
    *size = treeSize;
    int* arr = malloc(sizeof(int) * treeSize);
    arr[0] = root->number;
    findByIndex(root, &arr, 0);
    for (int i = 0; i < treeSize; ++i) printf("%d ", arr[i]);
    printf("\n");
    return 0;
}

NODE *treeFromArray(int* array, int size) {
    NODE* root = malloc(sizeof(NODE));
    root = createBinaryTree(root, array[0]);
    for (int i = 1; i < size; ++i) {
        addToTree(root, array[i]);
    }
    return root;
}
