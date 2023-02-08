// Usage: ./inredirect command1 ... pipe command2 ...
//            works like `command1 ... | command2 ...`

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void child_process(int argc, char **argv);

int main(int argc, char **argv) {
  if (argc < 4) {
    fprintf(stderr, "Usage: %s command1 ... pipe command2 ...\n", argv[0]);
    exit(1);
  }

  pid_t pid = fork();
  if (pid == -1)
    err(1, "fork");
  else if (pid == 0) {
    child_process(argc, argv);
  } else {
    waitpid(pid, 0, 0);
  }
}

static int find_pipe(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (!strncmp(argv[i], "pipe\0", 5))
      return i;
  }
  return -1;
}

static void child_process(int argc, char **argv) {
  int rw[2];
  if (pipe(rw) == -1)
    err(1, "pipe");
  int r = rw[0], w = rw[1];

  int sep = find_pipe(argc, argv);
  if (sep == -1) {
    fprintf(stderr, "Usage: %s command1 ... | command2\n", argv[0]);
    exit(1);
  }

  pid_t pid = fork();
  if (pid == -1)
    err(1, "err");
  else if (pid == 0) {
    close(r);
    dup2(w, 1);
    close(w);
    argv[sep] = NULL;
    execvp(argv[1], &argv[1]);
  } else {
    waitpid(pid, 0, 0);
    close(w);
    dup2(r, 0);
    close(r);
    execvp(argv[sep + 1], &argv[sep + 1]);
  }
}
