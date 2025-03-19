#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *file)
{
  struct dirent de;
  struct stat st;
  int fd;
  char buf[512], *p;
  fd = open(path, 0); // 打开目录

  while (read(fd, &de, sizeof(de)) == sizeof(de)) // 遍历目录项
  {
    if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) // 无用的文件
      continue;
    strcpy(buf, path); // 构造完整路径
    p = buf + strlen(buf);
    *p++ = '/';                           // 此时buf记录的是当前路径
    memmove(p, de.name, strlen(de.name)); // 路径拼接文件名
    p[strlen(de.name)] = 0;

    stat(buf, &st); // 采用直接根据路径读取元数据
    switch (st.type)
    {
    case T_FILE:                      // 目录项是文件
      if (strcmp(de.name, file) == 0) // 匹配成功时返回0
      {
        printf("%s\n", buf);
      }
      break;
    case T_DIR: // 如果是目录项是目录

      find(buf, file);
      break;
    }
  }
  close(fd);
}
int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}