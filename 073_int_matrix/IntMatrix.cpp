#include "IntMatrix.h"

#include "iostream"
IntMatrix::IntMatrix() : numRows(0), numColumns(0) {}
IntMatrix::IntMatrix(int r, int c) : numRows(r), numColumns(c) {
  rows = new IntArray *[numRows];
  for (int i = 0; i < numRows; i++) {
    rows[i] = new IntArray(numColumns);
  }
}
IntMatrix::IntMatrix(const IntMatrix & rhs) {
  numRows = rhs.getRows();
  numColumns = rhs.getColumns();
  rows = new IntArray *[numRows];
  for (int i = 0; i < numRows; i++) {
    rows[i] = new IntArray(rhs[i]);
  }
}
IntMatrix::~IntMatrix() {
  for (int i = 0; i < numRows; i++) {
    delete rows[i];
  }
  delete[] rows;
}
IntMatrix & IntMatrix::operator=(const IntMatrix & rhs) {
  if (this != &rhs) {
    int r = rhs.getRows();
    int c = rhs.getColumns();
    IntArray ** tmp = new IntArray *[r];
    for (int i = 0; i < r; i++) {
      tmp[i] = new IntArray(rhs[i]);
    }
    for (int i = 0; i < numRows; i++) {
      delete rows[i];
    }
    //    delete[] rows;
    rows = tmp;
    numRows = r;
    numColumns = c;
  }
  return *this;
}
int IntMatrix::getRows() const {
  return numRows;
}
int IntMatrix::getColumns() const {
  return numColumns;
}
const IntArray & IntMatrix::operator[](int index) const {
  assert(0 <= index && index < numRows);
  return *rows[index];
}
IntArray & IntMatrix::operator[](int index) {
  assert(0 <= index && index < numRows);
  return *rows[index];
}

bool IntMatrix::operator==(const IntMatrix & rhs) const {
  if (numRows != rhs.getRows())
    return false;
  if (numColumns != rhs.getColumns())
    return false;
  for (int i = 0; i < numRows; i++) {
    if ((*this)[i] != rhs[i])
      return false;
  }
  return true;
}

IntMatrix IntMatrix::operator+(const IntMatrix & rhs) const {
  assert(numRows == rhs.getRows());
  assert(numColumns == rhs.getColumns());
  IntMatrix _mat(numRows, numColumns);
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      _mat[i][j] = (*this)[i][j] + rhs[i][j];
    }
  }
  return _mat;
}

std::ostream & operator<<(std::ostream & s, const IntMatrix & rhs) {
  s << "[ ";
  for (int i = 0; i < rhs.getRows(); i++) {
    s << rhs[i];
    if (i + 1 != rhs.getRows())
      s << ",\n";
  }
  s << " ]";
  return s;
}
