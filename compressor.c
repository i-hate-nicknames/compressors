#include <stdio.h>

char *itoa(unsigned int val, int base, char buf[33]);

int main() {

  int c;
  char buf[33];
  while ((c = getchar()) != EOF) {
    char *bin = itoa(c, 2, buf);
    printf("%s\n", bin);
  }
  return 0;
}

char *itoa(unsigned int val, int base, char buf[33]) {
  buf[32] = 0;
  int i = 31;
  for (; i && val; --i, val /= base) {
    buf[i] = "01234567890abcde"[val % base];
  }
  return &buf[i+1];

}
