#ifndef BITSTREAM_H
#define BITSTREAM_H

#define BUF_SIZE 1024

// Bitstream is a wrapper over a FILE that allows writing
// and reading bits
typedef struct bitstream {
  // offset in the current byte, this is how many bits have
  // been written starting from the closest byte boundary
  int bit_offset;
  // byte position in the buffer
  int position;
  // buffer to hold data before returning to client in reads
  // and before the actual writing for writes
  char buf[BUF_SIZE];
  // file from/into which to perform the io
  FILE *file;
} Bitstream;

// make a new stream, positioned at the start of the file
Bitstream *make_stream(FILE *fp);

// close this stream, forcing writes to be called on the file
// stream pointer becomes invalid after this call
// Note: this doesn't close the underlying file
int close_stream(Bitstream *bs);

// write a single bit to the stream. The least significant
// bit of c will be written
int writebit(unsigned char c, Bitstream *bs);

// write first n bits from c into the stream
int writebits(unsigned char c, int n, Bitstream *bs);

// write the whole char into the bitstream
int writechar(char c, Bitstream *bs);

// read a single bit from the stream
unsigned int readbit(Bitstream *bs);

// read next n bits from the stream and return
// it in a number x, that will have its first n
// bits set to the bits read
unsigned int readbits(int n, Bitstream *bs);

// read next char from the stream
char readchar(Bitstream *bs);

#endif
