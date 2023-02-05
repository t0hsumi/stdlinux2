#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define _GNU_SOURCE
#include <getopt.h>

int invert = 0;

static struct option longopts[] = {{"ignore-case", no_argument, NULL, 'i'},
                                   {"invert-match", no_argument, NULL, 'v'},
                                   {0, 0, 0, 0}};

static void do_grep(regex_t *pat, FILE *f);

int main(int argc, char **argv) {
  regex_t pat[1];
  int err;
  int opt;
  int ignorecase = 0;

  while ((opt = getopt_long(argc, argv, "iv", longopts, NULL)) != -1) {
    switch (opt) {
    case 'i':
      ignorecase = 1;
      break;
    case 'v':
      invert = 1;
      break;
    case '?':
      fprintf(stderr, "Usage: %s [-iv] PATTERN [FILE ...]\n", argv[0]);
      exit(1);
    }
  }
  if (optind == argc) {
    fputs("no pattern\n", stderr);
    exit(1);
  }
  if (ignorecase) {
    err = regcomp(pat, argv[optind],
                  REG_EXTENDED | REG_NOSUB | REG_NEWLINE | REG_ICASE);
  } else {
    err = regcomp(pat, argv[optind], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);
  }
  if (err) {
    char buf[2048];

    regerror(err, pat, buf, sizeof buf);
    puts(buf);
    exit(1);
  }
  if (optind == argc - 1) {
    do_grep(pat, stdin);
  } else {
    for (int i = optind + 1; i < argc; i++) {
      FILE *f;

      f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_grep(pat, f);
      fclose(f);
    }
  }
  regfree(pat);
}

static void do_grep(regex_t *pat, FILE *f) {
  char buf[4096];
  int ret = 0;

  while (fgets(buf, sizeof buf, f)) {
    ret = regexec(pat, buf, 0, NULL, 0);
    if (!ret && !invert) {
      fputs(buf, stdout);
    } else if (ret && invert) {
      fputs(buf, stdout);
    }
  }
}
