#include "Huffman.h"
#include <stdio.h>
#include <string.h>
#include "../HuffmanTable/HuffmanTable.h"

FILE* encode(TABLELIST* table, FILE* file, char* fileName) {
    FILE* encoded = fopen(strcat(fileName, "enc.txt"), "w+b");
    rewind(file);
    char c;
    TABLENODE* node = table->first;
    do {
        // записать таблицу в файл (пока не знаю как) (что именно, частоту или код или что-то ещё)
        // я не знаю, как показывать, что последний байт незаконченный, поэтому записываем
        // символ один байт и следующий байт - кол-во этого символа. пихаем число в двоичный код в строку и записываем в несколько байт, если слишком большое число
        // а потом... как-то надо отделить от самого закодированного текста
        // а если несколько байт кол-во, то как тогда понимать где что... значит построчно надо записывать..

        // записываем кол-во букв, на каждой строчке символ и кол-во его использований

    } while((node = node->next) != NULL);
    node = table->first;
    char byte[8] = { 0 };
    int curLen = 0;
    int flag = 0;
    char rest[256] = { 0 };
    while ((c = getc(file)) != EOF) {
        do {
            if (node->letter == c) {
                int d = addBits(&byte, node->code, &curLen, encoded);
                if (d < strlen(node->code)) {
                    flag = 1;
                    while (flag) {
                        for (int i = 0; i < strlen(node->code); ++i) {
                            rest[i] = node->code[i + d];
                        }
                        int d = addBits(&byte, rest, &curLen, encoded);
                        if (d >= strlen(rest)) flag = 0;
                    }
                }
                printf("%s ", node->code);
            }
        } while((node = node->next) != NULL);
        node = table->first;
    }
    if (curLen) {
        // byte[curLen] = '2';
        writeByte(encoded, byte);
    }
    return encoded;
}