#include <stdlib.h>
size_t maxSeq(int * array, size_t n) {
  if (n == 0)
    return 0;
  size_t ans = 1;
  for (int i = 1; i < n; i++) {
    if (array[i] > array[i - 1]) {
      ans++;
    }
    else {
      ans = 1;
      i++;
    }
  }

  return ans;
}
