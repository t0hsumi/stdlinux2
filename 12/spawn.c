#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t pid;

  if (argc != 3) {
    fprintf(stderr, "usage: %s\n", argv[0]);
    exit(1);
  }
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "fork\n");
    exit(1);
  } else if (pid == 0) {
    execl(argv[1], argv[1], argv[2], NULL);
    perror(argv[1]);
    exit(1);

  } else {
    int status;
    waitpid(pid, &status, 0);
    printf("child (PID=%d) finished; ", pid);
    if (WIFEXITED(status))
      printf("exit, status=%d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
      printf("signal, sig=%d\n", WTERMSIG(status));
    else
      printf("abnormal stop\n");
    exit(0);
  }
}
