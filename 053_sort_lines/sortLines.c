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
  char ** str_array = NULL;
  int cnt = 0;
  while ((len = getline(&str, &sz, f)) != EOF) {
    str_array = realloc(str_array, ++cnt * sizeof(*str_array));
    str_array[cnt - 1] = str;
    str = NULL;
    sz = 0;
  }
  sortData(str_array, cnt);
  for (int i = 0; i < cnt; i++) {
    fprintf(stdout, "%s", str_array[i]);
    free(str_array[i]);
  }
  free(str_array);
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
        fprintf(stderr, "Can not find file");
        exit(EXIT_FAILURE);
      }
      func(f);
      if (fclose(f)) {
        fprintf(stderr, "Failed to close file");
        exit(EXIT_FAILURE);
      }
    }
  }
  return EXIT_SUCCESS;
}
