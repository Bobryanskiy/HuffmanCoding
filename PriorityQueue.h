#pragma once
#include "BinaryTree.h"

typedef struct Queue {
    // int *queue;
    // struct queue* queue;
    NODE** tree;
    int maxSize;
    int size;
    int maxFlagSort;
}QUEUE;

int* constructHeap(NODE** arr, int size, int maxFlag);
int* addToQueue(QUEUE* queue, NODE* treee);
QUEUE* initQueue(int size, int maxFlagSort);
NODE* deleteFromQueue(QUEUE* queue, int index);