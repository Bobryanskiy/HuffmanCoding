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
        unsigned char c;
        int chars[257] = { 0 };
        int kkk = 0;
        fseek(file, 0L, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        for (int i = 0; i < length; ++i) {
            chars[(unsigned char) fgetc(file)]++;
        }
        QUEUE* queue = initQueue(256, 0);
        for (int i = 0; i < 256; ++i) {
            if (chars[i]) {
                // printf("%c%d\n", i, chars[i]);
                NODE* node = createBinaryTree(chars[i], i);
                addToQueue(queue, node);
            }
        }
        while (queue->size - 1) {
            addToQueue(queue, uniteTwoTrees(deleteFromQueue(queue, 0), deleteFromQueue(queue, 0)));
        }
        TABLELIST* table = buildTable(queue->tree[0]);
        // printf("\n");
        // print_on_left_side(queue->tree[0], 0);
        fclose(encode(chars, table, file, fileName));
    } else if (!strcmp(command, "decode")) {
        char fN2[256] = { 0 };
        fN2[0] = '2';
        strncpy(&fN2[1], fileName, strlen(fileName) - 3);
        FILE* decoded = fopen(fN2, "w+b");
        BYTETOBIT byte;
        unsigned char c;
        fseek(file, -1L, SEEK_END);
        int size = 0;
        c = (unsigned char) fgetc(file);
        byte.byte = c;
        size += 128 * byte.bits.b7;
        size += 64 * byte.bits.b6;
        size += 32 * byte.bits.b5;
        size += 16 * byte.bits.b4;
        size += 8 * byte.bits.b3;
        size += 4 * byte.bits.b2;
        size += 2 * byte.bits.b1;
        size += byte.bits.b0;
        if (size == 0) size = 256;
        fseek(file, 0, SEEK_SET);
        unsigned char* chars = malloc(sizeof(unsigned char) * size);
        int* charss = malloc(sizeof(int) * size);
        for (int i = 0; i < size; ++i) {
            char t;
            chars[i] = (unsigned char) fgetc(file);
            fscanf(file, "%d", &charss[i]);
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
        // printf("\n");
        // print_on_left_side(queue->tree[0], 0);
        int count = 0;
        long length = ftell(file);
        fseek(file, -1L, SEEK_END);
        long length2 = ftell(file);
        fseek(file, length, SEEK_SET);
        length2 -= length;
        NODE* node = queue->tree[0];
        for (int i = 0; i < length2; ++i) {
            c = (unsigned char) fgetc(file);
            byte.byte = c;
            // printf("%c - ", c);
            // printf("%c%c%c%c%c%c%c%c\n", byte.bits.b0 + '0', byte.bits.b1 + '0', byte.bits.b2 + '0', byte.bits.b3 + '0', byte.bits.b4 + '0', byte.bits.b5 + '0', byte.bits.b6 + '0', byte.bits.b7 + '0');    
            node = byte.bits.b7 ? node->right : node->left;
            if (node->isLeaf != 0) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b6 ? node->right : node->left;
            if (node->isLeaf != 0) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b5 ? node->right : node->left;
            if (node->isLeaf != 0) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b4 ? node->right : node->left;
            if (node->isLeaf != 0) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b3 ? node->right : node->left;
            if (node->isLeaf != 0) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b2 ? node->right : node->left;
            if (node->isLeaf != 0) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b1 ? node->right : node->left;
            if (node->isLeaf != 0) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
            node = byte.bits.b0 ? node->right : node->left;
            if (node->isLeaf != 0) {
                fprintf(decoded, "%c", node->ch);
                node = queue->tree[0];
                count++;
            }
            if (count >= mxc) break;
        }
        fclose(decoded);
    } else {
        printf("Wrong command - encode/decode \"{file name}\"\n");
        goto again;
    }
    fclose(file);
    return 0;
}