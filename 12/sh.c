#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct cmd {
  long argc;
  char **argv;
  long capa;
} cmd;

static struct cmd *read_cmd(void);
static void free_cmd(struct cmd *cmd);
static struct cmd *parse_cmd(char *cmdline);
static void *xmalloc(size_t size);
static void *xrealloc(void *ptr, size_t size);
static void invoke_cmd(struct cmd *cmd);

int main() {
  for (;;) {
    struct cmd *cmd;
    fprintf(stdout, "$ ");
    fflush(stdout);
    cmd = read_cmd();
    if (cmd->argc > 0)
      invoke_cmd(cmd);
    free_cmd(cmd);
  }
}

static void invoke_cmd(struct cmd *cmd) {
  pid_t pid;

  pid = fork();
  if (pid == -1)
    err(1, "fork");
  else if (pid == 0) {
    execvp(cmd->argv[0], cmd->argv);
    fprintf(stderr, "%s: command not fount", cmd->argv[0]);
    exit(1);
  } else {
    waitpid(pid, 0, 0);
  }
}

static void free_cmd(struct cmd *cmd) {
  free(cmd->argv);
  free(cmd);
}

#define LINE_BUF_SIZE 2048

static struct cmd *read_cmd(void) {
  static char buf[LINE_BUF_SIZE];

  if (fgets(buf, LINE_BUF_SIZE, stdin) == NULL)
    exit(0);
  return parse_cmd(buf);
}

#define INIT_CAPA 16

static struct cmd *parse_cmd(char *cmdline) {
  char *p = cmdline;
  struct cmd *cmd;

  cmd = xmalloc(sizeof(struct cmd));
  cmd->argc = 0;
  cmd->capa = INIT_CAPA;
  cmd->argv = xmalloc(sizeof(char *) * INIT_CAPA);
  while (*p) {
    while (*p && isspace((int)*p))
      *p++ = '\0';
    if (*p) {
      if (cmd->capa <= cmd->argc + 1) {
        cmd->capa *= 2;
        cmd->argv = xrealloc(cmd->argv, cmd->capa);
      }
      cmd->argv[cmd->argc] = p;
      cmd->argc++;
    }
    while (*p && !isspace((int)*p))
      p++;
  }
  cmd->argv[cmd->argc] = NULL;
  return cmd;
}

static void *xmalloc(size_t size) {
  void *p;

  p = malloc(size);
  if (!p)
    err(1, "malloc");
  return p;
}

static void *xrealloc(void *ptr, size_t size) {
  void *p;

  if (!ptr)
    return xmalloc(size);
  p = realloc(ptr, size);
  if (!p)
    err(1, "realloc");
  return p;
}
