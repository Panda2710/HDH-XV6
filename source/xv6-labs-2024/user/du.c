#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int a_flag = 0;
int s_flag = 0;

long du(char *path) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    long total_size = 0;
    // return 0 nếu thư mục không tồn tại hoặc không thể mở
    if((fd = open(path, 0)) < 0) return 0;
    if(fstat(fd, &st) < 0) { close(fd); return 0; }
    //Xử lý nếu là file
    if(st.type == T_FILE) {
        // Nếu có -a và không có -s, in dung lượng file
        if(a_flag && !s_flag) printf("%ld\t%s\n", st.size, path); 
        close(fd);
        return st.size; 
    } else if(st.type == T_DIR) {
        // Xử lý nếu là thư mục
        // Chép đường dẫn vào buf để chuẩn bị cho việc đệ quy
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            // Bỏ qua "." và ".."
            if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue; 
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            // Cộng dồn đệ quy
            total_size += du(buf); 
        }
        // Mặc định in dung lượng thư mục nếu không có -s
        if(!s_flag) printf("%ld\t%s\n", total_size, path); 
        close(fd);
        return total_size;
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    char *target_path = "."; // Mặc định là "."

    // Xử lý tham số dòng lệnh để bật a_flag, s_flag và lấy target_path
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) a_flag = 1;
        else if (strcmp(argv[i], "-s") == 0) s_flag = 1;
        else target_path = argv[i];
    }

    long total = du(target_path);
    
    // Nếu có flag -s, chỉ in tổng dung lượng
    if(s_flag) printf("%ld\t%s\n", total, target_path); 

    exit(0);
}