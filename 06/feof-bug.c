#include <stdio.h>

#define MAXLEN 10

int main(int argc, char **argv) {
  FILE *f = stdin;
  char buf[MAXLEN];

  while (!feof(f)) {
    fgets(buf, MAXLEN, f);
    fputs(buf, stdout);
  }
}
