// Usage: ./outredirect outfile command ...
//            works like `command ... > outfile`

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 3)
    fprintf(stderr, "Usage: %s OUTFILE COMMAND ...\n", argv[0]);

  int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC);

  pid_t pid;
  pid = fork();
  if (pid == -1)
    err(1, "fork");
  else if (pid == 0) {
    dup2(fd, 1);
    close(fd);
    char **p = &argv[2];
    execvp(*p, p);
    err(1, "execvp");
  } else {
    close(fd);
    waitpid(pid, 0, 0);
  }
}
