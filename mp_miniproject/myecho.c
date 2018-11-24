/* myecho.c */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
  int j;

  for (int i = 0; i < 3; i++) {
    printf("%d\n", i);
  }

  for (j = 0; j < argc; j++)
    printf("argv[%d]: %s\n", j, argv[j]);

  exit(EXIT_SUCCESS);
}
