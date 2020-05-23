#include <stdio.h>
#include <stdlib.h>
#include "bitstream.h"

#define BITS_BYTE sizeof(char)
#define BUF_SIZE 1024

Bitstream *make_handler(FILE *fp) {
  
}

int close(Bitstream *bs) {

}

int writebit(char c, Bitstream *bs) {

}

int writebits(char c, int width, Bitstream *bs) {
  
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
