#include <stdio.h>
#include <stdlib.h>
unsigned power(unsigned x, unsigned y);
//void run_check(unsigned x, unsigned y, unsigned expected_ans);
int main(void) {
  //run_check(1, 1, 1);
  if (power(1, 1) != 1) {
    exit(EXIT_FAILURE);
  }
  if (power(0, 0) != 1) {
    exit(EXIT_FAILURE);
  }
  if (power(1, 2) != 1) {
    exit(EXIT_FAILURE);
  }
  /* if (power(2, 32) != 4294967296) { */
  /*   exit(EXIT_FAILURE); */
  /* } */
  if (power(1, 2) != 1) {
    exit(EXIT_FAILURE);
  }

  if (power(1, 3) != 1) {
    exit(EXIT_FAILURE);
  }

  if (power(2, 1) != 2) {
    exit(EXIT_FAILURE);
  }

  if (power(2, 2) != 4) {
    exit(EXIT_FAILURE);
  }

  /* if (power(10, 9) != 1000000000) { */
  /*   exit(EXIT_FAILURE); */
  /* } */

  return EXIT_SUCCESS;
}
