#pragma once
#include<type_traits>
#include<stdexcept>

namespace MySTL{

    template<typename T>
    class deque{
        private:

        T** ptr_arr=nullptr;
        size_t sz=0;
        size_t row_front,col_front,row_back,col_back;
        size_t count_rows;

        static const int SZ_COL=32;
        
        template<bool isConst>
        struct base_iterator{
             public:
             using value_type = T;
             using pointer = std::conditional_t<isConst,const T*,T*>;
             using reference = std::conditional_t<isConst,const T&,T&>;
             using iterator_category = std::random_access_iterator_tag;

             private:
               T** ptr_arr;
               size_t cur_row,cur_col;
             
             public:
               base_iterator(T** ptr_arr,size_t cur_row,size_t cur_col): ptr_arr(ptr_arr),cur_row(cur_row), cur_col(cur_col)
               {}

               base_iterator(const base_iterator&) = default;

               base_iterator& operator=(const base_iterator&) = default;

               reference operator*() const{
                return ptr_arr[cur_row][cur_col];
               }

               base_iterator& operator++(){
                    cur_col++;
                    if(cur_col==SZ_COL){
                        cur_row++;
                        cur_col=0;
                    }

                    return *this;
               }

               base_iterator& operator++(int){
                    base_iterator tmp=*this;

                    cur_col++;
                    if(cur_col==SZ_COL){
                        cur_row++;
                        cur_col=0;
                    }

                    return tmp;
               }

               base_iterator& operator--(){
                  cur_col--;
                  if(cur_col==-1){
                    cur_row--;
                    cur_col=SZ_COL-1;
                  }

                  return *this;
               }

               base_iterator& operator--(int){
                  base_iterator tmp=*this;

                  cur_col--;
                  if(cur_col==-1){
                    cur_row--;
                    cur_col=SZ_COL-1;
                  }

                  return tmp;
               }

               base_iterator& operator+=(int n){
                  cur_row+=(cur_col+n)/SZ_COL;
                  cur_col=(SZ_COL*1'000'000+cur_col+n)%SZ_COL;
                  return *this;
               }

               base_iterator& operator-=(int n){
                 cur_row+=((int)cur_col-n-(SZ_COL-1))/SZ_COL;
                 cur_col=(SZ_COL*1'000'000 + ((cur_col-n)))%SZ_COL;
                 return *this;
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


        //        Member functions

        deque();
        deque(size_t,const T&);
        deque(const MySTL::deque<T>&);
        
        ~deque();

        //       Capacity

        bool empty() const;

        size_t size() const;
        
        

        //       Modifieres

        void push_front(const T&);
        void push_back(const T&);

        void pop_front();
        void pop_back();
        



        //      Element access

        T& operator[](size_t);
        const T& operator[](size_t) const;

        T& at(size_t);
        const T& at(size_t) const;

        T& front();
        T& back();

        const T& front() const;
        const T& back() const;

        

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