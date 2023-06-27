#include "Huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HuffmanTable/HuffmanTable.h"
#include "../PriorityQueue/PriorityQueue.h"

void convert (FILE* file, int what) {
    char code[8] = { 0 };
    int temp, index = 0;
    temp = what;
    while(temp != 0) {
        code[index] = (temp % 2) + '0';
        temp /= 2;
        index++;
    }
    char codef[8] = { 0 };
    int i;
    for (i = 0; i < 8 - index; ++i)
        codef[i] = '0';
    for (int j = i; j < 8; ++j)
        codef[j] = code[--index];
    writeByte(file, codef);
}

FILE* encode(FILE* file, char* fileName) {
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

    FILE* encoded = fopen(strcat(fileName, "enc"), "w+b");
    rewind(file);
    TABLENODE* node = table->first;
    int d = 0;
    unsigned char ch;
    for (int i = 0; i < 256; ++i) {
        if (chars[i] != 0) {
            fprintf(encoded, "%c", i);
            // fprintf(encoded, "%d\n", chars[i]);
            int dm = chars[i];
            int count = 0;
            while (dm != 0) {
                dm /= 256;
                count++;
            }
            convert(encoded, count);
            int temp = chars[i];
            for (int j = 0; j <= chars[i] / 256; ++j) {
                char code[8] = { 0 };
                int index = 0;
                while(temp != 0 && index < 8) {
                    code[index] = (temp % 2) + '0';
                    temp /= 2;
                    index++;
                }
                char codef[8] = { 0 };
                int i2;
                for (i2 = 0; i2 < 8 - index; ++i2)
                    codef[i2] = '0';
                for (int j2 = i2; j2 < 8; ++j2)
                    codef[j2] = code[--index];
                writeByte(encoded, codef);
                // fprintf(encoded, "\n");
            }
            ++d;
        }
    }
    // printf("\n%d\n", d);
    // do {
    //     // записать таблицу в файл (пока не знаю как) (что именно, частоту или код или что-то ещё)
    //     // я не знаю, как показывать, что последний байт незаконченный, поэтому записываем
    //     // символ один байт и следующий байт - кол-во этого символа. пихаем число в двоичный код в строку и записываем в несколько байт, если слишком большое число
    //     // а потом... как-то надо отделить от самого закодированного текста
    //     // а если несколько байт кол-во, то как тогда понимать где что... значит построчно надо записывать..

    //     // записываем кол-во букв, на каждой строчке символ и кол-во его использований
    //     fprintf(encoded, "%c%d\n", node->letter, chars[node->letter]);
    //     ++d;
        // printf("%c-%s\n", node->letter, node->code);
    // } while((node = node->next) != NULL);
    node = table->first;
    char byte[8] = { 0 };
    int curLen = 0;
    int flag = 0;
    char rest[256] = { 0 };
    for (int ik = 0; ik < length; ++ik) {
        c = (unsigned char) getc(file);
        do {
            if (node->letter == c) {
                int d = addBits(&byte, node->code, &curLen, encoded);
                if (d < strlen(node->code)) {
                    int i;
                    for (i = d; i < strlen(node->code); ++i) {
                        rest[i - d] = node->code[i];
                    }
                    rest[i - d] = '\0';
                    flag = 1;
                    int d2 = 0;
                    while (flag) {
                        d2 += addBits(&byte, &rest[d2], &curLen, encoded);
                        if (d2 + d >= strlen(node->code)) flag = 0;
                    }
                }
                break;
                // printf("%s ", node->code);
            }
        } while((node = node->next) != NULL);
        node = table->first;
    }
    if (curLen) {
        // byte[curLen] = '2';
        writeByte(encoded, byte);
    }
    convert(encoded, d);
    return encoded;
}

long power(long num, long deg) {
    long result = 1;

    for(long i = 0; i < deg; i++) {
        result *= num;
    }

    return result;
}

FILE* decode(FILE* file, char* fileName) {
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
        byte.byte = (unsigned char) fgetc(file);
        int n = 0;
        n += 128 * byte.bits.b7;
        n += 64 * byte.bits.b6;
        n += 32 * byte.bits.b5;
        n += 16 * byte.bits.b4;
        n += 8 * byte.bits.b3;
        n += 4 * byte.bits.b2;
        n += 2 * byte.bits.b1;
        n += byte.bits.b0;
        charss[i] = 0;
        for (int j = 0; j < n; ++j) {
            byte.byte = (unsigned char) fgetc(file);
            charss[i] += j > 0 ? power(256, j) * 128 * byte.bits.b7 : 128 * byte.bits.b7;
            charss[i] += j > 0 ? power(256, j) * 64 * byte.bits.b6 : 64 * byte.bits.b6;
            charss[i] += j > 0 ? power(256, j) * 32 * byte.bits.b5 : 32 * byte.bits.b5;
            charss[i] += j > 0 ? power(256, j) * 16 * byte.bits.b4 : 16 * byte.bits.b4;
            charss[i] += j > 0 ? power(256, j) * 8 * byte.bits.b3 : 8 * byte.bits.b3;
            charss[i] += j > 0 ? power(256, j) * 4 * byte.bits.b2 : 4 * byte.bits.b2;
            charss[i] += j > 0 ? power(256, j) * 2 * byte.bits.b1 : 2 * byte.bits.b1;
            charss[i] += j > 0 ? power(256, j) * byte.bits.b0 : 1 * byte.bits.b0;
        }
        // fseek(file, 1L, SEEK_CUR);
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
    return decoded;
}