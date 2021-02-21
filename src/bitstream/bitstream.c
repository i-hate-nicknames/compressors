#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bitstream.h"

#define BITS_BYTE 8

#define MIN(x, y) ((x) > (y)) ? (y) : (x)

bool dump_buffer(BitWriter *bw);
bool append_bits(unsigned char c, int width, BitWriter *bw);
bool next_byte(BitReader *br);
unsigned int extract_bits(unsigned int from, int offset, int width);
unsigned char current_byte(struct bit_buffer buf);

BitWriter *make_writer(FILE *fp) {
  BitWriter *bw = (BitWriter *) malloc(sizeof(BitWriter));
  bw->buf.bit_offset = 0;
  bw->buf.position = 0;
  bw->file = fp;
  bw->bytes_written = 0;
  return bw;
}

BitReader *make_reader(FILE *fp) {
  BitReader *br = (BitReader *) malloc(sizeof(BitReader));
  br->buf.bit_offset = 0;
  br->buf.position = 0;
  br->file = fp;
  br->buf.size = 0;
  br->bytes_read = 0;
  next_byte(br);
  return br;
}

bool close_stream(BitWriter *bw) {
  bool err = dump_buffer(bw);
  free(bw);
  return err;
}

// dump contents of the buffer into the underlying file of given bit writer,
// and reset buffer state
// return true on success, and false otherwise
// check errno for i/o errors that happened during write
bool dump_buffer(BitWriter *bw) {
  if (bw->buf.bit_offset != 0) {
    bw->buf.position++;
  }
  int write_bytes = bw->buf.position;
  int written = fwrite(bw->buf.data, 1, write_bytes, bw->file);
  bw->buf.position = 0;
  bw->buf.bit_offset = 0;
  // when we dump the buffer, we want the new buffer
  // start with 0
  bw->buf.data[0] = 0;
  bw->bytes_written += written;
  if (write_bytes != written) {
    return false;
  }
  return true;
}

bool write_bit(bool c, BitWriter *bw) {
  return write_bits(c, 1, bw);
}

bool write_char(unsigned char c, BitWriter *bw) {
  return write_bits(c, BITS_BYTE, bw);
}

bool write_bits(unsigned int c, int width, BitWriter *bw) {
  int width_remaining = BITS_BYTE - bw->buf.bit_offset;
  // what we want to write doesn't fit in the current byte
  // we need to write the portion that fits
  if (width_remaining < width) {
    // write portion that fits in the current byte
    int res = append_bits(c, width_remaining, bw);
    if (res != 0) {
      return res;
    }
    c = c >> width_remaining;
    width = width - width_remaining;
  }
  return append_bits(c, width, bw);
}

// set first width bits of c into current byte in the buffer
// assume it fits (remaining space in the byte is less than or
// equal to width)
// return true on success, and false otherwise
// check errno for i/o errors that happened during write
bool append_bits(unsigned char c, int width, BitWriter *bw) {
  unsigned int data = extract_bits(c, 0, width);
  bw->buf.data[bw->buf.position] |= data << bw->buf.bit_offset;
  bw->buf.bit_offset += width;
  // if we crossed byte boundary, go to the remaining byte
  if (bw->buf.bit_offset == BITS_BYTE) {
    bw->buf.position++;
    bw->buf.bit_offset = 0;
    // set next byte in the buffer to 0, because there may
    // be data from the previous buffer run
    bw->buf.data[bw->buf.position] = 0;
  }
  // if we ran out of buffer space, dump the buffer
  if (bw->buf.position == BUF_SIZE) {
    if (!dump_buffer(bw)) {
      return false;
    }
  }
  return true;
}

bool read_bit(BitReader *br, unsigned int *into) {
  return 1 == read_bits(1, br, into);
}

bool read_char(BitReader *br, unsigned int *into) {
  return BITS_BYTE == read_bits(BITS_BYTE, br, into);
}

int read_bits(int width, BitReader *br, unsigned int *into) {
  // read what we can from the current byte
  int width_remaining = BITS_BYTE - br->buf.bit_offset;
  int to_read_width = MIN(width, width_remaining);
  *into = extract_bits(current_byte(br->buf), br->buf.bit_offset, to_read_width);
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
    int bits_read = read_bits(rest_width, br, &rest);
    *into = (*into << bits_read) | rest;
    return to_read_width + bits_read;
  } else {
    return to_read_width;
  }
}

// Advance buffer of this reader to the next byte
// If there are no bytes remaining in the buffer,
// iniate another read from the underlying file
// Return true on success, false otherwise. False signifies
// that no more reads are possible from this reader
bool next_byte(BitReader *br) {
  br->buf.bit_offset = 0;
  if (br->buf.position < br->buf.size-1) {
    br->buf.position++;
    return true;
  }
  int bytes_read = fread(br->buf.data, 1, BUF_SIZE, br->file);
  br->buf.size = bytes_read;
  br->buf.position = 0;
  br->bytes_read += bytes_read;
  return bytes_read > 0;
}

// extract width bits starting from offset
// for example, 0 offset and 5 width will extract
// first five bits
// return number with first width bits set to the extracted
// bits
unsigned int extract_bits(unsigned int from, int offset, int width) {
  unsigned char mask = ~(~0 << width) << offset;
  return (mask & from) >> offset;
}

unsigned char current_byte(struct bit_buffer buf) {
  return buf.data[buf.position];
}
