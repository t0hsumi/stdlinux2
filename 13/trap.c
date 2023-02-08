#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);

sighandler_t trap_signal(int sig, sighandler_t handler) {
  struct sigaction act, old;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;
  if (sigaction(sig, &act, &old) < 0)
    return NULL;
  return old.sa_handler;
}

void print_text(int sig) {
  printf("Got signal %d\n", sig);
  exit(0);
}

int main(void) {
  trap_signal(SIGINT, print_text);
  pause();
  exit(0);
}
