#include "PriorityQueue.h"
#include <stdlib.h>

int maxHeap(int x, int y) {
    return x > y;
}

int minHeap(int x, int y) {
    return x < y;
}

int (*sort[2])(int, int) = {minHeap, maxHeap};

int *heapify(NODE **arr, int size, int subtreeIndex, int maxFlag) {
    int largest = subtreeIndex;
    int right = 2 * subtreeIndex + 2, left = 2 * subtreeIndex + 1;
    if (left < size && sort[maxFlag](arr[left]->weight, arr[largest]->weight))
        largest = left;
    if (right < size && sort[maxFlag](arr[right]->weight, arr[largest]->weight))
        largest = right;
    if (largest != subtreeIndex) {
        NODE* temp = arr[subtreeIndex];
        arr[subtreeIndex] = arr[largest];
        arr[largest] = temp;
        heapify(arr, size, largest, maxFlag);
    }
}

int *constructHeap(NODE** arr, int size, int maxFlag) {
    int lastNonLeafNode = size / 2 - 1;
    for (int i = lastNonLeafNode; i >= 0; --i) {
        heapify(arr, size, i, maxFlag);
    }
}

int *addToQueue(QUEUE *queue, NODE* treee) {
    if (queue->maxSize <= queue->size + 1) {
        NODE** temp = queue->tree;
        queue->tree = malloc(sizeof(NODE) * queue->maxSize * 2 + 1);
        queue->maxSize = queue->maxSize * 2 + 1;
        for (int i = 0; i < queue->size; ++i) {
            queue->tree[i] = temp[i];
        }
        for (int i = queue->size; i < queue->maxSize; ++i) {
            queue->tree[i] = NULL;
        }
        free(temp);
    }
    queue->tree[queue->size++] = treee;
    constructHeap(queue->tree, queue->size, queue->maxFlagSort);
}

QUEUE *initQueue(int size, int maxFlagSort) {
    QUEUE* queue = malloc(sizeof(QUEUE));
    queue->maxFlagSort = maxFlagSort;
    queue->maxSize = size;
    queue->size = 0;
    queue->tree = malloc(sizeof(NODE*) * size);
    for (int i = 0; i < size; ++i) {
        queue->tree[i] = NULL;
    }
    return queue;
}

NODE* deleteFromQueue(QUEUE *queue, int index) {
    NODE* deleted = queue->tree[index];
    queue->tree[index] = queue->tree[queue->size - 1];
    queue->tree[queue->size--] = NULL;
    constructHeap(queue->tree, queue->size, queue->maxFlagSort);
    return deleted;
}
