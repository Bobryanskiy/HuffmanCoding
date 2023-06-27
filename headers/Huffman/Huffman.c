#include "Huffman.h"
#include <stdio.h>
#include <string.h>
#include "../HuffmanTable/HuffmanTable.h"

FILE* encode(int* chars, TABLELIST* table, FILE* file, char* fileName) {
    FILE* encoded = fopen(strcat(fileName, "enc"), "w+b");
    rewind(file);
    unsigned char c;
    TABLENODE* node = table->first;
    int d = 0;
    unsigned char ch;
    for (int i = 0; i < 256; ++i) {
        if (chars[i] != 0) {
            char code[8] = { 0 };
            int temp, index = 0;
            temp = i;
            while(temp != 0) {
                code[index] = (temp % 2) + '0';
                temp /= 2;
                index++;
            }
            char codef[8] = { 0 };
            int i2;
            for (i2 = 0; i2 < 8 - index; ++i2)
                codef[i2] = '0';
            for (int j = i2; j < 8; ++j)
                codef[j] = code[--index];
            writeByte(encoded, codef);

            // d = i;
            // printf("\n\n\n\n%c-%hhu\n", i, d);
            fprintf(encoded, "%d\n", chars[i]);
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
    fseek(file, 0L, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
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
    char code[8] = { 0 };
    int temp, index = 0;
    temp = d;
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
    writeByte(encoded, codef);
    return encoded;
}