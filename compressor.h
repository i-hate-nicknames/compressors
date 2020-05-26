#ifndef COMPRESSOR_H
#define COMPRESSOR_H

typedef void (*Processor)(FILE *in, FILE *out);

Processor get_compressor(char *name);
Processor get_decompressor(char *name);

void runlength_decompress(FILE *in, FILE *out);
void runlength_compress(FILE *in, FILE *out);

#endif
