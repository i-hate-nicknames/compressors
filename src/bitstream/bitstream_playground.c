#include <stdio.h>
#include <string.h>
#include "bitstream.c"

char *test_file = "test";

void write_some_bits() {
  FILE *fp = fopen(test_file, "w");
  BitWriter *bw = make_writer(fp);
  for (int i = 0; i < 256; i++) {
    write_bits(0, 8, bw);
    write_bits(~0, 8, bw);
  }
  printf("pos: %d, offset: %d\n", bw->buf.position, bw->buf.bit_offset);
  close_stream(bw);
  fclose(fp);
}

void read_test_bits() {
  FILE *fp = fopen(test_file, "r");
  BitReader *br = make_reader(fp);
  do {
    printf("%x\n", current_byte(br->buf)); 
  } while(next_byte(br));
  fclose(fp);
  printf("Reading bit by bit\n");
  fp = fopen("test", "r");
  br = make_reader(fp);
  unsigned int into = 0;
  while (read_bits(1, br, &into)) {
    printf("Here be bit: %d\n", into);
    into = 0;
  }
  fclose(fp);
}

int main(int argc, char *argv[]) {
  int do_read = 0, do_write = 0;
  for (int i = 1; i < argc; i++) {
    if (0 == strcmp("read", argv[i])) {
      do_read = 1;
      continue;
    }
    if (0 == strcmp("write", argv[i])) {
      do_write = 1;
      continue;
    }
  }
  if (do_write) {
    write_some_bits();
  }
  if (do_read) {
    read_test_bits();
  }
  return 0;
}
