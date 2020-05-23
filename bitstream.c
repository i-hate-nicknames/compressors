#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bitstream.h"

#define BITS_BYTE 8

int dump_buffer(Bitstream *bs);
int appendbits(unsigned char c, int width, Bitstream *bs);

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
  return writebits(c, 1, bs);
}

int writebits(unsigned char c, int width, Bitstream *bs) {
  int width_remaining = BITS_BYTE - bs->bit_offset;
  // what we want to write doesn't fit in the current byte
  // we need to write the portion that fits
  if (width_remaining < width) {
    // write portion that fits in the current byte
    int res = appendbits(c, width_remaining, bs);
    if (res != 0) {
      return res;
    }
    c = c >> width_remaining;
    width = width - width_remaining;
  }
  return appendbits(c, width, bs);
}

// set first width bits of c into current byte in the buffer
// assume it fits (remaining space in the byte is less than or
// equal to width)
int appendbits(unsigned char c, int width, Bitstream *bs) {
  // cut the right portion of c and set it in the current byte
  int mask = ~(-1 << width);
  bs->buf[bs->position] |= (c & mask) << bs->bit_offset;
  bs->bit_offset += width;
  // if we crossed byte boundary, go to the next byte
  if (bs->bit_offset == BITS_BYTE) {
    bs->position++;
    bs->bit_offset = 0;
  }
  // if we ran out of buffer space, dump the buffer
  if (bs->position == BUF_SIZE) {
    int err = dump_buffer(bs);
    if (err != 0) {
      return err;
    }
  }
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
  for (int i = 0; i < 256; i++) {
    writebits(~0, 1, bs);
    /* writebit(0, bs); */
  }
  printf("pos: %d, offset: %d\n", bs->position, bs->bit_offset);
  close_stream(bs);
  fclose(fp);
}
