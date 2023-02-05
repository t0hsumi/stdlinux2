#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <getopt.h>

int main(int argc, char **argv) {
  int i;
  int verbose = 0;
  int opt;

  while ((opt = getopt(argc, argv, "e")) != -1) {
    switch (opt) {
    case 'e':
      verbose = 1;
      break;
    case '?':
      fprintf(stderr, "Usage: %s [-e] [FILE ...]\n", argv[0]);
      exit(1);
    }
  }

  for (i = optind; i < argc; i++) {
    FILE *f;
    int c;

    f = fopen(argv[i], "r");
    if (!f) {
      perror(argv[i]);
      exit(EXIT_FAILURE);
    }
    while ((c = getc(f)) != EOF) {
      if (c == '\t' && verbose) {
        if (fwrite("\\t", sizeof(char), 2, stdout) != 2)
          exit(EXIT_FAILURE);
      } else if (c == '\n' && verbose) {
        if (fwrite("$\n", sizeof(char), 2, stdout) != 2)
          exit(EXIT_FAILURE);
      } else if (putchar(c) < 0)
        exit(EXIT_FAILURE);
    }
    if (fclose(f) == EOF) {
      perror(argv[i]);
      exit(EXIT_FAILURE);
    }
  }
}
