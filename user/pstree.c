#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/ptree.h"

static void
print_tree(struct ptreeinfo *nodes, int n, int idx, int indent)
{
  for(int i = 0; i < indent; i++)
    printf(" ");
  printf("%d %s state=%d mem=%d\n", nodes[idx].pid, nodes[idx].name, nodes[idx].state, (int)nodes[idx].memsize);
  for(int j = 0; j < n; j++){
    if(nodes[j].ppid == nodes[idx].pid){
      print_tree(nodes, n, j, indent + 2);
    }
  }
}

int
main(int argc, char *argv[])
{
  int max = NPROC;
  struct ptreeinfo *buf = malloc(sizeof(*buf) * max);
  if(buf == 0){
    printf("pstree: malloc failed\n");
    exit(1);
  }

  int n = ptree(buf, max);
  if(n < 0){
    printf("pstree: ptree syscall failed\n");
    exit(1);
  }

  // mark children
  char is_child[n];
  for(int i = 0; i < n; i++) is_child[i] = 0;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if(buf[j].ppid == buf[i].pid){
        is_child[j] = 1;
      }
    }
  }

  // print roots
  for(int i = 0; i < n; i++){
    if(!is_child[i]){
      print_tree(buf, n, i, 0);
    }
  }

  free(buf);
  exit(0);
}
