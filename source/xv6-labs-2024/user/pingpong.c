#include "kernel/types.h"
#include "user/user.h"

int main()
{

    int fd[2];  // Mang luu cha gui con
    int fd2[2]; // Mang luu con gui cha
    char buffer[50];
    // Tao pipe
    pipe(fd);
    pipe(fd2);

    // TTao tien trinh con
    if (fork() == 0)
    {
        // Tien trinh con
        close(fd[1]); // Dong dau ghi vi tien trinh con chi doc tu pipe1
        read(fd[0], buffer, sizeof(buffer));

        printf("%d: received %s\n", getpid(), buffer);
        close(fd[0]);
        close(fd2[0]); // Dong dau doc vi tien trinh con chi ghi vao pipe2
        write(fd2[1], "pong", 5);
        close(fd2[1]);
    }
    else
    {
        // Tien trinh cha

        close(fd[0]); // Dong dau doc vi tien trinh cha chi ghi vao pipe1
        write(fd[1], "ping", 5);
        close(fd[1]);
        close(fd2[1]); // Dong dau ghi vi tien trinh cha chi doc tu pipe2
        read(fd2[0], buffer, sizeof(buffer));
        printf("%d: received %s\n", getpid(), buffer);
        close(fd2[0]);
    }

    exit(0);
}
