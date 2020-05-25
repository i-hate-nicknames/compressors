#include <stdio.h>
#include "bitstream.h"

int write_header(int width, int height);
int write_pixels(unsigned char pixels[], int width, int height);
void draw_bits(unsigned char pixels[], int width, int height);

int width = 500, height = 500;

int main(void) {

  /* int width = 500, height = 500; */
  unsigned char pixels[width * height * 3];
  unsigned char *p;
  p = pixels;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      *p++ = 128;
      *p++ = 128;
      *p++ = 128;
    }
  }

  draw_bits(pixels, width, height);
  return write_header(width, height) &&
    write_pixels(pixels, width, height);
}

void draw_bits(unsigned char pixels[], int width, int height) {
  unsigned char *p = pixels;
  int drawn_total = 0;
  unsigned int bit = 0;
  BitReader *br = make_reader(stdin);
  while (drawn_total < (width * height) && readbit(br, &bit)) {
    *p++ = 255 * bit;
    *p++ = 255 * bit;
    *p++ = 255 * bit;
    drawn_total++;
  }
}

int write_header(int width, int height) {
  // static is used here to guarantee zero-initialization
  // of tga and pixels. Otherwise use calloc or manually zero
  static unsigned char tga[18];
  tga[2] = 2;
  tga[12] = 255 & width;
  tga[13] = 255 & (width >> 8);
  tga[14] = 255 & height;
  tga[15] = 255 & (height >> 8);
  tga[16] = 24;
  tga[17] = 32;
  return (1 == fwrite(tga, sizeof(tga), 1, stdout));
}

int write_pixels(unsigned char pixels[], int width, int height) {
  return (1 == fwrite(pixels, width * height * 3, 1, stdout));
}
