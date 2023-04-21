#include "HuffmanTable.h"
#include <stdlib.h>
#include <string.h>

void symmetric(const NODE* root, TABLELIST** table, int last, char code[256]) {
    if (root) {
        if (root->left == NULL && root->right == NULL) {
            code[last] = 0;
            TABLENODE* temp = malloc(sizeof(TABLENODE));
            temp->code = malloc(sizeof(char) * (strlen(code) + 1));
            strcpy(temp->code, code);
            temp->letter = root->ch;
            temp->next = NULL;
            if ((*table)->first == NULL) {
                (*table)->first = (*table)->last = temp;
            } else {
                (*table)->last->next = temp;
                (*table)->last = temp;
            }
        }
        code[last] = '0';
		symmetric(root->left, table, last + 1, code);
        code[last] = '1';
		symmetric(root->right, table, last + 1, code);
	}
}

TABLELIST* buildTable(NODE* tree) {
    TABLELIST* table = malloc(sizeof(table));
    table->first = NULL;
    table->last = NULL;
    char code[256];
    symmetric(tree, &table, 0, code);
    return table;
}

void writeByte(FILE* out, char* code) { 
    BYTETOBIT byte;
    // byte.bits.b0 = (code[0] == '1') ? 1 : 0;
    // byte.bits.b1 = (code[1] == '1') ? 1 : 0;
    // byte.bits.b2 = (code[2] == '1') ? 1 : 0;
    // byte.bits.b3 = (code[3] == '1') ? 1 : 0;
    // byte.bits.b4 = (code[4] == '1') ? 1 : 0;
    // byte.bits.b5 = (code[5] == '1') ? 1 : 0;
    // byte.bits.b6 = (code[6] == '1') ? 1 : 0;
    // byte.bits.b7 = (code[7] == '1') ? 1 : 0;
    byte.bits.b0 = code[0] + '0';
    byte.bits.b1 = code[1] + '0';
    byte.bits.b2 = code[2] + '0';
    byte.bits.b3 = code[3] + '0';
    byte.bits.b4 = code[4] + '0';
    byte.bits.b5 = code[5] + '0';
    byte.bits.b6 = code[6] + '0';
    byte.bits.b7 = code[7] + '0';
    fwrite(&byte, sizeof(char), 1, out);
}

int addBits(char (*byte)[8], char* code, int* curLen, FILE* encoded) {
    int d;
    strncat(*byte, code, d = (8 - *curLen));
    *curLen = strlen(*byte);
    if (*curLen == 8) {
        writeByte(encoded, *byte);
        for (int i = 0; i < 8; ++i) (*byte)[i] = 0;
        *curLen = 0;
    }
    return d;
}