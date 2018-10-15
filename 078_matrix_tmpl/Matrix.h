#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>

#include <cstdlib>
#include <iostream>
#include <vector>

//YOUR CODE GOES HERE!
template<typename T>
class Matrix
{
 private:
  int numRows;
  int numColumns;
  std::vector<std::vector<T> > rows;

 public:
  Matrix();
  Matrix(int r, int c);
  Matrix(const Matrix<T> & rhs);
  //~Matrix();
  Matrix<T> & operator=(const Matrix<T> & rhs);
  int getRows() const;
  int getColumns() const;
  const std::vector<T> & operator[](int index) const;
  std::vector<T> & operator[](int index);
  bool operator==(const Matrix<T> & rhs) const;
  Matrix<T> operator+(const Matrix<T> & rhs) const;
};

template<typename T>
std::ostream & operator<<(std::ostream & s, const std::vector<T> & rhs);

template<typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs);
#endif

template<typename T>
Matrix<T>::Matrix() : numRows(0), numColumns(0), rows(std::vector<std::vector<T> >()) {}

template<typename T>
Matrix<T>::Matrix(int r, int c) :
    numRows(r),
    numColumns(c),
    rows(std::vector<std::vector<T> >(r, std::vector<T>(c, T()))) {}

template<typename T>
Matrix<T>::Matrix(const Matrix & rhs) :
    numRows(rhs.getRows()),
    numColumns(rhs.getColumns()),
    rows(rhs.rows) {}

//template<typename T>
//Matrix<T>::~Matrix() {}

template<typename T>
Matrix<T> & Matrix<T>::operator=(const Matrix<T> & rhs) {
  numColumns = rhs.getColumns();
  numRows = rhs.getRows();
  rows = rhs.rows;
  return *this;
}

template<typename T>
int Matrix<T>::getRows() const {
  return numRows;
}

template<typename T>
int Matrix<T>::getColumns() const {
  return numColumns;
}

template<typename T>
const std::vector<T> & Matrix<T>::operator[](int index) const {
  assert(0 <= index && index < (int)rows.size());
  return rows[index];
}

template<typename T>
std::vector<T> & Matrix<T>::operator[](int index) {
  assert(0 <= index && index < (int)rows.size());
  return rows[index];
}

template<typename T>
bool Matrix<T>::operator==(const Matrix<T> & rhs) const {
  return rows == rhs.rows && numColumns == rhs.getColumns() && numRows == rhs.getRows();
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> & rhs) const {
  assert(numColumns == rhs.getColumns() && numRows == rhs.getRows());
  Matrix<T> mat(numRows, numColumns);
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      mat[i][j] = rows[i][j] + rhs[i][j];
    }
  }
  return mat;
}

template<typename T>
std::ostream & operator<<(std::ostream & s, const std::vector<T> & rhs) {
  s << "{";
  for (int i = 0; i < (int)rhs.size(); i++) {
    s << rhs[i];
    if (i != (int)rhs.size() - 1) {
      s << ", ";
    }
  }
  s << "}";
  return s;
}

template<typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
  s << "[ ";
  for (int i = 0; i < rhs.getRows(); i++) {
    s << rhs[i];
    if (i != rhs.getRows() - 1) {
      s << ",\n";
    }
  }
  s << " ]";
  return s;
}
