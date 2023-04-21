#include "BinaryTree.h"
#include <stdio.h>
#include <stdlib.h>

NODE **findLast(NODE *root, int *size) {
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

int nodeCount(NODE *root) {
    if (root == NULL)
    return (0);
  return (1 + nodeCount(root->left) + nodeCount(root->right));
}

NODE *createNode(int value, char ch) {
    NODE* node = malloc(sizeof(NODE));
    node->left = node->right = NULL;
    node->weight = value;
    node->ch = ch;
    return node;
}

NODE *createBinaryTree(int val, char ch) {
    NODE* root = createNode(val, ch);
    return root;
}

NODE *addToTree(NODE *root, int value, char ch) {
    int size = 0;
    *findLast(root, &size) = createNode(value, ch);
    return 0; 
}

NODE *deleteFromTree(NODE *root, int value) {
    return 0;
}

NODE *findByIndex(NODE *root, int** arr, int index) {
    if (root->right) {
        (*arr)[(index + 1) * 2] = root->right->weight;
        findByIndex(root->right, arr, (index + 1) * 2);
    }
    if (root->left) {
        (*arr)[index * 2 + 1] = root->left->weight;
        findByIndex(root->left, arr, index * 2 + 1);
    }
    return root;
}

int *arrayFromTree(NODE *root, int *size) {
    int temp = 0;
    int treeSize = nodeCount(root);
    *size = treeSize;
    int* arr = malloc(sizeof(int) * treeSize);
    arr[0] = root->weight;
    findByIndex(root, &arr, 0);
    return 0;
}

NODE *treeFromArray(int* array, int size, char *chars) {
    NODE* root = createBinaryTree(array[0], chars[0]);
    for (int i = 1; i < size; ++i) {
        addToTree(root, array[i], chars[i]);
    }
    return root;
}

void print_on_left_side(const NODE* root, int level)
{
	if (root)
	{
		print_on_left_side(root->right, level + 1);
		for (int i = 0; i < level; ++i)
			printf("\t");
		printf("%d-%c\n", root->weight, root->ch);
		print_on_left_side(root->left, level + 1);
	}
}

NODE* uniteTwoTrees(NODE* tree1, NODE* tree2) {
    NODE* newRoot = malloc(sizeof(NODE));
    newRoot->left = tree1;
    newRoot->right = tree2;
    if (tree1 != NULL) newRoot->weight += tree1->weight;
    if (tree2 != NULL) newRoot->weight += tree2->weight;
    return newRoot;
}
