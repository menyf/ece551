#include <iostream>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  if (low >= high - 1)
    return low;
  int mid = (low + high) / 2;
  int val = f->invoke(mid);
  //  std::cout << val << std::endl;
  if (val > 0)
    return binarySearchForZero(f, low, mid);
  else if (val < 0)
    return binarySearchForZero(f, mid, high);

  return mid;
}
