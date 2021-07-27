#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int generic_routine(int r_file)
{
  int prime, ret, num;
  ret = read(r_file, &prime, sizeof(prime));
  if (!ret)
    return ret;

  printf("prime %d\n", prime);

  int fd[2];
  ret = pipe(fd);
  if (ret) {
    printf("pipe fail\n");
    return ret;
  }

  int pid = fork();
  if (pid < 0) {
    printf("fork fail\n");
    return pid;
  }

  if (pid) {
    close(fd[0]);

    while (read(r_file, &num, sizeof(num))) {
      if (num % prime) {
          write(fd[1], &num, sizeof(num));
      }
    }

    close(fd[1]);
    wait(&ret);

  } else {
    close(fd[1]);
    ret = generic_routine(fd[0]);
    close(fd[0]);

    exit(ret);
  }

  return ret;
}

int main(int argc, char *argv[])
{
  int ret, fd[2];

  ret = pipe(fd);
  if (ret)
    return ret;

  int pid = fork();
  if (pid) {
    close(fd[0]);
    for (int i = 2; i <= 35; i++)
      write(fd[1], &i, sizeof(i));
    close(fd[1]);
    wait(&ret);

  } else {
    close(fd[1]);
    ret = generic_routine(fd[0]);
    close(fd[0]);
  }

  exit(ret);
}
