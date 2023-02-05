#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void do_mkdir(char *path);

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }
  for (int i = 1; i < argc; i++) {
    do_mkdir(argv[i]);
  }
  exit(0);
}

static void makedir(char *path) {
  struct stat st[1];
  if (mkdir(path, 0777) < 0) {
    switch (errno) {
    case EACCES:
      fprintf(stderr, "permission denied\n");
      exit(1);
    case EEXIST:
      if (stat(path, st) < 0)
        err(1, "stat");
      if (!S_ISDIR(st->st_mode)) {
        fprintf(stderr, "file exists but is not a directory %s\n", path);
      }
      exit(1);
    }
  }
}

static void do_mkdir(char *path) {
  ssize_t len = strlen(path);
  char *buf = malloc(len + 1);
  if (!buf) {
    err(1, "malloc");
  }
  for (size_t i = 0; i < len; i++) {
    if (path[i] == '/') {
      buf = strncpy(buf, path, i + 1);
      buf[i + 1] = '\0';
      makedir(buf);
    }
  }
  makedir(path);
  free(buf);
}
