#include <stdio.h>
#include <string.h>
#include "bitstream.h"
#include "compressor.h"

#define MAX_LEN 255

void runlength_decompress(FILE *in, FILE *out) {
  BitWriter *bw = make_writer(out);
  unsigned int current_value = 0;
  int current_len = 0;
  while ((current_len = fgetc(in)) != EOF) {
    for (int i = 0; i < current_len; i++) {
      writebit(current_value, bw);
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
