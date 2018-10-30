#include <assert.h>

#include <cstdlib>

#include "il.h"
void testList() {
  IntList();
  // IntList a;
  // {
  //   IntList b;
  //   a = b;
  // }
  // a.getSize();
  IntList b;
  assert(b.getSize() == 0);
  assert(b.head == NULL);
  assert(b.tail == NULL);
  for (int i = 0; i < 10; i++) {
    assert(b.getSize() == i);
    b.addBack(i);
    assert(b.getSize() == i + 1);
    assert(b[i] == i);
    assert(b.find(i) == i);
  }

  for (int i = 0; i < 10; i++) {
    assert(b[i] == i);
    assert(b.find(i) == i);
    assert(b.find(i + 100) < 0);
  }

  IntList * c = new IntList();
  for (int i = 0; i < 10; i++) {
    assert(c->getSize() == i);
    c->addFront(i);
    assert(c->getSize() == i + 1);
    assert((*c)[0] == i);
    assert((*c)[i] == 0);
    assert(c->find(i) == 0);
  }

  IntList a(b);
  assert(a.getSize() == b.getSize());
  IntList::Node * head_a = a.head;
  IntList::Node * head_b = b.head;
  assert(a.tail != b.tail);
  assert(a.tail);
  assert(b.tail);
  for (int i = 0; i < 10; i++) {
    assert(a[i] == b[i]);
    if (head_a && head_b) {
      assert(head_a->data == i);
      assert(head_b->data == i);
    }
    head_a = head_a->next;
    head_b = head_b->next;

    if (head_a && head_b) {
      assert(head_a->prev != head_b->prev);
    }
  }

  b = b;
  assert(b.getSize() == 10);
  b.remove(0);
  assert(b.getSize() == 9);
  b.remove(9);
  assert(b.getSize() == 8);

  b.addBack(0);
  b.addFront(0);
  b.addBack(9);
  for (int i = 0; i < 10; i++) {
    b.addFront(i);
    b.remove(i);
    b.remove(i);
    b.addFront(i);
    b.remove(i);
    b.remove(i);

    b.remove(i);
    b.remove(i + 100);
    assert(b.getSize() == 9 - i);
  }

  assert(b.getSize() == 0);
  assert(b.head == NULL);
  assert(b.tail == NULL);

  b = b;

  assert(b.getSize() == 0);
  assert(b.head == NULL);
  assert(b.tail == NULL);

  b = IntList();

  assert(b.getSize() == 0);
  assert(b.head == NULL);
  assert(b.tail == NULL);

  a.remove(0);
  assert(a.getSize() == 9);
  a.remove(9);
  assert(a.getSize() == 8);
  a.remove(3);
  assert(a.getSize() == 7);

  IntList d = b;
  assert(d.getSize() == 0);
  assert(d.head == NULL);
  assert(d.tail == NULL);
  for (int i = 0; i < 10; i++) {
    assert(d.find(i) < 0);
  }

  b.addBack(1);
  IntList e = b;
  assert(e.getSize() == 1);
  assert(e.head != NULL);
  assert(e.tail != NULL);

  if (true) {
    IntList f;
    for (int i = 11; i <= 20; i++) {
      f.addBack(i);
    }
    b = f;
  }
  assert(b.getSize() == 10);
  for (int i = 11; i <= 20; i++) {
    assert(b[i - 11] == i);
    assert(b.find(i - 10) < 0);
  }

  for (int i = 11; i <= 20; i++) {
    b.remove(i);
  }

  for (int i = 0; i < 100; i++) {
    b.addBack(-1);
  }
  assert(b.getSize() == 100);
  assert(b[0] == -1);
  for (int i = 0; i < 100; i++) {
    b.remove(-1);
    assert(b.getSize() == 99 - i);
  }

  for (int i = 0; i < 10; i++) {
    b.addBack(i);
  }

  IntList::Node * tmp = b.head->next->next;
  b.remove(1);
  assert(tmp == b.head->next);
  assert(tmp->prev == b.head);
  delete c;
}
int main(void) {
  testList();
  return EXIT_SUCCESS;
}
