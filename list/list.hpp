#pragma once
#include<type_traits>
#include<iterator>

namespace MySTL{
    template<typename T>
    class list{
        private:
            
            struct BaseNode{
                BaseNode* next;
                BaseNode* prev;
                BaseNode(BaseNode*,BaseNode*);
            };

            struct Node: BaseNode{
                T value;
                Node(const T&,BaseNode*,BaseNode*);
            };
            
            BaseNode FakeNode;
            size_t sz;

            template<bool isConst>
            struct base_iterator{
                public:
                 using value_type = T;
                 using pointer = conditional_t<isConst,const T*,T*>;
                 using reference = conditional_t<isConst,const T&,T&>;
                 using iterator_category = std::bidirectional_iterator_tag;
 
                private:
                 Node* ptr;

                public:
                  base_iterator(Node* ptr): ptr(ptr) {}
                  base_iterator(const base_iterator&)=default;
                  base_iterator& operator=(const base_iterator&)=default;

                  reference operator*() const{
                     return ptr->value;
                  }

                  base_iterator& operator++(){
                     ptr=static_cast<Node*>(ptr->next);
                     return *this;
                  }

                  base_iterator& operator++(int){
                      base_iterator tmp=*this;
                      ptr=static_cast<Node*>(ptr->next);
                      return tmp;
                  }

                  base_iterator& operator--(){
                     ptr=static_cast<Node*>(ptr->prev);
                     return *this;
                  }

                  base_iterator& operator--(int){
                     base_iterator tmp=*this;
                     ptr=static_cast<Node*>(ptr->prev);
                     return tmp;
                  }
        };


        public:    
            using value_type = T;
            using reference = T&;
            using const_referense = const T&;

            using iterator = base_iterator<false>;
            using const_iterator = base_iterator<true>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;
            
            //         Member functions

            list();
            list(size_t,const T&);
            list(const MySTL::list<T>&);

            ~list();
            

            //       Capacity

            bool empty() const;
            size_t size() const;


            //       Element access

            T& front();
            T& back();

            const T& front() const;
            const T& back() const;
             

            //       Modifieres

            void push_front(const T&);
            void push_back(const T&);

            void pop_front();
            void pop_back();

            //      Iterators

            iterator begin();
            iterator end();

            const_iterator begin() const;
            const_iterator end() const;

            const_iterator cbegin() const;
            const_iterator cend() const;

            reverse_iterator rbegin();
            reverse_iterator rend();

            const_reverse_iterator rbegin() const;
            const_reverse_iterator rend() const;

            const_reverse_iterator crbegin() const;
            const_reverse_iterator crend() const;
    };
};