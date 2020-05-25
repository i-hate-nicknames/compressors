#include <stdio.h>
#include "bitstream.h"


int main() {
  FILE *fp = fopen("test", "w");
  BitWriter *bw = make_writer(fp);
  for (int i = 0; i < 2; i++) {
    writebits(0, 1, bw);
    writebits(~0, 1, bw);
  }
  printf("pos: %d, offset: %d\n", bw->buf.position, bw->buf.bit_offset);
  close_stream(bw);
  fclose(fp);

  printf("Reading byte by byte\n");
  fp = fopen("test", "r");
  BitReader *br = make_reader(fp);
  do {
    printf("%x\n", current_byte(br->buf)); 
  } while(next_byte(br));
  fclose(fp);
  printf("Reading bit by bit\n");
  fp = fopen("test", "r");
  br = make_reader(fp);
  unsigned int into = 0;
  while (readbits(1, br, &into)) {
    printf("Here be bit: %d\n", into);
    into = 0;
  }
  fclose(fp);
}
