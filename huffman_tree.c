#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
  while ((end = readbit(br, &bit))) {
    Htree *node = root;
    for (; !is_leaf(node); end = readbit(br, &bit)) {
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
    writechar(node->val, bw);
  }
}

// -------------------------
// Encoding
// -------------------------

static char *table[256];

char *append_char(char *path, char val);

void encode_node(char *table[256], Htree *node, char *path);

// populate given table with encodings for every character
// in the tree.
// After that, table contains null terminated encodings for every character
// for example, table['a'] contains a null-terminated string
// of '0' and '1' that is used to encode character 'a'
// Characters not present in the tree will not be changed in the table,
// so to guarantee the table is valid, it should be initialized to
// NULL for every entry
void encode_tree(char *table[256], Htree *tree) {
  // make empty string on the heap
  char *empty = (char *) malloc(1);
  empty[0] = '\0';
  encode_node(table, tree, empty);
}

// encode given node and put entries in the table.
// If node is a leaf, add a character entry to the table with the path
// If node is an internal node, encode all children and prepend path
// to all encodings
// Effectively, path is the way to get up to this node
void encode_node(char *table[256], Htree *node, char *path) {
  if (is_leaf(node)) {
    table[node->val] = path;
    return;
  }
  char *path_left = append_char(path, '0');
  char *path_right = append_char(path, '1');
  // we only want to store leaf node encodings, so we don't need
  // previous path anymore. Newly created paths are guaranteed
  // to be either stored or freed by recursive calls
  free(path);
  encode_node(table, node->left, path_left);
  encode_node(table, node->right, path_right);
}

// append given character to the path, return a newly allocated
// string that ends with the given character.
char *append_char(char *path, char val) {
  int len = strlen(path);
  char *result = (char *) malloc(len+2);
  strcpy(result, path);
  result[len] = val;
  result[len+1] = '\0';
  return result;
}

int main() {
  Htree *root = make_node();
  Htree *l = make_node();
  Htree *r = make_node();
  Htree *ll = make_node();
  Htree *lr = make_node();
  root->left = l;
  root->right = r;
  l->left = ll;
  l->right = lr;
  r->val = 'a';
  ll->val = 'b';
  lr->val = 'c';
  encode_tree(table, root);
  printf("Encoding %c: %s\n", 'a', table['a']);
  printf("Encoding %c: %s\n", 'b', table['b']);
  printf("Encoding %c: %s\n", 'c', table['c']);
  return 0;
}
