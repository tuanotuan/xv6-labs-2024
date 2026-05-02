#include "user.h"

#define MAXARG 32

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  if(argc < 3) {
    fprintf(2, "Usage: %s mask command ...\n", argv[0]);
    exit(1);
  }

  // Convert mask string to integer
  int mask = atoi(argv[1]);
  
  // Call trace syscall with the mask
  trace(mask);

  // Fork to create a child process
  int pid = fork();
  if(pid < 0){
    fprintf(2, "fork failed\n");
    exit(1);
  }

  if(pid == 0){
    // Child process: prepare arguments for exec
    // argv[0] is trace, argv[1] is mask, argv[2] is command
    // We need to build a new argv for exec starting from argv[2]
    for(i = 2; i < argc && i-2 < MAXARG; i++){
      nargv[i-2] = argv[i];
    }
    nargv[i-2] = 0;

    // Execute the command
    exec(nargv[0], nargv);
    fprintf(2, "exec %s failed\n", nargv[0]);
    exit(1);
  }

  // Parent process: wait for child to complete
  wait(0);
  exit(0);
}
