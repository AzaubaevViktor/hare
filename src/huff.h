#ifndef HUFF_H
#define HUFF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT_SYMBOLS 256
#define MAX_SIZE_CODE 64

struct Node
{
    struct Node* next;
    struct Node* left;
    struct Node* right;

    size_t data;
    int symbol;
};

struct Code
{
    int size;
    char code[MAX_SIZE_CODE];
};

void printTree(struct Node* node, int n);
void printList(struct Node* head);
struct Node* createNode(size_t data, int symbol);
void insertNodeInList(struct Node** list, struct Node* node);
struct Node* createList(size_t* table);
struct Node* createTree(struct Node* list);
void createCodes(struct Code* codes, struct Node* node, char* buf);
size_t* createTableFrequencies(FILE* file);
void printTable(size_t* table);
void printCodes(struct Code* codes);

#endif // HUFF_H
