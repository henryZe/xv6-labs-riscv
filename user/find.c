#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *file)
{
  int fd;
  char buf[512], *p;
  struct stat st;
  struct dirent de;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    goto failed;
  }

  if (st.type != T_DIR) {
    goto failed;
  }

  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
    printf("ls: path too long\n");
    goto failed;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if (stat(buf, &st) < 0) {
      printf("ls: cannot stat %s\n", buf);
      continue;
    }

    if (!strlen(de.name)) {
      continue;
    }

    if (!strcmp(de.name, "..") || !strcmp(de.name, ".")) {
      continue;
    }

    if (st.type == T_DIR) {
      find(buf, file);
    } else {
      if (!strcmp(file, de.name)) {
        printf("%s\n", buf);
      }
    }
  }

failed:
  close(fd);
}

int main(int argc, char *argv[])
{
  if(argc < 3){
    find(".", argv[1]);
  } else {
    find(argv[1], argv[2]);
  }

  exit(0);
}
