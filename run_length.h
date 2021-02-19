#ifndef RUN_LENGTH_H
#define RUN_LENGTH_H
#include <stdio.h>

void runlength_decompress(FILE *in, FILE *out);
void runlength_compress(FILE *in, FILE *out);

#endif