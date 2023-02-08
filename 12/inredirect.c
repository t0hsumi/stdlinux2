// Usage: ./inredirect infile command ...
//            works like `< infile | command ...`

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s INFILE COMMAND ...", argv[0]);
    exit(1);
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    err(1, "open");

  pid_t pid;
  pid = fork();
  if (pid == -1)
    err(1, "fork");
  else if (pid == 0) {
    dup2(fd, 0);
    close(fd);
    char **p = &argv[2];
    execvp(*p, p);
    err(1, "execvp");
  } else {
    close(fd);
    waitpid(pid, 0, 0);
  }
}
