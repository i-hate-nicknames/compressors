#ifndef CMDUTIL_H
#define CMDUTIL_H

#include <stdio.h>
#include <stdbool.h>

// Read + or - sign from first argument, given original program
// arguments 
// If there are not enough arguments (argc == 1), or the first argument
// is ont + or -, then exit program and print error
bool should_compress(int argc, char *argv[]);

// Get input file from arguments. Input file is the first argument in
// given argv. If absent (argc == 1), return stdin
// number of elements in argv should be equal to argc
FILE *get_input(int argc, char *argv[]);

// Get output file from arguments. Output file is the second argument in
// given argv. If absent (argc == 2), return stdout
// number of elements in argv should be equal to argc
FILE *get_output(int argc, char *argv[]);

void close_files(FILE *in, FILE *out);

#endif