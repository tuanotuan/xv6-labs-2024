#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define BUFSIZE 512

int main(int argc, char *argv[]) {
    int fd_src, fd_dst, n;
    char buf[BUFSIZE];

    // kiem tra trong so
    if (argc != 3) {
        fprintf(2, "usage: cp src dst\n");
        exit(1);
    }

    // Mo file nguon (read-only)
    fd_src = open(argv[1], O_RDONLY);
    if (fd_src < 0) {
        fprintf(2, "cp: cannot open %s\n", argv[1]);
        exit(1);
    }

    // Mo/ tao file dich (write/ tao moi/ ghi de)
    fd_dst = open(argv[2], O_WRONLY | O_CREATE | O_TRUNC);
    if (fd_dst < 0) {
        fprintf(2, "cp: cannot open %s\n", argv[2]);
        close(fd_src);
        exit(1);
    }

    // Copy theo buffer 512 bytes
    while ((n = read(fd_src, buf, BUFSIZE)) > 0) {
        if (write(fd_dst, buf, n) != n) {
            fprintf(2, "cp: write error\n");
            close(fd_src);
            close(fd_dst);
            exit(1);
        }
    }

    // Kiem tra loi doc
    if (n < 0) {
        fprintf(2, "cp: read error\n");
        close(fd_src);
        close(fd_dst);
        exit(1);
    }

    close(fd_src);
    close(fd_dst);
    exit(0);
}