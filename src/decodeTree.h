#ifndef DECODETREE_H
#define DECODETREE_H

#include <stdlib.h>

typedef struct _Tree {
  struct _Tree *left;
  struct _Tree *right;
  bool type; // 0 -- leaf, twig
  union {
    bool code; // twig
    char sym; // leaf
  };
} Tree;

#endif // DECODETREE_H
