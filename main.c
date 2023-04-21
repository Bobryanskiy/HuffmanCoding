#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/BinaryTree/BinaryTree.h"
#include "headers/PriorityQueue/PriorityQueue.h"
#include "headers/HuffmanTable/HuffmanTable.h"
#include "headers/Huffman/Huffman.h"

// осталось записать табличку вначале в кодированном файле и сделать чтение(декодирование) этого. так же добавляем узлы(деревья) в очередь,
// а потом идем по дереву в зависимости от цифры направо или налево, если встречается крайний узел (без детей), то получаем число и идем обратно в начало дерева
int main() {
    char buffer[300];
    char command[7];
    char fileName[255+3];
    again:
    fgets(buffer, 300, stdin);
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
        while (queue->size - 1) {
            addToQueue(queue, uniteTwoTrees(deleteFromQueue(queue, 0), deleteFromQueue(queue, 0)));
        }
        TABLELIST* table = buildTable(queue->tree[0]);
        encode(table, file, fileName);
    } else if (!strcmp(command, "decode")) {
        char c;
        BYTETOBIT byte;
        fseek(file, 0L, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        for (int i = 0; i < length; ++i) {
            c = getc(file);
            byte.byte = c;
            printf("%c - ", c);
            printf("%c%c%c%c%c%c%c%c\n", byte.bits.b0 + '0', byte.bits.b1 + '0', byte.bits.b2 + '0', byte.bits.b3 + '0', byte.bits.b4 + '0', byte.bits.b5 + '0', byte.bits.b6 + '0', byte.bits.b7 + '0');
        }
    } else {
        printf("Wrong command - encode/decode \"{file name}\"\n");
        goto again;
    }
    return 0;
}