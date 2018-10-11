#include "IntArray.h"

#include <assert.h>

#include <ostream>

IntArray::IntArray() : data(NULL), numElements(0) {}
IntArray::IntArray(int n) : numElements(n) {
  data = new int[n];
}

IntArray::IntArray(const IntArray & rhs) {
  //IntArray(rhs.size());
  numElements = rhs.size();
  data = new int[numElements];
  for (int i = 0; i < numElements; i++) {
    data[i] = rhs[i];
  }
}
IntArray::~IntArray() {
  delete[] data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
  if (this != &rhs) {
    int * tmp = new int[rhs.numElements];
    for (int i = 0; i < rhs.numElements; i++) {
      tmp[i] = rhs[i];
    }
    delete[] data;
    data = tmp;
    numElements = rhs.numElements;
  }
  return *this;
}
const int & IntArray::operator[](int index) const {
  assert(index < numElements && index >= 0);
  return data[index];
}
int & IntArray::operator[](int index) {
  assert(index < numElements && index >= 0);
  return data[index];
}

int IntArray::size() const {
  return numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
  if (numElements != rhs.size())
    return false;
  for (int i = 0; i < numElements; i++) {
    if (data[i] != rhs[i])
      return false;
  }
  return true;
}

bool IntArray::operator!=(const IntArray & rhs) const {
  return !(*this == rhs);
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
  s << "{";
  for (int i = 0; i < rhs.size(); i++) {
    s << rhs[i];
    if (i != rhs.size() - 1)
      s << ", ";
  }

  s << "}";
  return s;
}
