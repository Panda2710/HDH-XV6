#include "kernel/types.h"
#include "user/user.h"
void checkPrime(int oldPipe[2])
{
    int p;
    int n;

    close(oldPipe[1]); // Dong dau ghi cua pipe cu
    // Ghi so dau tien cua pipe cu vao p (luon la so nguyen to)
    if (read(oldPipe[0], &p, sizeof(p)) <= 0)
    {
        close(oldPipe[0]);
        exit(0);
    }
    printf("prime %d\n", p);
    // Tao pipe moi cho tien trinh tiep theo
    int newPipe[2];
    pipe(newPipe);

    if (fork() == 0)
    {
        // Con
        close(oldPipe[0]);   // Dong dau doc cua pipe cu
        checkPrime(newPipe); // Goi de quy
    }
    else
    {
        // Cha
        close(newPipe[0]); // Dong dau doc cua pipe moi
        // Doc tung phan tu trong pipe cu va thuc hien sang so nguyen to
        while (read(oldPipe[0], &n, sizeof(int)))
        {
            // Neu nhu khong phai la boi so cua p thi ghi vao pipe moi, khong thi se khong ghi
            if (n % p != 0)
                write(newPipe[1], &n, sizeof(int));
        }
    }
    
    // Dong cac pipe cu
    close(oldPipe[0]);
    close(newPipe[1]);
    wait(0);
    exit(0);
}

int main()
{
    int fd[2];
    pipe(fd);
    if (fork() == 0)
    {
        checkPrime(fd);
    }
    else
    {
        close(fd[0]); // Dong dau doc chi su dung dau ghi
        int i;
        for (i = 2; i <= 280; i++)
        {
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
        wait(0);
    }
    exit(0);
}
