#ifndef DECODETREE_H
#define DECODETREE_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Tree {
  struct _Tree *parent;
  struct _Tree *left;
  struct _Tree *right;
  bool isTwig;
  char sym; // leaf
} Tree;

bool _getbit(char *str, int pos);
unsigned char _getchar(char *str, int pos);
void _setbit(char *str, int pos, bool bit);
Tree *decodeTree(char *str, int len);

#endif // DECODETREE_H
