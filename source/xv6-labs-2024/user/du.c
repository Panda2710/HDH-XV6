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
    // return 0 neu khong mo duoc file hoac thu muc
    if((fd = open(path, 0)) < 0) return 0;
    if(fstat(fd, &st) < 0) { close(fd); return 0; }
    //Neu la file
    if(st.type == T_FILE) {
        // In ra dung luong file neu co flag -a
        if(a_flag && !s_flag) printf("%ld\t%s\n", st.size, path); 
        close(fd);
        return st.size; 
    } else if(st.type == T_DIR) {
        // Xu ly neu lla thu muc
        // Chep path vao buf de chuan bi cho viec de quy
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            // Bo qua "." va ".."
            if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue; 
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            // Cong de quy 
            total_size += du(buf); 
        }
        // Mac dinh in dung luong thu muc neu khong co flag -s
        if(!s_flag) printf("%ld\t%s\n", total_size, path); 
        close(fd);
        return total_size;
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[]) {
    char *target_path = "."; // Mac dinh la "." (thu muc hien tai)

    // Xu ly tham so truyen vao
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) a_flag = 1;
        else if (strcmp(argv[i], "-s") == 0) s_flag = 1;
        else target_path = argv[i];
    }

    long total = du(target_path);
    
    // NNeu co flag -s, chi in tong dung luong
    if(s_flag) printf("%ld\t%s\n", total, target_path); 

    exit(0);
}
