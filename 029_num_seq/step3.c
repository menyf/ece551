// This file is for Step 3.
// You should do
//  Step 3 (A): write seq3
//  Step 3 (B): write main to test seq3
//  Step 3 (C): write countEvenInSeq3Range
//  Step 3 (D): add test cases to main to test countEvenInSeq3Range
//
// Be sure to #include any header files you need!
#include <stdio.h>
#include <stdlib.h>
int seq3(int x, int y);
int countEvenInSeq3Range(int xLow, int xHi, int yLow, int yHi);
int main(void) {
  int pos[] = {0, 0, -5, -5, -5, 5, 5, 5, 5, -5, -4, -5, -1, -1, -1, 1, 1, 1, 1, -1};
  for (int i = 0; i < 20; i += 2) {
    printf("seq3(%d, %d) = %d\n", pos[i], pos[i + 1], seq3(pos[i], pos[i + 1]));
  }
  printf(
      "countEvenInSeq3Range(%d, %d, %d, %d) = %d\n", 0, 2, 0, 3, countEvenInSeq3Range(0, 2, 0, 3));
  printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",
         -5,
         6,
         -5,
         6,
         countEvenInSeq3Range(-5, 6, -5, 6));

  return EXIT_SUCCESS;
}
int seq3(int x, int y) {
  return -3 * x + y * (x + 2);
}

int countEvenInSeq3Range(int xLow, int xHi, int yLow, int yHi) {
  int cnt = 0;
  for (int i = xLow; i < xHi; i++) {
    for (int j = yLow; j < yHi; j++) {
      if (seq3(i, j) % 2 == 0)
        cnt++;
    }
  }
  return cnt;
}
