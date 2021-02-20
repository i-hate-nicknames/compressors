#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include "cmdutil.h"

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

bool should_compress(int argc, char *argv[]) {
    if (argc < 2) {
        printf("compress argument not found, expected + or -\n");
        exit(1);
    }
    if (strcmp(argv[1], "+")) {
        return false;
    }
    if (strcmp(argv[1], "-")) {
        return true;
    } else {
        printf("compress argument not found, expected + or -, got %s\n", argv[1]);
        exit(1);
    }
}

FILE *get_input(int argc, char *argv[]) {
    if (argc == 0) {
        return stdin;
    }
    return open_or_fail(argv[0], "wb+");
}

FILE *get_output(int argc, char *argv[]) {
    if (argc == 0) {
        printf("too few arguments passed to get_output, expected 2 or more, got %d\n", argc);
        exit(1);
    }
    if (argc == 1) {
        return stdout;
    }
    return open_or_fail(argv[1], "rb");
}

void close_files(FILE *in, FILE *out) {
    if (in != stdin) {
        fclose(in);
    }
    if (out != stdout) {
        fclose(out);
    }
}
