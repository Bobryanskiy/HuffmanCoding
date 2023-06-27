#pragma once
#include <stdio.h>
#include "../BinaryTree/BinaryTree.h"

typedef struct tableNode {
    char* code;
    unsigned char letter;
    struct tableNode* next;
}TABLENODE;

typedef struct tableList {
    TABLENODE* first;
    TABLENODE* last;
}TABLELIST;

typedef union byteToBit {
    char byte;
    struct bits {
        unsigned b0 : 1;
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
    }bits;
}BYTETOBIT;

void writeByte(FILE* out, char* code);
int addBits(char (*byte)[8], char* code, int* curLen, FILE* encoded);
TABLELIST* buildTable(NODE* tree);
void symmetric(const NODE* root, TABLELIST** table, int last, char code[256]);
