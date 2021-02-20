#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include "cmdutil.h"

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
    FILE *res = fopen(argv[0], "rb");
    if (res == NULL) {
        perror("error opening input file");
        exit(1);
    }
    return res;
}

FILE *get_output(int argc, char *argv[]) {
    if (argc < 2) {
        return stdout;
    }
    FILE *res = fopen(argv[1], "wb");
    if (res == NULL) {
        perror("error opening output file");
        exit(1);
    }
    return res;
}

void close_files(FILE *in, FILE *out) {
    if (in != stdin && fclose(in) != 0) {
        perror("error closing input");
    }
    if (fflush(out) != 0) {
        perror("error flushing output");
    }
    if (out != stdout && fclose(out) != 0) {
        perror("error closing output");
    }
}
