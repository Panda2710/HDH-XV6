#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/types.h"

void cp(const char *src, const char *dst)
{
  int fd_src = open(src, O_RDONLY);//file dùng để đọc
  if (fd_src < 0) {
    fprintf(2, "cp: cannot open source file %s\n", src);
    return;
  }

  int fd_dst = open(dst, O_WRONLY | O_CREATE | O_TRUNC);//file dùng để ghi, nếu file đã tồn tại sẽ bị xóa và tạo mới
  if (fd_dst < 0) {
    fprintf(2, "cp: cannot open destination file %s\n", dst);
    close(fd_src);
    return;
  }

  char buf[512];//bộ đệm để đọc dữ liệu 
  int n;
  while ((n = read(fd_src, buf, sizeof(buf))) > 0) {
    if (write(fd_dst, buf, n) != n) {//chỉ ghi n byte đã đọc
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