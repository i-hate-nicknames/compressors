#include <stdio.h>

char *ctoa(unsigned char val, char buf[9]);

int main() {

  int c;
  char buf[33];
  while ((c = getchar()) != EOF) {
    char *bin = ctoa(c, buf);
    printf("%s", bin);
  }
  printf("\n");
  return 0;
}

// Convert given value into string representation in binary
// value, reversed direction.
// Since the system stores bits right to left, we effectively
// have "little endianness" in a bit world, but we want
// to print it in the order bits were put
char *ctoa(unsigned char val, char buf[9]) {
  buf[8] = 0;
  int i;
  for (i = 0; i < 8; i++, val /= 2) {
    buf[i] = "01"[val % 2];
  }
  return buf;

}
