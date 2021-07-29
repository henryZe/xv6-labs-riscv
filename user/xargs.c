#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  char c, word[64];
  int len;

  int i;
  for (i = 0; argv[i + 1]; i++) {
    argv[i] = argv[i + 1];
  }

  while (1) {
    len = 0;

    while (read(0, &c, 1)) {
      if (c == '\n') {
        break;
      }
      word[len++] = c;
    }

    if (!len) {
      break;
    }

    word[len] = 0;
    argv[i] = word;

    int pid = fork();
    if (pid) {
      wait(0);
    } else {
      exec(argv[0], argv);
    }
  }

  exit(0);
}
