#include"list.hpp"




template<typename T>
MySTL::list<T>::BaseNode::BaseNode(BaseNode* next, BaseNode* prev) :next(next), prev(prev)
{}

template<typename T>
MySTL::list<T>::Node::Node(const T& value, BaseNode* next, BaseNode* prev) : BaseNode(next,prev),value(value)
{}



//               Member functions

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
            cur = static_cast<Node*>(cur->next);

        }
        FakeNode.prev = cur;
    }
    catch (...) {
        Node* curr = static_cast<Node*>(FakeNode.next);
        while (curr != cur) {
            Node* temp=curr;
            curr = static_cast<Node*>(curr->next);
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
    Node* other_cur = static_cast<Node*>(other.FakeNode.next);

    Node* cur = new Node(other_cur->value, &FakeNode, &FakeNode);

    other_cur = static_cast<Node*>(other_cur->next);

    FakeNode.next = cur;
    FakeNode.prev = cur;

    try {
        for (size_t i = 1; i < other.sz; ++i) {
            cur->next = new Node(other_cur->value,&FakeNode,cur);
            cur = static_cast<Node*>(cur->next);
            other_cur = static_cast<Node*>(other_cur->next);
        }
        FakeNode.prev = cur;
    }
    catch (...) {
        Node* curr = static_cast<Node*>(FakeNode.next);
        while (curr != cur) {
            Node* temp = curr;
            curr = static_cast<Node*>(curr->next);
            delete temp;
        }

        delete cur;
        FakeNode.next = FakeNode.prev = &FakeNode;

        throw;
    }

    sz = other.sz;
}

template<typename T>
MySTL::list<T>::~list(){
   Node* cur=static_cast<Node*>(FakeNode.next);

   while(cur->next!=(&FakeNode)){
      Node* tmp=cur;
      cur=static_cast<Node*>(cur->next);
      delete tmp;
   }

   delete cur;
}



//              Capacity

template<typename T>
bool MySTL::list<T>::empty() const{
   return (FakeNode.next==(&FakeNode) && FakeNode.prev==(&FakeNode));
}

template<typename T>
size_t MySTL::list<T>::size() const{
   return sz;
}



//             Modifieres

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
    Node* tmp = static_cast<Node*>(FakeNode.prev);

    tmp->prev->next = &FakeNode;
    FakeNode.prev = tmp->prev;

    delete tmp;

    sz--;
}

template<typename T>
void MySTL::list<T>::pop_front() {
    Node* tmp = static_cast<Node*>(FakeNode.next);

    tmp->next->prev = &FakeNode;
    FakeNode.next = tmp->next;

    delete tmp;

    sz--;
}



//          Element access

template<typename T>
T& MySTL::list<T>::front() {
    return static_cast<Node*>(FakeNode.next)->value;
}

template<typename T>
T& MySTL::list<T>::back() {
    return static_cast<Node*>(FakeNode.prev)->value;
}

template<typename T>
const T& MySTL::list<T>::front() const{
    return static_cast<Node*>(FakeNode.next)->value;
}

template<typename T>
const T& MySTL::list<T>::back() const{
    return static_cast<Node*>(FakeNode.prev)->value;
}


//         Iterators

template<typename T>
typename MySTL::list<T>::iterator MySTL::list<T>::begin() {
    return iterator(static_cast<Node*>(FakeNode.next));
}

template<typename T>
typename MySTL::list<T>::iterator MySTL::list<T>::end() {
    return iterator(&Node(-1,FakeNode.next,FakeNode.prev));
}


template<typename T>
typename MySTL::list<T>::const_iterator MySTL::list<T>::begin() const {
    return const_iterator(static_cast<Node*>(FakeNode.next));
}

template<typename T>
typename MySTL::list<T>::const_iterator MySTL::list<T>::end() const {
    return const_iterator(&Node(-1,FakeNode.next,FakeNode.prev));
}

template<typename T>
typename MySTL::list<T>::const_iterator MySTL::list<T>::cbegin() const {
    return const_iterator(static_cast<Node*>(FakeNode.next));
}

template<typename T>
typename::MySTL::list<T>::const_iterator MySTL::list<T>::cend() const {
    return const_iterator(&Node(-1,FakeNode.next,FakeNode.prev));
}

template<typename T>
typename::MySTL::list<T>::reverse_iterator MySTL::list<T>::rbegin() {
    return reverse_iterator(end());
}

template<typename T>
typename MySTL::list<T>::reverse_iterator MySTL::list<T>::rend() {
    return reverse_iterator(begin());
}

template<typename T>
typename MySTL::list<T>::const_reverse_iterator MySTL::list<T>::rbegin() const {
    return const_reverse_iterator(end());
}

template<typename T>
typename MySTL::list<T>::const_reverse_iterator MySTL::list<T>::rend() const {
    return const_reverse_iterator(begin());
}

template<typename T>
typename MySTL::list<T>::const_reverse_iterator MySTL::list<T>::crbegin() const {
    return const_reverse_iterator(end());
}

template<typename T>
typename MySTL::list<T>::const_reverse_iterator MySTL::list<T>::crend() const {
    return const_reverse_iterator(begin());
}
