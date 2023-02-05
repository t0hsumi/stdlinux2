#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int i;

  for (i = 1; i < argc; i++) {
    FILE *f;
    int c;

    f = fopen(argv[i], "r");
    if (!f) {
      perror(argv[i]);
      exit(EXIT_FAILURE);
    }
    while ((c = getc(f)) != EOF) {
      if (c == '\t') {
        if (fwrite("\\t", sizeof(char), 2, stdout) != 2)
          exit(EXIT_FAILURE);
      } else if (c == '\n') {
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
