#include "node.h"
#include "queue"

Node * buildTree(uint64_t * counts) {
  //WRITE ME!
  priority_queue_t pq;
  for (int i = 0; i < 257; i++) {
    if (counts[i])
      pq.push(new Node(i, counts[i]));
  }
  while (pq.size() > 1) {
    Node * a = pq.top();
    pq.pop();
    Node * b = pq.top();
    pq.pop();
    Node * c = new Node(a, b);
    pq.push(c);
  }

  return pq.top();
}
