#include <stdio.h>
#include <stdlib.h>

static void do_cat(char *filepath);

int main(int argc, char **argv) {
  if (argc == 1)
    do_cat(NULL);
  else {
    for (int i = 1; i < argc; i++) {
      do_cat(argv[i]);
    }
  }
}

#define BUFFER_SIZE 2048

static void do_cat(char *filepath) {
  FILE *f;
  char buf[BUFFER_SIZE];
  size_t nread;

  if (filepath)
    f = fopen(filepath, "r");
  else
    f = stdin;
  if (!f) {
    perror(filepath);
    exit(1);
  }
  while (1) {
    nread = fread(buf, sizeof(char), BUFFER_SIZE, f);
    if (nread == EOF || nread == 0)
      break;
    if (fwrite(buf, sizeof(char), nread, stdout) < nread)
      break;
  }
  if (fclose(f) == EOF) {
    perror(filepath);
    exit(1);
  }
}
