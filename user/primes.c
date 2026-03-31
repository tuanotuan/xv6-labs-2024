#include "kernel/types.h"
#include "user/user.h"
void primes(int pleft[2]) __attribute__((noreturn));
void primes(int pleft[2]){
    int p;
    if (read(pleft[0], &p, sizeof(p)) == 0){
        exit(0);
    }
    printf("prime %d\n", p);
    int pright[2];
    pipe(pright);
    int pid = fork();
    if (pid == 0) {
        close(pright[1]);
        close(pleft[0]);
        primes(pright);
    } 
    else{
        close(pright[0]);
        int n;
        while(read(pleft[0], &n, sizeof(n)) > 0){
            if (n % p != 0)
                write(pright[1], &n, sizeof(n));
        }
        close(pleft[0]);
        close(pright[1]);
        wait(0);
        exit(0);
    }
}

int main() {
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0) {
        close(p[1]);
        primes(p);
    } else {
        close(p[0]);
        for (int i = 2; i <= 280; i++){
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
}