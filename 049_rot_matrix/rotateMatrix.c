#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char ** argv) {
  if (argc != 2) {
    perror("Wrong number of arguments");
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Can not find this file");
    return EXIT_FAILURE;
  }

  char str[11][12];
  int idx = 0;
  while (fgets(str[idx], 12, f) != NULL) {
    if (strchr(str[idx], '\n') - str[idx] != 10) {
      //fprintf(stderr, "error with something %d\n", (int)(strchr(str[idx], '\n') - str[idx]));
      perror("Wrong number of characters in a line");
      return EXIT_FAILURE;
    }
    idx++;
  }
  //  printf("idx: %d\n", idx);
  if (idx != 10) {
    perror("Wrong number of lines");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      printf("%c", str[9 - j][i]);
    }
    printf("\n");
  }

  return EXIT_SUCCESS;
}
