#ifndef __BSTMAP_H__
#define __BSTMAP_H__

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

    Node(Node * left = NULL, Node * right = NULL, K key = 0, V val = 0) :
        left(left),
        right(right),
        key(key),
        val(val) {}
  };

  Node * root;

  void destroy(Node * curr) {
    if (curr == NULL)
      return;
    destroy(curr->left);
    destroy(curr->right);
    delete curr;
  }

 public:
  BstMap() : root(NULL) {}
  virtual void add(const K & key, const V & value) {
    if (!root) {
      root = new Node(NULL, NULL, key, value);
      return;
    }
    Node ** curr = &root;
    while (*curr != NULL) {
      if ((*curr)->key == key) {
        (*curr)->val = value;
        return;
      }
      else if (key > (*curr)->key) {
        curr = &((*curr)->right);
      }
      else {
        curr = &((*curr)->left);
      }
    }
    *curr = new Node(NULL, NULL, key, value);
  }

  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    if (!root)
      throw std::invalid_argument("invalid argument");
    Node ** curr = &root;
    while (*curr != NULL) {
      if ((*curr)->key == key) {
        return (*curr)->val;
      }
      else if (key > (*curr)->key) {
        curr = &((*curr)->right);
      }
      else {
        curr = &((*curr)->left);
      }
    }
    throw std::invalid_argument("invalid argument");
  }
  Node * remove(Node * curr, const K & key) {
    if (curr == NULL)
      return NULL;

    // NOT FOUND
    if (curr->key > key) {
      return curr->left = remove(curr->left, key);
    }
    else if (curr->key < key) {
      return curr->right = remove(curr->right, key);
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

    // remove the one recursively.
    curr->left = remove(curr->left, curr->key);
    return curr;
  }

  virtual void remove(const K & key) { root = remove(root, key); }
  virtual ~BstMap<K, V>() { destroy(root); }
};
#endif
