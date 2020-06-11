#include <stdbool.h>
#include <malloc.h>
#include <stdio.h>

void printmallinfo(void) {
  struct mallinfo info = mallinfo();
  printf("arena: %d\n", info.arena);
  printf("usmblks: %d\n", info.usmblks);
  printf("uordblks: %d\n", info.ordblks);
  printf("fordblks: %d\n", info.fordblks);
}