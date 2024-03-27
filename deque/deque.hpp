#pragma once
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
        
        public:
        
        deque();
        deque(size_t,const T&);
        deque(const MySTL::deque<T>&);

        size_t size() const;
        
        bool empty() const;

        void push_front(const T&);
        void push_back(const T&);

        void pop_front();
        void pop_back();
        
        T& operator[](size_t);
        const T& operator[](size_t) const;

        T& at(size_t);
        const T& at(size_t) const;

        T& front();
        T& back();

        const T& front() const;
        const T& back() const;

        ~deque();
    };
};