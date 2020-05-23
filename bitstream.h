#ifndef BITSTREAM_H
#define BITSTREAM_H

#define BUF_SIZE 1024

typedef struct bitstream {
  int bit_offset;
  int position;
  char buf[BUF_SIZE];
  FILE *fp;
} Bitstream;

Bitstream *make_handler(FILE *fp);

int close(Bitstream *bs);

int writebit(char c, Bitstream *bs);

int writebits(char c, int width, Bitstream *bs);

int writechar(char c, Bitstream *bs);

unsigned int readbit(Bitstream *bs);

unsigned int readbits(int width, Bitstream *bs);

char readchar(Bitstream *bs);

#endif
