#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int readline(int fd, char *buf, int max) {
    int i = 0;
    char c;
    
    // Doc tung byte mot tu fd
    while (i < max - 1 && read(fd, &c, 1) > 0) {
        if (c == '\n') {
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0'; // Them null terminator vao cuoi chuoi (de chuan C)
    
    return i; // Return do dai cua dong da doc (khong tinh '\n') (return 0 neu la EOF)
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: diff file1 file2 [-q]\n");
        exit(1);
    }

    int q_flag = 0; // Co kiem tra flag -q
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
        //Doc 2 file va luu do dai moi dong vao len1 va len2
        int len1 = readline(fd1, buf1, sizeof(buf1));
        int len2 = readline(fd2, buf2, sizeof(buf2));
        //Truong hop 2 file deu da doc het (len1 va len2 deu bang 0) thi thoat vong lap
        if (len1 == 0 && len2 == 0) break; 
        //Truong hop 2 dong khac nhau thi in ra theo format -q neu co, neu khong thi in ra dong khac nhau va EOF neu co
        if (strcmp(buf1, buf2) != 0) {
            // In theo -q
            if (q_flag) {
                printf("diff: files differ\n");
                break;
            }
            // Neu dong cua file 1 > 0 -> in ra, khong thi in EOF
            if (len1 > 0) printf("%s:%d: < %s\n", f1_name, line_num, buf1);
            else printf("%s:%d: < EOF\n", f1_name, line_num);
            // Neu dong cua file 2 > 0 -> in ra, khong thi in EOF
            if (len2 > 0) printf("%s:%d: > %s\n", f2_name, line_num, buf2);
            else printf("%s:%d: > EOF\n", f2_name, line_num);
        }
        line_num++;
    }

    close(fd1);
    close(fd2);
    exit(0);
}
