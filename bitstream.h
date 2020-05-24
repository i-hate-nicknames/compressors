#ifndef BITSTREAM_H
#define BITSTREAM_H

#define BUF_SIZE 1024

struct bit_buffer {
  // offset in the current byte, this is how many bits have
  // been written starting from the closest byte boundary
  int bit_offset;
  // byte position in the buffer
  int position;
  // buffer to hold data before returning to client in reads
  // and before the actual writing for writes
  char data[BUF_SIZE];
};

// Bitstream is a wrapper over a FILE that allows writing
// and reading bits
typedef struct {
  // file from/into which to perform the io
  FILE *file;
  struct bit_buffer buf;
} BitWriter;

typedef struct {
  int size_bytes;
  FILE *file;
  struct bit_buffer buf;
} BitReader;

// make a new writer, positioned at the start of the file
// assume file is opened for writing
BitWriter *make_writer(FILE *fp);

// make a new reader, positioned at the start of the file
// assume file is opened for reading
BitReader *make_reader(FILE *fp);

// close this stream, forcing writes to be called on the file
// stream pointer becomes invalid after this call
// Note: this doesn't close the underlying file
int close_stream(BitWriter *bw);

// write a single bit to the stream. The least significant
// bit of c will be written
int writebit(unsigned char c, BitWriter *bw);

// write first n bits from c into the stream
int writebits(unsigned char c, int n, BitWriter *bw);

// write the whole char into the bitstream
int writechar(char c, BitWriter *bw);

// read a single bit from the stream
int readbit(BitReader *br, unsigned int *into);

// read next n bits from the stream and set them in the number
// into, that will have its first n bits set to the bits read
// return number of bits read. If that number is less than n,
// assume an error or end of file.
int readbits(int n, BitReader *br, unsigned int *into);

// read next char from the stream
int readchar(BitReader *br, unsigned int *into);

#endif
