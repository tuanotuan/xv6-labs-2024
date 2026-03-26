#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p2c[2];
    int c2p[2];
    char buffer[1] = {'t'};
    pipe(p2c);
    pipe(c2p);
    int pid = fork();
    if(pid > 0){
        close(p2c[0]);
        close(c2p[1]);
        write(p2c[1], buffer, sizeof(buffer));
        read(c2p[0], buffer, sizeof(buffer));
        printf("%d: received pong\n", getpid());
        close(p2c[1]);
        close(c2p[0]);
        wait(0);
        exit(0);
    }
    else{
        close(p2c[1]);
        close(c2p[0]);
        read(p2c[0], buffer, sizeof(buffer));
        printf("%d: received ping\n", getpid());
        write(c2p[1], buffer, sizeof(buffer));
        close(p2c[0]);
        close(c2p[1]);
        exit(0);
    }
}