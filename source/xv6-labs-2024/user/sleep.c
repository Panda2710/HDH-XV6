
#include "kernel/types.h"
#include "user/user.h"
int main(int agrc, char **argv) {
if(agrc != 2) {
 printf("not enough arguments\n");
 exit(1);
}
 printf("Sleeping for %s ticks...\n", argv[1]);
 sleep(atoi(argv[1]));
 exit(0);
}
