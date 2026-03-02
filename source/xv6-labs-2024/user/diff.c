#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int readline(int fd, char *buf, int max) {
    int i = 0;
    char c;
    
    // Đọc từng byte một từ file descriptor
    while (i < max - 1 && read(fd, &c, 1) > 0) {
        if (c == '\n') {
            break; // Dừng khi gặp xuống dòng
        }
        buf[i++] = c;
    }
    buf[i] = '\0'; // Kết thúc chuỗi chuẩn C
    
    return i; // Trả về số lượng ký tự đã đọc (0 nếu là EOF)
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: diff file1 file2 [-q]\n");
        exit(1);
    }

    int q_flag = 0; // Cờ để kiểm tra flag -q
    char *f1_name = 0;
    char *f2_name = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-q") == 0) {
            q_flag = 1;
        } else if (f1_name == 0) {
            f1_name = argv[i];
        } else if (f2_name == 0) {
            f2_name = argv[i];
        }
    }

    int fd1 = open(f1_name, 0);
    int fd2 = open(f2_name, 0);

    if (fd1 < 0 || fd2 < 0) {
        printf("diff: cannot open files\n");
        exit(1);
    }

    char buf1[512], buf2[512];
    int line_num = 1;

    while (1) {
        //Đọc 2 file và lưu độ dài của mỗi dòng vào len1 và len2
        int len1 = readline(fd1, buf1, sizeof(buf1));
        int len2 = readline(fd2, buf2, sizeof(buf2));
        //Trường hợp 2 file đều đã EOF
        if (len1 == 0 && len2 == 0) break; 
        //Trường hợp 2 dòng khác nhau
        if (strcmp(buf1, buf2) != 0) {
            // In theo format -q
            if (q_flag) {
                printf("diff: files differ\n");
                break;
            }
            // Nếu dòng của file 1 > 0 -> in ra, không thì in EOF
            if (len1 > 0) printf("%s:%d: < %s\n", f1_name, line_num, buf1);
            else printf("%s:%d: <EOF\n", f1_name, line_num);
            // Nếu dòng của file 2 > 0 -> in ra, không thì in EOF
            if (len2 > 0) printf("%s:%d: > %s\n", f2_name, line_num, buf2);
            else printf("%s:%d: <EOF\n", f2_name, line_num);
        }
        line_num++;
    }

    close(fd1);
    close(fd2);
    exit(0);
}