#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bitstream.h"

#define BITS_BYTE 8
#define BUF_SIZE 1024

Bitstream *make_stream(FILE *fp) {
  Bitstream *bs = (Bitstream *) malloc(sizeof(Bitstream));
  bs->bit_offset = 0;
  bs->position = 0;
  bs->file = fp;
  return bs;
}

int close_stream(Bitstream *bs) {
  // todo: error checks
  if (bs->bit_offset != 0) {
    bs->position++;
  }
  fwrite(bs->buf, 1, bs->position, bs->file);
  free(bs);
  return 0;
}

int writebit(unsigned char c, Bitstream *bs) {
  
  if (bs->bit_offset == BITS_BYTE) {
    bs->position++;
    bs->bit_offset = 0;
  }
  char to_write = (1 & c)<<bs->bit_offset;
  char current = bs->buf[bs->position];
  current |= to_write;
  bs->buf[bs->position] = current;
  bs->bit_offset++;
  return 0;

  // todo: implement writebits and do this
  // return writebits(c, 1, bs);
}

int writebits(unsigned char c, int width, Bitstream *bs) {
  // todo: write to the file if the buffer overflows

  // what we want to write doesn't fit in the current byte
  // we need to write the portion that fits
  if (bs->bit_offset + width > BITS_BYTE) {
    // write portion that fits in the current byte
    int width_remaining = BITS_BYTE - bs->bit_offset;
    int mask_remaining = ~(-1 << width_remaining);
    bs->buf[bs->position] |= c & mask_remaining;
    c = c >> width_remaining;
    width = width - width_remaining;
    bs->position++;
    bs->bit_offset = 0;
  }
  char current = bs->buf[bs->position];
  int mask = ~(-1 << width);
  current = current | ((c & mask) << bs->bit_offset);
  bs->buf[bs->position] = current;
  bs->bit_offset += width;
  return 0;
}

int writechar(char c, Bitstream *bs) {
  return writebits(c, BITS_BYTE, bs);
}

unsigned int readbit(Bitstream *bs) {
  return readbits(1, bs);
}

unsigned int readbits(int width, Bitstream *bs) {

}

char readchar(Bitstream *bs) {

}

int main() {
  FILE *fp = fopen("test", "w");
  Bitstream *bs = make_stream(fp);
  for (int i = 0; i < 128; i++) {
    writebits(3, 2, bs);
    /* writebit(0, bs); */
  }
  printf("pos: %d, offset: %d\n", bs->position, bs->bit_offset);
  close_stream(bs);
  fclose(fp);
}
