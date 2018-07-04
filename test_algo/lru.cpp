#include <iostream>
class Node {
  int key, value;
  Node* prev;
  Node* next;
  Node(int k, int v): key(k), value(v), prev(NULL), next(NULL) {}
};

class DoubleLinkedList {
  Node *front_, rear_;
  bool isEmpty() {
    return rear_ == NULL;
  }
public:
  DoubleLinkedList():front_(NULL), rear_(NULL) {}
  Node* add_page_to_head(int key, int value) {
    Node* node = new Node(key, value);
    if (front_ == NULL)
      front_ = rear_ = node;
    else {
      node->next = front_;
      node->prev = NULL;
      front_->prev = node;
      front_ = node;
    }
    return node;
  }
  void move_page_to_head(Node* node) {
   if (node == front_)
     return;
   else if if (node == rear) {
    node->prev->next = NULL;
    rear = node->prev;
    rear->next = NULL;
   } else {
    node->prev->next = node->next;
    node->next->prev = node->prev;
   }
   node->prev = NULL;
   node->next= front_;
   front->prev = node;
   front = node;
  }
  void remove_rear_page() {
    Node* node = rear;
    if (isEmpty())
      return;
  }
}

