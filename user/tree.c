#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// In thụt lề bằng nét dọc Unicode
void print_indent(int level, int is_last[]) {
    for(int i = 1; i < level; i++) {
        if (is_last[i]) {
            printf("    "); 
        } else {
            printf("│   "); 
        }
    }
}

void tree(char *path, int level, int maxDepth, int onlyDir, int is_last[]) {
    if(maxDepth != -1 && level > maxDepth) return;

    int fd = open(path, 0);
    if(fd < 0){
        printf("cannot open %s\n", path);
        return;
    }

    struct stat st;
    if(fstat(fd, &st) < 0 || st.type != T_DIR){
        close(fd);
        return;
    }

    int total_entries = 0;
    struct dirent de;
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        char child_path[512];
        memmove(child_path, path, strlen(path));
        char *p = child_path + strlen(path);
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (onlyDir) {
            struct stat cst;
            if(stat(child_path, &cst) == 0 && cst.type == T_DIR) {
                total_entries++;
            }
        } else {
            total_entries++;
        }
    }
    close(fd);

    if (total_entries == 0) return;

    fd = open(path, 0);
    int current_entry = 0;

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        char child_path[512];
        memmove(child_path, path, strlen(path));
        char *p = child_path + strlen(path);
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        struct stat cst;
        if(stat(child_path, &cst) < 0) continue;
        if (onlyDir && cst.type != T_DIR) continue;

        current_entry++;
        int last_node = (current_entry == total_entries);

        print_indent(level, is_last);

        // Nét đứt rẽ nhánh bằng Unicode
        if (last_node) {
            printf("└── ");
        } else {
            printf("├── ");
        }

        printf("%s\n", de.name);

        if(cst.type == T_DIR){
            is_last[level] = last_node;
            tree(child_path, level + 1, maxDepth, onlyDir, is_last);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    char *path = ".";
    int maxDepth = -1;
    int onlyDir = 0;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-d") == 0){
            onlyDir = 1;
        } else if(strcmp(argv[i], "-L") == 0 && i+1 < argc){
            maxDepth = atoi(argv[i+1]);
            i++;
        } else {
            path = argv[i];
        }
    }

    printf("%s\n", path);
    
    int is_last[64] = {0};
    tree(path, 1, maxDepth, onlyDir, is_last);

    exit(0);
}