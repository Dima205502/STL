#pragma once
#include<functional>
#include<type_traits>
#include<vector>

namespace MySTL{
    
    template<typename Key,typename Value,typename Hash=std::hash<Key>,typename KeyEqual=std::equal_to<Key>>
    class unordered_map{
        public:
           using value_type = std::pair<const Key,Value>;
           using reference = std::pair<const Key,Value>&;
           using const_reference = const std::pair<const Key,Value>&;

        private:
          struct BaseNode{
            BaseNode* prev;
            BaseNode* next;
            BaseNode(BaseNode*,BaseNode*);

          };

          struct Node: BaseNode{
            value_type kv;
            size_t cache;
            Node(BaseNode*,BaseNode*,value_type,size_t);
          };

          template<bool isConst>
          struct base_iterator{
             public:
               using value_type = std::pair<const Key,Value>;
               using reference = std::conditional_t<isConst,const std::pair<const Key,Value>,std::pair<const Key,Value>>;
               using pointer = std::conditional_t<isConst,const std::pair<const Key,Value>*,std::pair<const Key,Value>*>;
               using iterator_category = std::forward_iterator_tag;

             private:
               BaseNode* ptr;
            
             public:
               base_iterator(BaseNode* ptr): ptr(ptr) {}
               base_iterator(const base_iterator&) = default;
               base_iterator& operator=(const base_iterator&) = default;

               reference operator*() const{
                 return static_cast<Node*>(ptr)->kv;
               }

               base_iterator& operator++(){
                 ptr=ptr->next;
                 return *this;
               }

               base_iterator& operator++(int){
                base_iterator tmp=*this;
                ptr=ptr->next;

                return tmp;
               }

               bool operator==(const base_iterator& other){
                return ptr == other.ptr;
               }


               friend class unordered_map<Key,Value,Hash,KeyEqual>;
          };

          size_t sz;
          size_t bucket_cnt;
          
          double max_load_factor;
          
          Node** buckets;
          size_t* bucket_sz;

          BaseNode FakeNode;
          
          bool isPrime(size_t value){

            for(size_t i=2;i*i<=value;++i){
              if(value%i==0){
                return false;
              }
            }

            return value>1;
          }

          size_t nextPrime(size_t value){
            
               for(size_t i=value+1;;++i){
                   if(isPrime(i)){
                      return i;
                   }
               }
          }

        public:
          using iterator = base_iterator<false>;
          using const_iterator = base_iterator<true>;


          //        Member functions

          unordered_map();
          unordered_map(size_t);
          unordered_map(const unordered_map<Key,Value,Hash,KeyEqual>&);

          ~unordered_map();


          //          Capacity

          bool empty() const;
          size_t size() const;


          //          Modifieres

          std::pair<iterator,bool> insert(value_type);
          size_t erase(const Key&) ;
          iterator erase(iterator);


          //          Element access

          Value& operator[](const Key&);

          size_t count(const Key&) const;

          iterator find(const Key&);
          const_iterator find(const Key&) const;

          //        Bucket interface
 
          iterator begin();
          iterator end();

          const_iterator begin() const;
          const_iterator end() const;

          const_iterator cbegin() const;
          const_iterator cend() const;    

          size_t bucket_count() const;
          size_t bucket_size(size_t) const;


          //        Hash policy    

          double loud_factor() const;
          double max_loud_factor() const;

          void rehash(size_t n);
          void reserve(size_t n);
    };
}