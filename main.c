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
        encode(chars, table, file, fileName);
    } else if (!strcmp(command, "decode")) {
        FILE* decoded = fopen(strcat(fileName, "dec"), "w+b");
        char c;
        BYTETOBIT byte;
        fseek(file, -1L, SEEK_END);
        int size;
        fscanf(file, "%d", &size);
        // printf("%d\n", size);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        char* chars = malloc(sizeof(char) * size);
        int* charss = malloc(sizeof(int) * size);
        for (int i = 0; i < size; ++i) {
            fscanf(file, "%c%d", &chars[i], &charss[i]);
            fseek(file, 1L, SEEK_CUR);
        }
        QUEUE* queue = initQueue(size, 0);
        
        int mxc = 0;
        for (int i = 0; i < size; ++i) {
            NODE* node = createBinaryTree(charss[i], chars[i]);
            addToQueue(queue, node);
            mxc += charss[i];
        }
        while (queue->size - 1) {
            addToQueue(queue, uniteTwoTrees(deleteFromQueue(queue, 0), deleteFromQueue(queue, 0)));
        }
        int count = 0;
        for (int i = 0; i < length - size * 3 - 1; ++i) {
            c = getc(file);
            byte.byte = c;
            printf("%c - ", c);
            printf("%c%c%c%c%c%c%c%c\n", byte.bits.b0 + '0', byte.bits.b1 + '0', byte.bits.b2 + '0', byte.bits.b3 + '0', byte.bits.b4 + '0', byte.bits.b5 + '0', byte.bits.b6 + '0', byte.bits.b7 + '0');
            NODE* node = queue->tree[0];
            node = byte.bits.b0 ? node->right : node->left;
            if (node->ch != -1) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b1 ? node->right : node->left;
            if (node->ch != -1) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b2 ? node->right : node->left;
            if (node->ch != -1) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b3 ? node->right : node->left;
            if (node->ch != -1) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b4 ? node->right : node->left;
            if (node->ch != -1) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b5 ? node->right : node->left;
            if (node->ch != -1) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b6 ? node->right : node->left;
            if (node->ch != -1) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b7 ? node->right : node->left;
            if (node->ch != -1) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
        }
    } else {
        printf("Wrong command - encode/decode \"{file name}\"\n");
        goto again;
    }
    return 0;
}