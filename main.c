#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BinaryTree.h"
#include "PriorityQueue.h"

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

int main() {
    char buffer[300];
    char command[7];
    char fileName[255+3];
    // int arr[11] = { 3, 5, 8, 10, 17, 11, 13, 19, 22, 24, 29 };
    // NODE* root = treeFromArray(arr, 11);
    // print_on_left_side(root, 0);
    // printf("\n");
    // printf("\n");
    // printf("\n");
    // int size;
    // int* arr2 = arrayFromTree(root, &size);
    // constructHeap(arr, 11, 0);
    // NODE* root2 = treeFromArray(arr, 11);
    // print_on_left_side(root2, 0);
    
    // QUEUE* queue = initQueue(256, 1);
    // addToQueue(queue, 9);
    // addToQueue(queue, 3);
    // addToQueue(queue, 7);
    // addToQueue(queue, 1);
    // addToQueue(queue, 5);
    // addToQueue(queue, 4);
    // addToQueue(queue, 2);
    // int dd = deleteFromQueue(queue, 3);
    // printf("%d\n", dd);
    //     printf("%d\n", queue->size);
    // for (int i = 0; i < queue->size; ++i) {
    //     printf("%d ", queue->queue[i]);
    // }


    again:
    gets(buffer);
    int d = sscanf(buffer, "%s \"%s\"", command, fileName);
    fileName[strlen(fileName) - 1] = 0;
    if (d != 2) {
        printf("Wrong command format - encode/decode \"{file name}\"\n");
        goto again;
    }
    FILE* file = fopen(fileName, "r+b");
    if (file == NULL) {
        printf("File open error\n");
        goto again;
    }
    if (!strcmp(command, "encode")) {
        char c;
        int chars[256] = { 0 }; 
        while ((c = getc(file)) != EOF) {
            (unsigned char) chars[c]++;
        }
        QUEUE* queue = initQueue(256, 0);
        for (int i = 0; i < 256; ++i) {
            if (chars[i]) {
                NODE* node = createBinaryTree(chars[i], i);
                addToQueue(queue, node);
            }
        }
        while (queue->size) {
            for (int i = 0; i < queue->size; ++i) printf("%c - %d | ", queue->tree[i]->ch, queue->tree[i]->weight);
            printf("\n\n");
            print_on_left_side(queue->tree[0], 0);
            printf("\n\n");
            addToQueue(queue, uniteTwoTrees(deleteFromQueue(queue, 0), deleteFromQueue(queue, 0)));
        }
        FILE* decode = fopen(strcat(fileName, "dec.txt"), "w+b");
    } else if (!strcmp(command, "decode")) {

    } else {
        printf("Wrong command - encode/decode \"{file name}\"\n");
        goto again;
    }
    return 0;
}