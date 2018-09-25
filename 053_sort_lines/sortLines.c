#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}
void func(FILE * f) {
  char * str = NULL;
  size_t sz = 0;
  int len = 0;
  while ((len = getline(&str, &sz, f)) != EOF) {
    sortData(&str, len);
    printf("%s\n", str);
  }
  free(str);
}
int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  if (argc == 1) {
    func(stdin);
  }
  else {
    for (int i = 1; i < argc; i++) {
      FILE * f = fopen(argv[i], "r");
      if (f == NULL) {
        perror("Can not find file");
        exit(EXIT_FAILURE);
      }
      func(f);
      if (fclose(f)) {
        perror("Failed to close file");
        exit(EXIT_FAILURE);
      }
    }
  }
  return EXIT_SUCCESS;
}
