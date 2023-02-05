#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <err.h>
#include <getopt.h>

static void do_tail(FILE *f, long nlines);

#define DEFAULT_N_LINES 10

static struct option longopts[] = {{"lines", required_argument, NULL, 'n'},
                                   {"help", no_argument, NULL, 'h'},
                                   {0, 0, 0, 0}};

int main(int argc, char **argv) {
  int opt;
  long nlines = DEFAULT_N_LINES;

  while ((opt = getopt_long(argc, argv, "n:", longopts, NULL)) != -1) {
    switch (opt) {
    case 'n':
      nlines = atol(optarg);
      break;
    case 'h':
      fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
      exit(0);
    case '?':
      fprintf(stderr, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
      exit(1);
    }
  }

  if (optind == argc)
    do_tail(stdin, nlines);
  else {
    int i;

    for (i = optind; i < argc; i++) {
      FILE *f;

      f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_tail(f, nlines);
      if (fclose(f) == EOF) {
        perror(argv[i]);
        exit(1);
      }
    }
  }
  exit(0);
}

#define MAX_LINE_LENGTH 2048

static void do_tail(FILE *f, long nlines) {
  char **buf;
  int index = 0;
  int nread = 0;

  if (nlines <= 0)
    return;
  buf = malloc(sizeof(char *) * nlines);
  if (!buf) {
    err(1, "malloc");
    exit(1);
  }
  for (int i = 0; i < nlines; i++) {
    buf[i] = malloc(sizeof(char) * MAX_LINE_LENGTH);
    if (!buf[i]) {
      err(1, "malloc");
      exit(1);
    }
  }

  while (fgets(buf[index], MAX_LINE_LENGTH, f)) {
    index = (index == nlines - 1 ? 0 : index + 1);
    nread++;
  }
  if (nread < nlines) {
    index = 0;
  }

  for (int n = nlines; n > 0 && nread; n--, nread--) {
    printf("%s", buf[index]);
    index = (index == nlines - 1 ? 0 : index + 1);
  }

  for (int i = 0; i < nlines; i++) {
    free(buf[i]);
  }
  free(buf);
}
