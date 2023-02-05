#include <asm-generic/errno-base.h>
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

static void do_mkdir(char *path) {
  if (mkdir(path, 0777) < 0) {
    if (errno == EACCES) {
      fprintf(stderr, "permission denied\n");
      exit(1);
    } else if (errno == EEXIST) {
      struct stat st[1];
      if (stat(path, st) < 0)
        err(1, "stat");
      if (!S_ISDIR(st->st_mode)) {
        fprintf(stderr, "file exists but is not a directory %s\n", path);
        exit(1);
      }
    } else if (errno == ENOENT) {
      char *parent_path = strdup(path);
      if (!parent_path) {
        err(1, "strdup");
      }
      size_t len = strlen(parent_path);
      for (size_t i = 0; i < len; i++) {
        if (parent_path[len - i - 1] == '/') {
          parent_path[len - i - 1] = '\0';
          do_mkdir(parent_path);
          if (mkdir(path, 0777) < 0)
            err(1, "mkdir");
          return;
        }
      }
      free(parent_path);
    }
  }
}
