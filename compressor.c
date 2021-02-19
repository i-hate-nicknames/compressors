#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "run_length.h"

// Processor processes input file and dumps result
// of the processing in the output file
typedef void (*Processor)(FILE *in, FILE *out);

Processor get_compressor(char *name);
Processor get_decompressor(char *name);


static char *usage = "Please, provide an argument: -"
    "for compression, + for decompression\n";

// todo: move to fileutils
// open file identified by name with given mode
// if file cannot be opened, exit the program and
// print errno to standard output
FILE *open_or_fail(char *name, char *mode) {
  FILE *res;
  res = fopen(name, mode);
  if (NULL == res) {
    printf("Cannot open %s, errno: %d\n", name, errno);
    exit(1);
  }
  return res;
}

int main(int argc, char *argv[]) {
  int compress = 0;
  // todo: extract to function and move to fileutils
  FILE *in = stdin, *out = stdout;
  Processor proc;
  if (argc < 2) {
    printf(usage);
    return 1;
  }
  char *arg = argv[1];
  if (0 == strcmp("+", arg)) {
    compress = 0;
  } else if (0 == strcmp("-", arg)) {
    compress = 1;
  } else {
    printf(usage);
    return 1;
  }

  if (argc >= 4) {
    out = open_or_fail(argv[3], "wb+");
    in = open_or_fail(argv[2], "rb");
  } else if (argc == 3) {
    in = open_or_fail(argv[2], "wb+");
  }

  // todo: more compression algorithms be here

  if (compress) {
    proc = get_compressor("runlength");
  } else {
    proc = get_decompressor("runlength");
  }
  proc(in, out);
  return 0;
}

Processor get_decompressor(char *name) {
  return runlength_decompress;
}

Processor get_compressor(char *name) {
  return runlength_compress;
}
