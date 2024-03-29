#include"list.hpp"

template<typename T>
MySTL::list<T>::BaseNode::BaseNode(BaseNode* next, BaseNode* prev) :next(next), prev(prev)
{}

template<typename T>
MySTL::list<T>::Node::Node(const T& value, BaseNode* next, BaseNode* prev) : BaseNode(next,prev),value(value)
{}

template<typename T>
MySTL::list<T>::list() : FakeNode{ &FakeNode,&FakeNode }, sz(0)
{}

template<typename T>
MySTL::list<T>::list(size_t n, const T& value) : list() {
    if (n == 0) return;

    Node* cur= new Node(value,&FakeNode,&FakeNode);
    FakeNode.next = cur;
    FakeNode.prev = cur;

    try {
        for (size_t i = 1; i < n; ++i) {
            cur->next = new Node(value,&FakeNode,cur);
            cur = reinterpret_cast<Node*>(cur->next);

        }
        FakeNode.prev = cur;
    }
    catch (...) {
        Node* curr = reinterpret_cast<Node*>(FakeNode.next);
        while (curr != cur) {
            Node* temp=curr;
            curr = reinterpret_cast<Node*>(curr->next);
            delete temp;
        }

        delete cur;

        throw;
    }

    sz = n;
}

template<typename T>
MySTL::list<T>::list(const list<T>& other) :list() {
    if (other.empty()) return;
    Node* other_cur = reinterpret_cast<Node*>(other.FakeNode.next);

    Node* cur = new Node(other_cur->value, &FakeNode, &FakeNode);

    other_cur = reinterpret_cast<Node*>(other_cur->next);

    FakeNode.next = cur;
    FakeNode.prev = cur;

    try {
        for (size_t i = 1; i < other.sz; ++i) {
            cur->next = new Node(other_cur->value,&FakeNode,cur);
            cur = reinterpret_cast<Node*>(cur->next);
            other_cur = reinterpret_cast<Node*>(other_cur->next);
        }
        FakeNode.prev = cur;
    }
    catch (...) {
        Node* curr = reinterpret_cast<Node*>(FakeNode.next);
        while (curr != cur) {
            Node* temp = curr;
            curr = reinterpret_cast<Node*>(curr->next);
            delete temp;
        }

        delete cur;
        FakeNode.next = FakeNode.prev = &FakeNode;

        throw;
    }

    sz = other.sz;
}


template<typename T>
void MySTL::list<T>::push_back(const T& value) {
    Node* new_node = new Node(value, &FakeNode, FakeNode.prev);
    FakeNode.prev->next = new_node;
    FakeNode.prev = new_node;

    sz++;
}


template<typename T>
void MySTL::list<T>::push_front(const T& value) {
    Node* new_node = new Node(value, FakeNode.next, &FakeNode);

    FakeNode.next->prev = new_node;
    FakeNode.next = new_node;

    sz++;
}

template<typename T>
void MySTL::list<T>::pop_back() {
    Node* tmp = reinterpret_cast<Node*>(FakeNode.prev);

    tmp->prev->next = &FakeNode;
    FakeNode.prev = tmp->prev;

    delete tmp;

    sz--;
}

template<typename T>
void MySTL::list<T>::pop_front() {
    Node* tmp = reinterpret_cast<Node*>(FakeNode.next);

    tmp->next->prev = &FakeNode;
    FakeNode.next = tmp->next;

    delete tmp;

    sz--;
}

template<typename T>
T& MySTL::list<T>::front() {
    return reinterpret_cast<Node*>(FakeNode.next)->value;
}

template<typename T>
T& MySTL::list<T>::back() {
    return reinterpret_cast<Node*>(FakeNode.prev)->value;
}

template<typename T>
const T& MySTL::list<T>::front() const{
    return reinterpret_cast<Node*>(FakeNode.next)->value;
}

template<typename T>
const T& MySTL::list<T>::back() const{
    return reinterpret_cast<Node*>(FakeNode.prev)->value;
}

template<typename T>
bool MySTL::list<T>::empty() const{
   return (FakeNode.next==(&FakeNode) && FakeNode.prev==(&FakeNode));
}

template<typename T>
size_t MySTL::list<T>::size() const{
   return sz;
}

template<typename T>
MySTL::list<T>::~list(){
   Node* cur=reinterpret_cast<Node*>(FakeNode.next);

   while(cur->next!=(&FakeNode)){
      Node* tmp=cur;
      cur=reinterpret_cast<Node*>(cur->next);
      delete tmp;
   }

   delete cur;
}