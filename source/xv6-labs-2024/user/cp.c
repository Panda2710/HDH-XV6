#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/types.h"

void cp(const char *src, const char *dst)
{
  int fd_src = open(src, O_RDONLY);//file dung de doc
  if (fd_src < 0) {
    fprintf(2, "cp: cannot open source file %s\n", src);
    return;
  }

  int fd_dst = open(dst, O_WRONLY | O_CREATE | O_TRUNC);// file dung de ghi, neu file da ton tai thi xoa va tao lai, neu khong thi tao moi
  if (fd_dst < 0) {
    fprintf(2, "cp: cannot open destination file %s\n", dst);
    close(fd_src);
    return;
  }

  char buf[512];//bo dem
  int n;
  while ((n = read(fd_src, buf, sizeof(buf))) > 0) {
    if (write(fd_dst, buf, n) != n) {//chi ghi n byyte da doc
      fprintf(2, "cp: write error to %s\n", dst);
      break;
    }
  }
  if (n < 0) {
    fprintf(2, "cp: read error from %s\n", src);
  }

  close(fd_src);
  close(fd_dst);
}
void main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(2, "Usage: cp <source> <destination>\n");
    exit(1);
  }
  cp(argv[1], argv[2]);
  exit(0);
}
