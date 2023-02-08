#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INIT_BUFSIZE 1024

static char *my_getcwd(void) {
  char *buf, *tmp;
  size_t size = INIT_BUFSIZE;

  buf = malloc(size);
  if (!buf)
    err(1, "malloc");

  for (;;) {
    errno = 0;
    if (getcwd(buf, size))
      return buf;
    if (errno != ERANGE)
      break;
    size *= 2;
    tmp = realloc(buf, size);
    if (!tmp)
      break;
    buf = tmp;
  }
  free(buf);
  return NULL;
}

int main(int argc, char **argv) {
  char *buf = my_getcwd();
  printf("%s\n", buf);
  free(buf);
  exit(0);
}
