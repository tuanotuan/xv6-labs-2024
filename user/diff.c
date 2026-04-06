#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAXLINE 512

// Doc 1 dong tu fd den khi gap '\n' hoặc EOF
// Tra ve so ky tu doc dc, 0 neu EOF , -1 neu loi
int readline(int fd, char *buf, int max) {
    int i = 0, n;
    char c;

    while (i < max - 1) {
        n = read(fd, &c, 1);
        if (n < 0) return -1;       // loi doc
        if (n == 0) break;          // EOF
        if (c == '\n') break;       // het dong
        buf[i++] = c;
    }
    buf[i] = '\0';

    // Tra ve -1 neu chua doc dc gi vaf da EOF
    if (i == 0 && n == 0) return -1;
    return i;
}

// So sanh 2 chuoi, return 0 neu giong nhau
int strequal(char *a, char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

int main(int argc, char *argv[]) {
    int fd1, fd2;
    int quiet = 0;
    char *file1, *file2;
    char line1[MAXLINE], line2[MAXLINE];
    int r1, r2;
    int linenum = 1;
    int differ = 0;

    // Phan tich doi so
    if (argc == 4 && strequal(argv[3], "-q")) {
        quiet = 1;
        file1 = argv[1];
        file2 = argv[2];
    } else if (argc == 4 && strequal(argv[1], "-q")) {
        quiet = 1;
        file1 = argv[2];
        file2 = argv[3];
    } else if (argc == 3) {
        file1 = argv[1];
        file2 = argv[2];
    } else {
        fprintf(2, "usage: diff file1 file2 [-q]\n");
        exit(1);
    }

    // Mở 2 file
    fd1 = open(file1, O_RDONLY);
    if (fd1 < 0) {
        fprintf(2, "diff: cannot open %s\n", file1);
        exit(1);
    }

    fd2 = open(file2, O_RDONLY);
    if (fd2 < 0) {
        fprintf(2, "diff: cannot open %s\n", file2);
        close(fd1);
        exit(1);
    }

    // Doc va so sanh tung dong
    while (1) {
        r1 = readline(fd1, line1, MAXLINE);
        r2 = readline(fd2, line2, MAXLINE);

        // Ca 2 EOF -> dung
        if (r1 < 0 && r2 < 0) break;

        //Kiem tra tung dong khac nhau ko
        int is_diff = 0;

        if (r1 >= 0 && r2 >= 0) {
            // Ca 2 dong thoa -> so sanh
            if (!strequal(line1, line2))
                is_diff = 1;
        } else {
            // 1 trong 2 EOF -> khac
            is_diff = 1;
        }

        if (is_diff) {
            differ = 1;
            if (quiet) break;   // -q: chi can biet khac la dc

            //In dong cua file1
            if (r1 >= 0)
                printf("%s:%d: < %s\n", file1, linenum, line1);
            else
                printf("%s:%d: < EOF\n", file1, linenum);

            // In dong của file2
            if (r2 >= 0)
                printf("%s:%d: > %s\n", file2, linenum, line2);
            else
                printf("%s:%d: > EOF\n", file2, linenum);
        }

        linenum++;
    }

    // Xu ly output cuoi
    if (differ && quiet)
        printf("diff: files differ\n");

    close(fd1);
    close(fd2);
    exit(0);
}