#include <stdio.h>
#include <stdlib.h>
#include "bitstream.h"

typedef struct h_tree {
  struct h_tree *left, *right;
  char val;
  int freq;
} Htree;

Htree *make_node() {
  Htree *node = (Htree *) malloc(sizeof(Htree));
  node->left = NULL;
  node->right = NULL;
  return node;
}

int dump_htree(Htree *tree, BitWriter *bw) {
  return 0;
}

Htree *generate_tree(BitReader *br) {
  return NULL;
}

int main() {
  printf("Huffman be here\n");
  return 0;
}
