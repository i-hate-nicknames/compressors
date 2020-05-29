#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "compressor.h"

static char *usage = "Please, provide an argument: -"
    "for compression, + for decompression\n";

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
