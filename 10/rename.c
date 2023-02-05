#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  char *filename = "in";

  int fd = open(filename, O_RDONLY);
  if (fd == -1)
    err(1, "open");
  if (rename(filename, "new") == -1)
    err(1, "rename");
  if (close(fd))
    err(1, "close");
}
