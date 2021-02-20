#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../bitstream/bitstream.h"
#include "../cmdutil/cmdutil.h"

#define MAX_LEN 255

void runlength_decompress(FILE *in, FILE *out) {
  BitWriter *bw = make_writer(out);
  unsigned int current_value = 0;
  int current_len = 0;
  bool written;
  while ((current_len = fgetc(in)) != EOF) {
    for (int i = 0; i < current_len; i++) {
      written = writebit(current_value, bw);
      if (!written) {
        // todo: when error handling is added to the bitstream
        // module, use its error functions to determine the nature
        // of the error
        printf("Error writing file\n");
        exit(1);
      }
    }
    current_value = !current_value;
  }
  close_stream(bw);
}

void runlength_compress(FILE *in, FILE *out) {
  BitReader *br = make_reader(in);
  unsigned int into = 0, current_value = 0;
  unsigned char current_len = 0;
  while (readbit(br, &into)) {
    if (into != current_value) {
      putc(current_len, out);
      current_len = 0;
      current_value = !current_value;
    } else {
      if (current_len == 255) {
        putc(current_len, out);
        putc(0, out);
        current_len = 0;
      }
      
    }
    current_len++;
  }
  putc(current_len, out);
}

int main(int argc, char *argv[]) {
  FILE *in, *out;
  bool compress = should_compress(argc, argv);
  // skip program name and compressing option, pass pointer to the rest of arguments
  in = get_input(argc-2, argv+2);
  out = get_output(argc-2, argv+2);
  if (compress) {
    runlength_compress(in, out);
  } else {
    runlength_decompress(in, out);
  }
  close_files(in, out);
}
