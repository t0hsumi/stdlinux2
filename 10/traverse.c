#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void traverse(char *path);

int main(int argc, char **argv) {
  int i;

  if (argc < 2) {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }
  for (i = 1; i < argc; i++) {
    traverse(argv[i]);
  }
  exit(0);
}

static void traverse(char *path) {
  DIR *d;
  struct dirent *ent;

  d = opendir(path);
  if (!d) {
    perror(path);
    exit(1);
  }
  while ((ent = readdir(d))) {
    if (!strncmp(ent->d_name, ".\0", 2))
      continue;
    if (!strncmp(ent->d_name, "..\0", 3))
      continue;

    char *newpath;
    newpath = malloc(sizeof(char) * 2048);
    if (!newpath) {
      err(1, "malloc");
    }
    size_t pathlen = strlen(path);
    for (size_t i = 0; i < pathlen; i++) {
      newpath[i] = path[i];
    }
    newpath[pathlen] = '/';
    for (size_t i = 0; i < strlen(ent->d_name); i++) {
      newpath[pathlen + i + 1] = ent->d_name[i];
    }
    newpath[pathlen + strlen(ent->d_name) + 1] = '\0';

    struct stat buf[1];
    if (lstat(newpath, buf) < 0) {
      err(1, "lstat");
    }
    if (S_ISDIR(buf->st_mode)) {
      printf("%s\n", newpath);
      traverse(newpath);
    } else {
      printf("%s\n", newpath);
    }

    free(newpath);
  }
  if (closedir(d) == -1) {
    perror(path);
    exit(1);
  }
}
