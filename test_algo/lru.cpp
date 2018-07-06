#include <iostream>
#include <map>
#include<list>
using namespace std;
class Node{ 
public:
  int key, value;
  Node* prev;
  Node* next;
  Node(int k, int v): key(k), value(v), prev(NULL), next(NULL) {}
};

class DoubleLinkedList {
  Node *front_, *rear_;
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
   else if (node == rear_) {
    node->prev->next = NULL;
    rear_ = node->prev;
    rear_->next = NULL;
   } else {
    node->prev->next = node->next;
    node->next->prev = node->prev;
   }
   node->prev = NULL;
   node->next= front_;
   front_->prev = node;
   front_ = node;
  }
  void remove_rear_page() {
    if (isEmpty())
      return;
    if (front_ == rear_) {
      delete rear_;
      front_ =  rear_  = NULL;
    } else {
      Node* node  = rear_;
      node->prev->next = NULL;
      rear_ = node->prev;
     delete node; 
    }
  }
  Node* get_rear_page() {
    return rear_;  
  }

};

class LRUCache {
  int capacity_, size;
  DoubleLinkedList *nodeList_;
  map<int, Node*> nodeMap_; 
  public:
   LRUCache(int capacity) {
     this->capacity_ = capacity;
     size = 0;
     nodeList_ = new DoubleLinkedList();
     nodeMap_ = map<int, Node*>();
   }

   int get(int key) {
     if (nodeMap_.find(key) != nodeMap_.end()) {
       return -1;
     }
     int value = nodeMap_[key]->value;
     nodeList_->move_page_to_head(nodeMap_[key]);
     return value;
   }

   void put(int key, int value) {
     if (nodeMap_.find(key) != nodeMap_.end()) {
       nodeMap_[key]->value = value;
       nodeList_->move_page_to_head(nodeMap_[key]);
       return;
     }
     if (size == capacity_) {
       int k = nodeList_->get_rear_page()->key;
       nodeMap_.erase(k);
       nodeList_->remove_rear_page();
       size--;
     }
     Node* n = nodeList_->add_page_to_head(key, value);
     nodeMap_[key] = n;
     size++;
   }
   ~LRUCache() {
     map<int, Node*>::iterator i;
     for (i = nodeMap_.begin(); i != nodeMap_.end(); i++) {
       delete i->second;
     }
     delete nodeList_;
   }

   void dumpCacheContent() {
     map<int, Node*>::iterator i;
     std::cout << "LRUCache content" << std::endl;
     for (i = nodeMap_.begin(); i != nodeMap_.end(); i++) {
       std::cout << "  key: " << i->first << "value: " << i->second->value <<std::endl;
     }
   }
};


class LRUCache2 {
  int capacity_;
  std::list<Node*> nodeList_;
  map<int, Node*> nodeMap_;
public:
  LRUCache2(int capacity) {
    this->capacity_ = capacity;
    nodeMap_ = map<int, Node*>();
  }
  int get(int key) {
    if(nodeMap_.find(key) == nodeMap_.end()) {
      return -1;
    }
    int value = nodeMap_.find(key)->second->value;  
    nodeList_.remove(nodeMap_.find(key)->second);
    nodeList_.push_front(nodeMap_.find(key)->second);
  }
  
  void put(int key, int value) {
    if (nodeMap_.find(key) != nodeMap_.end()) {
      Node* n = nodeMap_.find(key)->second;
      nodeList_.remove(n);
      nodeList_.push_front(n);
      return;
    }
    if (nodeList_.size() == capacity_) {
      Node* n_last =  nodeList_.back();
      nodeList_.pop_back();
      int key = n_last->key;
      nodeMap_.erase(key);
    }
    Node* n = new Node(key, value);
    nodeList_.push_front(n);
    nodeMap_[key] = n;
  }

  ~LRUCache2() {
    map<int, Node*>::iterator i;
    for (i = nodeMap_.begin(); i != nodeMap_.end(); i++) {
      delete i->second;
    }
  }

  void dumpCacheContent() {
    map<int, Node*>::iterator i;
    std::cout << "LRUCache2 content" << std::endl;
    for (i = nodeMap_.begin(); i != nodeMap_.end(); i++) {
      std::cout << "  key:" << i->first << "value:" << i->second->value <<std::endl;
   }
  }
};


 int main() {
  LRUCache *c = new LRUCache(3); 
  c->put(1, 1);
  c->dumpCacheContent();
  c->put(2, 2);
  c->dumpCacheContent();
  c->put(3, 3);
  c->dumpCacheContent();
  c->put(4, 4);
  c->dumpCacheContent();
  c->put(3, 3);
  c->dumpCacheContent();
  c->put(5, 5);
  c->dumpCacheContent();


  LRUCache2 *c2 = new LRUCache2(3); 
  c2->put(1, 1);
  c2->dumpCacheContent();
  c2->put(2, 2);
  c2->dumpCacheContent();
  c2->put(3, 3);
  c2->dumpCacheContent();
  c2->put(4, 4);
  c2->dumpCacheContent();
  c2->put(3, 3);
  c2->dumpCacheContent();
  c2->put(5, 5);
  c2->dumpCacheContent();
 }
