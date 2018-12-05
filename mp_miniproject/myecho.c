/* myecho.c */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
  int j;
  char str[100];
  printf("[stdout]hi there, this is a helper progam\n");
  for (j = 0; j < argc; j++)
    printf("[stdout]argv[%d]: %s\n", j, argv[j]);

  printf("[stdout] Please enter something:");
  scanf("%s", str);
  printf("[stdout]What you just input is: %s\n", str);
  fprintf(stderr, "[stderr]This should be print to stderr stream\n");

  exit(EXIT_SUCCESS);
}
