#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

kvpair_t * read_kv_from_string(const char * str, const int len) {
  kvpair_t * kv_pair = malloc(sizeof(*kv_pair));
  kv_pair->key = NULL;
  kv_pair->value = NULL;
  for (int i = 0; i < len; i++) {
    if (str[i] == '=') {
      kv_pair->key = strndup(str, i);
      kv_pair->value = strndup(str + i + 1, len - i - 2);
      break;
    }
  }
  return kv_pair;
}

kvarray_t * read_kv_from_file(FILE * f) {
  kvarray_t * ans = malloc(sizeof(*ans));
  ans->kv_array = NULL;
  ans->length = 0;  // the count of kv pairs
  int len = 0;      // the length of current string
  char * str = NULL;
  size_t sz = 0;

  while ((len = getline(&str, &sz, f)) != EOF) {
    kvpair_t * kv_pair = read_kv_from_string(str, len);
    ans->kv_array = realloc(ans->kv_array, ++ans->length * sizeof(*(ans->kv_array)));
    ans->kv_array[ans->length - 1] = kv_pair;
  }
  free(str);
  return ans;
}

kvarray_t * readKVs(const char * fname) {
  //WRITE ME
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "No such file.\n");
    exit(EXIT_FAILURE);
  }

  kvarray_t * ans = read_kv_from_file(f);

  if (fclose(f)) {
    fprintf(stderr, "Can not close file.\n");
    exit(EXIT_FAILURE);
  }
  return ans;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->length; i++) {
    free(pairs->kv_array[i]->key);
    free(pairs->kv_array[i]->value);
    free(pairs->kv_array[i]);
  }
  free(pairs->kv_array);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->length; i++) {
    fprintf(
        stdout, "key = '%s' value = '%s'\n", pairs->kv_array[i]->key, pairs->kv_array[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  for (int i = 0; i < pairs->length; i++) {
    if (!strcmp(pairs->kv_array[i]->key, key)) {
      return pairs->kv_array[i]->value;
    }
  }
  return NULL;
}
