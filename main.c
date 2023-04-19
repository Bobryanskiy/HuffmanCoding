#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BinaryTree.h"

void print_on_left_side(const NODE* root, int level)
{
	if (root)
	{
		print_on_left_side(root->right, level + 1);
		for (int i = 0; i < level; ++i)
			printf("\t");
		printf("%d\n", root->number);
		print_on_left_side(root->left, level + 1);
	}
}

int main() {
    char buffer[300];
    char command[7];
    char fileName[255+3];
    int arr[10] = { 1, 2, 3, 10, 4, 5, 5, 9, 14, 0 };
    NODE* root = treeFromArray(arr, 10);
    print_on_left_side(root, 0);
    printf("\n");
    int size;
    int* arr2 = arrayFromTree(root, &size);
    again:
    gets(buffer);
    int d = sscanf(buffer, "%s \"%s\"", command, fileName);
    fileName[strlen(fileName) - 1] = 0;
    if (d != 2) {
        printf("Wrong command format - encode/decode \"{file name}\"\n");
        goto again;
    }
    FILE* file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("File open error\n");
        goto again;
    }
    if (!strcmp(command, "encode")) {
        
    } else if (!strcmp(command, "decode")) {

    } else {
        printf("Wrong command - encode/decode \"{file name}\"\n");
        goto again;
    }
    printf("%d %s - %s\n", d, command, fileName);
    return 0;
}