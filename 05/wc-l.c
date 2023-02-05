#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static ssize_t do_cat(const char *path);
static void die(const char *s);

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("stdin : %ld\n",do_cat(NULL));
  }
  for (int i = 1; i < argc; i++) {
    printf("%s : %ld\n", argv[i], do_cat(argv[i]));
  }
  exit(0);
}

#define BUFFER_SIZE 2048

static ssize_t do_cat(const char *path) {
  int fd;
  unsigned char buf[BUFFER_SIZE];
  ssize_t n;
  ssize_t nlines = 0;

  if (path)
    fd = open(path, O_RDONLY);
  else
    fd = STDIN_FILENO;
  if (fd < 0)
    die(path);
  for (;;) {
    n = read(fd, buf, sizeof buf);
    if (n < 0)
      die(path);
    if (n == 0)
      break;
    for (ssize_t i = 0; i < n; i++)
      if (buf[i] == '\n')
        nlines++;
  }
  if (close(fd) < 0)
    die(path);
  return nlines;
}

static void die(const char *s) {
  perror(s);
  exit(1);
}
