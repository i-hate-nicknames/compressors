#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bitstream.h"

// A huffman tree node
typedef struct h_tree {
  // used in internal nodes
  struct h_tree *left, *right;
  // used in leaf nodes to encode characters
  char val;
  // frequency of the character in the stream, used
  // in leaf nodes during construction
  int freq;
} Htree;

Htree *make_node() {
  Htree *node = (Htree *) malloc(sizeof(Htree));
  node->left = NULL;
  node->right = NULL;
  node->freq = 0;
  node->val = 0;
  return node;
}

bool is_leaf(Htree *tree) {
  return tree->left == NULL && tree->right == NULL;
}

int dump_htree(Htree *tree, BitWriter *bw) {
  return 0;
}

Htree *generate_tree(BitReader *br) {
  return NULL;
}

void expand(BitReader *br, BitWriter *bw, Htree *tree) {
  Htree *root = tree;
  unsigned int bit;
  bool end = false;
  while ((end = read_bit(br, bit))) {
    for (Htree *node = root; !is_leaf(node); end = read_bit(br, bit)) {
      if (end) {
        fprintf(stderr, "Error: invalid encoding, unexpected stream ending\n");
        exit(1);
      }
      if (bit == 0) {
        node = node->left;
      } else {
        node = node->right;
      }
    }
    if (!end) {
      write_char(node->val, bw);
    }
  }
}

int main() {
  printf("Huffman be here\n");
  return 0;
}
