#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bitstream.h"

#define BITS_BYTE 8

int dump_buffer(Bitstream *bs);

Bitstream *make_stream(FILE *fp) {
  Bitstream *bs = (Bitstream *) malloc(sizeof(Bitstream));
  bs->bit_offset = 0;
  bs->position = 0;
  bs->file = fp;
  return bs;
}

int close_stream(Bitstream *bs) {
  int err = dump_buffer(bs);
  free(bs);
  return err;
}

int dump_buffer(Bitstream *bs) {
  if (bs->bit_offset != 0) {
    bs->position++;
  }
  int write_bytes = bs->position;
  int written = fwrite(bs->buf, 1, write_bytes, bs->file);
  bs->position = 0;
  bs->bit_offset = 0;
  if (write_bytes != written) {
    return 1;
  }
  return 0;
}

int writebit(unsigned char c, Bitstream *bs) {
  // todo: maybe use writebits to write a single bit
  // but I think this one is more efficient. Need to measure
  if (bs->bit_offset == BITS_BYTE) {
    bs->position++;
    bs->bit_offset = 0;
    // todo: if run out of buffer space, dump the buffer
  }
  char to_write = (1 & c)<<bs->bit_offset;
  char current = bs->buf[bs->position];
  current |= to_write;
  bs->buf[bs->position] = current;
  bs->bit_offset++;
  return 0;
}

int writebits(unsigned char c, int width, Bitstream *bs) {
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
    // todo: if we run out of buffer space, then dump
    // the buffer
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
    writebits(~0, 1, bs);
    /* writebit(0, bs); */
  }
  printf("pos: %d, offset: %d\n", bs->position, bs->bit_offset);
  close_stream(bs);
  fclose(fp);
}
