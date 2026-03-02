#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// In khoảng trắng đầu dòng
void printIndent(const int level) {
    if (level < 1)
        return;
    for (int i = 1; i <= level; i++)
        printf("   ");
}

// Lấy tên từ dirent và thêm null terminator
void parseName(char *output, struct dirent *inputEntry) {
    memmove(output, inputEntry->name, DIRSIZ);
    output[DIRSIZ] = '\0';
}

// Trích tên file/thư mục từ path (chỉ dùng một lần, cho path được truyền vào lệnh)
void nameFromPath(char *output, const char *inputPath) {
    for (int i = strlen(inputPath); i >= 0; i--) {
        if (inputPath[i] == '/') {
            memmove(output, inputPath + i + 1, DIRSIZ);
            output[DIRSIZ] = '\0';
            return;
        }
    }
    strcpy(output, inputPath);
}

// Gộp path và tên entry để tạo path mới
void concatPath(char *output, const char *ogPath, const char *entryName) {
    memmove(output, ogPath, strlen(ogPath));
    output[strlen(ogPath)] = '/';
    memmove(output + strlen(ogPath) + 1, entryName, strlen(entryName));
    output[strlen(ogPath) + strlen(entryName) + 1] = '\0';
}

// In cây (đệ quy)
void tree(const char *path, const char *name, const int level, const int maxLevel, const int dirOnly) {
    if (level > maxLevel)
        return;

    int fd = open(path, O_RDONLY);
    struct stat st;

    if (fd < 0) {
        printf("%s%s\n", "Could not get file descriptor for: ", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        printf("%s%s\n", "Could not parse fstat for: ", path);
        close(fd);
        return;
    }

    // Nếu có flag dirOnly thì chỉ in nếu là thư mục
    if (dirOnly == 1) {
        if (st.type == T_DIR) {
            printIndent(level);
            printf("%s\n", name);
        }
    }
    // Nếu không thì in hết, không cần biết type
    else {
        printIndent(level);
        printf("%s\n", name);
    }

    // Nếu là thư mục, duyệt qua các entry
    if (st.type == T_DIR) {
        struct dirent de;
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            // Nếu entry trống, bỏ qua
            if (de.inum == 0)
                continue;
            
            // Lấy tên từ dirent
            char entryName[DIRSIZ + 1];
            parseName(entryName, &de);
            // Bỏ qua .. và .
            if (strcmp(entryName, "..") == 0 || strcmp(entryName, ".") == 0)
                continue;
            
            // Gộp path
            char newPath[strlen(path) + strlen(entryName) + 2];
            concatPath(newPath, path, entryName);
            // Tiến hành đệ quy
            tree(newPath, entryName, level + 1, maxLevel, dirOnly);
        }
    }

    // Nếu không là thư mục, ngưng đệ quy
    close(fd);
    return;
}

int main(int argc, char *argv[]) {
    // Các giá trị mặc định
    char *path = ".";
    char name[DIRSIZ];
    int depth = 256;
    int foldersOnly = 0;

    if (argc < 2)
        tree(path, ".", 0, depth, foldersOnly);
    else {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-L") == 0)
                depth = atoi(argv[i + 1]) > 0 ? atoi(argv[i + 1]) : 256;
            else if (strcmp(argv[i], "-d") == 0)
                foldersOnly = 1;
            else {
                if (strcmp(argv[i - 1], "-L") != 0 && strcmp(argv[i - 1], "-d") != 0)
                    path = argv[i];
            }
        }
        nameFromPath(name, path);
        tree(path, name, 0, depth, foldersOnly);
    }

    exit(0);
}