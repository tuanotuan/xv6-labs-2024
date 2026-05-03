#ifndef PTREE_H
#define PTREE_H

#include "types.h"

// Information about a process returned to user space by ptree syscall.
struct ptreeinfo {
  int pid;
  int ppid;
  int state;
  uint64 memsize;   // user memory size in bytes
  char name[16];
};

#endif // PTREE_H
