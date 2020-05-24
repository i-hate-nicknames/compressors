#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bitstream.h"

#define BITS_BYTE 8

int dump_buffer(BitWriter *bw);
int appendbits(unsigned char c, int width, BitWriter *bw);

BitWriter *make_stream(FILE *fp) {
  BitWriter *bw = (BitWriter *) malloc(sizeof(BitWriter));
  bw->buf.bit_offset = 0;
  bw->buf.position = 0;
  bw->file = fp;
  return bw;
}

int close_stream(BitWriter *bw) {
  int err = dump_buffer(bw);
  free(bw);
  return err;
}

int dump_buffer(BitWriter *bw) {
  if (bw->buf.bit_offset != 0) {
    bw->buf.position++;
  }
  int write_bytes = bw->buf.position;
  int written = fwrite(bw->buf.data, 1, write_bytes, bw->file);
  bw->buf.position = 0;
  bw->buf.bit_offset = 0;
  if (write_bytes != written) {
    return 1;
  }
  return 0;
}

int writebit(unsigned char c, BitWriter *bw) {
  return writebits(c, 1, bw);
}

int writechar(char c, BitWriter *bw) {
  return writebits(c, BITS_BYTE, bw);
}

int writebits(unsigned char c, int width, BitWriter *bw) {
  int width_remaining = BITS_BYTE - bw->buf.bit_offset;
  // what we want to write doesn't fit in the current byte
  // we need to write the portion that fits
  if (width_remaining < width) {
    // write portion that fits in the current byte
    int res = appendbits(c, width_remaining, bw);
    if (res != 0) {
      return res;
    }
    c = c >> width_remaining;
    width = width - width_remaining;
  }
  return appendbits(c, width, bw);
}

// set first width bits of c into current byte in the buffer
// assume it fits (remaining space in the byte is less than or
// equal to width)
int appendbits(unsigned char c, int width, BitWriter *bw) {
  // cut the right portion of c and set it in the current byte
  int mask = ~(-1 << width);
  bw->buf.data[bw->buf.position] |= (c & mask) << bw->buf.bit_offset;
  bw->buf.bit_offset += width;
  // if we crossed byte boundary, go to the next byte
  if (bw->buf.bit_offset == BITS_BYTE) {
    bw->buf.position++;
    bw->buf.bit_offset = 0;
  }
  // if we ran out of buffer space, dump the buffer
  if (bw->buf.position == BUF_SIZE) {
    int err = dump_buffer(bw);
    if (err != 0) {
      return err;
    }
  }
  return 0;
}

unsigned int readbit(BitReader *br) {
  return readbits(1, br);
}

char readchar(BitReader *br) {
  return readbits(BITS_BYTE, br);
}

unsigned int readbits(int width, BitReader *br) {
  
}

int main() {
  FILE *fp = fopen("test", "w");
  BitWriter *bw = make_stream(fp);
  for (int i = 0; i < 256; i++) {
    writebits(~0, 1, bw);
    /* writebit(0, bs); */
  }
  printf("pos: %d, offset: %d\n", bw->buf.position, bw->buf.bit_offset);
  close_stream(bw);
  fclose(fp);
}
