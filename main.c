#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
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
        fclose(encode(file, fileName));
    } else if (!strcmp(command, "decode")) {
        fclose(decode(file, fileName));
    } else {
        printf("Wrong command - encode/decode \"{file name}\"\n");
        goto again;
    }
    fclose(file);
    return 0;
}