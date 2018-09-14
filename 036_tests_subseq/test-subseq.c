#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
size_t maxSeq(int * array, size_t n);
void run_check(int * array, size_t n, size_t len) {
  if (maxSeq(array, n) != len)
    exit(EXIT_FAILURE);
}
int main(void) {
  int a[] = {1, 2, 1, 3, 5, 8, 2, 4, 6, 9, 10, 11};  // ascending(2) + ascending(4) + ascending(6)
  int b[] = {-3, -2, -1, 0, 1, 2};                   // ascending with negative and positive numbers
  int c[] = {2, 4, 6, 9, 10, 11, 1, 2, 1, 3, 5, 8};  // ascending(6) + ascending(4) + ascending(2)
  int d[] = {-2, -1, -5, -4, -3, -2, -1, 0};         // ascending(2) + ascending(6)
  int e[] = {5, 4, 3, 2, 1, 7, 6, 5, 4, 3, 2, 1};    // descending(5) + descending(7)
  int f[] = {3, 2, 1, 2, 3, 4, 5};                   // descending(3) + ascending(5) - common(1)
  int g[] = {1, 2, 3, 4, 5, 4, 3, 2, 1};             // ascending(5) + descending(5) - common(1)
  int h[] = {INT_MAX, 0, INT_MIN};                   // boundary test
  int i[] = {-1, -2, -3, -4, -5, -6, 0, 1, 2, 3};
  int j[] = {1, 1, 2, 3, 4};
  run_check(a, 0, 0);  // len is 0
  run_check(a, 1, 1);  // len is 1
  run_check(a, 2, 2);  // len is 2
  run_check(a + 2, 5, 4);
  run_check(a, 12, 6);
  run_check(b, 6, 6);
  run_check(b, 3, 3);
  run_check(c, 12, 6);
  run_check(d, 8, 6);
  run_check(d + 1, 2, 1);
  run_check(e, 9, 2);
  run_check(f, 7, 5);
  run_check(g, 9, 5);
  run_check(h, 3, 1);
  run_check(i, 10, 5);
  //  run_check(j, size_t n, size_t len)
  run_check(j, 5, 4);
  return EXIT_SUCCESS;
}
