#ifndef HUFF_H
#define HUFF_H

#include <stdio.h>
#include <stdlib.h>

#define COUNT_SYMBOLS 256

struct Node
{
    struct Node* next;
    struct Node* left;
    struct Node* right;

    size_t data;
    int symbol;
};

void printTree(struct Node* node, int n);
void printList(struct Node* head);
struct Node* createNode(size_t data, int symbol);
void insertNodeInList(struct Node** list, struct Node* node);
struct Node* createList(size_t* table);
struct Node* createTree(struct Node* list);
size_t* createTableFrequencies(FILE* file);
void printTable(size_t* table);

#endif // HUFF_H
