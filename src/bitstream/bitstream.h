#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <stdbool.h>
#include <stdio.h>
// Module that implements reading and writing on the bit level
// Writing is aligned on the byte level, just like regular i/o
// Consequently, if the number of written bits is not a multiple
// of 8, the rest will be padded with 0s, that will be indistinguishible
// from the regular writes. If you need to tell the actual writes
// from the padding consider encoding the length somewhere

// Bits are placed in "little endian" fashion, the first bit to
// be written goes to the 0th position (the rightmost), and next
// bits will go right to left.
// This does not affect functions in this module, but any other
// program that will work with the output of writebits must
// take bit order into account

// todo: when the size is changed, calculate real size and bufsize bytes
// via sizeof
#define BUF_SIZE 1024

struct bit_buffer {
  // offset in the current byte, this is how many bits have
  // been written starting from the closest byte boundary
  int bit_offset;
  // byte position in the buffer
  int position;
  // buffer to hold data before returning to client in reads
  // and before the actual writing for writes

  // todo: use the widest type possible (unsigned long long?) to reduce
  // the number of boundaries in the buffer
  char data[BUF_SIZE];
  // current buffer size in bytes
  int size;
};

// BitWriter is a wrapper over FILE that allows writing on bit level
typedef struct {
  // file from/into which to perform the io
  FILE *file;
  struct bit_buffer buf;
  unsigned long long bytes_written;
} BitWriter;

// BitReader is a wrapper over FILE that allows reading on bit level
typedef struct {
  FILE *file;
  struct bit_buffer buf;
  unsigned long long bytes_read;
} BitReader;

// make a new writer, positioned at the start of the file
// fp should be opened for writing
BitWriter *make_writer(FILE *fp);

// make a new reader, positioned at the start of the file
// fp should be opened for reading
BitReader *make_reader(FILE *fp);

// close this stream, forcing writes to be called on the file
// stream pointer becomes invalid after this call
// Note: this doesn't close the underlying file
// Return true on success, false on error
// When false is returned, errno is set to the number code of
// the error
bool close_stream(BitWriter *bw);

// The following functions use fread/fwrite under the hood.

// write a single bit to the stream. The least significant
// bit of c will be written
// return true on success, false on error
// When false is returned, errno is set to the number code of
// the error
bool write_bit(bool c, BitWriter *bw);

// write first n bits from c into the stream
// return true on success, false on error
// When false is returned, errno is set to the number code of
// the error
bool write_bits(unsigned int c, int n, BitWriter *bw);

// write the whole char into the bitstream
// return true on success, false on error
// When false is returned, errno is set to the number code of
// the error
bool write_char(unsigned char c, BitWriter *bw);

// read a single bit from the stream, return success status
// set the bit to the least significant bit of into
// return true on success, false on error
// When false is returned, errno is set to the number code of
// the error
bool read_bit(BitReader *br, unsigned int *into);

// read next n bits from the stream and set them in the number
// into, that will have its first n bits set to the bits read
// return number of bits read. If that number is less than n,
// assume an error or end of file. In that case errno is set
// to the number code of the error
int read_bits(int n, BitReader *br, unsigned int *into);

// read next char from the stream, return success status
// return true on success, false on error
// When false is returned, errno is set to the number code of
// the error
bool read_char(BitReader *br, unsigned int *into);

#endif
