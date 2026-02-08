#include "kernel/types.h"
#include "user/user.h"

int main()
{

    int fd[2];  // Mảng lưu cha gửi con
    int fd2[2]; // Mảng lưu con gửi cha
    char buffer[50];
    // Tạo pipe
    pipe(fd);
    pipe(fd2);

    // Tạo tiến trình con
    if (fork() == 0)
    {
        // Tiến trình con
        close(fd[1]); // Đóng đầu ghi vì tiến trình con chỉ đọc pipe1
        read(fd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(fd[0]);
        close(fd2[0]); // Đóng đầu đọc pipe2  tiến trình con chỉ ghi vào pipe2
        write(fd2[1], "pong", 5);
        close(fd2[1]);
    }
    else
    {
        // Tiến trình cha

        close(fd[0]); // Đóng đầu đọc vì tiến trình cha chỉ ghi vào pipe1
        write(fd[1], "ping", 5);
        close(fd[1]);
        close(fd2[1]); // Đóng đầu ghi pipe2 vì tiến trình cha chỉ đọc từ pipe2
        read(fd2[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);
        close(fd2[0]);
    }

    exit(0);
}
