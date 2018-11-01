#ifndef _LL_H_
#define _LL_H_
#include <assert.h>

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
//YOUR CODE GOES HERE

class OutOfBoundaryException : public std::exception
{
 public:
  const char * what() const throw() { return "Out of boundary"; }
};

template<typename T>
class LinkedList
{
 private:
  class Node
  {
   public:
    T data;
    Node * prev;
    Node * next;

    Node(T v, Node * _prev, Node * _next) : data(v), prev(_prev), next(_next) {}
  };

  Node * head;
  Node * tail;
  int size;

  //  bool remove(T & data, Node * current);

 public:
  LinkedList() : head(NULL), tail(NULL), size(0) {}
  LinkedList(const LinkedList & rhs);
  LinkedList<T> & operator=(const LinkedList & rhs);
  ~LinkedList() {
    while (head != NULL) {
      Node * temp = head;
      head = head->next;
      delete temp;
    }
  }

  T & operator[](int index);
  const T & operator[](int index) const;

  int getSize() const { return size; }
  int getSize() { return size; }
  void addFront(const T & item);
  void addBack(const T & item);
  bool remove(const T & item);
  int find(const T & item) const;
  friend void testList(void);
};

// template<typename T>
// bool LinkedList<T>::remove(T & data, Node * current) {
//   if (!current)
//     return false;
//   if (data == current->data) {
//     Node * next = current->next;
//     Node * prev = current->prev;
//     if (prev) {
//       prev->next = next;
//     }
//     else {
//       head = next;
//     }
//     if (next) {
//       next->prev = prev;
//     }
//     else {
//       tail = prev;
//     }
//     delete current;
//     size--;
//     return true;
//   }
//   return remove(data, current->next);
// }

template<typename T>
LinkedList<T>::LinkedList(const LinkedList & rhs) : head(NULL), tail(NULL), size(0) {
  Node * temp = rhs.head;
  while (temp) {
    addBack(temp->data);
    temp = temp->next;
  }
}

template<typename T>
LinkedList<T> & LinkedList<T>::operator=(const LinkedList & rhs) {
  if (this != &rhs) {
    LinkedList temp(rhs);
    std::swap(temp.tail, tail);
    std::swap(temp.head, head);
    std::swap(temp.size, size);
  }
  return *this;
}

template<typename T>
void LinkedList<T>::addFront(const T & item) {
  head = new Node(item, NULL, head);
  if (tail == NULL) {
    tail = head;
  }
  else {
    head->next->prev = head;
  }
  size++;
}

template<typename T>
void LinkedList<T>::addBack(const T & item) {
  tail = new Node(item, tail, NULL);
  if (head == NULL) {
    head = tail;
  }
  else {
    tail->prev->next = tail;
  }
  size++;
}

template<typename T>
T & LinkedList<T>::operator[](int index) {
  try {
    if (index < 0 || index > (int)size)
      throw OutOfBoundaryException();
  }
  catch (OutOfBoundaryException & e) {
    std::cerr << e.what() << std::endl;
  }
  catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
  }
  Node * current = head;
  for (int i = 0; i < index; i++) {
    current = current->next;
  }
  return current->data;
}

template<typename T>
const T & LinkedList<T>::operator[](int index) const {
  try {
    if (index < 0 || index > (int)size)
      throw OutOfBoundaryException();
  }
  catch (OutOfBoundaryException & e) {
    std::cerr << e.what() << std::endl;
  }
  catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
  }
  Node * current = head;
  for (int i = 0; i < index; i++) {
    current = current->next;
  }
  return current->data;
}

template<typename T>
int LinkedList<T>::find(const T & item) const {
  int pos = 0;
  while (pos < size) {
    if (item == (*this)[pos])
      return pos;
    pos++;
  }
  return pos == size ? -1 : pos;
}

template<typename T>
bool LinkedList<T>::remove(const T & item) {
  Node * temp = head;
  while (temp && !(temp->data == item)) {
    temp = temp->next;
  }
  if (!temp)
    return false;
  Node * next = temp->next;
  Node * prev = temp->prev;
  if (next)
    next->prev = prev;
  else
    tail = prev;
  if (prev)
    prev->next = next;
  else
    head = next;
  size--;
  delete temp;
  return true;
}
#endif
