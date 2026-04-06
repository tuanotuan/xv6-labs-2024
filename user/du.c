#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int flag_a = 0;
int flag_s = 0;

int du_calc(char *path, int is_root) {
    int fd = open(path, 0);
    if(fd < 0) {
        fprintf(2, "du: cannot open %s\n", path);
        return 0;
    }

    struct stat st;
    if(fstat(fd, &st) < 0) {
        fprintf(2, "du: cannot stat %s\n", path);
        close(fd);
        return 0;
    }

    // Nếu là file thường
    if(st.type != T_DIR) {
        close(fd);
        if(is_root || (flag_a && !flag_s)) {
            printf("%d\t%s\n", (int)st.size, path);
        }
        return st.size;
    }

    // NẾU LÀ THƯ MỤC: Bắt đầu từ 0 (Bỏ qua size vỏ thư mục theo ý thầy)
    int total = 0; 
    struct dirent de;
    
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        char child[512];
        int p_len = strlen(path);
        memmove(child, path, p_len);
        
        if(path[p_len - 1] != '/') {
            child[p_len] = '/';
            memmove(child + p_len + 1, de.name, strlen(de.name));
            child[p_len + 1 + strlen(de.name)] = '\0';
        } else {
            memmove(child + p_len, de.name, strlen(de.name));
            child[p_len + strlen(de.name)] = '\0';
        }

        total += du_calc(child, 0);
    }
    close(fd);

    if(is_root || !flag_s) {
        printf("%d\t%s\n", total, path);
    }

    return total;
}

int main(int argc, char *argv[]) {
    char *path = ".";

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-a") == 0) {
            flag_a = 1;
        } else if(strcmp(argv[i], "-s") == 0) {
            flag_s = 1;
        } else {
            path = argv[i];
        }
    }

    du_calc(path, 1);
    exit(0);
}