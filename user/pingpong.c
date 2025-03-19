#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
  int pp2c[2], pc2p[2];
  pipe(pp2c);
  pipe(pc2p);
  if (fork() == 0) // 子进程先读后写
  {
    char *buf = (char *)malloc(5 * sizeof(char));
    read(pp2c[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    free(buf);
    close(pp2c[0]);

    write(pc2p[1], "pong", 4);
    close(pc2p[1]);
  }
  else // 父进程先写后读
  {
    write(pp2c[1], "ping", 4);
    close(pp2c[1]);

    char *buf = (char *)malloc(5 * sizeof(char));
    read(pc2p[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    free(buf);
    close(pc2p[0]);

    wait(0);
  }
  exit(0);
}