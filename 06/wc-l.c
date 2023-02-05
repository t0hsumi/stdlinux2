#include <stdio.h>
#include <stdlib.h>

static void do_wc(char *filename);

int main(int argc, char **argv) {
  if (argc < 2) {
    do_wc(NULL);
  } else {
    for (int i = 1; i < argc; i++) {
      do_wc(argv[i]);
    }
  }
}

static void do_wc(char *filename) {
  FILE *f;
  ssize_t nlines = 0;
  int c;

  if (filename)
    f = fopen(filename, "r");
  else
    f = stdin;
  if (!f) {
    perror(filename);
    exit(1);
  }
  while ((c = fgetc(f)) != EOF) {
    if (c == '\n')
      nlines++;
  }
  printf("%ld\n", nlines);
  if (fclose(f) == EOF) {
    perror(filename);
    exit(1);
  }
}
