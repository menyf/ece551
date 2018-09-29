#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counts.h"
#include "kv.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  //WRITE ME
  counts_t * ret = createCounts();
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open the file '%s'\n", filename);
    exit(EXIT_FAILURE);
  }

  char * str = NULL;
  size_t sz = 0;
  int len = 0;
  while ((len = getline(&str, &sz, f)) >= 0) {
    str[len - 1] = str[len - 1] == '\n' ? '\0' : str[len - 1];
    addCount(ret, lookupValue(kvPairs, str));
  }
  free(str);
  if (fclose(f)) {
    fprintf(stderr, "Something wrong when close the file\n");
    exit(EXIT_FAILURE);
  }
  return ret;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)

  //read the key/value pairs from the file named by argv[1] (call the result kv)
  if (argc < 2) {  // todo
    fprintf(stderr, "Wrong number of input files\n");
    exit(EXIT_FAILURE);
  }
  kvarray_t * resultkv = readKVs(argv[1]);

  //count from 2 to argc (call the number you count i)
  //count the values that appear in the file named by argv[i], using kv as the key/value pair
  for (int i = 2; i < argc; i++) {
    //   (call this result c)
    counts_t * c = countFile(argv[i], resultkv);

    //compute the output file name from argv[i] (call this outName)
    char * outName = computeOutputFileName(argv[i]);

    //open the file named by outName (call that f)
    FILE * f = fopen(outName, "w");
    if (f == NULL) {
      fprintf(stderr, "Can not open the file: %s\n", outName);
      exit(EXIT_FAILURE);
    }

    //print the counts from c into the FILE f
    printCounts(c, f);

    //close f
    if (fclose(f)) {
      fprintf(stderr, "Can not close the file: %s\n", outName);
      exit(EXIT_FAILURE);
    }

    //free the memory for outName and c
    free(outName);
    freeCounts(c);
  }
  //free the memory for kv
  freeKVs(resultkv);

  return EXIT_SUCCESS;
}
