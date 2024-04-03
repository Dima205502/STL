#pragma once
#include<memory>
#include<utility>
#include<iterator>
#include<stdexcept>
#include<type_traits>
#include<stddef.h>

namespace MySTL{

  template<typename T,typename Alloc = std::allocator<T>>
  class vector {

    private:
    Alloc alloc;

    T* arr = nullptr;
    size_t sz = 0;
    size_t cp = 0;

    using AllocTraits = std::allocator_traits<Alloc>;

       template<bool isConst>
       struct base_iterator {
       public:
          using value_type = T;
          using pointer = conditional_t<isConst, const T*, T*>;
          using reference = conditional_t<isConst, const T&, T&>;
          using iterator_category = std::random_access_iterator_tag;
 
       private:
          pointer ptr;

       public:
          base_iterator(pointer ptr) : ptr(ptr) {}
          base_iterator(const base_iterator&) = default;
          base_iterator& operator=(const base_iterator&) = default;

          reference operator*() const {
             return *ptr;
          }

          pointer operator->() const {
             return ptr;
          }

          base_iterator& operator++() {
             ++ptr;
             return *this;
          }

          base_iterator& operator++(int) {
             base_iterator tmp = *this;
             ++ptr;
             return tmp;
          }

          base_iterator& operator--() {
             --ptr;
             return *this;
          }

          base_iterator& operator--(int) {
             base_iterator tmp = *this;
             --ptr;
             return tmp;
          }

          base_iterator& operator+=(int n) {
             ptr += n;
             return *this;
          }

          base_iterator& operator-=(int n) {
             ptr -= n;
             return *this;
          }

          bool operator==(const base_iterator& other) {
              return ptr == other.ptr;
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


      //    Member functions

      vector();
      vector(size_t, const T&);

      vector(vector&&);
      vector(const vector<T,Alloc>&);

      vector& operator=(const vector<T, Alloc>&);
      vector& operator=(vector&&);

      ~vector();



      //     Capacity 

      bool empty() const;
      size_t size() const;
      size_t capacity() const;

      void reserve(size_t);


      //     Modifieres

      void push_back(const T&);
      void push_back(T&&);

      template<typename... Args>
      void emplace_back(Args&...);

      void pop_back();

      void resize(size_t, const T&);


      //    Element access

      T& operator[](size_t);
      const T& operator[](size_t) const;

      T& at(size_t);
      const T& at(size_t) const;

      T& back();
      T& front();

      const T& back() const;
      const T& front() const;


      //       Iterators

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



  template<>
  class vector<bool>{
    private:
      uint8_t* arr=nullptr;
      size_t sz=0;
      size_t cp=0;
      
      struct BitReference{
        uint8_t* ptr;
        uint8_t pos;

        BitReference(uint8_t*,uint8_t);

        BitReference operator=(bool);

        operator bool() const;
      };
      

      template<bool isConst>
       struct base_iterator{
           public:
              using value_type = bool;
              using pointer = conditional_t<isConst,const bool*,bool*>;
              using reference = conditional_t<isConst,const bool&,bool&>;
              using iterator_category = std::contiguous_iterator_tag;

           private:
              BitReference bit_ptr;

          public:
              base_iterator(BitReference bit_ptr): bit_ptr(bit_ptr) {}
              base_iterator(const base_iterator&)=default;
              base_iterator& operator=(const base_iterator&)=default;

              BitReference operator*() const{
                 return bit_ptr;    
              }

              base_iterator& operator++(){
                bit_ptr.pos++;
                if(bit_ptr.pos==8) bit_ptr.pos=0,bit_ptr.ptr++;
                return *this;
              }

              base_iterator& operator++(int){
                base_iterator tmp=*this;
                bit_ptr.pos++;
                if(bit_ptr.pos==8) bit_ptr.pos=0,bit_ptr.ptr++;
                return tmp;
              }

              base_iterator& operator--(){
                bit_ptr.pos--;
                if(bit_ptr.pos==-1) bit_ptr.pos=7,bit_ptr.ptr--;
                return *this;
              }

              base_iterator& operator--(int){
                base_iterator tmp=*this;
                bit_ptr.pos--;
                if(bit_ptr.pos==-1) bit_ptr.pos=7,bit_ptr.ptr--;
                return tmp;
              }

              base_iterator& operator+=(int n){
                bit_ptr.ptr+=(bit_ptr.pos+n)/8;
                bit_ptr.pos=(1'000'000*8+bit_ptr.pos+n)%8;
                return *this;
              }

              base_iterator& operator-=(int n){
                bit_ptr.ptr+=(bit_ptr.pos-n-7)/8;
                bit_ptr.pos=(1'000'000*8-((bit_ptr.pos-n)))%8;
                return *this;
              } 

              bool operator==(const base_iterator& other){
                return bit_ptr.ptr == other.bit_ptr.ptr && bit_ptr.pos == other.bit_ptr.pos;
              }
       };

    public:
      using value_type = bool;
      using reference = bool&;
      using const_referense = const bool&;

      using iterator = base_iterator<false>;
      using const_iterator = base_iterator<true>;
      using reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;



      //           Member functions

      vector();
      vector(size_t,bool);
      vector(const MySTL::vector<bool>&);

      ~vector();




      //          Capacity
      
      bool empty() const;

      size_t size() const;
      size_t capacity() const;
      void reserve(size_t);
      



      //        Modifieres

      void push_back(const bool&);
      void pop_back();
      
      void resize(size_t,bool);


      //         Element access

      BitReference operator[](size_t);
      const BitReference operator[](size_t) const;

      BitReference at(size_t);
      const BitReference at(size_t) const;

      BitReference back();
      BitReference front();

      const BitReference back() const;
      const BitReference front() const;

      


      //         Iterators

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
