#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if(argc < 2){
        printf("Usage: sleep <ticks>\n");
        exit(1);
    }
    int ticks = atoi(argv[1]);
    printf("Sleeping for %d ticks\n", ticks);
    sleep(ticks);
    printf("Woke up\n");
    exit(0);
}