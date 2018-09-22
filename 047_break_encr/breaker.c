#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int decrypt(FILE * f) {
  int ch;
  int cnt[26] = {0};
  while ((ch = fgetc(f)) != EOF) {
    if (isalpha(ch)) {
      ch = tolower(ch);
      cnt[ch - 'a'] += 1;
    }
  }
  int mx = cnt[0];
  int key = 0;
  for (int i = 1; i < 26; i++) {
    if (mx < cnt[i]) {
      key = i;
      mx = cnt[i];
    }
  }
  //  printf("the most common char is %c\n", key + 'a');
  return (key + 26 - 4) % 26;
}
int main(int argc, char ** argv) {
  if (argc != 2) {
    perror("The number of arguments is wrong");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("The file does not exist");
    return EXIT_FAILURE;
  }

  printf("%d\n", decrypt(f));

  fclose(f);
  return EXIT_SUCCESS;
}
