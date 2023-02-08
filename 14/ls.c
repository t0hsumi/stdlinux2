#include <dirent.h>
#include <err.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static char *allocate_path_buffer(size_t required_len);
static void do_ls(char *path);

int main(int argc, char **argv) {
  int i;

  if (argc < 2) {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }
  for (i = 1; i < argc; i++) {
    do_ls(argv[i]);
  }
  exit(0);
}

static void do_ls(char *path) {
  DIR *d;
  struct dirent *ent;

  d = opendir(path);
  if (!d) {
    perror(path);
    exit(1);
  }
  while ((ent = readdir(d))) {
    char *buf = allocate_path_buffer(strlen(path) + 1 + strlen(ent->d_name));
    sprintf(buf, "%s/%s", path, ent->d_name);

    struct stat st;
    if (lstat(buf, &st) < 0)
      err(1, "lstat");
    char *mtime = ctime(&st.st_mtime);
    mtime[strlen(mtime) - 1] = '\0';
    struct passwd *pw = getpwuid(st.st_uid);
    if (pw) {
      printf("%s\towner=%s\tmtime=%s\n", ent->d_name, pw->pw_name, mtime);
    } else {
      printf("%s\towner=%d\tmtime=%s\n", ent->d_name, st.st_uid, mtime);
    }
  }
  if (closedir(d) == -1) {
    perror(path);
    exit(1);
  }
}

static char *file_path = NULL;
size_t path_len = 0;

static char *allocate_path_buffer(size_t required_len) {
  size_t len = path_len;
  while (len < required_len) {
    len += 1024;
  }
  if (len > path_len) {
    if (!file_path) {
      file_path = malloc(len);
      if (!file_path)
        err(1, "malloc");
    } else {
      char *tmp = realloc(file_path, len);
      if (!tmp)
        err(1, "realloc");
      file_path = tmp;
    }
  }
  return file_path;
}
