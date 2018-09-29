#include "counts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_string(counts_t * c, const char * name) {
  for (int i = 0; i < c->n; i++) {
    if (strcmp(name, c->arr[i].str) == 0) {
      c->arr[i].count++;
      return 1;
    }
  }
  return 0;
}

void add_string(counts_t * c, const char * name) {
  c->arr = realloc(c->arr, ++c->n * sizeof(*(c->arr)));
  c->arr[c->n - 1].count = 1;
  c->arr[c->n - 1].str = malloc((strlen(name) + 1) * sizeof(*(c->arr[c->n - 1].str)));
  strncpy(c->arr[c->n - 1].str, name, strlen(name) + 1);
}

counts_t * createCounts(void) {
  //WRITE ME
  counts_t * counter = malloc(sizeof(*counter));
  counter->arr = NULL;
  counter->n = 0;
  return counter;
}
void addCount(counts_t * c, const char * name) {
  //WRITE ME
  const char * unknown = "<unknown>";
  if (!find_string(c, name == NULL ? unknown : name)) {
    add_string(c, name == NULL ? unknown : name);
  }
}
void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  for (int i = 0; i < c->n; i++) {
    fprintf(outFile, "%s: %d\n", c->arr[i].str, c->arr[i].count);
  }
}

void freeCounts(counts_t * c) {
  //WRITE ME
  for (int i = 0; i < c->n; i++) {
    free(c->arr[i].str);
  }
  free(c->arr);
  free(c);
}
