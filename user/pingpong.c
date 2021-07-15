#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ret;
  int fd1[2], fd2[2];
  ret = pipe(fd1);
  if (ret)
    goto failed;

  ret = pipe(fd2);
  if (ret)
    goto failed;

  int pid = fork();

  char send = 'a';
  char recv;

  if (pid) {
    ret = write(fd1[1], &send, 1);
    if (!ret)
      goto failed;

    ret = read(fd2[0], &recv, 1);
    if (!ret)
      goto failed;
    else if (recv != send)
      printf("parent data error %c\n", recv);

    printf("%d: received pong\n", getpid());

  } else {

    ret = read(fd1[0], &recv, 1);
    if (!ret)
      goto failed;
    else if (recv != send)
      printf("child data error %c\n", recv);

    printf("%d: received ping\n", getpid());

    ret = write(fd2[1], &send, 1);
    if (!ret)
      goto failed;
  }

  close(fd1[0]);
  close(fd1[1]);
  close(fd2[0]);
  close(fd2[1]);

  exit(0);

failed:
  exit(-1);
}
