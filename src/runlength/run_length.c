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
  unsigned long long total_read = 0;
  while ((current_len = fgetc(in)) != EOF) {
    total_read++;
    for (int i = 0; i < current_len; i++) {
      written = write_bit(current_value, bw);
      if (!written) {
        perror("error writing file");
        exit(1);
      }
    }
    current_value = !current_value;
  }
  close_stream(bw);
  print_stats(total_read, bw->bytes_written);
}

void runlength_compress(FILE *in, FILE *out) {
  BitReader *br = make_reader(in);
  unsigned int into = 0, current_value = 0;
  unsigned char current_len = 0;
  unsigned long long total_written = 0;
  while (read_bit(br, &into)) {
    if (into != current_value) {
      putc(current_len, out);
      current_len = 0;
      current_value = !current_value;
      total_written += 1;
    } else {
      if (current_len == MAX_LEN) {
        putc(current_len, out);
        putc(0, out);
        current_len = 0;
        total_written += 2;
      }
    }
    current_len++;
  }
  putc(current_len, out);
  total_written++;
  print_stats(br->bytes_read, total_written);
}

int main(int argc, char *argv[]) {
  FILE *in, *out;
  bool compress = should_compress(argc, argv);
  // skip program name and compressing option, pass pointer to the rest of arguments
  in = get_input(argc-2, argv+2);
  out = get_output(argc-2, argv+2);
  if (compress) {
    fprintf(stderr, "compressing\n");
    runlength_compress(in, out);
  } else {
    fprintf(stderr, "decompressing\n");
    runlength_decompress(in, out);
  }
  close_files(in, out);
}
