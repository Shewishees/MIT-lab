#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void seive(int pleft[2])
{
  int p;
  if (read(pleft[0], &p, sizeof(int)) != sizeof(int))
  {
    exit(0);
  } // 边界条件，提前退出，防止fork
  printf("prime %d\n", p); // 能进，说明肯定可以打印了

  int pright[2]; // 准备传给右邻居的管道
  pipe(pright);

  if (fork() == 0) // 右邻居
  {
    close(pright[1]); // 右邻居用不到写端
    close(pleft[0]);
    seive(pright);
    close(pright[0]); // 读完关闭
    exit(0);
  }
  else
  {
    close(pright[0]); // 左——>右，左不需要读
    int n;
    while (read(pleft[0], &n, sizeof(int)))
    {
      if (n % p != 0)
      {
        write(pright[1], &n, sizeof(int));
      }
    }
    close(pleft[0]);  // 读完关闭
    close(pright[1]); // 写完关闭
    wait(0);
    exit(0);
  }
}
int main()
{
  int p[2];
  pipe(p);

  for (int i = 2; i <= 35; i++)
  {
    write(p[1], &i, sizeof(int));
  }

  if (fork() == 0)
  {
    close(p[1]); // 子进程用不到写端
    seive(p);
    close(p[0]); // 读完再关
    exit(0);
  }
  else
  {
    close(p[0]); // 为了fork子进程能读，必须放到这里读
    close(p[1]);
    wait(0);
    exit(0);
  }
}