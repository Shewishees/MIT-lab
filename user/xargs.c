#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[])
{
  char *temp[32];
  int index = 0; // 记录参数下标
  for (int i = 1; i < argc; i++)
  {
    temp[index++] = argv[i]; // temp保存传入命令的参数，argv保存shell进程的参数，多一个xargs命令
  }

  char c;

  char *p = malloc(128); // 初始化
  temp[index] = p;
  int start = index;
  while (read(0, &c, 1) > 0)
  {
    if (c == ' ' || c == '\n') // 空格，说明该行读完了一个参数
    {
      index++;
      *p = '\0';

      if (c == '\n') // 读到了行末
      {
        temp[index] = 0;
        if (fork() == 0)
        {
          exec(temp[0], temp);
        }
        wait(0);
        for (int i = start; i < index; i++)
        {
          free(temp[i]); // 新开辟的参数全部释放，保留固定参数
        }
        index = argc - 1; // 重置index
      }
      temp[index] = malloc(128); // 下一行做准备
      p = temp[index];
    }
    else // 至关重要的else
    {
      *p++ = c;
    }
  }

  exit(0);
}