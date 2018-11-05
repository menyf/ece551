#ifndef __BSTSET_H__
#define __BSTSET_H__

#include "algorithm"
#include "set.h"

template<typename T>
class BstSet : public Set<T>
{
 public:
  struct Node {
    Node * left;
    Node * right;
    T key;

    Node() : left(NULL), right(NULL), key(T()) {}
    Node(Node * left, Node * right, T key) : left(left), right(right), key(key) {}
  };

  Node * root;

  void destroy(Node * curr) {
    if (curr == NULL)
      return;
    destroy(curr->left);
    destroy(curr->right);
    delete curr;
  }

  void copy(Node * curr) {
    if (curr == NULL)
      return;
    add(curr->key);
    copy(curr->left);
    copy(curr->right);
  }

 public:
  BstSet() : root(NULL) {}
  BstSet(const BstSet<T> & rhs) : root(NULL) { copy(rhs.root); }
  BstSet & operator=(const BstSet<T> & rhs) {
    if (this != &rhs) {
      BstSet temp(rhs);
      std::swap(temp.root, root);
    }
    return *this;
  }

  virtual void add(const T & key) {
    if (!root) {
      root = new Node(NULL, NULL, key);
      return;
    }
    Node ** curr = &(this->root);
    while (*curr != NULL) {
      if ((*curr)->key == key) {
        return;
      }
      else if (key > (*curr)->key) {
        curr = &((*curr)->right);
      }
      else {
        curr = &((*curr)->left);
      }
    }
    *curr = new Node(NULL, NULL, key);
  }

  virtual bool contains(const T & key) const {
    if (!root)
      return false;
    Node * const * curr = &(this->root);
    while (*curr != NULL) {
      if ((*curr)->key == key) {
        return true;
      }
      else if (key > (*curr)->key) {
        curr = &((*curr)->right);
      }
      else {
        curr = &((*curr)->left);
      }
    }
    return false;
  }

  Node * remove(Node * curr, const T & key) {
    if (curr == NULL)
      return NULL;

    // NOT FOUND
    if (curr->key > key) {
      curr->left = remove(curr->left, key);
      return curr;
    }
    else if (curr->key < key) {
      curr->right = remove(curr->right, key);
      return curr;
    }

    // FOUND
    if (curr->left == NULL) {
      Node * temp = curr->right;
      delete curr;
      return temp;
    }
    else if (curr->right == NULL) {
      Node * temp = curr->left;
      delete curr;
      return temp;
    }

    // Neither left nor right is NULL
    // Find the one just less than current node
    Node * temp = curr->left;
    while (temp->right) {
      temp = temp->right;
    }

    // Copy the key and keyue to current
    curr->key = temp->key;
    curr->key = temp->key;

    // remove the one recursively.
    curr->left = remove(curr->left, curr->key);
    return curr;
  }

  virtual void remove(const T & key) { root = remove(root, key); }

  virtual ~BstSet<T>() { destroy(root); }
};
#endif
