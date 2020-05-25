#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bitstream.h"

#define BITS_BYTE 8

#define MIN(x, y) ((x) > (y)) ? (y) : (x)

int dump_buffer(BitWriter *bw);
int appendbits(unsigned char c, int width, BitWriter *bw);
int next_byte(BitReader *br);

BitWriter *make_writer(FILE *fp) {
  BitWriter *bw = (BitWriter *) malloc(sizeof(BitWriter));
  bw->buf.bit_offset = 0;
  bw->buf.position = 0;
  bw->file = fp;
  return bw;
}

BitReader *make_reader(FILE *fp) {
  BitReader *br = (BitReader *) malloc(sizeof(BitReader));
  br->buf.bit_offset = 0;
  br->buf.position = 0;
  br->file = fp;
  br->size_bytes = 0;
  next_byte(br);
  return br;
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
  // if we crossed byte boundary, go to the remaining byte
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

int readbit(BitReader *br, unsigned int *into) {
  return readbits(1, br, into);
}

int readchar(BitReader *br, unsigned int *into) {
  return readbits(BITS_BYTE, br, into);
}

int readbits(int width, BitReader *br, unsigned int *into) {
  // read what we can from the current byte
  int width_remaining = BITS_BYTE - br->buf.bit_offset;
  int to_read_width = MIN(width, width_remaining);
  unsigned char mask = ~(~0 << to_read_width) << br->buf.bit_offset;
  *into |= (mask & br->buf.data[br->buf.position]) >> br->buf.bit_offset;
  br->buf.bit_offset += to_read_width;
  /* printf("read smth: DDD\n"); */
  if (to_read_width < width) {
    // try reading remaining byte, if it fails return number
    // of bits already read
    if (!next_byte(br)) {
      return to_read_width;
    }
    unsigned int rest = 0;
    int rest_width = width - to_read_width;
    int bits_read = readbits(rest_width, br, &rest);
    *into = (*into << bits_read) | rest;
    return to_read_width + bits_read;
  } else {
    return to_read_width;
  }
}

int next_byte(BitReader *br) {
  br->buf.bit_offset = 0;
  if (br->buf.position < br->size_bytes-1) {
    br->buf.position++;
    return 1;
  }
  int bytes_read = fread(br->buf.data, 1, BUF_SIZE, br->file);
  br->size_bytes = bytes_read;
  br->buf.position = 0;
  return (bytes_read > 0);
}

int main() {
  FILE *fp = fopen("test", "w");
  BitWriter *bw = make_writer(fp);
  for (int i = 0; i < 3; i++) {
    writebits(0, 1, bw);
    writebits(~0, 1, bw);
  }
  printf("pos: %d, offset: %d\n", bw->buf.position, bw->buf.bit_offset);
  close_stream(bw);
  fclose(fp);

  printf("Reading byte by byte\n");
  fp = fopen("test", "r");
  BitReader *br = make_reader(fp);
  char c;
  while (next_byte(br)) {
    c = br->buf.data[br->buf.position];
    printf("%x\n", c); 
  }
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
