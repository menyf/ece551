/* myecho.c */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
  int j;
  char str[100];
  printf("whatever input, hello\n");
  scanf("%s", str);
  for (int i = 0; i < 3; i++) {
    printf("%d\n", i);
  }
  printf("Your input from cli is: %s\n", str);
  for (j = 0; j < argc; j++)
    printf("argv[%d]: %s\n", j, argv[j]);

  fprintf(stderr, "This should be print to stderr stream %s\n", argv[0]);

  exit(EXIT_SUCCESS);
}
