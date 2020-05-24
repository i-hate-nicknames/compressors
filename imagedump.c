#include <stdio.h>

int main(void) {

  int width = 200, height = 200;
  // static is used here to guarantee zero-initialization
  // of tga and pixels. Otherwise use calloc or manually zero
  static unsigned char pixels[width * height * 3];
  static unsigned char tga[18];
  unsigned char *p;

  p = pixels;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      *p++ = 255 * ((float) i / height);
      *p++ = 255 * ((float) j / width);
      *p++ = 255 * ((float) j / height);
    }
  }
  tga[2] = 2;
  tga[12] = 255 & width;
  tga[13] = 255 & (width >> 8);
  tga[14] = 255 & height;
  tga[15] = 255 & (height >> 8);
  tga[16] = 24;
  tga[17] = 32;
  return !((1 == fwrite(tga, sizeof(tga), 1, stdout)) &&
           (1 == fwrite(pixels, sizeof(pixels), 1, stdout)));
}
