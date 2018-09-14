// This file is for Step 1.
// You should do
//  Step 1 (A): write seq1
//  Step 1 (B): write main to test seq1
//  Step 1 (C): write printSeq1Range
//  Step 1 (D): add test cases to main to test printSeq1Range
//
// Be sure to #include any header files you need!
#include <stdio.h>
#include <stdlib.h>
int seq1(int x);
void printSeq1Range(int low, int high);
int main(void) {
  for (int i = -2; i < 3; i++) {
    printf("seq1(%d) = %d\n", i, seq1(i));
  }
  printf("printSeq1Range(%d, %d)\n", 0, 4);
  printSeq1Range(0, 4);
  printf("printSeq1Range(%d, %d)\n", -2, 6);
  printSeq1Range(-2, 6);

  printf("printSeq1Range(%d, %d)\n", 7, 3);
  printSeq1Range(7, 3);

  printf("printSeq1Range(%d, %d)\n", 0, 0);
  printSeq1Range(0, 0);

  printf("printSeq1Range(%d, %d)\n", 0, 1);
  printSeq1Range(0, 1);
  return EXIT_SUCCESS;
}
int seq1(int x) {
  return -3 + 4 * x;
}
void printSeq1Range(int low, int high) {
  for (int i = low; i < high; i++) {
    printf("%d", seq1(i));
    if (i != high - 1)
      printf(", ");
  }
  printf("\n");
}
