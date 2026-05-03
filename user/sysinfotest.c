#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/sysinfo.h"

void
testcall(void)
{
  struct sysinfo info;
  int ret;

  ret = sysinfo(&info);
  if(ret < 0) {
    printf("sysinfo failed\n");
    exit(1);
  }
  
  if(info.freemem <= 0) {
    printf("freemem is invalid\n");
    exit(1);
  }

  if(info.nproc <= 0) {
    printf("nproc is invalid\n");
    exit(1);
  }
}

void
testmem(void)
{
  struct sysinfo info1, info2;

  sysinfo(&info1);
  
  // Allocate some memory
  void *ptr = malloc(4096);
  if(ptr == 0) {
    printf("malloc failed\n");
    exit(1);
  }

  sysinfo(&info2);
  
  // After malloc, freemem should be less or equal
  if(info2.freemem > info1.freemem) {
    printf("freemem didn't decrease after malloc\n");
    exit(1);
  }

  free(ptr);
}

void
testproc(void)
{
  struct sysinfo info1, info2;
  int pid;

  sysinfo(&info1);
  
  pid = fork();
  if(pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  sysinfo(&info2);
  
  if(pid == 0) {
    // Child process
    exit(0);
  } else {
    // Parent process
    // After fork, nproc should increase
    if(info2.nproc <= info1.nproc) {
      printf("nproc didn't increase after fork\n");
      exit(1);
    }
    
    // Wait for child
    wait(0);
  }
}

int
main(void)
{
  printf("sysinfotest: starting\n");

  testcall();
  printf("testcall passed\n");

  testmem();
  printf("testmem passed\n");

  testproc();
  printf("testproc passed\n");

  printf("sysinfotest: OK\n");
  exit(0);
}
