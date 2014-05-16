#ifndef DECODETREE_H
#define DECODETREE_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Tree {
  struct _Tree *parent;
  struct _Tree *left;
  struct _Tree *right;
  bool type; // 0,1 -- leaf, twig
  char code[16];
  int codeLen;
  char sym; // leaf
} Tree;

bool _getbit(char *str, int pos);
Tree *decodeTree(char *str, int len);

#endif // DECODETREE_H
