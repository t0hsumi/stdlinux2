#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage\n");
    exit(1);
  }
  for (int i = 1; i < argc; i++) {
    if (unlink(argv[i]) < 0) {
      perror(argv[i]);
      exit(1);
    }
  }
  exit(0);
}
