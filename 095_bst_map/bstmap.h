#ifndef __BSTMAP_H__
#define __BSTMAP_H__

#include "algorithm"
#include "map.h"

template<typename K, typename V>
class BstMap : public Map<K, V>
{
 private:
  struct Node {
    Node * left;
    Node * right;
    K key;
    V val;

    Node(K key, V val) : left(NULL), right(NULL), key(key), val(val) {}
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
    add(curr->key, curr->val);
    copy(curr->left);
    copy(curr->right);
  }

  Node ** search(const K & key) {
    Node ** curr = &(this->root);
    while (*curr != NULL) {
      if ((*curr)->key == key)
        return curr;
      else if (key > (*curr)->key)
        curr = &((*curr)->right);
      else
        curr = &((*curr)->left);
    }
    return curr;
  }

 public:
  BstMap() : root(NULL) {}
  BstMap(const BstMap<K, V> & rhs) : root(NULL) { copy(rhs.root); }
  BstMap & operator=(const BstMap<K, V> & rhs) {
    if (this != &rhs) {
      BstMap temp(rhs);
      std::swap(temp.root, root);
    }
    return *this;
  }

  virtual void add(const K & key, const V & value) {
    Node ** curr = search(key);
    if (*curr)
      (*curr)->val = value;
    else
      *curr = new Node(key, value);
  }

  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    Node ** curr = search(key);
    if (*curr)
      return (*curr)->val;
    else
      throw std::invalid_argument("invalid argument");
  }

  Node * remove(Node * curr, const K & key) {
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

    // Copy the key and value to current
    curr->key = temp->key;
    curr->val = temp->val;

    // Remove the one recursively.
    curr->left = remove(curr->left, curr->key);
    return curr;
  }

  virtual void remove(const K & key) { root = remove(root, key); }
  virtual ~BstMap<K, V>() { destroy(root); }
};
#endif
